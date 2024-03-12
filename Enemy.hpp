#ifndef ENEMY
#define ENEMY

#include "Player.hpp"
#include <raylib.h>
#include <raymath.h>

//You create classes and functions here, but definitions are done outside of the header

class Enemy;

class EnemyState {
public: 
	//Virtual means you can overrite it in future child classes
	virtual ~EnemyState() {} //Destructor, for when we remove instance
	virtual void Enter(Enemy& e) = 0; //For initializing certain things when you enter a state
	virtual void Update(Enemy& e, float delta_time) = 0; //For updating the player every frame
};

class EnemyWandering : public EnemyState {
public:
	float counter;
  float time;
  float randx, randy;

	void Enter(Enemy& e);
	void Update(Enemy& e, float delta_time);
};

class EnemyChase : public EnemyState {
public:
	void Enter(Enemy& e);
	void Update(Enemy& e, float delta_time);
};

class EnemyReady : public EnemyState {
public:
	float counter;

	void Enter(Enemy& e);
	void Update(Enemy& e, float delta_time);
};

class EnemyAttack : public EnemyState {
public:
	float counter;

	void Enter(Enemy& e);
	void Update(Enemy& e, float delta_time);
};

class Enemy {
public:
	Vector2 pos;
	Vector2 d; //For direction
	float r;
	Color c;
	int hp;
	float s;

	Vector2 player_pos;
	Vector2 player_d; //For direction
	float player_r;
	int player_hp;

	float player_s;

	Enemy(Vector2 pos, Vector2 direction, float rad, float spd, Player p); //Constructs the enemy



	void Update(float delta_time); //Updates player per frame
	void Draw(); //Draws the enemy
	void SetState(EnemyState* new_state); //Accepts state, which changes the state of the enemy

	EnemyWandering wandering;
	EnemyChase chase;
	EnemyReady ready;
	EnemyAttack attack;

private:
	EnemyState* current_state;
};

#endif
