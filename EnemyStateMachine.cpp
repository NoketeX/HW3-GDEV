#include <raylib.h>
#include <raymath.h>
#include <iostream>

#include "Enemy.hpp"
#include "Player.hpp"


void Enemy::Update(float delta_time) {
	current_state->Update(*this, delta_time);
}

void Enemy::Draw() { 
	DrawRectangleV(pos, {40, 40}, c);
	DrawCircleLines(pos.x+20, pos.y+20, 60, RED); // Attack
	DrawCircleLines(pos.x+20, pos.y+20, 100, BLUE); // Detection 
	DrawCircleLines(pos.x+20, pos.y+20, 180, GREEN); // Aggro
}
// https://www.youtube.com/watch?v=b6OvrRbGU68

//Defining the SetState function
void Enemy::SetState(EnemyState* new_state) { //We can define things outside of the class
	//Where you assign the new state
	current_state = new_state;
	current_state->Enter(*this);
} 

//Defining the enemy constructor

Enemy::Enemy(Vector2 position, Vector2 direction, float spd, float rad, Player* p) {
	pos = position;
	s = spd;
	d = direction;
	r = rad;
  player = p;
	SetState(&wandering);
}

//Editing the states of the Enemy
void EnemyWandering::Enter(Enemy& e) {
	e.c = PINK;
  counter = 0;
  randx = 0;
  randy = 0;
}

void EnemyWandering::Update(Enemy& e, float delta_time){
  counter += delta_time;  
  if(CheckCollisionCircleRec(e.player->pos, e.player->r, {e.pos.x, e.pos.y, 40, 40}) && e.player->invframes <= 0){ 
		e.player->TakeDamage(2);
    e.player->invframes = 0.5f;
  }

  if(counter >= 2){
    e.pos = Vector2Add(e.pos, Vector2Scale({randx, randy}, e.s * delta_time / 2.0f));
  } 
  if(counter >= 5){
    randx = GetRandomValue(-1, 1);
    randy = GetRandomValue(-1, 1);
    counter = 0;
  }
  if(CheckCollisionCircles({e.pos.x + 20, e.pos.y + 20}, 100, e.player->pos, e.player->r)){
    e.SetState(&e.chase);
  }
}

void EnemyChase::Enter(Enemy& e) {
	e.c = YELLOW;
}

void EnemyChase::Update(Enemy& e, float delta_time) {
  if(CheckCollisionCircleRec(e.player->pos, e.player->r, {e.pos.x, e.pos.y, 40, 40}) && e.player->invframes <= 0){ 
		e.player->TakeDamage(2);
    e.player->invframes = 0.5f;
  }

	if(CheckCollisionCircles({e.pos.x + 20, e.pos.y + 20}, 60, e.player->pos, e.player->r)){
		e.SetState(&e.ready);
	}
  if(CheckCollisionCircles({e.pos.x + 20, e.pos.y + 20}, 180, e.player->pos, e.player->r) == false){
    e.SetState(&e.wandering);
  }
//  else if() check if player is outside of the 180 circle
}

//Enemy is preparing to attack
void EnemyReady::Enter(Enemy& e) {
	e.c = ORANGE;
	counter = 0.170;
}

void EnemyReady::Update(Enemy& e, float delta_time) {
	counter -= delta_time;
  if(CheckCollisionCircleRec(e.player->pos, e.player->r, {e.pos.x, e.pos.y, 40, 40}) && e.player->invframes <= 0){ 
		e.player->TakeDamage(2);
    e.player->invframes = 0.5f;
  }

	if(counter<=0){
		e.SetState(&e.attack);
	}
}

void EnemyAttack::Enter(Enemy& e) {
	e.c = RED;
  counter = 0.170;
}

void EnemyAttack::Update(Enemy& e, float delta_time) {
	counter -= delta_time;
  if(CheckCollisionCircleRec(e.player->pos, e.player->r, {e.pos.x, e.pos.y, 40, 40}) && e.player->invframes <= 0){ 
		e.player->TakeDamage(2);
    e.player->invframes = 0.5f;
  }

  if(counter >= 0){
	  e.pos.x += ((e.s * (e.player->pos.x - e.pos.x)) * 2) * delta_time;
	  e.pos.y += ((e.s * (e.player->pos.y - e.pos.y)) * 2) * delta_time; //rush towards rge position of the player
  }
  else if(counter <= 0){
    e.SetState(&e.chase);
  }
}
