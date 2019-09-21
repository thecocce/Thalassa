#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Scene1.h"
#include "j1Collisions.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"


j1Player::j1Player(int x, int y, ENTITY_TYPE type) : j1Entity(x, y, ENTITY_TYPE::PLAYER) 
{
	animation = NULL;

	idle.LoadAnimations("idle");
	jetpack.LoadAnimations("jetpack");

	godAnim.loop = true;
	godAnim.speed = 0.01f;
	godAnim.PushBack({ 43,8,14,23 });
	godAnim.PushBack({ 63,8,14,23 });

	idle.loop = true;
	idle.speed = 0.008f;
	idle.PushBack({ 4,36,13,21 });
	idle.PushBack({ 24,36,13,21 });

	run.loop = true;
	run.speed = 0.01f;
	run.PushBack({ 1,298,13,21 });
	run.PushBack({ 17,298,13,21 });
	run.PushBack({ 33,298,13,21 });
	run.PushBack({ 49,298,13,21 });

	jetpack.loop = true;
	jetpack.speed = 0.01f;
	jetpack.PushBack({ 4,8,14,23 });
	jetpack.PushBack({ 24,8,14,23 });

	jump.loop = false;
	jump.speed = 0.001f;
	//jump.PushBack({ 3,112,15,17 });
	//jump.PushBack({ 23,112,15,17 });
	//jump.PushBack({ 43,112,15,17 });
	//jump.PushBack({ 23,112,15,17 });
	//jump.PushBack({ 3,112,15,17 });
	jump.PushBack({ 24,87,13,18 });
}

j1Player::~j1Player() {}

bool j1Player::Start() {

	sprites = App->tex->Load("textures/Character_Spritesheet.png");

	position = { 55,75 };
	godSpeed = 0.15f;
	speed.y = 0.15f;
	speed.x = 0.15f;
	gravity = 0.006f;
	animation = &idle;
	playerCreated = true;

	collider = App->collisions->AddCollider({ (int)position.x + 3, (int)position.y, 12, 20 }, COLLIDER_PLAYER, App->entity_manager);

	return true;
}

bool j1Player::PreUpdate() {

	return true;
}

bool j1Player::Update(float dt) {

	if (playerCreated)
	{
		animation = &idle;

		if (godMode) 
		{
			animation = &godAnim;
			if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT) {
				position.x += godSpeed;
				flip = true;
			}

			if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT) {
				position.x -= godSpeed;
				flip = false;
			}

			if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_REPEAT) {
				position.y -= godSpeed;
			}

			if (App->input->GetKey(SDL_SCANCODE_S) == j1KeyState::KEY_REPEAT) {
				position.y += godSpeed;
			}
		}
		else
		{
			if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT) {
				if (!ColRight)
				{
					position.x += speed.x;
					animation = &run;
					flip = true;
				}
			}

			if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT) {
				if (!ColLeft)
				{
					position.x -= speed.x;
					animation = &run;
					flip = false;
				}
			}

			if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_DOWN && doubleJump != 0) {
				isJumping = true;
				onFloor = false;
				jumpForce = 0.85f;
				doubleJump -= 1;
			}

			if (App->input->GetKey(SDL_SCANCODE_S) == j1KeyState::KEY_REPEAT) {
				position.y += speed.y;
				animation = &idle;
			}

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_DOWN) {
				jetpackActive = true;
				onFloor = false;
				jetForce = 0.2f;
				speed.y = 0.15f;
			}

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_UP) {
				jetpackActive = false;
				speed.y = 0.15f;
			}

			if (isJumping) Jumping();

			if (jetpackActive) JetPack();

			if (!isJumping && !godMode && !ColDown) 
			{
				if (!onFloor) {
					position.y += speed.y;
					speed.y += gravity;
				}
			}
		}

		
		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			godMode = !godMode;
			
			if (godMode) 
			{
				collider->type = COLLIDER_GOD;
			}
			else if(!godMode)
			{
				collider->type = COLLIDER_PLAYER;
			}
		}
	}

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	BlitEntity(animation->GetCurrentFrame(), flip, position.x, position.y);

	return true;
}

bool j1Player::PostUpdate() {

	ColRight = false;
	ColLeft = false;
	ColDown = false;

	return true;
}

bool j1Player::Load(pugi::xml_node& data) {

	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();

	return true;
}

bool j1Player::Save(pugi::xml_node& data) const {

	pugi::xml_node player = data.append_child("position");

	player.append_attribute("x") = position.x;
	player.append_attribute("y") = position.y;

	return true;
}

bool j1Player::CleanUp() {
	LOG("Freeing the player");

	return true;
}

void j1Player::OnCollision(Collider * c1, Collider * c2)
{
	if (c1->type == COLLIDER_PLAYER)
	{
		if (c2->type == COLLIDER_WALL)
		{
			// Right & Left Collisions
			if (collider->rect.y <= c2->rect.y + c2->rect.h && collider->rect.y + collider->rect.h >= c2->rect.y)
			{
				if (collider->rect.x + collider->rect.w >= c2->rect.x && collider->rect.x <= c2->rect.x)
				{
					ColRight = true;
					LOG("TOUCHES RIGHT");
				}
				else if (collider->rect.x <= c2->rect.x + c2->rect.w && collider->rect.x + collider->rect.w >= c2->rect.x + c2->rect.w)
				{
					ColLeft = true;
					LOG("TOUCHES LEFT");
				}
			}

			// Up & Down Collisions
			if (collider->rect.x + collider->rect.w >= c2->rect.x + 4
				&& collider->rect.x + 4 <= c2->rect.x + c2->rect.w)
			{
				if (collider->rect.y + collider->rect.h >= c2->rect.y
					&& collider->rect.y < c2->rect.y) {

					onFloor = true;
					isJumping = false;
					jetpackActive = false;

					position.y = c2->rect.y - collider->rect.h;
					doubleJump = 2;

					ColDown = true;
					ColUp = false;
					
					LOG("TOUCHING DOWN");
				}
				
				else if (collider->rect.y <= c2->rect.y + c2->rect.h
					&& collider->rect.y > c2->rect.y) {
					
					onFloor = false;

					position.y = c2->rect.y + c2->rect.h;

					ColDown = false;
					ColUp = true;

					LOG("TOUCHING UP");
				}
			}
		}
	}
	
	if (c2->type == COLLIDER_DEATH)
	{
		//
	}

}

void j1Player::Jumping() {

	animation = &jump;

	if (!onFloor && !jetpackActive) {
		position.y -= jumpForce;
		jumpForce -= gravity;
	}
}

void j1Player::JetPack() {

	animation = &jetpack;

	if (!onFloor) {
		position.y -= jetForce;
	}
}