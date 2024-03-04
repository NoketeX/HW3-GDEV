#include <raylib.h>
#include <raymath.h>

#include "Enemy.hpp"

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

//Defining the player constructor
Enemy::Enemy(Vector2 position, float spd) {
	pos = position;
	s = spd;
	SetState(&wandering);
}

//Editing the states of the Enemy
void EnemyWandering::Enter(Enemy& e) {
	e.c = PINK;
}

void EnemyWandering::Update(Enemy& e, float delta_time) {
}

void EnemyChase::Enter(Enemy& e) {
	e.c = YELLOW;
}

void EnemyChase::Update(Enemy& e, float delta_time) {

}

void EnemyReady::Enter(Enemy& e) {
	e.c = ORANGE;
}

void EnemyReady::Update(Enemy& e, float delta_time) {
	
}

void EnemyAttack::Enter(Enemy& e) {
	e.c = RED;
}

void EnemyAttack::Update(Enemy& e, float delta_time) {
	
}