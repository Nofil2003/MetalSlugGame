#include <ctime>
#include <cstdlib>
#include <raylib.h>
#include <iostream>
using namespace std;


struct Tank {
	int x;
	int y;
	Rectangle collider;
	Texture2D texture;
};

struct TankBullet {
	int x;
	int y;
	Rectangle collider;
	Texture2D texture;
};

struct MainPlayer {
	int x;
	int y;
	Rectangle collider;
	Texture2D texture;
};
struct Enemy {
	int x;
	int y;
	Rectangle collider;
	Texture2D texture;
};
struct Bullet {
	int x;
	int y;
	Rectangle collider;
	Texture2D texture;
};

void enemy_getsShot(Rectangle* collider1, Rectangle* collider2) {
	if (CheckCollisionRecs(*collider1, *collider2)) {
		//code for when the enemy gets shot
	}
}

void player_getsShot(Rectangle* collider1, Rectangle* collider2) {
	if (CheckCollisionRecs(*collider1, *collider2)) {
		//code for when the player gets shot
	}
}

void playerCollision(Rectangle* collider1, Rectangle* collider2) {
	if (CheckCollisionRecs(*collider1, *collider2)) {
		//code for player collision
	}
}
void explosionCollision(Rectangle* collider1, Rectangle* collider2) {
	if (CheckCollisionRecs(*collider1, *collider2)) {
		//code for explosion
	}
}


const int screenWidth = 1200;
const int screenHeight = 800;
int health = 100;

int main()
{
	InitWindow(screenWidth, screenHeight, "Metal Slug Prototype");
	SetTargetFPS(60);
	Texture2D background1 = LoadTexture("bg1.png");
	Texture2D MainPlayerTexture = LoadTexture("player.png");
	MainPlayer player;
	player.x = screenWidth / 2 - (MainPlayerTexture.width) / 2;
	player.y = screenHeight- (MainPlayerTexture.height);
	player.collider = { (float)player.x, (float)player.y, (float)MainPlayerTexture.width ,(float) MainPlayerTexture.height  };
	const int playerSpeed = 600;

	while (!WindowShouldClose) {
		
		if (IsKeyDown(KEY_LEFT)) {
			player.x -= playerSpeed * GetFrameTime();
		}
		if (IsKeyDown(KEY_RIGHT)) {
			player.x += playerSpeed * GetFrameTime();
		}


		if (player.x > screenWidth - MainPlayerTexture.width)
			player.x = screenWidth - MainPlayerTexture.width;



		player.collider.x = player.x;
		BeginDrawing();
		ClearBackground(RAYWHITE);
		//display
		DrawTextureEx(background1, { 0, 0 }, 0.0f, (float)screenWidth / background1.width, WHITE);
		DrawTexture(MainPlayerTexture, player.x, player.y, WHITE);
		EndDrawing();
	}
	CloseWindow();
	//code for player health bar
	return 0;
}

