#ifndef __j1SCENE1_H__
#define __j1SCENE1_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "j1Entity.h"
#include "j1EntityManager.h"
#include "j1Timer.h"
#include "p2DynArray.h"
#include "SDL/include/SDL_mouse.h"

struct SDL_Texture;
class Animation;

class j1Scene1 : public j1Module
{
public:

	j1Scene1();

	// Destructor
	virtual ~j1Scene1();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node& data);
	bool Save(pugi::xml_node& data) const;

	void LevelChangeLogic();

	void LoadTutorial();
	void LoadLevel1();
	void LoadMidLevel();

	void CameraMovement(float dt);
	void BlitKeys();

	void LoadNewLevel();

	void LoadSceneInfo();

	void SpawnTutorialEntities();
	void SpawnLevel1Entities();
	void SpawnMidLevelEntities();

	void ReSpawnEntities();
	void DrawJetLife();

	void ShowCursor(bool hide);
	
	void DrawPath(p2DynArray<iPoint>* path);

	ENTITY_TYPE RandomItem();

public:

	bool death = false;
	bool win1 = false;

	bool tutorial_active = true;
	bool level1_active = false;
	bool midlevel_active = false;

	bool midlevel_completed = false;

	bool cameraMoving = true;
	bool lateralMove = false;

	bool cullingView = false;

	j1Timer scene_timer;

	bool done_anim = false;

private:
	
	j1Timer deathTimer;

	bool clicked = false;
	Animation* mouseBlinking;
	Animation blinkAnim;

	SDL_Texture* keys;
	SDL_Texture* mouse;
	SDL_Texture* debugPath;
	SDL_Texture* jetPack_tex;
	SDL_Texture* cursor_tex;

	int cameraLimitX;
	int cameraLimitY;
	int cameraPositionMoving;
	int timer;

	std::string current_points;
	int score_player = 0;

	bool hide_cursor = true;

	iPoint doorPosition;
	iPoint cameraLimit;
	iPoint camera_speed;

	iPoint batPos1, batPos2, batPos3, batPos4;
	iPoint demonPos1, demonPos2;

	iPoint item1, item2, item3, item4, item5, item6, item7, item8;

	SDL_Rect cursor;
	fPoint mouse_position;

	iPoint obstacle1;
	iPoint obstacle2;

	int differenceY;

	SDL_Rect jetPackBar, jetPackLife;
	iPoint pos_bar, pos_bar2;

	SDL_Rect W, WPressed, A, APressed, D, DPressed, SPACE, SPACEPressed, mouseLeft, mouseRight, mouseIdle;
	iPoint posA, posD, posW1, posW2, posW3, posW4, posSPACE, posMLeft, posMRight, posMIdle;
};

#endif // __j1SCENE1_H__