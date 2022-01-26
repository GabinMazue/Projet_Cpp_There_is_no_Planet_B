#include "personnage.hh"

class Combat;

// génération d'un rectangle
RectangleShape creation_rectangle(int x, int y, int sizeX, int sizeY, Color couleur);

// load d'un texte
void load_texte(Font & f, Text & t, string & t_a, int char_size, Color col);

// affichage d'un gif
void gif(RenderWindow & window, Texture & texture, Sprite & sprite, int x, int y, float scale);

class Menu
{
  public :
    static int compt; // compteur
    int id; // id
    bool on; // si le menu est actif
    int widthWindow; // taille de la fenetre
    int heightWindow;
    float scale; // pour l'affichage
    string file; // repertory pour les éléments graphiques
    map<string, Texture> textures; // textures du menu
    map<string, Sprite> sprites; // sprites du menu
    map<string, map<string, int>> pos; // positions des éléments
    map<string, map<string, int>> info_sprites; // Information sur les tailes,...
    Clock clock; // Horloge
    map<string, SoundBuffer> sound_buffers; // buffers des sons
    map<string, Sound> sounds; // sons

    Menu(bool o, int w, int h, float s, string f, vector<tuple<string, string, int, int, int, int>> & v,
      vector<tuple<string,float>> & v_sound);
    virtual ~Menu() {};

    virtual void show_menu(RenderWindow & window) = 0;
    virtual void load_menu() = 0;
};

class Menu_Start : public Menu
{
  public :
    Font font; // Police des textes
    Text texte;  // texte du menu

    Menu_Start(string t_a, int x_t, int y_t, int w, int h, float s,
      string f, vector<tuple<string, string, int, int, int, int>> & v, vector<tuple<string,float>> & v_sound);

    void show_menu(RenderWindow & window);
    void load_menu() {};
    void action_menu(Menu & menu);
};

class Menu_Selection : public Menu
{
  public :
    map<string, Font> font; // police des textes
    map<string, Text> texte; // textes des menus
    map<string, bool> switch_select; // booléen utile pour la selection et les mouvements des curseurs
    map<string, string> selected; // éléments sélectionnés dans le menu

    void press();

    Menu_Selection(int w, int h, float s, string f, vector<tuple<string, string, int, int, int, int>> & v,
      vector<tuple<string, string, int, int, int>> & v_text, vector<string> & v_touch, vector<tuple<string,float>> & v_sound);

    virtual string find_select_by_pos(int x, int y) = 0;
    virtual void reboot_switch(Menu & menu) = 0;
};

class Menu_Character_Map : public Menu_Selection
{
  public :
    map<string, Texture> textures_select; // texture des éléments sélectionnés
    map<string, Sprite> sprites_select; // sprite des éléments sélectionnés
    map<string, int> size_select; // Tailles des éléments sélectionnés
    map<string, RectangleShape> select; // curseurs
    map<string, int> size_rect; // tailles curseurs
    bool is_clock_restart; // Si la clock est restart

    vector<int> placement_character(int nb_c, int sizeX, int sizeY);

    Menu_Character_Map(int w, int h, float s, string f, vector<tuple<string, string, int, int, int, int>> & v,
      vector<string> & v_select, int sizeX_select, int sizeY_select,
      vector<tuple<string, string, int, int, int>> & v_text, int nb_curseur, int x_change, int y_change,
      int sizeX_change, int sizeY_change, vector<string> & v_touch, vector<tuple<string,float>> & v_sound);

    void show_menu(RenderWindow & window);
    void moove_cursor(Keyboard::Key & k, bool & switch_moove, RectangleShape & cursor, int new_x, int new_y);
    void release_button(map<string, Keyboard::Key> touche, int i);
    void limit_cursor(RectangleShape & cursor);
    string find_select_by_pos(int x, int y);
    void change_dynamic(RectangleShape & cursor, int i);
    void selection(RectangleShape & cursor, Keyboard::Key & k, int i);
    void unselect(RectangleShape & cursor, Keyboard::Key & k, int i);
    void reboot_switch(Menu & menu);
    void return_back(Menu & menu);
    void switch_next(Menu_Character_Map & menu);
    void load_menu();
    void action_menu(Menu & previous_menu, Menu_Character_Map & next_menu);
    Combat* fight(RenderWindow & window, Music & music);
};

class Menu_Pause : public Menu_Selection
{
  public :
    RectangleShape cursor; // curseur
    RectangleShape fond; // fond du menu
    map<string, int> size_rect; // tailles du curseur

    void placement_texte(int nb_p, int sizeX, int sizeY, int posX, int posY);

    Menu_Pause(int w, int h, float s, string f, bool back_ground, int pos_selX, int pos_selY, vector<tuple<string, string, int, int, int, int>> & v,
      vector<tuple<string, string, int, int, int>> & v_text, vector<string> & v_touch, vector<tuple<string,float>> & v_sound);
    ~Menu_Pause() {};

    void show_menu(RenderWindow & window);
    void moove_cursor(Keyboard::Key & k, bool & switch_moove, int new_x, int new_y);
    void release_button(map<string, Keyboard::Key> touche);
    void limit_cursor();
    string find_select_by_pos(int x, int y);
    void change_dynamic();
    void selection();
    void reboot_switch(Menu & menu);
    void switch_next(tuple<Menu_Start*, Menu_Character_Map*, Menu_Character_Map*> menu, Combat** combat, Music & music);
    void action_menu(tuple<Menu_Start*, Menu_Character_Map*, Menu_Character_Map*> next_menu, Combat** combat, Music & music);
    void load_menu();
};
