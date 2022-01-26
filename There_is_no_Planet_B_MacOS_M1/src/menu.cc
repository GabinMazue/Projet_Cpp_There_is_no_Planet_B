#include "../include/combat.hh"

// Création d'un rectanle
RectangleShape creation_rectangle(int x, int y, int sizeX, int sizeY, Color couleur)
{
  RectangleShape rectangle;
  rectangle.setPosition(x, y);
  rectangle.setSize(Vector2f(sizeX, sizeY));
  rectangle.setFillColor(couleur);
  return rectangle;
}

// load du texte depuis un string
void load_texte(Font & f, Text & t, string & t_a, int char_size, Color col)
{
  f.loadFromFile("./graphics_and_sounds/Harribi.ttf");
  t.setFont(f);
  t.setString(t_a);
  t.setFillColor(col);
  t.setCharacterSize(char_size);
}

// affichage d'un gif
void gif(RenderWindow & window, Texture & texture, Sprite & sprite, int x, int y, float scale)
{
  sprite.setTexture(texture);
  sprite.setScale(scale, scale);
  sprite.setPosition(x - texture.getSize().x*scale/2, y - texture.getSize().y*scale/2);
  window.draw(sprite);
}

// initialisation du compteur
int Menu::compt = 0;

// Constructeur
Menu::Menu(bool o, int w, int h, float s, string f, vector<tuple<string, string, int, int, int, int>> & v,
  vector<tuple<string,float>> & v_sound) : id(compt++), on(o),  widthWindow(w), heightWindow(h), scale(s), file(f)
{
  // Gestion des textures/sprites
  for(size_t i = 0; i < v.size(); i++)
  {
    Texture t;
    load_texture(t, f + get<0>(v[i]));
    textures[get<1>(v[i])] = t;
    sprites[get<1>(v[i])].setTexture(textures[get<1>(v[i])]);
    sprites[get<1>(v[i])].setScale(scale, scale);
    pos[get<1>(v[i])] = {{"x", get<2>(v[i])*scale}, {"y", get<3>(v[i])*scale}};
    info_sprites[get<1>(v[i])] = {{"sizeX", get<4>(v[i])}, {"sizeY", get<5>(v[i])}};
  }
  // Gestion des sons
  for(size_t i = 0; i < v_sound.size(); i++)
    load_sound(sound_buffers, sounds, file + "../sounds_menu/", v_sound[i]);
}









// Constructeur
Menu_Start::Menu_Start(string t_a, int x_t, int y_t, int w, int h, float s, string f,
  vector<tuple<string, string, int, int, int, int>> & v, vector<tuple<string,float>> & v_sound) : Menu(true, w, h, s, f, v, v_sound)
{
  load_texte(font, texte, t_a, widthWindow/30, Color::White);
  FloatRect r = texte.getGlobalBounds();
  pos["texte"] = {{"x", x_t - r.width/2}, {"y", y_t}};
}

// affichage du menu
void Menu_Start::show_menu(RenderWindow & window)
{
  // Les sprites
  for(map<string, Sprite>::iterator it = sprites.begin(); it != sprites.end(); it++)
  {
      it->second.setPosition(pos[it->first]["x"], pos[it->first]["y"]);
      window.draw(it->second);
  }
  // Le texte
  texte.setPosition(pos["texte"]["x"], pos["texte"]["y"]);
  if(clock.getElapsedTime().asMilliseconds() <= 800)
      window.draw(texte);

  if(clock.getElapsedTime().asMilliseconds() >= 1200)
      clock.restart();
}

// Actions du menu
void Menu_Start::action_menu(Menu & menu)
{
  if(Keyboard::isKeyPressed(Keyboard::Space))
  {
    sounds["confirm"].play();
    on = false;
    menu.on = true;
    menu.clock.restart();
  }
}









// Gestion de la pression des touches à la construction
void Menu_Selection::press()
{
  switch_select["press_E"] = switch_select["press_Tab"] = true;
  if(!Keyboard::isKeyPressed(Keyboard::E))
    switch_select["press_E"] = false;
  if(!Keyboard::isKeyPressed(Keyboard::Tab))
    switch_select["press_Tab"] = false;
}

// Constructeur
Menu_Selection::Menu_Selection(int w, int h, float s, string f, vector<tuple<string, string, int, int, int, int>> & v,
  vector<tuple<string, string, int, int, int>> & v_text, vector<string> & v_touch, vector<tuple<string,float>> & v_sound) :
  Menu(false, w, h, s, f, v, v_sound)
{
  // Gestion des switchs
  for(size_t i = 0; i < v_touch.size(); i++)
    switch_select[v_touch[i]] = false;
  press();
  // Gestion des textes
  for(size_t i = 0; i < v_text.size(); i++)
  {
    load_texte(font[get<1>(v_text[i])], texte[get<1>(v_text[i])], get<0>(v_text[i]), get<4>(v_text[i]), Color::White);
    FloatRect r = texte[get<1>(v_text[i])].getGlobalBounds();
    if(get<1>(v_text[i]) == "retour")
      pos[get<1>(v_text[i])] = {{"x", get<2>(v_text[i])*scale - r.width/2}, {"y", get<3>(v_text[i])*scale}};
    else
      pos[get<1>(v_text[i])] = {{"x", get<2>(v_text[i]) - r.width/2}, {"y", get<3>(v_text[i])}};
    texte[get<1>(v_text[i])].setPosition(pos[get<1>(v_text[i])]["x"], pos[get<1>(v_text[i])]["y"]);
  }
}









// Placement des personnages pour la selection
// On ne considère pas le cas où il y a un nombre paire de personnages par ligne
vector<int> Menu_Character_Map::placement_character(int nb_c, int sizeX, int sizeY)
{
  int size_select = nb_c*sizeX + nb_c*widthWindow/50;
  int nb_lines = 2*size_select/widthWindow + 1;
  int nb_columns = nb_c/nb_lines;
  int line = 0;
  int column = 0;
  for(map<string, Sprite>::iterator it = sprites_select.begin(); it != sprites_select.end(); it++)
  {
    int x;
    if(nb_columns%2 != 0)
      x = widthWindow/2 - (nb_columns/2)*sizeX -sizeX/2 - (nb_columns-1)*widthWindow/100 + column*(sizeX + widthWindow/50);
    else
      x = widthWindow/2 - (nb_columns/2)*sizeX- (nb_columns-1)*widthWindow/100 + column*(sizeX + widthWindow/50);
    pos[it->first] = {{"x", x}, {"y", heightWindow/2 - (nb_lines/2)*sizeY - (nb_lines-1)*widthWindow/100 + line*(sizeY + widthWindow/50)}};
    column++;
    if(column == nb_columns)
    {
      line++;
      column = 0;
    }
  }
  return {pos[sprites_select.begin()->first]["x"], pos[sprites_select.begin()->first]["y"],
    pos[(--sprites_select.end())->first]["x"], pos[(--sprites_select.end())->first]["y"]};
}

// Constructeur
Menu_Character_Map::Menu_Character_Map(int w, int h, float s, string f, vector<tuple<string, string, int, int, int, int>> & v,
  vector<string> & v_select, int sizeX_select, int sizeY_select,
  vector<tuple<string, string, int, int, int>> & v_text, int nb_curseur, int x_change, int y_change,
  int sizeX_change, int sizeY_change, vector<string> & v_touch, vector<tuple<string,float>> & v_sound) :
  Menu_Selection(w, h, s, f, v, v_text, v_touch, v_sound), is_clock_restart(false)
{
  // Gestion des éléments à sélectioné
  for(size_t i = 0; i < v_select.size(); i++)
  {
    load_texture(textures_select[v_select[i]], f + "select_" + v_select[i] + ".png");
    sprites_select[v_select[i]].setTexture(textures_select[v_select[i]]);
    sprites_select[v_select[i]].setScale(scale, scale);
  }
  size_select = {{"sizeX", sizeX_select*scale}, {"sizeY", sizeY_select*scale}};
  vector<int> place = placement_character(v_select.size(), size_select["sizeX"], size_select["sizeY"]);
  size_rect = {{"sizeX", size_select["sizeX"] + widthWindow/50}, {"sizeY", size_select["sizeY"] + widthWindow/50}};
  vector<Color> col = {Color::Yellow, Color::White};
  // Creation des curseurs et gestion des éléments dynamiques
  for(size_t i = 0; i < (size_t) nb_curseur; i++)
  {
    select["select" + to_string(i+1)] = creation_rectangle(place[2*i] - widthWindow/100, place[2*i + 1] - widthWindow/100,
      size_rect["sizeX"], size_rect["sizeY"], col[i]);
    if(i == 0)
      load_texture(textures["change" + to_string(i+1)], f + "fond_" + v_select[0] + ".png");
    else
      load_texture(textures["change" + to_string(i+1)], f + "fond_" + v_select[v_select.size()-1] + "_left.png");
    pos["change" + to_string(i+1)] = {{"x", (widthWindow - sizeX_change*scale)*i + x_change*pow(-1,i)}, {"y", y_change}};
    sprites["change" + to_string(i+1)].setTexture(textures["change" + to_string(i+1)]);
    sprites["change" + to_string(i+1)].setScale(scale, scale);
  }
}

// affichage du menu
void Menu_Character_Map::show_menu(RenderWindow & window)
{
  // Les sprites
  for(map<string, Sprite>::iterator it = sprites.begin(); it != sprites.end(); it++)
  {
    if(it->first != "gif" && it->first != "change1" && it->first != "change2" && it->first != "return")
    {
      it->second.setPosition(pos[it->first]["x"], pos[it->first]["y"]);
      window.draw(it->second);
    }
  }
  // l'élément dynamique 1
  sprites["change1"].setPosition(pos["change1"]["x"], pos["change1"]["y"]);
  window.draw(sprites["change1"]);
  // le sprite de retour
  sprites["return"].setPosition(pos["return"]["x"], pos["return"]["y"]);
  window.draw(sprites["return"]);
  if(select.size() > 1)
  {
    // l'élément dynamique 2
    sprites["change2"].setPosition(pos["change2"]["x"], pos["change2"]["y"]);
    window.draw(sprites["change2"]);
  }
  RectangleShape r = creation_rectangle(0, 11*heightWindow/12, widthWindow, heightWindow/15, Color::Black);
  window.draw(r);
  // Curseurs
  for(map<string, RectangleShape>::iterator it = select.begin(); it != select.end(); it++)
    window.draw(it->second);
  // Les éléments à sélectionner
  for(map<string, Sprite>::iterator it = sprites_select.begin(); it != sprites_select.end(); it++)
  {
    it->second.setPosition(pos[it->first]["x"], pos[it->first]["y"]);
    RectangleShape r = creation_rectangle(pos[it->first]["x"], pos[it->first]["y"],
      size_select["sizeX"], size_select["sizeY"], Color::Black);
    window.draw(r);
    window.draw(it->second);
  }
  // Les textes
  for(map<string, Text>::iterator it = texte.begin(); it != texte.end(); it++)
    window.draw(it->second);
}

// Déplacement des curseurs
void Menu_Character_Map::moove_cursor(Keyboard::Key & k, bool & switch_moove, RectangleShape & cursor, int new_x, int new_y)
{
  if(Keyboard::isKeyPressed(k) && switch_moove == false)
  {
    sounds["cursor"].play();
    cursor.setPosition(new_x, new_y);
    switch_moove = true;
  }
}

// Quand on relache les différents boutons
void Menu_Character_Map::release_button(map<string, Keyboard::Key> touche, int i)
{
  if(switch_select["select" + to_string(i)] == false)
  {
    if(!Keyboard::isKeyPressed(touche["Up"]) && switch_select["up" + to_string(i)] == true)
      switch_select["up" + to_string(i)] = false;
    if(!Keyboard::isKeyPressed(touche["Left"]) && switch_select["left" + to_string(i)] == true)
      switch_select["left" + to_string(i)] = false;
    if(!Keyboard::isKeyPressed(touche["Down"]) && switch_select["down" + to_string(i)] == true)
      switch_select["down" + to_string(i)] = false;
    if(!Keyboard::isKeyPressed(touche["Right"]) && switch_select["right" + to_string(i)] == true)
      switch_select["right" + to_string(i)] = false;
    if(!Keyboard::isKeyPressed(Keyboard::E) && switch_select["press_E"] == true)
      switch_select["press_E"] = false;
    if(!Keyboard::isKeyPressed(Keyboard::Tab) && switch_select["press_Tab"] == true)
      switch_select["press_Tab"] = false;
  }
}

// On fixe les limites des curseurs
void Menu_Character_Map::limit_cursor(RectangleShape & cursor)
{
  if(cursor.getPosition().x < pos[sprites_select.begin()->first]["x"] - widthWindow/100)
    cursor.setPosition(pos[(--sprites_select.end())->first]["x"] - widthWindow/100, cursor.getPosition().y);
  if(cursor.getPosition().x > pos[(--sprites_select.end())->first]["x"] - widthWindow/100)
    cursor.setPosition(pos[sprites_select.begin()->first]["x"] - widthWindow/100, cursor.getPosition().y);
  if(cursor.getPosition().y < pos[sprites_select.begin()->first]["y"] - widthWindow/100)
    cursor.setPosition(cursor.getPosition().x, pos[(--sprites_select.end())->first]["y"] - widthWindow/100);
  if(cursor.getPosition().y > pos[(--sprites_select.end())->first]["y"] - widthWindow/100)
    cursor.setPosition(cursor.getPosition().x, pos[sprites_select.begin()->first]["y"] - widthWindow/100);
}

// Trouver un éléments en fonction de la position du curseur
string Menu_Character_Map::find_select_by_pos(int x, int y)
{
  for(map<string, Sprite>::iterator it = sprites_select.begin(); it != sprites_select.end(); it++)
  {
    if(pos[it->first]["x"] == x && pos[it->first]["y"] == y)
      return it->first;
  }
  return "";
}

// affichage dynamique
void Menu_Character_Map::change_dynamic(RectangleShape & cursor, int i)
{
  if(i == 1)
    load_texture(textures["change" + to_string(i)], file + "fond_" + find_select_by_pos(cursor.getPosition().x + widthWindow/100,
      cursor.getPosition().y + widthWindow/100) + ".png");
  else
    load_texture(textures["change" + to_string(i)], file + "fond_" + find_select_by_pos(cursor.getPosition().x + widthWindow/100,
      cursor.getPosition().y + widthWindow/100) + "_left.png");
  sprites["change" + to_string(i)].setTexture(textures["change" + to_string(i)]);
}

// Sélection d'un élément
void Menu_Character_Map::selection(RectangleShape & cursor, Keyboard::Key & k, int i)
{
  if(Keyboard::isKeyPressed(k) && switch_select["select" + to_string(i)] == false && (k != Keyboard::E || switch_select["press_E"] == false))
  {
    sounds["confirm"].play();
    switch_select["select" + to_string(i)] = true;
    if(select.size() > 1)
      selected["select" + to_string(i)] = find_select_by_pos(cursor.getPosition().x + widthWindow/100,
        cursor.getPosition().y + widthWindow/100);
    else
      selected["select_map"] = find_select_by_pos(cursor.getPosition().x + widthWindow/100,
        cursor.getPosition().y + widthWindow/100);
    cursor.setFillColor(Color::Red);
  }
}

// Annuler la sélection
void Menu_Character_Map::unselect(RectangleShape & cursor, Keyboard::Key & k, int i)
{
  if(Keyboard::isKeyPressed(k) && switch_select["select" + to_string(i)] == true)
  {
    sounds["unselect"].play();
    switch_select["select" + to_string(i)] = false;
    if(i == 1)
      cursor.setFillColor(Color::Yellow);
    else
      cursor.setFillColor(Color::White);
  }
}

// reboot du menu avant de passer au suivant
void Menu_Character_Map::reboot_switch(Menu & menu)
{
  select["select1"].setPosition(pos[(sprites_select.begin())->first]["x"] - widthWindow/100, pos[(sprites_select.begin())->first]["y"] - widthWindow/100);
  switch_select["select1"] = false;
  select["select1"].setFillColor(Color::Yellow);
  if(select.size() > 1)
  {
    select["select2"].setPosition(pos[(--sprites_select.end())->first]["x"] - widthWindow/100, pos[(--sprites_select.end())->first]["y"] - widthWindow/100);
    switch_select["select2"] = false;
    select["select2"].setFillColor(Color::White);
  }
  on = false;
  menu.on = true;
  menu.load_menu();
  menu.clock.restart();
}

// retourner au menu précedant
void Menu_Character_Map::return_back(Menu & menu)
{
  if(Keyboard::isKeyPressed(Keyboard::Tab) && switch_select["press_Tab"] == false)
  {
    sounds["back"].play();
    switch_select["press_Tab"] = true;
    reboot_switch(menu);
  }
}

// aller au menu suivant
void Menu_Character_Map::switch_next(Menu_Character_Map & menu)
{
  if(switch_select["select1"] && switch_select["select2"] && switch_select["press_E"] == false)
  {
    switch_select["press_E"] = true;
    menu.selected = selected;
    reboot_switch(menu);
  }
}

// load du menu avant l'affichage
void Menu_Character_Map::load_menu()
{
  is_clock_restart = false;
  if(Keyboard::isKeyPressed(Keyboard::E))
    switch_select["press_E"] = true;
  if(Keyboard::isKeyPressed(Keyboard::Tab))
    switch_select["press_Tab"] = true;
}

// Actions du menu
void Menu_Character_Map::action_menu(Menu & previous_menu, Menu_Character_Map & next_menu)
{
  vector<map<string, Keyboard::Key>> touche = {{{"Up",Keyboard::Z}, {"Down",Keyboard::S}, {"Left", Keyboard::Q},
    {"Right", Keyboard::D}, {"Select",Keyboard::E}, {"UnSelect",Keyboard::R}}, {{"Up",Keyboard::Up}, {"Down",Keyboard::Down},
    {"Left", Keyboard::Left}, {"Right", Keyboard::Right}, {"Select",Keyboard::K}, {"UnSelect",Keyboard::L}}};
  for(size_t i = 0; i < select.size(); i++)
  {
    RectangleShape & cursor = select["select" + to_string(i+1)];
    if(switch_select["select" + to_string(i+1)] == false)
    {
      moove_cursor(touche[i]["Up"], switch_select["up" + to_string(i+1)], cursor,
        cursor.getPosition().x, cursor.getPosition().y - size_rect["sizeY"]);
      moove_cursor(touche[i]["Left"], switch_select["left" + to_string(i+1)], cursor,
        cursor.getPosition().x - size_rect["sizeX"], cursor.getPosition().y);
      moove_cursor(touche[i]["Down"], switch_select["down" + to_string(i+1)], cursor,
        cursor.getPosition().x, cursor.getPosition().y + size_rect["sizeY"]);
      moove_cursor(touche[i]["Right"], switch_select["right" + to_string(i+1)], cursor,
        cursor.getPosition().x + size_rect["sizeX"], cursor.getPosition().y);
      limit_cursor(cursor);
      change_dynamic(cursor, i+1);
      release_button(touche[i], i+1);
      selection(cursor, touche[i]["Select"], i+1);
      if(select.size() == 1)
      {
        unselect(cursor, touche[i]["UnSelect"], i+1);
        return_back(previous_menu);
      }
    }
    if(select.size() > 1)
    {
      unselect(cursor, touche[i]["UnSelect"], i+1);
      return_back(previous_menu);
    }
  }
  if(select.size() > 1)
    switch_next(next_menu);
}

// Génération du combat et affichage du gif VS
Combat* Menu_Character_Map::fight(RenderWindow & window, Music & music)
{
  if(switch_select["select1"])
  {
    if(!is_clock_restart)
    {
      is_clock_restart = true;
      for(size_t i = 0; i < 6; i++)
        load_texture(textures["gif"+to_string(i)], file + "gif_" + selected["select1"] + "_" + selected["select2"] + "/image" + to_string(i) + ".png");
      music.stop();
      sounds["vs"].play();
      clock.restart();
    }
    if(clock.getElapsedTime().asMilliseconds() < 900)
      gif(window, textures["gif" + to_string((clock.getElapsedTime().asMilliseconds()/150)%6)],
        sprites["gif" + to_string((clock.getElapsedTime().asMilliseconds()/150)%6)], widthWindow/2, heightWindow/2, 3*scale);
    else
    {
      is_clock_restart = false;
      select["select1"].setPosition(pos[(sprites_select.begin())->first]["x"] - widthWindow/100, pos[(sprites_select.begin())->first]["y"] - widthWindow/100);
      switch_select["select1"] = false;
      select["select1"].setFillColor(Color::Yellow);
      on = false;
      clock.restart();
      Combat* combat = new Combat(widthWindow/1.9, widthWindow, heightWindow, scale, file + "../combat/", selected["select1"], selected["select2"], selected["select_map"]);
      return combat;
    }
  }
  return nullptr;
}









// placement des éléments
void Menu_Pause::placement_texte(int nb_p, int sizeX, int sizeY, int posX, int posY)
{
  int line = 0;
  for(map<string, Text>::iterator it = texte.begin(); it != texte.end(); it++)
  {
    int x = posX - it->second.getGlobalBounds().width/2;
    int y = posY - nb_p*sizeY/2 - (nb_p-1)*widthWindow/100 + line*(sizeY + widthWindow/50) + (it->second.getPosition().y - it->second.getGlobalBounds().top);;
    it->second.setPosition(x, y + (sizeY - it->second.getGlobalBounds().height)/2);
    pos[it->first]["x"] = x;
    pos[it->first]["y"] = it->second.getGlobalBounds().top - (sizeY - it->second.getGlobalBounds().height)/2;
    line++;
  }
}

// Constructeur
Menu_Pause::Menu_Pause(int w, int h, float s, string f, bool back_ground, int pos_selX, int pos_selY, vector<tuple<string, string, int, int, int, int>> & v,
  vector<tuple<string, string, int, int, int>> & v_text, vector<string> & v_touch, vector<tuple<string,float>> & v_sound) :
  Menu_Selection(w, h, s, f, v, v_text, v_touch, v_sound)
{
  int max_width = texte.begin()->second.getGlobalBounds().width;
  int max_height = texte.begin()->second.getGlobalBounds().height;
  // Gestion des textes
  for(map<string, Text>::iterator it = texte.begin()++; it != texte.end(); it++)
  {
    if(it->second.getGlobalBounds().width > max_width)
      max_width = it->second.getGlobalBounds().width;
    else if(it->second.getGlobalBounds().height > max_height)
      max_height = it->second.getGlobalBounds().height;
  }
  placement_texte(texte.size(), max_width, max_height, pos_selX, pos_selY);
  int sizeX = widthWindow;
  if(!back_ground)
    sizeX = max_width + widthWindow/25;
  int sizeY = (max_height + widthWindow/50)*texte.size() + widthWindow/25;
  // fond du menu
  fond = creation_rectangle(pos_selX - sizeX/2, pos_selY - (sizeY)/2,
    sizeX, sizeY, Color::Black);
  size_rect = {{"sizeX", max_width + widthWindow/50}, {"sizeY", max_height + widthWindow/50}};
  // curseur du menu
  cursor = creation_rectangle(widthWindow/2 - max_width/2 - widthWindow/100, pos[texte.begin()->first]["y"] - widthWindow/100,
    size_rect["sizeX"], size_rect["sizeY"], Color::Yellow);
}

// Affichage
void Menu_Pause::show_menu(RenderWindow & window)
{
  for(map<string, Sprite>::iterator it = sprites.begin(); it != sprites.end(); it++)
  {
      it->second.setPosition(pos[it->first]["x"], pos[it->first]["y"]);
      window.draw(it->second);
  }
  window.draw(fond);
  window.draw(cursor);
  for(map<string, Text>::iterator it = texte.begin(); it != texte.end(); it++)
    window.draw(it->second);
}

// déplacement des curseurs
void Menu_Pause::moove_cursor(Keyboard::Key & k, bool & switch_moove, int new_x, int new_y)
{
  if(Keyboard::isKeyPressed(k) && switch_moove == false)
  {
    sounds["cursor"].play();
    cursor.setPosition(new_x, new_y);
    switch_moove = true;
  }
}

// Actions à faire lorsque les boutons sont relachés
void Menu_Pause::release_button(map<string, Keyboard::Key> touche)
{
  if(switch_select["select"] == false)
  {
    if(!Keyboard::isKeyPressed(touche["Up"]) && switch_select["up"] == true)
      switch_select["up"] = false;
    if(!Keyboard::isKeyPressed(touche["Down"]) && switch_select["down"] == true)
      switch_select["down"] = false;
    if(!Keyboard::isKeyPressed(Keyboard::E) && switch_select["press_E"] == true)
      switch_select["press_E"] = false;
  }
}

// Limites du curseur
void Menu_Pause::limit_cursor()
{
  if(cursor.getPosition().y < pos[texte.begin()->first]["y"] - widthWindow/100)
    cursor.setPosition(cursor.getPosition().x, pos[(--texte.end())->first]["y"] - widthWindow/100);
  if(cursor.getPosition().y > pos[(--texte.end())->first]["y"] - widthWindow/100)
    cursor.setPosition(cursor.getPosition().x, pos[texte.begin()->first]["y"] - widthWindow/100);
}

// trouver un élement depuis la position du curseur
string Menu_Pause::find_select_by_pos(int x, int y)
{
  for(map<string, Text>::iterator it = texte.begin(); it != texte.end(); it++)
  {
    if(pos[it->first]["y"] == y)
      return it->first;
  }
  return "";
}

// changement dynamique du menu
void Menu_Pause::change_dynamic()
{
  string change = find_select_by_pos(cursor.getPosition().x + widthWindow/100, cursor.getPosition().y + widthWindow/100);
  for(map<string, Text>::iterator it = texte.begin(); it != texte.end(); it++)
  {
    if(it->first == change)
      texte[change].setFillColor(Color::Black);
    else
      texte[it->first].setFillColor(Color::White);
  }
}

// Selection
void Menu_Pause::selection()
{
  if(Keyboard::isKeyPressed(Keyboard::E) && switch_select["select"] == false && switch_select["press_E"] == false)
  {
    sounds["confirm"].play();
    switch_select["select"] = true;
    selected["select"] = find_select_by_pos(cursor.getPosition().x + widthWindow/100,
      cursor.getPosition().y + widthWindow/100);
  }
}

// Reboot avant le switch
void Menu_Pause::reboot_switch(Menu & menu)
{
  on = false;
  menu.on = true;
  menu.load_menu();
  menu.clock.restart();
}

// Passer au menu/combat suivant
void Menu_Pause::switch_next(tuple<Menu_Start*, Menu_Character_Map*, Menu_Character_Map*> menu, Combat** combat, Music & music)
{
  if(switch_select["select"] && switch_select["press_E"] == false)
  {
    switch_select["press_E"] = true;
    switch_select["select"] = false;
    sounds["pause"].play();
    if(selected["select"] == "resume")
    {
      (*combat)->pause = false;
      (*combat)->press_key1["press_cac"] = true;
    }
    else
    {
      if(selected["select"] == "restart")
      {
        get<2>(menu)->switch_select["select1"] = true;
        reboot_switch(*get<2>(menu));
      }
      else
      {
        if(selected["select"] == "select_charac")
          reboot_switch(*get<1>(menu));
        else if(selected["select"] == "select_map")
          reboot_switch(*get<2>(menu));
        else if(selected["select"] == "presentation_menu")
          reboot_switch(*get<0>(menu));
        else if(selected["select"] == "commands")
        {
          //reboot_switch(menu);
        }
      }
      (*combat)->music_fight.stop();
      music.play();
      (*combat)->~Combat();
      (*combat) = nullptr;
    }
  }
}

// Actions du menu
void Menu_Pause::action_menu(tuple<Menu_Start*, Menu_Character_Map*, Menu_Character_Map*> next_menu, Combat** combat, Music & music)
{
  map<string, Keyboard::Key> touche = {{"Up", Keyboard::Z}, {"Down", Keyboard::S},  {"Select", Keyboard::E}};
  if(switch_select["select"] == false)
  {
    moove_cursor(touche["Up"], switch_select["up"], cursor.getPosition().x, cursor.getPosition().y - size_rect["sizeY"]);
    moove_cursor(touche["Down"], switch_select["down"], cursor.getPosition().x, cursor.getPosition().y + size_rect["sizeY"]);
    limit_cursor();
    change_dynamic();
  }
  release_button(touche);
  selection();
  switch_next(next_menu, combat, music);
}

// load du menu avant affichage
void Menu_Pause::load_menu()
{
  if(Keyboard::isKeyPressed(Keyboard::E))
    switch_select["press_E"] = true;
  cursor.setPosition(cursor.getPosition().x, pos[texte.begin()->first]["y"] - widthWindow/100);
  sounds["pause"].play();
}
