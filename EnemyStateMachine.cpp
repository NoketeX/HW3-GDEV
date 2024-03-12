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
	DrawCircleLines(pos.x+20, pos.y+20, 60, RED);
	DrawCircleLines(pos.x+20, pos.y+20, 100, BLUE);
	DrawCircleLines(pos.x+20, pos.y+20, 180, GREEN);
}

//Defining the SetState function
void Enemy::SetState(EnemyState* new_state) { //We can define things outside of the class
	//Where you assign the new state
	current_state = new_state;
	current_state->Enter(*this);
}

//Defining the enemy constructor

Enemy::Enemy(Vector2 position, Vector2 direction, float spd, float rad, Player p) {
	pos = position;
	s = spd;
	d = direction;
	r = rad;
	player_pos = p.pos;
	player_s = p.s;
	player_d = p.d;
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
  if(counter >= 2){
    e.pos = Vector2Add(e.pos, Vector2Scale({randx, randy}, 30.0f * delta_time));
  } 
  if(counter >= 5){
    randx = GetRandomValue(-1, 1);
    randy = GetRandomValue(-1, 1);
    counter = 0;
  }
  

//	if(CheckCollisionCircles(player.pos, player.rad, e.)){
//		e.SetState(&e.chase);
//	} // coliding with circle, then change state to chase
	

}

void EnemyChase::Enter(Enemy& e) {
	e.c = YELLOW;
}

void EnemyChase::Update(Enemy& e, float delta_time) {
	if(e.player_pos.x >= e.pos.x && e.player_pos.y>= e.pos.y){
		e.pos.x+= (e.player_pos.x - e.pos.x) * e.s  * delta_time;
		e.pos.y+= (e.player_pos.y - e.pos.y) * e.s  * delta_time;
	} // player is to the right and below the enemey
	else if(e.player_pos.x >= e.pos.x && e.player_pos.y<= e.pos.y){
		e.pos.x+= (e.player_pos.x - e.pos.x) * e.s  * delta_time;
		e.pos.y-= (e.pos.y - e.player_pos.y) * e.s  * delta_time;
	}// player is to the right and above the enemey
	else if(e.player_pos.x <= e.pos.x && e.player_pos.y<= e.pos.y){
		e.pos.x-= (e.pos.x - e.player_pos.x ) * e.s  * delta_time;
		e.pos.y-= (e.pos.y - e.player_pos.y) * e.s  * delta_time;
	}// player is to the left and above the enemey
	else if(e.player_pos.x <= e.pos.x && e.player_pos.y>= e.pos.y){
		e.pos.x-= (e.pos.x - e.player_pos.x ) * e.s  * delta_time;
		e.pos.y+= (e.player_pos.y - e.pos.y) * e.s  * delta_time;
	}// player is to the left and below the enemey
	else{
		e.SetState(&e.ready);
	}
	
	
}

void EnemyReady::Enter(Enemy& e) {
	e.c = ORANGE;
	counter = 0.170;
}

void EnemyReady::Update(Enemy& e, float delta_time) {
	counter -= delta_time;
	if(counter==0){
		e.SetState(&e.attack);
	}
}

void EnemyAttack::Enter(Enemy& e) {
	e.c = RED;
	counter = 0.170;
}

void EnemyAttack::Update(Enemy& e, float delta_time) {
		counter -= delta_time;
		e.pos.x += ((e.s * (e.player_pos.x - e.pos.x)) * 2) * delta_time;
		e.pos.y += ((e.s * (e.player_pos.x - e.pos.x)) * 2) * delta_time; //rush towards rge position of the player

	if(e.player_pos.x == e.player_pos.x && e.player_pos.y == e.player_pos.x){
		e.player_hp-=2;
		e.SetState(&e.chase);
	}
	else{
		e.SetState(&e.chase);
	}
		
	
}
