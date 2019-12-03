#ifndef __J1ENTITYMANAGER_H__
#define __J1ENTITYMANAGER_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1Entity.h"

#define MAX_ENTITIES 100

class j1Entity;
class j1Bat;
class j1Demon;
class j1BatEnemy;
class j1Player;
class j1MovingObstacle;
class j1Door;
class j1LifeItem;
class j1Platform;
class j1JetpackItem;
class j1Coin;

struct SDL_Texture;

enum ENTITY_TYPE
{
	PLAYER,
	OBSTACLE,
	DOOR,
	BAT,
	DEMON,
	BAT_E,
	LIFE_ITEM, 
	JETPACK_ITEM, 
	DAMAGE_ITEM,
	COIN,
	PLATFORM,

	NONE
};

struct EntityInfo
{
	ENTITY_TYPE type = ENTITY_TYPE::NONE;
	fPoint position;
};

struct PlatformInfo
{
	ENTITY_TYPE type = ENTITY_TYPE::NONE;
	fPoint position;
	iPoint limit;
	int type_platform;
	bool vertical;
};

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	j1Entity* EntityFactory(ENTITY_TYPE type, float x = 0, float y = 0);
	void CreateEntity(ENTITY_TYPE type, float x = 0, float y = 0);
	void AddEnemy(float x, float y, ENTITY_TYPE type);
	void AddEntity(float x, float y, ENTITY_TYPE type);
	void AddPlatform(float x, float y, ENTITY_TYPE type, iPoint limit, int type_plat = 1, bool vertical = false);
	void SpawnEnemy(const EntityInfo& info);
	void SpawnEntity(const EntityInfo& info);
	void SpawnPlatform(const PlatformInfo& info);
	void DestroyAllEntities();

	void OnCollision(Collider* c1, Collider* c2);

public:

	p2List<j1Entity*>	entityList;
	j1Player*			player = nullptr;
	j1MovingObstacle*	obstacle = nullptr;
	j1Door*				door = nullptr;
	j1Bat*				bat = nullptr;
	j1Demon*			demon = nullptr;
	j1BatEnemy*			bat_e = nullptr;
	j1LifeItem*			lifeItem = nullptr;
	j1JetpackItem*		jetpackItem = nullptr;
	j1Platform*			platform = nullptr;
	j1Coin*				coin = nullptr;

private:

	EntityInfo			queue[MAX_ENTITIES];
	PlatformInfo		platform_queue[MAX_ENTITIES];

};

#endif 
