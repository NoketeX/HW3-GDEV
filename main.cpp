//Only detect damage at the first frame, not all frames
//Use melee; Create circle around player when it attacks

#include <raylib.h>
#include <raymath.h>

#include "PlayerStateMachine.cpp"
#include "EnemyStateMachine.cpp"

const float FPS(60.0f);

int camType;

Camera2D camera_view = {0};

int main() {
	InitWindow(1280, 720, "Castro_Hung_Taino_Homework03");

	Player p({400, 300}, {0, 0}, 20, 300);
	Enemy e({200, 200}, {0,0}, 20, 100, &p);
	Enemy e2({600, 600},{0,0}, 20, 100, &p);
	//Enemy::Enemy(Vector2 position, Vector2 direction, float rad, float spd, Player p)

	//Setting up camera
	camera_view.target = {p.pos.x, p.pos.y};
	camera_view.offset = {1280/2, 720/2};
	camera_view.zoom = 1.0f;

	while(!WindowShouldClose()) {
		float delta_time = GetFrameTime();

		camera_view.target = {p.pos.x, p.pos.y};

		p.Update(delta_time);
		e.Update(delta_time);
		e2.Update(delta_time);

		BeginDrawing();
		BeginMode2D(camera_view);
		ClearBackground(BLACK);

		p.Draw();
		e.Draw();
		e2.Draw();

		EndMode2D();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
