#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../include/combat.hh"

// Paramètres de la fenêtre
int widthWindow = 2*VideoMode::getDesktopMode().width/3;
int heightWindow = 2*VideoMode::getDesktopMode().height/3;
float scale = widthWindow/1920.;
string path = "/Users/gabin/OneDrive/Bureau/MAIN4/C++/There_is_no_Planet_B_MacOS/graphics_and_sounds/";
RenderWindow window(VideoMode(widthWindow, heightWindow), "Jeu", Style::Default);
vector<tuple<string, float>> v_sound = {{"confirm",10.0}, {"cursor", 10.0}};

TEST_CASE("1: Menu_Start"){
  vector<tuple<string, string, int, int, int, int>> v = {{"fond.png", "fond", 0, 0, widthWindow, heightWindow}};
  Menu_Start menu_start1("bonjour", widthWindow/2, 3*heightWindow/4,
    widthWindow, heightWindow, scale, path + "menu1/", v, v_sound);
  Menu_Start menu_start2("je suis un menu", widthWindow/2, 3*heightWindow/4,
    widthWindow, heightWindow, scale, path + "menu1/", v, v_sound);
  REQUIRE(menu_start1.id != menu_start2.id);
  REQUIRE(menu_start1.texte.getString() != menu_start2.texte.getString());
  REQUIRE(menu_start1.sprites.size() != 0);
  REQUIRE(menu_start1.sprites.size() != 0);
  REQUIRE(menu_start1.sounds.size() != 0);
  REQUIRE(menu_start1.sounds.size() != 0);
}

TEST_CASE("2: Menu_Character_Map")
{
  vector<tuple<string, string, int, int, int, int>> v1 = {{"fond.png", "fond", 0, 0, widthWindow, heightWindow}};
  vector<string> v_select1 = {"greta", "trump", "bouteille"};
  vector<tuple<string, string, int, int, int>> v_text1 = {{"SELECT CHARACTERS", "title", widthWindow/2, heightWindow/10, widthWindow/20}};
  vector<string> v_touch1 = {"select1", "select2", "up1", "up2", "down1", "down2", "left1", "left2", "right1", "right2"};
  Menu_Character_Map menu_char_map1(widthWindow, heightWindow, scale, path + "menu2/", v1, v_select1, 150, 150, v_text1, 2,
    -heightWindow/15, heightWindow - 850*scale, 642, 850, v_touch1, v_sound);

  vector<string> v_touch2 = {"select1", "up1", "down1", "left1", "right1"};
  Menu_Character_Map menu_char_map2(widthWindow, heightWindow, scale, path + "menu2/", v1, v_select1, 150, 150, v_text1, 1,
    -heightWindow/15, heightWindow - 850*scale, 642, 850, v_touch2, v_sound);
  REQUIRE(menu_char_map1.id != menu_char_map2.id);
  REQUIRE(menu_char_map1.sprites.size() != 0);
  REQUIRE(menu_char_map1.sprites.size() != 0);
  REQUIRE(!menu_char_map1.is_clock_restart);
  REQUIRE(!menu_char_map1.is_clock_restart);
  REQUIRE(menu_char_map1.select.size() != menu_char_map2.select.size());
}

TEST_CASE("3: Menu_Pause")
{
  vector<tuple<string, string, int, int, int, int>> v1 = {};
  vector<tuple<string, string, int, int, int>> v_text2 = {{"Resume", "resume", 0, 0, widthWindow/30}};
  vector<string> v_touch3 = {"select", "up", "down"};
  Menu_Pause menu_pause(widthWindow, heightWindow, scale, path + "menu3/", false, widthWindow/2, heightWindow/2,
    v1, v_text2, v_touch3, v_sound);
  REQUIRE(menu_pause.size_rect["sizeX"] < menu_pause.fond.getSize().x);
  REQUIRE(menu_pause.size_rect["sizeY"] < menu_pause.fond.getSize().y);
  REQUIRE(menu_pause.texte.size() == 1);
}

TEST_CASE("3: Combat/Jauge/Personnage/Attaque")
{
  Combat combat(widthWindow/2, widthWindow, heightWindow, scale, path + "combat/", "greta", "trump", "desert");
  REQUIRE(combat.jauge_vie1.valeur["val_reach"] == 100);
  REQUIRE(combat.jauge_vie2.valeur["val_reach"] == 100);
  REQUIRE(combat.jauge_mana1.valeur["val_reach"] == 50);
  REQUIRE(combat.jauge_mana2.valeur["val_reach"] == 50);
  REQUIRE(combat.perso1.nom != combat.perso2.nom);
  REQUIRE(combat.perso1.sounds.size() == combat.perso2.sounds.size());
  REQUIRE(combat.attaque.degat == 5);
  REQUIRE(combat.perso1.att_spe.size() == 0);
  REQUIRE(combat.perso2.att_ult == nullptr);
}
