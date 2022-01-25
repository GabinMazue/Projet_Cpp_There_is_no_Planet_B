#include "../include/combat.hh"

// load personnage depuis un fichier .csv
Personnage csv_to_perso(string path, string name_perso, int x, int ground, Direction d, map<string, Keyboard::Key> & touche,
  Jauge_Vie & j_v, Jauge_Mana & j_m, Attaque & cac, float scale)
{
  vector<string> vect_string_perso;
  vector<int> vect_int_perso;
  vector<float> vect_float_perso;
  ifstream fichier(path + "perso_" + name_perso + "/" + name_perso + ".csv");
  string line;
  int i = 1;
  while(getline(fichier, line))
  {
    // Create a stringstream of the current line
    stringstream ss(line);
    if(i <= 5)
      vect_string_perso.push_back(line);
    else if(i <= 18)
      vect_int_perso.push_back(stoi(line));
    else
      vect_float_perso.push_back(stof(line));
    i++;
  }
  fichier.close();
  int pos_x = x;
  if(d == LEFT)
    pos_x -= vect_int_perso[0]*scale;

  return Personnage(name_perso, scale, path + "perso_" + name_perso + "/" + vect_string_perso[0], vect_int_perso[0], vect_int_perso[1], vect_int_perso[2], vect_int_perso[3],
    vect_int_perso[4], vect_int_perso[5], vect_int_perso[6], vect_int_perso[7], vect_int_perso[8], vect_int_perso[9],
    vect_int_perso[10], vect_int_perso[11], vect_int_perso[12], pos_x, ground, vect_float_perso[0]*scale, vect_float_perso[1]*scale,
    vect_float_perso[2]*scale, d, touche, j_v, j_m, cac, path + "perso_" + name_perso + "/" + vect_string_perso[1],
    path + "perso_" + name_perso + "/" + vect_string_perso[2], path + "perso_" + name_perso + "/" +vect_string_perso[3],
    path + "perso_" + name_perso + "/" +vect_string_perso[4], path + "perso_" + name_perso + "/");
}


// Constructeur
Combat::Combat(int g, int width, int height, float s, string path, string p1, string p2, string m) :
  fight(false), pause(false), end(false), file(path), ground(g), widthWindow(width), heightWindow(height), scale(s),
  jauge_vie1(Jauge_Vie(scale, path + "perso_" + p1 + "/" + p1 + "_portrait.png", 100, heightWindow/100, widthWindow/100 + 200*scale,
    widthWindow/100 + 100*scale - widthWindow/60, widthWindow/2.5 - 200*scale, widthWindow/30, 200, 200, widthWindow, true)),
  jauge_mana1(Jauge_Mana(scale, 50, heightWindow/100, widthWindow/100 + 200*scale, widthWindow/100 + heightWindow/100 + 100*scale,
    (widthWindow/2.5 - 200*scale)/2, widthWindow/60, true, path + "sounds_fight/")),
  touche1({{"Up",Keyboard::Z}, {"Left", Keyboard::Q}, {"Right", Keyboard::D},
    {"Attack",Keyboard::E}, {"Attack_Spe",Keyboard::R}, {"Attack_Ult",Keyboard::T}}),
  press_key1({{"press_cac", false}, {"press_spe", false}, {"press_ult", false}}), phrase1(false),
  jauge_vie2(Jauge_Vie(scale, path + "perso_" + p2 + "/" + p2 + "_portrait.png", 100, heightWindow/100,
    widthWindow - widthWindow/100 - widthWindow/2.5, widthWindow/100 + 100*scale - widthWindow/60, widthWindow/2.5 - 200*scale,
    widthWindow/30, 200, 200, widthWindow, false)),
  jauge_mana2(Jauge_Mana(scale, 50, heightWindow/100,  widthWindow - widthWindow/100 - widthWindow/2.5 + (widthWindow/2.5 - 200*scale)/2,
    widthWindow/100 + heightWindow/100 + 100*scale, (widthWindow/2.5 - 200*scale)/2, widthWindow/60, false, path + "sounds_fight/")),
  touche2({{"Up",Keyboard::Up}, {"Left", Keyboard::Left}, {"Right", Keyboard::Right},
    {"Attack",Keyboard::K}, {"Attack_Spe",Keyboard::L}, {"Attack_Ult",Keyboard::M}}),
  press_key2({{"press_cac", false}, {"press_spe", false}, {"press_ult", false}}), phrase2(false),
  attaque(Attaque(scale, 5)), perso1(csv_to_perso(path, p1, widthWindow/100 + 200*scale, ground, RIGHT, touche1, jauge_vie1, jauge_mana1, attaque, scale)),
  perso2(csv_to_perso(path, p2, widthWindow - widthWindow/100 - 200*scale, ground, LEFT, touche2, jauge_vie2, jauge_mana2, attaque, scale)),
  is_clock_restart(false), music_winner_on(false)
{
  load_texture(textures["map"], path + "map/fond_" + m + ".png");
  sprites["map"].setTexture(textures["map"]);
  sprites["map"].setScale(scale, scale);
  switch_pause = true;
  if(!Keyboard::isKeyPressed(Keyboard::Tab))
    switch_pause = false;
  if(!music_fight.openFromFile(path + "sounds_fight/music_fight.ogg"))
    cout << "Erreur chargement music fight" << endl;
  if(!music_winner.openFromFile(path + "sounds_fight/music_winner.ogg"))
    cout << "Erreur chargement music winner" << endl;
  music_fight.setVolume(10.0);
  music_winner.setVolume(10.0);
  music_fight.setLoop(true);
  vector<tuple<string, float>> v_sound = {{"three",35.0}, {"two", 35.0}, {"one", 35.0} , {"go",35.0}, {"ko",35.0}};
  for(size_t i = 0; i < v_sound.size(); i++)
    load_sound(sound_buffers, sounds, path + "sounds_fight/", v_sound[i]);
}

// Mouvement des persos
void Combat::mouvement_perso(Personnage & perso)
{
  perso.mouvement(ground, widthWindow);
  if(Keyboard::isKeyPressed(perso.touche["Up"]))
    perso.up(ground);

  if(Keyboard::isKeyPressed(perso.touche["Left"]))
    perso.left(ground);

  if(Keyboard::isKeyPressed(perso.touche["Right"]))
    perso.right(ground);

  if(!Keyboard::isKeyPressed(perso.touche["Left"]) && !Keyboard::isKeyPressed(perso.touche["Right"]))
    perso.nothing(ground);
}

// Gestion des attaques corps à corps
void Combat::attaque_corps_a_corps(Personnage & perso, Personnage & adversaire, bool & press_cac)
{
  if(!Keyboard::isKeyPressed(perso.touche["Attack"]) && perso.is_attacking == false)
    press_cac = false;

  if(Keyboard::isKeyPressed(perso.touche["Attack"]) && press_cac == false && perso.is_hurt == false)
  {
    perso.corps_a_corps();
    perso.is_attacking = true;
    perso.cac.gestion_collision(perso, adversaire);
    press_cac = true;
  }
}

// Gestion des attaques speciales du combat
void Combat::attaque_special(Personnage & perso, Personnage & adversaire, RenderWindow & window, bool & press_spe)
{
  if(!Keyboard::isKeyPressed(perso.touche["Attack_Spe"]) && perso.is_attacking == false)
    press_spe = false;

  if(Keyboard::isKeyPressed(perso.touche["Attack_Spe"]) && press_spe == false && perso.is_hurt == false)
  {
    perso.special();
    press_spe = true;
  }

  for(size_t i = 0; i < (perso.att_spe).size(); i++)
  {
    perso.att_spe[i]->mouvement(widthWindow);
    perso.att_spe[i]->gestion_collision(perso, adversaire);
  }
}


// gestion des attaques ultimes du combat
void Combat::attaque_ultime(Personnage & perso, Personnage & adversaire, RenderWindow & window, bool & press_ult)
{
  if(!Keyboard::isKeyPressed(perso.touche["Attack_Ult"]) && perso.is_attacking == false)
    press_ult = false;

  if(Keyboard::isKeyPressed(perso.touche["Attack_Ult"]) && press_ult == false && perso.is_hurt == false)
  {
    perso.ultime();
    press_ult = true;
  }

  if(perso.att_ult != NULL)
  {
    perso.att_ult->mouvement(widthWindow);
    perso.att_ult->gestion_collision(perso, adversaire);
  }
}

// Mouvement dynamique des différentes jauges
void Combat::change_jauge(Jauge & jauge, int val)
{
  if(jauge.valeur["val"] < jauge.valeur["val_reach"])
    jauge.jauge_augmente(val);
  else if(jauge.valeur["val"] > jauge.valeur["val_reach"])
    jauge.jauge_baisse(val);
}

// Test si les personnages sont mort et actions en conscequence
Menu_Pause* Combat::dead(Personnage & perso, Personnage & adversaire, Jauge_Vie & jauge_vie, RenderWindow & window)
{
  if(jauge_vie.valeur["val_reach"] == 0)
  {
    fight = false;
    if(!end)
    {
      clock.restart();
      for(size_t i = 0; i < 24; i++)
        load_texture(textures["gif"+to_string(i)], file + "gif_ko/image" + to_string(i) + ".png");
    }
    end = true;
    for(size_t i = 0; i < (perso.att_spe).size(); i++)
      perso.att_spe[i]->suppr = true;
    if(perso.att_ult != NULL)
      perso.att_ult->suppr = true;
    for(size_t i = 0; i < (adversaire.att_spe).size(); i++)
      adversaire.att_spe[i]->suppr = true;
    if(adversaire.att_ult != NULL)
      adversaire.att_ult->suppr = true;
    perso.dead(ground, widthWindow);
    // draw winner
    if(clock.getElapsedTime().asMilliseconds() < 2160)
    {
      gif(window, textures["gif" + to_string((clock.getElapsedTime().asMilliseconds()/90)%24)],
        sprites["gif" + to_string((clock.getElapsedTime().asMilliseconds()/90)%24)], widthWindow/2, heightWindow/2, 2*scale);
      if(clock.getElapsedTime().asMilliseconds() < 200)
        sounds["ko"].play();
    }
    else if(!phrase1)
    {
      window.draw(sprites["gif23"]);
      music_fight.stop();
      perso.sounds["loose"].play();
      phrase1 = true;
    }
    else if(!phrase2 && perso.sounds["hurt"].getStatus() != Sound::Playing)
    {
      Clock c;
      while(c.getElapsedTime().asMilliseconds() < 1500) {}
      adversaire.sounds["win"].play();
      phrase2 = true;
    }
    else if(music_winner_on == false && perso.sounds["loose"].getStatus() != Sound::Playing && adversaire.sounds["win"].getStatus() != Sound::Playing)
    {
      music_winner.play();
      music_winner_on = true;
      vector<tuple<string, string, int, int, int, int>> v = {{"perso_" + adversaire.nom + "/" + adversaire.nom + "_win.png", "portrait",
        1030, 20, 800*scale, 1060*scale},
        {"victory.png", "victory", 415, 126, 550*scale, 611*scale}};
      vector<tuple<string, string, int, int, int>> v_text = {{"Restart", "restart", 0, 0, widthWindow/30},
        {"Return to character selection", "select_charac", 0, 0, widthWindow/30}, {"Return to presentation menu", "presentation_menu", 0, 0, widthWindow/30}};
      vector<string> v_touch = {"select", "up", "down"};
      vector<tuple<string, float>> v_sound = {{"confirm",10.0}, {"cursor", 10.0}, {"back", 20.0} , {"unselect",10.0} ,
        {"choix_perso", 50.0}, {"pause", 15.0}, {"vs", 15.0}};
      Menu_Pause* menu_end = new Menu_Pause(widthWindow, heightWindow, scale, file, true, widthWindow/2, 4*heightWindow/5,
        v, v_text, v_touch, v_sound);
      return menu_end;
    }
  }
  return nullptr;
}

// Pause du combat
void Combat::pause_jeu(Menu_Pause & menu_pause)
{
  if(!Keyboard::isKeyPressed(Keyboard::Tab) && switch_pause == true)
    switch_pause = false;
  if(Keyboard::isKeyPressed(Keyboard::Tab) && !pause && !switch_pause)
  {
    menu_pause.on = true;
    menu_pause.load_menu();
    switch_pause = true;
    pause = true;
  }
  else if(Keyboard::isKeyPressed(Keyboard::Tab) && pause && !switch_pause)
  {
    switch_pause = true;
    pause = false;
    menu_pause.on = false;
    menu_pause.sounds["pause"].play();
  }
}

// affichage du comat
void Combat::draw(Personnage & perso, Jauge_Mana & jauge_mana, Jauge_Vie & jauge_vie, RenderWindow & window)
{
  if(!is_clock_restart)
  {
    clock.restart();
    is_clock_restart = true;
    for(size_t i = 0; i < 41; i++)
      load_texture(textures["gif"+to_string(i)], file + "gif_fight/image" + to_string(i) + ".png");
  }
  if(clock.getElapsedTime().asMilliseconds() < 3690 && !end)
  {
    gif(window, textures["gif" + to_string((clock.getElapsedTime().asMilliseconds()/90)%41)],
      sprites["gif" + to_string((clock.getElapsedTime().asMilliseconds()/90)%41)], widthWindow/2, heightWindow/2, 1.5*scale);
    if((clock.getElapsedTime().asMilliseconds()/90)%41 == 0)
      sounds["three"].play();
    else if((clock.getElapsedTime().asMilliseconds()/90)%41 == 8)
      sounds["two"].play();
    else if((clock.getElapsedTime().asMilliseconds()/90)%41 == 16)
      sounds["one"].play();
    else if((clock.getElapsedTime().asMilliseconds()/90)%41 == 25)
      sounds["go"].play();
  }
  else if(fight == false && end == false)
  {
    fight = true;
    music_fight.play();
  }
  window.draw(jauge_vie.jauge);
  window.draw(jauge_vie.remplissage);
  window.draw(jauge_vie.sprite);
  window.draw(jauge_mana.jauge);
  window.draw(jauge_mana.remplissage);
  window.draw(perso.sprite);
  for(size_t i = 0; i < (perso.att_spe).size(); i++)
    window.draw(perso.att_spe[i]->sprite);
  if(perso.att_ult != NULL)
    window.draw(perso.att_ult->sprite);
}
