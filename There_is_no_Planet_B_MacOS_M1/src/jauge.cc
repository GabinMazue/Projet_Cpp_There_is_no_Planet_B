#include "../include/jauge.hh"

// load d'un son dans la map de buffer et la map de son
void load_sound(map<string,SoundBuffer> & sound_buffers, map<string,Sound> & sounds, string path, tuple<string, float> sound)
{
  SoundBuffer b;
  if(!b.loadFromFile(path + get<0>(sound) + ".wav"))
    cout << "Erreur chargement de " << get<0>(sound) << ".wav" << endl;
  Sound s;
  sound_buffers[get<0>(sound)] = b;
  sounds[get<0>(sound)] = s  ;
  sounds[get<0>(sound)].setBuffer(sound_buffers[get<0>(sound)]);
  sounds[get<0>(sound)].setVolume(get<1>(sound));
}

// Constructeur
Jauge::Jauge(float s, int val, int c, int x, int y, int sizeX, int sizeY, Color col, bool u_l) : scale(s), up_left(u_l)
{
  valeur = {{"val", val}, {"val_reach", val}, {"val_max", 100}};
  jauge.setPosition(x, y);
  jauge.setSize(Vector2f(sizeX, sizeY));
  jauge.setFillColor(Color::Black);
  contour_size = c;
  pos = {{"x", x}, {"y",y}}; // coordonnees
  dim = {{"x", (sizeX -2 * c)*(1. - (float) (valeur["val_max"]-valeur["val"])/valeur["val_max"])}, {"y",sizeY - 2 * c}}; // dimension de la barre
  dim_full = {{"x", sizeX -2 * c}, {"y",sizeY - 2 * c}}; // dimension de la barre
  if(up_left == false)
    remplissage.setPosition(pos["x"] + dim_full["x"] - dim["x"] + c, pos["y"] + c);
  else
    remplissage.setPosition(x + c, y + c);
  remplissage.setSize(Vector2f((sizeX -2 * c)*(val/100.), (sizeY - 2 * c)));
  remplissage.setFillColor(col);
}

// Diminuer la valeur de la jauge
void Jauge::operator-=(int val)
{
  if(valeur["val_reach"] >= val)
    valeur["val_reach"] -= val;
  else
    valeur["val_reach"] = 0;
}

// Augmenter la valeur de la jauge
void Jauge::operator+=(int val)
{
  if(valeur["val_max"] - valeur["val_reach"] >= val)
    valeur["val_reach"] += val;
  else
    valeur["val_reach"] = valeur["val_max"];
}

// Constructeur
Jauge_Vie::Jauge_Vie(float s, string t, int val, int c, int x, int y, int sizeX, int sizeY,
  int size_textX, int size_textY, int widthWindow, bool u_l) :
Jauge(s, val, c, x, y, sizeX, sizeY, Color(57,220,20,255), u_l)
{
  if (!texture.loadFromFile(t)) // load texture from file
    cout << "Erreur de chargement du personnage\n"; // message d'erreur
  texture.setSmooth(true); // smooth
  sprite.setTexture(texture);
  if(up_left == true)
    sprite.setPosition(x - size_textX*scale, y - 100*scale + widthWindow/60);
  else
    sprite.setPosition(x + widthWindow/2.5 - size_textX*scale, y - 100*scale + widthWindow/60);
  sprite.setScale(scale, scale); // Ajustement de la taille
}

// Constructeur
Jauge_Mana::Jauge_Mana(float s, int val, int c, int x, int y, int sizeX, int sizeY, bool u_l, string sound_path) :
  Jauge(s, val, c, x, y, sizeX, sizeY, Color(0, 191, 255, 255), u_l)
{
  if(!jauge_mana_max_buffer.loadFromFile(sound_path + "jauge_mana_max.wav"))
    cout << "Erreur de chargement" << endl;
  jauge_mana_max.setBuffer(jauge_mana_max_buffer);
  jauge_mana_max.setVolume(5.0);
}


// Fonction qui fait baisser la vie dans l'interphace graphique, en fonction de la valeur actuelle et de la valeur à atteindre
void Jauge_Vie::jauge_baisse(int val)
{
  if(abs(valeur["val"] - valeur["val_reach"]) < val)
    valeur["val"] = valeur["val_reach"];
  else if(valeur["val"] >= val)
    valeur["val"] -= val;
  else
    valeur["val"] = 0;
  dim["x"] = dim_full["x"]*(1. - (float) (valeur["val_max"]-valeur["val"])/valeur["val_max"]);
  remplissage.setSize(Vector2f(dim["x"], dim["y"]));
  if(up_left == false)
    remplissage.setPosition(pos["x"] + dim_full["x"] - dim["x"] + contour_size, pos["y"] + contour_size);
  if(valeur["val"] <= 50 && valeur["val"] > 20)
    remplissage.setFillColor(Color(255,165,0,255));
  else if(valeur["val"] <= 20)
    remplissage.setFillColor(Color(240,0,32,255));
}

// Fonction qui fait monter la vie dans l'interphace graphique, en fonction de la valeur actuelle et de la valeur à atteindre
void Jauge_Vie::jauge_augmente(int val)
{
  if(abs(valeur["val"] - valeur["val_reach"]) < val)
    valeur["val"] = valeur["val_reach"];
  else if(valeur["val_max"] - valeur["val"] >= val)
    valeur["val"] += val;
  else
    valeur["val"] = valeur["val_max"];
  dim["x"] = dim_full["x"]*(1. - (float) (valeur["val_max"]-valeur["val"])/valeur["val_max"]);
  remplissage.setSize(Vector2f(dim["x"], dim["y"]));
  if(up_left == false)
    remplissage.setPosition(pos["x"] + dim_full["x"] - dim["x"] + contour_size, pos["y"] + contour_size);
  if(valeur["val"] >= 50)
    remplissage.setFillColor(Color(57,220,20,255));
  else if(valeur["val"] >= 20 && valeur["val"] < 50)
    remplissage.setFillColor(Color(255,165,0,255));
}

// Fonction qui fait baisser le mana dans l'interphace graphique, en fonction de la valeur actuelle et de la valeur à atteindre
void Jauge_Mana::jauge_baisse(int val)
{
  if(abs(valeur["val"] - valeur["val_reach"]) < val)
    valeur["val"] = valeur["val_reach"];
  else if(valeur["val"] >= val)
    valeur["val"] -= val;
  else
    valeur["val"] = 0;
  dim["x"] = dim_full["x"]*(1. - (float) (valeur["val_max"]-valeur["val"])/valeur["val_max"]);
  remplissage.setSize(Vector2f(dim["x"], dim["y"]));
  if(up_left == false)
    remplissage.setPosition(pos["x"] + dim_full["x"] - dim["x"] + contour_size, pos["y"] + contour_size);
  if(valeur["val"] < 100)
    jauge.setFillColor(Color::Black);
}

// Fonction qui fait augmenter le mana dans l'interphace graphique, en fonction de la valeur actuelle et de la valeur à atteindre
void Jauge_Mana::jauge_augmente(int val)
{
  if(abs(valeur["val"] - valeur["val_reach"]) < val)
    valeur["val"] = valeur["val_reach"];
  else if(valeur["val_max"] - valeur["val"] >= val)
    valeur["val"] += val;
  else
    valeur["val"] = valeur["val_max"];
  dim["x"] = dim_full["x"]*(1. - (float) (valeur["val_max"]-valeur["val"])/valeur["val_max"]);
  remplissage.setSize(Vector2f(dim["x"], dim["y"]));
  if(up_left == false)
    remplissage.setPosition(pos["x"] + dim_full["x"] - dim["x"] + contour_size, pos["y"] + contour_size);
  if(valeur["val"] == 100)
    jauge.setFillColor(Color(255,215,0,255));
}
