#include "../include/personnage.hh"

// gestion des collision entre deux personnages
void Attaque::gestion_collision(Personnage & perso, Personnage & adversaire)
{
  if(collision_A_B(perso, adversaire))
  {
    adversaire.jauge_vie -= degat;
    adversaire.hurt();
    adversaire.jauge_mana += 10;
  }
}

// Constructeur
Attaque_special::Attaque_special(float sc, int d, float s, Texture & t, Texture & exp, int x, int y,
  int sizeX, int sizeY, Direction dir, int anim_max,/* int anim_exp_max,*/ int a/*, int a_exp*/) :
  Attaque(sc, d), speed(s), texture(t), explosion(exp), direction(dir), suppr(false)
{
  pos = {{"x", x}, {"y", y}};
  info_sprite = {{"sizeX", sizeX}, {"sizeY", sizeY}, {"anim", a}/*, {"anim_exp", a_exp}*/};
  sprite.setTextureRect(IntRect(0,0,sizeX,sizeY)); // On affecte la texture au sprite
  sprite.setTexture(texture);
  sprite.setScale(scale, scale);
  anim={{"anim_mouv", {0, anim_max}}/*, {"anim_exp", {0, anim_exp_max}}*/};
}

// Animation de l'attaque
void Attaque_special::animate(tuple<int, int> & anim, const string info)
{
  if(get<0>(anim) >= get<1>(anim)) // si l'animation arrive à la fin
    get<0>(anim) = 0; // on recommence
  sprite.setTextureRect(IntRect(get<0>(anim)*info_sprite["sizeX"],info_sprite[info],info_sprite["sizeX"],info_sprite["sizeY"]));
  get<0>(anim)++;
}

// On fixe les limites du terrain
void Attaque_special::limitX(const int & widthWindow)
{
  if(pos["x"] + info_sprite["sizeX"]*scale <= 0) // Bordure gauche
  {
    suppr = true;
  }
  else if((pos["x"]) >= widthWindow) // Bordure droite
  {
    suppr = true;
  }
}

// Gestion des mpuvements
void Attaque_special::mouvement(const int & widthWindow)
{
  sprite.setPosition(pos["x"],pos["y"]); // On bouge le sprite aux coordonnees x, y
  if(direction == RIGHT)
    pos["x"] += speed; // on ajoute constament la vitesse
  else
    pos["x"] -= speed; // on ajoute constament la vitesse
  animate(anim["anim_mouv"], "anim");
  limitX(widthWindow);
}

// Gestion des collisions pour les attaques speciales
void Attaque_special::gestion_collision(Personnage & perso, Personnage & adversaire)
{
  // collision spe/adversaire
  if(collision_A_B(*this, adversaire))
  {
    suppr = true;
    // do explosion
    adversaire.jauge_vie -= degat;
    adversaire.hurt();
    adversaire.jauge_mana += 10;
  }
  // collision spe/spe_adversaire
  for(size_t i = 0; i < (adversaire.att_spe).size(); i++)
  {
    if(collision_A_B(*this, *adversaire.att_spe[i]))
    {
      suppr = true;
      adversaire.att_spe[i]->suppr = true;
      // do explosion
    }
  }
  // collision spe/ult_adversaire
  if(adversaire.att_ult != NULL && collision_A_B(*this, *adversaire.att_ult))
  {
    suppr = true;
    // do explosion
  }
  // collision spe/ult_perso
  if(perso.att_ult != NULL && collision_A_B(*this, *perso.att_ult))
  {
    suppr = true;
    // do explosion
  }
}

// Gestion des collisions pour les attaques ultimes
void Attaque_ultime::gestion_collision(Personnage & perso, Personnage & adversaire)
{
  // collision ult/adversaire
  if(collision_A_B(*this, adversaire))
  {
    suppr = true;
    // do explosion
    adversaire.jauge_vie -= degat;
    adversaire.hurt();
    adversaire.jauge_mana += 60;
  }

  // collision ult/ult_adversaire
  if(adversaire.att_ult != NULL && collision_A_B(*this, *adversaire.att_ult))
  {
    suppr = true;
    adversaire.att_ult->suppr = true;
    // do explosion
  }
}
