#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <map>
#include <vector>
#include <math.h>
#include <typeinfo>

using namespace sf;
using namespace std;

//load d'un son dans la map de buffer et la map de son
void load_sound(map<string,SoundBuffer> & sound_buffers, map<string,Sound> & sounds, string path, tuple<string, float> sound);

class Jauge
{
  public :
    float scale; // scale pour l'affichage
    map<string, int> valeur; // différentes valeurs des jauges
    RectangleShape jauge; // contoure de la jauge
    int contour_size; // taille du contoure
    RectangleShape remplissage; // jauge
    map<string, int> pos; // position des éléments graphiques
    map<string, int> dim; // dimensions actuelles
    map<string, int> dim_full; // dimensions quand les jauges sont pleines
    bool up_left; // true si la jauge est à gauche

    Jauge(float s, int val, int c, int x, int y, int sizeX, int sizeY, Color col, bool u_l);

    virtual void jauge_baisse(int val) = 0;
    virtual void jauge_augmente(int val) = 0;
    void operator-=(int val);
    void operator+=(int val);
};

class Jauge_Vie : public Jauge
{
  public :
    Texture texture; // texture du portrait du joueur
    Sprite sprite; // sprite du portrait du joueur
    map<string, int> dim_texture; // dimensions de la texture

    Jauge_Vie(float s, string t, int val, int c, int x, int y, int sizeX, int sizeY,
      int size_textX, int size_textY, int widthWindow, bool u_l);

    void jauge_baisse(int val);
    void jauge_augmente(int val);
};

class Jauge_Mana : public Jauge
{
  public :
    SoundBuffer jauge_mana_max_buffer; // buffer du son
    Sound jauge_mana_max; // son quand la jauge mana est pleine

    Jauge_Mana(float s, int val, int c, int x, int y, int sizeX, int sizeY, bool u_l, string sound_path);

    void jauge_baisse(int val);
    void jauge_augmente(int val);
};
