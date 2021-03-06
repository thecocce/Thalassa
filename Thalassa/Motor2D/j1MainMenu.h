#ifndef __j1MAINMENU_H__
#define __j1MAINMENU_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2Defs.h"
#include "p2List.h"
#include "p2Animation.h"

struct SDL_Texture;
struct SDL_Rect;
class j1Button;
class j1Label;
class j1Box;
struct _TTF_Font;

enum SCENE_TYPE
{
	MENU = 0, 
	SCENE, 
	CREDITS
};

struct Button_rects
{
	int x, y, w, h;
};

class j1MainMenu : public j1Module
{
public:

	j1MainMenu();
	virtual ~j1MainMenu();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void ChangeScene(SCENE_TYPE next);

	bool startScene1 = false;
	bool startGame = false;
	bool startCredits = false;

	p2List<j1Button*> buttons_menu;
	p2List<j1Label*> labels_menu;
	p2List<j1Box*> boxes_menu;

	void LoadConfig();
	void LoadScene1(bool save_game = false);

	bool run_game = true;

public:

	j1Box* settings_window = nullptr;

	Animation logo_anim;

private:

	SDL_Texture* texture = nullptr;
	SDL_Texture* logo_text = nullptr;
	SDL_Texture* gui_texture = nullptr;
	SDL_Texture* set_win_mainmenu = nullptr;

	Button_rects button1_idle, button1_hover, button1_click;
	Button_rects button2_idle, button2_hover, button2_click;
	Button_rects button3_idle, button3_hover, button3_click;

	_TTF_Font* font = nullptr;

	iPoint play_button, continue_button, exit_button;

};

#endif // !__j1MAINMENU_H___
