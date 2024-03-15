#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <cmath>

#include "Enemy.hpp"
#include "Player.hpp"


void Enemy::Update(float delta_time) {
	current_state->Update(*this, delta_time);
}

void Enemy::Draw() { 
  DrawRectanglePro({pos.x + 20, pos.y + 20, 40, 40}, {20, 20}, angle * 180/M_PI, c);
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

void Enemy::TakeDamage(int damage){
  hp -= damage;  
  std::cout << hp;
}

//Defining the enemy constructor

Enemy::Enemy(Vector2 position, float spd, float rad, Player* p) {
	pos = position;
	s = spd;
	r = rad;
  player = p;
	SetState(&wandering);
}

//Editing the states of the Enemy
void EnemyWandering::Enter(Enemy& e) {
	e.c = PINK;
  e.angle = 0;
  counter = 0;
  randx = GetRandomValue(-20, 20);
  randy = GetRandomValue(-20, 20);
}

void EnemyWandering::Update(Enemy& e, float delta_time){
  counter += delta_time;  
  if(CheckCollisionCircleRec(e.player->pos, e.player->r, {e.pos.x, e.pos.y, 40, 40}) && e.player->invframes <= 0){ 
		e.player->TakeDamage(2);
    e.player->invframes = 0.5f;
  }

  e.dmgtimer -= delta_time;
    if(CheckCollisionCircleRec(e.player->pos, e.player->dmgrng, {e.pos.x, e.pos.y, 40, 40}) && e.dmgtimer <= 0.0f){
      e.TakeDamage(2);
      e.dmgcount += 1;
      if(e.dmgcount == 2){
        e.dmgtimer = 2.0f;
        e.dmgcount = 0;
    }
  }

  if(counter >= 1){
    e.pos = Vector2Add(e.pos, Vector2Scale({randx, randy}, e.s * delta_time / 2.0f));
  } 
  if(counter >= 3){
    randx = GetRandomValue(-20, 20);
    randy = GetRandomValue(-20, 20);
    counter = 0;
  }
  if(CheckCollisionCircles({e.pos.x + 20, e.pos.y + 20}, 100, e.player->pos, e.player->r)){
    e.SetState(&e.chase);
  }
}

void EnemyChase::Enter(Enemy& e) {
	e.c = YELLOW;
  e.s = 10;
}

void EnemyChase::Update(Enemy& e, float delta_time) {
  e.angle = Vector2Angle(e.player->pos,e.pos);
  Vector2 direction = Vector2Normalize(Vector2Subtract(e.player->pos, e.pos));
  e.pos = Vector2Add(e.pos, Vector2Scale(direction, (e.s * 8) * delta_time));
  if(CheckCollisionCircleRec(e.player->pos, e.player->r, {e.pos.x, e.pos.y, 40, 40}) && e.player->invframes <= 0){ 
		e.player->TakeDamage(2);
    e.player->invframes = 0.5f;
  }

 e.dmgtimer -= delta_time;
  if(CheckCollisionCircleRec(e.player->pos, e.player->dmgrng, {e.pos.x, e.pos.y, 40, 40}) && e.dmgtimer <= 0.0f){
    e.TakeDamage(2);
    e.dmgcount += 1;
    if(e.dmgcount == 2){
      e.dmgtimer = 2.0f;
      e.dmgcount = 0;
    }
  }

	if(CheckCollisionCircles({e.pos.x + 20, e.pos.y + 20}, 60, e.player->pos, e.player->r)){
		e.SetState(&e.ready);
	}
  if(CheckCollisionCircles({e.pos.x + 20, e.pos.y + 20}, 180, e.player->pos, e.player->r) == false){
    e.SetState(&e.wandering);
  }
}

//Enemy is preparing to attack
void EnemyReady::Enter(Enemy& e) {
	e.c = ORANGE;
	counter = 0.500f;
}

void EnemyReady::Update(Enemy& e, float delta_time) {
	counter -= delta_time;
  e.angle = Vector2Angle(e.player->pos,e.pos);
  if(CheckCollisionCircleRec(e.player->pos, e.player->r, {e.pos.x, e.pos.y, 40, 40}) && e.player->invframes <= 0){ 
		e.player->TakeDamage(2);
    e.player->invframes = 0.5f;
  }

  e.dmgtimer -= delta_time;
  if(CheckCollisionCircleRec(e.player->pos, e.player->dmgrng, {e.pos.x, e.pos.y, 40, 40}) && e.dmgtimer <= 0.0f){
    e.TakeDamage(2);
    e.dmgcount += 1;
    if(e.dmgcount == 2){
      e.dmgtimer = 2.0f;
      e.dmgcount = 0;
    }
  }

	if(counter<=0){
		e.SetState(&e.attack);
	}
}

void EnemyAttack::Enter(Enemy& e) {
	e.c = RED;
  e.s = 30;
  counter = 0.170;
}

void EnemyAttack::Update(Enemy& e, float delta_time) {
	counter -= delta_time;
  e.angle = Vector2Angle(e.player->pos,e.pos);
  if(CheckCollisionCircleRec(e.player->pos, e.player->r, {e.pos.x, e.pos.y, 40, 40}) && e.player->invframes <= 0){ 
		e.player->TakeDamage(2);
    e.player->invframes = 0.5f;
  }
  
  e.dmgtimer -= delta_time;
  if(CheckCollisionCircleRec(e.player->pos, e.player->dmgrng, {e.pos.x, e.pos.y, 40, 40}) && e.dmgtimer <= 0.0f){
    e.TakeDamage(2);
    e.dmgcount += 1;
    if(e.dmgcount == 2){
      e.dmgtimer = 2.0f;
      e.dmgcount = 0;
    }
  }


  if(counter >= 0){
    Vector2 direction = Vector2Normalize(Vector2Subtract(e.player->pos, e.pos));
    e.pos = Vector2Add(e.pos, Vector2Scale(direction, e.s * 30 * delta_time));
  }

  else if(counter <= 0){
    e.SetState(&e.wandering);
  }
}
