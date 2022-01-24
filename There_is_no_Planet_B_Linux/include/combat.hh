#include "menu.hh"

Personnage csv_to_perso(string path, string name_perso, int x, int ground, Direction d,
  map<string, Keyboard::Key> & touche, Jauge_Vie & j_v, Jauge_Mana & j_m, Attaque & cac, float scale);

class Combat
{
  public :
    bool fight; // Si les personnages se battent
    bool pause; // Si le jeu est en pause
    bool end; // Fin du combat
    string file; // path vers les fichiers des ressources graphiques
    int ground; // le sol
    int widthWindow; // taille de la fenetre
    int heightWindow;
    float scale; // pour l'affichage
    bool switch_pause; // Pour tester si le bouton à été relaché
    Music music_fight; // Musique du combat
    Music music_winner; // Musique du vainqueur
    map<string,SoundBuffer> sound_buffers; // les buffers des sons
    map<string,Sound> sounds; // les sons

    Jauge_Vie jauge_vie1; // Jauve vie du perso1
    Jauge_Mana jauge_mana1; // Jauge mana du perso1
    map<string, Keyboard::Key> touche1; // Touches du perso1
    map<string, bool> press_key1; // Voir si les touches sont enfoncées
    bool phrase1; // phrase finale du perso1

    Jauge_Vie jauge_vie2; // Jauge vie du perso2
    Jauge_Mana jauge_mana2; // Jauge mana du perso2
    map<string, Keyboard::Key> touche2; // Touches du perso2
    map<string, bool> press_key2; // Voir si les touches sont enfoncées
    bool phrase2; // phrase finale du perso2

    Attaque attaque; // Attaque corps à corps (la même pour les deux persos)
    Personnage perso1; // perso1
    Personnage perso2; // perso2
    map<string, Texture> textures; // textures du combat
    map<string, Sprite> sprites; // sprites du combat
    Clock clock; // Horloge (pour le gif)
    bool is_clock_restart; // Si la clock est restart
    bool music_winner_on; // Si la music du vainqueur est on

    Combat(int g, int width, int height, float s, string path, string p1, string p2, string m);
    ~Combat() {};

    void mouvement_perso(Personnage & perso);
    void attaque_corps_a_corps(Personnage & perso, Personnage & adversaire, bool & press_cac);
    void attaque_special(Personnage &perso, Personnage & adversaire, RenderWindow & window, bool & press_spe);
    void attaque_ultime(Personnage &perso, Personnage & adversaire, RenderWindow & window, bool & press_spe);
    void change_jauge(Jauge & jauge, int val);
    Menu_Pause* dead(Personnage & perso, Personnage & adversaire, Jauge_Vie & jauge_vie);
    void pause_jeu(Menu_Pause & menu_pause);
    void draw(Personnage & perso, Jauge_Mana & jauge_mana, Jauge_Vie & jauge_vie, RenderWindow & window);
};
