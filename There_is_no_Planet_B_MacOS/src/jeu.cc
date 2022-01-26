#include "../include/combat.hh"

int main()
{
  // Paramètres de la fenêtre
  int widthWindow = VideoMode::getDesktopMode().width;
  int heightWindow = VideoMode::getDesktopMode().height;
  if(widthWindow*9/16 > heightWindow)
    widthWindow = heightWindow*16/9;
  else
    heightWindow = widthWindow*9/16;
  // Échelle image
  float scale = widthWindow/1920.;
  string path = "./graphics_and_sounds/";
  RenderWindow window(VideoMode(widthWindow, heightWindow), "There is no Planet B Fighter", Style::Default);

  // vecteur de sons et volumes Personnages
  vector<tuple<string, float>> v_sound = {{"confirm",10.0}, {"cursor", 10.0}, {"back", 20.0} , {"unselect",10.0} ,
    {"choix_perso", 50.0}, {"pause", 15.0}, {"vs", 15.0}};

  //vecteur Menu présentation
  vector<tuple<string, string, int, int, int, int>> v = {{"fond.png", "fond", 0, 0, widthWindow, heightWindow}};

  // création du menu de présentation
  Menu_Start menu1("Press Space to continue", widthWindow/2, 3*heightWindow/4,
    widthWindow, heightWindow, scale, path + "menu1/", v, v_sound);

  //vecteurs menu selection Personnages
  vector<tuple<string, string, int, int, int, int>> v2 = {{"fond.png", "fond", 0, 0, widthWindow, heightWindow}, {"return.png", "return", 15, 15, 75, 75}};
  vector<string> v_select = {"greta", "trump", "bouteille"};
  vector<tuple<string, string, int, int, int>> v_text = {{"SELECT CHARACTERS", "title", widthWindow/2, heightWindow/10,
    widthWindow/20}, {"J1   select : E  unselect : R", "j1_select", 3*widthWindow/16, 11*heightWindow/12, widthWindow/50},
    {"J2   select : K  unselect : L", "j2_select", 13*widthWindow/16, 11*heightWindow/12, widthWindow/50},
    {"TAB", "retour", 52, 90, widthWindow/50}};
  vector<string> v_touch = {"select1", "select2", "up1", "up2", "down1", "down2", "left1", "left2", "right1", "right2"};
  // création du menu de choix de persos
  Menu_Character_Map menu2(widthWindow, heightWindow, scale, path + "menu2/", v2, v_select, 150, 150, v_text, 2,
    -heightWindow/15, heightWindow - 850*scale, 642, 850, v_touch, v_sound);


  // vecteurs menu choix de la map
  vector<tuple<string, string, int, int, int, int>> v3 = {{"return.png", "return", 15, 15, 75, 75}};
  vector<string> v_select2 = {"banquise", "street", "planet_b", "desert"};
  vector<tuple<string, string, int, int, int>> v_text2 = {{"SELECT MAP", "title", widthWindow/2, heightWindow/10,
    widthWindow/20}, {"move cursor : Z, Q, S, D     select : E", "map_select", 1*widthWindow/4, 11*heightWindow/12, widthWindow/50},
    {"TAB", "retour", 52, 90, widthWindow/50}};
  vector<string> v_touch2 = {"select1", "up1", "down1", "left1", "right1"};

  // création du menu de choix de la map
  Menu_Character_Map menu3(widthWindow, heightWindow, scale, path + "menu3/", v3, v_select2, 150, 150, v_text2, 1, 0, 0,
    widthWindow, heightWindow, v_touch2, v_sound);

  // vecteurs menu pause
  vector<tuple<string, string, int, int, int, int>> v4 = {};
  vector<tuple<string, string, int, int, int>> v_text3 = {{"Resume", "resume", 0, 0, widthWindow/30},
    {"Restart", "restart", 0, 0, widthWindow/30}, {"Return to character selection", "select_charac", 0, 0, widthWindow/30},
    {"Return to map selection", "select_map", 0, 0, widthWindow/30}, {"Return to presentation menu", "presentation_menu", 0, 0, widthWindow/30}};
  vector<string> v_touch3 = {"select", "up", "down"};
  Menu_Pause menu_pause(widthWindow, heightWindow, scale, path + "menu3/", false, widthWindow/2, heightWindow/2,
    v4, v_text3, v_touch3, v_sound);

  // container des menus
  tuple<Menu_Start*, Menu_Character_Map*, Menu_Character_Map*> menu = {&menu1, &menu2, &menu3};
  Combat* combat = nullptr;
  Menu_Pause* menu_end = nullptr;

  // chargement et lecture des musiques des menus
  Music intro, music;
  if(!intro.openFromFile(path + "sounds_menu/intro.ogg"))
    cout << "Erreur chargement de intro.ogg" << endl;
  if(!music.openFromFile(path + "sounds_menu/music.ogg"))
    cout << "Erreur chargement de music.ogg" << endl;

  intro.play();
  music.play();
  music.setLoop(true);
  intro.setVolume(35.0);
  music.setVolume(15.0);
  bool annonce_choix_perso = false;

  bool fin_jeu = false;

  // Ouverture de la fenêtre
  while(window.isOpen())
  {
      window.setFramerateLimit(60); // Framerate
      Event event; // Evenement
      window.clear(Color(119, 181, 254, 255));

      // Gestion des evenements
      while(window.pollEvent(event))
      {
        // La fenêtre se ferme si on clique sur la croix
        if(event.type == Event::Closed)
        {
          music.stop();
          delete combat;
          delete menu_end;
          fin_jeu = true;
          window.close();
        }
      }

      // si l'on poursuit le jeu
      if(!fin_jeu)
      {
        //lancement du menu de présentation
        if(menu1.on)
        {
          if(annonce_choix_perso == true)
            annonce_choix_perso = false;
          menu1.show_menu(window);
          menu1.action_menu(menu2);
        }

        // lancement du menu de choix de persos
        if(menu2.on)
        {
          intro.stop();
          if(annonce_choix_perso==false)
          {
            menu2.sounds["choix_perso"].play();
            annonce_choix_perso=true;
          }
          menu2.show_menu(window);
          menu2.action_menu(menu1, menu3);
        }

        // lancement du menu de choix de la map
        if(menu3.on)
        {
          annonce_choix_perso=false;
          menu3.show_menu(window);
          menu3.action_menu(menu2, menu3);
          combat = menu3.fight(window, music);
        }

        // lancement du combat
        if(combat != nullptr)
        {
          window.draw(combat->sprites["map"]);
          if(menu_end == nullptr)
          {
            //affichage des personages
            combat->draw(combat->perso1,combat->jauge_mana1,combat->jauge_vie1,window);
            combat->draw(combat->perso2,combat->jauge_mana2,combat->jauge_vie2,window);

            // gestion de la mort des personnages
            menu_end = combat->dead(combat->perso1, combat->perso2, combat->jauge_vie1, window);
            if(menu_end == nullptr)
              menu_end = combat->dead(combat->perso2, combat->perso1, combat->jauge_vie2, window);

            //suppression des attaques et de leurs sprites
            combat->perso1.suppr_spe();
            combat->perso1.suppr_ult();
            combat->perso2.suppr_spe();
            combat->perso2.suppr_ult();

            // mise a jour des jauges des joueurs 1 et 2
            combat->change_jauge(combat->jauge_vie1, 2);
            combat->change_jauge(combat->jauge_mana1, 4);
            combat->change_jauge(combat->jauge_vie2, 2);
            combat->change_jauge(combat->jauge_mana2, 4);

            // Si les persos ne combattent pas
            combat->perso1.no_fight();
            combat->perso2.no_fight();

            // pause du combat
            if(combat->pause)
            {
              combat->pause_jeu(menu_pause);

              menu_pause.show_menu(window);
              menu_pause.action_menu(menu, &combat, music);
            }
            else if(combat != nullptr && combat->fight)
            {
              combat->pause_jeu(menu_pause);

              // gestion des mouvements des personages
              combat->mouvement_perso(combat->perso1);
              combat->mouvement_perso(combat->perso2);

              // gestion des attaques corps
              combat->attaque_corps_a_corps(combat->perso1, combat->perso2, combat->press_key1["press_cac"]);
              combat->attaque_corps_a_corps(combat->perso2, combat->perso1, combat->press_key2["press_cac"]);

              // gestion des attaques spéciales du combat
              combat->attaque_special(combat->perso1, combat->perso2, window, combat->press_key1["press_spe"]);
              combat->attaque_special(combat->perso2, combat->perso1, window, combat->press_key2["press_spe"]);

              // Gestion des attaques ultimes du combat
              combat->attaque_ultime(combat->perso1, combat->perso2, window, combat->press_key1["press_ult"]);
              combat->attaque_ultime(combat->perso2, combat->perso1, window, combat->press_key2["press_ult"]);
            }
          }
          // Fin du combat
          else
          {
            menu_end->show_menu(window);
            menu_end->action_menu(menu, &combat, music);
            if(menu1.on || menu2.on || menu3.on)
            {
              menu_end->~Menu_Pause();
              menu_end = nullptr;
            }
          }
        }
      }
      window.display();
  }
  return 0;
}
