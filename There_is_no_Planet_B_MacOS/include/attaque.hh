#include "jauge.hh"

enum Direction {LEFT,RIGHT};

class Personnage;

// Retourne si il y a collision entre deux entités
template<typename T, typename U>
bool collision_A_B(T & obj1, U & obj2)
{
  int xA = obj1.pos["x"] + 2*obj1.info_sprite["sizeX"]*obj1.scale/7;
  int yA = obj1.pos["y"] + obj1.info_sprite["sizeY"]*obj1.scale/8;
  int sizeXA = 3*obj1.info_sprite["sizeX"]*obj1.scale/7;
  int sizeYA = 3*obj1.info_sprite["sizeY"]*obj1.scale/4;

  int xB = obj2.pos["x"] + 2*obj2.info_sprite["sizeX"]*obj2.scale/7;
  int yB = obj2.pos["y"] + obj2.info_sprite["sizeY"]*obj2.scale/8;
  int sizeXB = 3*obj2.info_sprite["sizeX"]*obj2.scale/7;
  int sizeYB = 3*obj2.info_sprite["sizeY"]*obj2.scale/4;

  return !(xA + sizeXA < xB || xB + sizeXB < xA || yA + sizeYA < yB || yB + sizeYB < yA);
}

class Attaque
{
  public :
    float scale; // Pour l'affichage
    int degat; // degat de l'attaque

    Attaque(float sc, int d) : scale(sc), degat(d){};
    virtual ~Attaque() {};

    virtual void gestion_collision(Personnage & perso, Personnage & adversaire);
};

class Attaque_special : public Attaque
{
  public :
    float speed; // vitesse des projectils
    Texture & texture; // l'image png
    Texture & explosion; // explosion de l'attaque
    Sprite sprite; // sprite du projectils
    map<string, int> info_sprite; // Information sur le sprite
    map<string, int> pos; // coordonnees
    Direction direction; // direction
    map<string, tuple<int, int>> anim; // animation de l'attaque
    bool suppr; // Pour savoir si il faut la supprimer

    Attaque_special(float sc, int d, float s, Texture & t, Texture & exp, int x, int y, int sizeX, int sizeY,
      Direction dir, int anim_max,/* int anim_exp_max,*/ int a/*, int a_exp*/);

    void animate(tuple<int, int> & anim, const string info);
    void limitX(const int & widthWindow);
    void mouvement(const int & widthWindow);
    void anim_explosion();
    void gestion_collision(Personnage & perso, Personnage & adversaire);
};

class Attaque_ultime : public Attaque_special
{
  public:
    Attaque_ultime(float sc, int d, float s, Texture & t, Texture & exp, int x, int y, int sizeX, int sizeY,
      Direction dir, int anim_max,/* int anim_exp_max,*/ int a/*, int a_exp*/) :
      Attaque_special(sc, d, s, t, exp, x, y, sizeX, sizeY, dir, anim_max,/* anim_exp_max,*/ a/*, a_exp*/) {};
    virtual ~Attaque_ultime() {};

    void gestion_collision(Personnage & perso, Personnage & adversaire);
};
