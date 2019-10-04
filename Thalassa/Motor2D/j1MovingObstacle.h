#ifndef __j1MOVINGOBSTACLE_H__
#define __j1MOVINGOBSTACLE_H__

#include "j1Entity.h"
#include "p2Point.h"

struct Collider;
struct SDL_Texture;

enum ENTITY_TYPE;

class j1MovingObstacle : public j1Entity {

public:

	j1MovingObstacle(int x, int y, ENTITY_TYPE type);
	virtual ~j1MovingObstacle();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

public:

	fPoint position;
	fPoint speed;

	Animation idle;

	bool active = false;
	bool playedFx = false;
};

#endif