#include "../include/personnage.hh"

// Permet de load une texture à partir d'une image
void load_texture(Texture & t, const string s)
{
  if (!t.loadFromFile(s)) // load texture from file
    cout << "Erreur de chargement de la texture\n"; // message d'erreur
  t.setSmooth(true); // smooth
}

// Constructeur
Personnage::Personnage(string n, float s, string t, int sizeX, int sizeY, int anim_left, int anim_right, int anim_up_left, int anim_up_right,
  int anim_fight_left, int anim_fight_right, int anim_death, int nb_a_w, int a_j, int nb_a_c, int nb_a_d, int x, int y, float spX, float spY,
  float g, Direction d, map<string, Keyboard::Key> & k, Jauge_Vie & j_v, Jauge_Mana & j_m,
  Attaque & cc, string t_att_spe, string t_exp_att_spe, string t_att_ult, string t_exp_att_ult, string p) :
  nom(n), scale(s), gravity(g), direction(d),  touche(k), jauge_vie(j_v), jauge_mana(j_m),
  cac(cc), att_ult(NULL), is_attacking(false), is_hurt(false), path(p), punch(0)
{
  load_texture(texture, t);
  // création de la map des infos du sprite
  info_sprite = {{"sizeX", sizeX}, {"sizeY", sizeY}, {"anim_left", anim_left}, {"anim_right", anim_right},
    {"anim_up_left", anim_up_left}, {"anim_up_right", anim_up_right}, {"anim_fight_left", anim_fight_left},
    {"anim_fight_right",anim_fight_right}, {"anim_death",anim_death}};
  animation = {{"anim_walk", {0, nb_a_w}}, {"anim_jump", {a_j, 1}}, {"anim_combat", {0, nb_a_c}}, {"anim_death", {0, nb_a_d}}}; // map animation
  pos = {{"x", x}, {"y", y - sizeY*scale}}; // map position
  if(direction == RIGHT) // si la dernière direction était droite
    sprite.setTextureRect(IntRect(0,info_sprite["anim_right"],info_sprite["sizeX"],info_sprite["sizeY"])); // on reprend le sprite de droite
  else if(direction == LEFT) // si la dernière direction etait à gauche
    sprite.setTextureRect(IntRect(0,info_sprite["anim_left"],info_sprite["sizeX"],info_sprite["sizeY"])); // on reprend le sprite de gauche
  sprite.setTexture(texture);
  velocity = {{"x", 0} , {"y", 0}}; // vitesse du personnage lors d'une action (variable)
  speed = {{"x", spX} , {"y", spY}};  // attribution de la vitesse propre du personnage (fixe)
  load_texture(texture_att_spe, t_att_spe);
  load_texture(explosion_att_spe, t_exp_att_spe);
  load_texture(texture_att_ult, t_att_ult);
  load_texture(explosion_att_ult, t_exp_att_ult);
  sprite.setPosition(pos["x"],pos["y"]); // On bouge le sprite aux coordonnees x, y
  sprite.setScale(scale, scale);

  vector<tuple<string, float>> v_sound = {{"jump",35.0}, {"step", 35.0}, {"special", 30.0} , {"ultime",20.0}, {"win",50.0}, {"loose",50.0}};
  vector<tuple<string, float>> v_sound_commun = {{"punch1", 10.0}, {"punch2", 10.0}, {"punch3", 10.0}, {"hurt", 10.0}};
  for(size_t i = 0; i < v_sound.size(); i++)
    load_sound(sound_buffers, sounds, p, v_sound[i]);
  for(size_t i = 0; i < v_sound_commun.size(); i++)
    load_sound(sound_buffers, sounds, p + "../sounds_fight/", v_sound_commun[i]);
}

// Destructeur
Personnage::~Personnage()
{
  for(size_t i = 0; i < att_spe.size(); i++)
  {
    delete att_spe[i];
  }
  delete att_ult;
}

// Animation du saut du personnage
void Personnage::animate_jump()
{
  if(direction == LEFT)
    sprite.setTextureRect(IntRect(get<0>(animation["anim_jump"])*info_sprite["sizeX"],info_sprite["anim_up_left"],info_sprite["sizeX"],info_sprite["sizeY"]));
  else
    sprite.setTextureRect(IntRect(get<0>(animation["anim_jump"])*info_sprite["sizeX"],info_sprite["anim_up_right"],info_sprite["sizeX"],info_sprite["sizeY"]));
}

// Fonction d'animation des mouvement gauche, droite et fight
void Personnage::animate(tuple<int, int> & anim, const string info)
{
  if(get<0>(anim) >= get<1>(anim)) // si l'animation arrive à la fin
    get<0>(anim) = 0; // on recommence
  if(direction == LEFT)
    sprite.setTextureRect(IntRect(get<0>(anim)*info_sprite["sizeX"],info_sprite[info + "_left"],info_sprite["sizeX"],info_sprite["sizeY"]));
  else
    sprite.setTextureRect(IntRect(get<0>(anim)*info_sprite["sizeX"],info_sprite[info + "_right"],info_sprite["sizeX"],info_sprite["sizeY"]));
  get<0>(anim)++;
}

// Animation de la mort
void Personnage::animate_death()
{
  if(get<0>(animation["anim_death"]) >= get<1>(animation["anim_death"])) // si l'animation arrive à la fin
    get<0>(animation["anim_death"]) = get<1>(animation["anim_death"]); // on reste à la fin
  sprite.setTextureRect(IntRect(get<0>(animation["anim_death"])*info_sprite["sizeX"],info_sprite["anim_death"],info_sprite["sizeX"],info_sprite["sizeY"]));
  get<0>(animation["anim_death"])++;
}

// Fixe les limites en X de la map
void Personnage::limitX(const int & widthWindow)
{
  if(pos["x"] + 2*info_sprite["sizeX"]*scale/7 <= 0) // Bordure gauche
      pos["x"] = -2*info_sprite["sizeX"]*scale/7; // on bloque
  else if((pos["x"] + 5*info_sprite["sizeX"]*scale/7) >= widthWindow) // Bordure droite
      pos["x"] = widthWindow - 5*info_sprite["sizeX"]*scale/7; // on bloque
}

// Fixe les limites en Y de la map
void Personnage::limitY(const int & ground)
{
  pos["y"] = ground - info_sprite["sizeY"]*scale; // la position en y est celle du sol
  velocity["y"] = 0.; // pas de velocité en Y
}

// Fonction du mouvement perpétuel
void Personnage::mouvement(const int & ground, const int & widthWindow)
{
  sprite.setPosition(pos["x"],pos["y"]); // On bouge le sprite aux coordonnees x, y
  pos["x"] += velocity["x"]; // on ajoute constament la velocité
  pos["y"] += velocity["y"];
  limitX(widthWindow);
  if(pos["y"] < (int) (ground - info_sprite["sizeY"]*scale)) // si on est en l'air,
  {
    if(velocity["y"] >= 0.)
      animate_jump();
    velocity["y"] += gravity; // la gravité nous fait tomber petit à petit
  }
  else
    limitY(ground);
  if(is_attacking == true)
    animate(animation["anim_combat"], "anim_fight");
}

// Saut du Personnage
void Personnage::up(const int & ground)
{
  Sound::Status status = sounds["jump"].getStatus();
  if((status != Sound::Playing) && (pos["y"] == (int) (ground - info_sprite["sizeY"]*scale)))
    sounds["jump"].play();
  get<0>(animation["anim_walk"]) = get<1>(animation["anim_walk"])/2;
  if(pos["y"] >= (int) (ground - info_sprite["sizeY"]*scale - gravity))
    velocity["y"] = -speed["y"];
}

// Déplacement vers la gauche du personnage
void Personnage::left(const int & ground)
{
  Sound::Status status = sounds["step"].getStatus();
  if((status != Sound::Playing) && (pos["y"] == (int) (ground - info_sprite["sizeY"]*scale)))
    sounds["step"].play();
  direction = LEFT; // le personnage regarde à gauche
  if(pos["y"] == (int) (ground - info_sprite["sizeY"]*scale) && is_attacking == false)
    animate(animation["anim_walk"], "anim");
  velocity["x"] = -speed["x"]; // on modifie la velocité
}

// Déplacement vers la droite du personnage
void Personnage::right(const int & ground)
{
  Sound::Status status = sounds["step"].getStatus();
  if((status != Sound::Playing) && (pos["y"] == (int) (ground - info_sprite["sizeY"]*scale)))
    sounds["step"].play();
  direction = RIGHT; // le personnage regarde à droite
  if(pos["y"] == (int) (ground - info_sprite["sizeY"]*scale)  && is_attacking == false)
    animate(animation["anim_walk"], "anim");
  velocity["x"] = speed["x"]; // on modifie la vélocité
}

// faire un son quand on tape
void Personnage::corps_a_corps()
{
  sounds["punch" + to_string(punch%3)].play();
  punch++;
}

// Lancement d'une attaque spéciale
void Personnage::special()
{
  if(jauge_mana.valeur["val_reach"] >= 10)
  {
    sounds["special"].play();
    is_attacking = true;
    Attaque_special* new_att_spe;
    if(direction == RIGHT)
    {
      new_att_spe = new Attaque_special(scale, 5, speed["x"]*1.2, texture_att_spe, explosion_att_spe,
      pos["x"] + info_sprite["sizeX"]*scale/2, pos["y"] + (info_sprite["sizeY"]/2 - 100)*scale, 200, 200, direction, 11, 200);
    }
    else
    {
      new_att_spe = new Attaque_special(scale, 5, speed["x"]*1.2, texture_att_spe, explosion_att_spe,
      pos["x"], pos["y"] + (info_sprite["sizeY"]/2 - 100)*scale, 200, 200, direction, 11, 0);
    }
    att_spe.push_back(new_att_spe);
    jauge_mana -= 10;
  }
}

// Supression des attaques speciales qui ont touché l'adversaire ou qui sont hors map
void Personnage::suppr_spe()
{
  for(size_t i = 0; i < att_spe.size(); i++)
  {
    if(att_spe[i]->suppr == true)
    {
      delete att_spe[i];
      att_spe.erase(att_spe.begin() + i);
    }
  }
}

// Lancement de l'attaque ultime
void Personnage::ultime()
{
  if(jauge_mana.valeur["val_reach"] == 100)
  {
    sounds["ultime"].play();
    is_attacking = true;
    Attaque_ultime* new_att_ult;
    if(direction == RIGHT)
    {
      new_att_ult = new Attaque_ultime(scale, 40, speed["x"]*1.2, texture_att_ult, explosion_att_ult,
      pos["x"] + info_sprite["sizeX"]*scale/2 , pos["y"] + (info_sprite["sizeY"]/2 - 300)*scale, 600, 600, direction, 11, 600);
    }
    else
    {
      new_att_ult = new Attaque_ultime(scale, 40, speed["x"]*1.2, texture_att_ult, explosion_att_ult,
      pos["x"] + info_sprite["sizeX"]*scale/2 - 600*scale, pos["y"] + (info_sprite["sizeY"]/2 - 300)*scale, 600, 600, direction, 11, 0);
    }
    att_ult = new_att_ult;
    jauge_mana -= 100;
  }
}

// Suppression de l'attaque ultime
void Personnage::suppr_ult()
{
  if(att_ult != NULL && att_ult->suppr == true)
  {
    delete att_ult;
    att_ult = NULL;
  }
}

// Si le personnage ne se bat pas
void Personnage::no_fight()
{
  if(is_attacking == true && get<0>(animation["anim_combat"]) == get<1>(animation["anim_combat"]))
  {
    get<0>(animation["anim_combat"]) = 0;
    is_attacking = false;
  }
}

// Son quand le personnage est touché
void Personnage::hurt()
{
  sounds["hurt"].play();
}

// Si le personnage est mort
void Personnage::dead(const int & ground, const int & widthWindow)
{
  if(pos["y"] < (int) (ground - info_sprite["sizeY"]*scale))
    mouvement(ground, widthWindow);
  animate_death();
}

// Pas de déplacement
void Personnage::nothing(const int & ground)
{
  get<0>(animation["anim_walk"]) = 0;
  if(pos["y"] == (int) (ground - info_sprite["sizeY"]*scale) && is_attacking == false)
  {
    if(direction == RIGHT) // si la dernière direction était droite
        sprite.setTextureRect(IntRect(0,info_sprite["anim_right"],info_sprite["sizeX"],info_sprite["sizeY"])); // on reprend le sprite de droite
    else if(direction == LEFT) // si la dernière direction etait à gauche
        sprite.setTextureRect(IntRect(0,info_sprite["anim_left"],info_sprite["sizeX"],info_sprite["sizeY"])); // on reprend le sprite de gauche
  }
  velocity["x"] = 0.; // la vitesse en x est nulle (on ne bouge pas)
}
