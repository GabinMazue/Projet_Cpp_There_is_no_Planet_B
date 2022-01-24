#include "attaque.hh"

void load_texture(Texture & t, const string s);

class Personnage
{
  public :
    string nom; // son nom
    float scale; // scale pour l'affichage
    Texture texture; // l'image png
    Sprite sprite; // le sprite
    map<string, int> info_sprite; // Information sur le sprite
    map<string,tuple<int, int>> animation; // animation de déplacement et combat
    map<string, int> pos; // coordonnees
    map<string, float> velocity; // vitesse variable
    map<string, float> speed; // vitesse de propre au personnage
    float gravity; // vitesse de chute
    Direction direction; // direction
    map<string, Keyboard::Key> & touche; // touche pour jouer
    Jauge_Vie & jauge_vie; // jauge de vie
    Jauge_Mana & jauge_mana; //jauge de mana
    Attaque & cac; // attaque corps a corps
    vector<Attaque_special*> att_spe;
    Texture texture_att_spe; // l'image png
    Texture explosion_att_spe; // explosion de l'attaque speciale
    Attaque_ultime* att_ult; // Attaque ultime
    Texture texture_att_ult; // l'image png
    Texture explosion_att_ult; // explosion de l'attaque speciale
    bool is_attacking; // Si le personnage attaque
    bool is_hurt; // Si il est touché
    string path; // path vers son fichier où trouver les éléments graphiques
    int punch; // compteur pour savoir quel son de punch faire
    map<string,SoundBuffer> sound_buffers; // les buffer des sons
    map<string,Sound> sounds; // les sons


    Personnage(string n, float s, string t, int sizeX, int sizeY, int anim_left, int anim_right, int anim_up_left, int anim_up_right,
      int anim_fight_left, int anim_fight_right, int anim_death, int nb_a_w, int a_j, int nb_a_c, int nb_a_d, int x, int y, float spX, float spY,
      float g, Direction d, map<string, Keyboard::Key> & k, Jauge_Vie & j_v, Jauge_Mana & j_m,
      Attaque & cc, string t_att_spe, string t_exp_att_spe, string t_att_ult, string t_exp_att_ult, string p);
    ~Personnage();

    void animate_jump();
    void animate(tuple<int, int> & anim, const string info);
    void animate_death();
    void limitX(const int & widthWindow);
    void limitY(const int & ground);
    void mouvement(const int & ground, const int & widthWindow);
    void up(const int & ground);
    void left(const int & ground);
    void right(const int & ground);
    void nothing(const int & ground);
    void corps_a_corps();
    void special();
    void suppr_spe();
    void ultime();
    void suppr_ult();
    void no_fight();
    void hurt();
    void dead(const int & ground, const int & widthWindow);
};
