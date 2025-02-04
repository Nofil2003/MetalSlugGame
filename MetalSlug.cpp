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
const int screenHeight = 500;
int health = 100;

int main()
{
	InitWindow(screenWidth, screenHeight, "Metal Slug Prototype");
	SetTargetFPS(60);
	Texture2D background1 = LoadTexture("bg1.png");
	Texture2D background2 = LoadTexture("bg2.png");
	Texture2D MainPlayerTexture = LoadTexture("player.png");

	//changing background
	float bg1 = 0;
	float bg2 = screenWidth;
	float bgSpeed = 500;
	

	if (background1.id == 0) cout << "Failed to load background texture!" << endl;

	
	if (MainPlayerTexture.id == 0) cout << "Failed to load player texture!" << endl;

	const float scaleFactor = 0.1f;  // Adjust this factor to resize the player

	MainPlayer player;
	player.x = screenWidth / 2 - (MainPlayerTexture.width * scaleFactor) / 2;
	player.y = screenHeight - (MainPlayerTexture.height * scaleFactor);
	player.collider = {
		(float)player.x,
		(float)player.y,
		MainPlayerTexture.width * scaleFactor,
		MainPlayerTexture.height * scaleFactor
	};

	const int playerSpeed = 600;



	while (!WindowShouldClose()) {

		//background shift
		bg1 -= bgSpeed * GetFrameTime();
		bg2 -= bgSpeed * GetFrameTime();


		if (bg1 <= -screenWidth)
			bg1 = screenWidth;
		if (bg2 <= -screenWidth)
			bg2 = screenWidth;
		
		if (IsKeyDown(KEY_A)) {
			player.x = player.x - 7;
			player.collider.x = (float)player.x;
		}
		if (IsKeyDown(KEY_D)) {
			player.x = player.x + 7;;
			player.collider.y = (float)player.y;
		}

		// Sample.

		if (player.x < screenWidth - MainPlayerTexture.width)
			player.x = screenWidth - MainPlayerTexture.width;
		if (player.x > screenWidth  - 150)
			player.x = screenWidth - 150 ;

		


		
		

		BeginDrawing();
		ClearBackground(RAYWHITE);
		//display
		DrawTextureEx(background1, { bg1, 0 }, 0.0f, (float)screenWidth / background1.width, WHITE);
		DrawTextureEx(background2, { bg2, 0 }, 0.0f, (float)screenWidth / background2.width, WHITE);
		DrawTextureEx(MainPlayerTexture, { (float)player.x, (float)player.y }, 0.0f, scaleFactor, WHITE);

		EndDrawing();
	}
	UnloadTexture(background1);
	UnloadTexture(MainPlayerTexture);

	CloseWindow();
	//code for player health bar
	return 0;
}

