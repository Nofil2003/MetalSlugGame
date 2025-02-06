#include <ctime>
#include <cstdlib>
#include <raylib.h>
#include <iostream>

using namespace std;

const int screenWidth = 1200;
const int screenHeight = 500;
int health = 100;

struct MainPlayer {
    int x;
    int y;
    Rectangle collider;
    Texture2D texture;
    float velocityY = 0;
};

struct Tank {
    int x;
    int y;
    Rectangle collider;
    Texture2D texture;
};

struct Bullet {
    Vector2 position;
    float speed;
    bool active;
};

struct Rocket {
    Vector2 position;
    float speed;
    bool active;
};

void player_movement(MainPlayer& player, float scaleFactor, bool& isJumping, float jumpForce, float gravity) {
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player.x -= 7;
        player.collider.x = (float)player.x;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player.x += 7;
        player.collider.x = (float)player.x;
    }

    if ((IsKeyPressed(KEY_SPACE) && !isJumping) || (IsKeyPressed(KEY_W) && !isJumping)) {
        player.velocityY = jumpForce;
        isJumping = true;
    }

    player.velocityY += gravity;
    player.y += player.velocityY;
    player.collider.y = (float)player.y;

    float groundY = screenHeight - (player.texture.height * scaleFactor);
    if (player.y >= groundY) {
        player.y = groundY;
        player.velocityY = 0;
        isJumping = false;
    }

    if (player.x < 0) player.x = 0;
    if (player.x > screenWidth - 150) player.x = screenWidth - 150;
}

void tank_movement(Tank& tank) {
    tank.x -= 5;
    if (tank.x < -tank.texture.width) {
        tank.x = screenWidth;
    }
}

int main() {
    InitWindow(screenWidth, screenHeight, "Metal Slug Prototype");
    SetTargetFPS(60);
    srand(time(0));

    
    Texture2D background1 = LoadTexture("bg1.png");
    Texture2D background2 = LoadTexture("bg2.png");
    Texture2D MainPlayerTexture = LoadTexture("player.png");
    Texture2D Startbg = LoadTexture("Start_Screen.png");
    Texture2D TankTexture = LoadTexture("tank.png");

    if (background1.id == 0 || background2.id == 0 || MainPlayerTexture.id == 0 || Startbg.id == 0 || TankTexture.id == 0) {
        cout << "Failed to load one or more textures!" << endl;
        return -1;
    }

    float bg1 = 0, bg2 = screenWidth, bgSpeed = 500;
    const float scaleFactor = 0.1f, gravity = 0.5f, jumpForce = -12.5f;
    bool isJumping = false;

    MainPlayer player;
    player.x = screenWidth / 2 - (MainPlayerTexture.width * scaleFactor) / 2;
    player.y = screenHeight - (MainPlayerTexture.height * scaleFactor);
    player.texture = MainPlayerTexture;
    player.collider = { (float)player.x, (float)player.y, MainPlayerTexture.width * scaleFactor, MainPlayerTexture.height * scaleFactor };

    Tank tank;
    tank.x = screenWidth - (TankTexture.width * scaleFactor) / 2;
    tank.y = screenHeight - (TankTexture.height * scaleFactor);
    tank.texture = TankTexture;
    tank.collider = { (float)tank.x, (float)tank.y, TankTexture.width * scaleFactor, TankTexture.height * scaleFactor };

    
    Bullet bullet = { {0, 0}, 5.0f, false };

    
    Rocket rocket = { {0, 0}, 4.0f, false };
    float lastRocketTime = 0.0f;
    float rocketSpawnInterval = 2.5f; 

    bool startScreen = true;
    while (!WindowShouldClose()) {
        float currentTime = GetTime(); 

        if (startScreen) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTexturePro(Startbg, { 0, 0, (float)Startbg.width, (float)Startbg.height }, { 0, 0, (float)screenWidth, (float)screenHeight }, { 0, 0 }, 0.0f, WHITE);
            EndDrawing();
            if (IsKeyPressed(KEY_ENTER)) startScreen = false;
        }
        else {
            
            bg1 -= bgSpeed * GetFrameTime();
            bg2 -= bgSpeed * GetFrameTime();

            if (bg1 <= -screenWidth) bg1 = screenWidth;
            if (bg2 <= -screenWidth) bg2 = screenWidth;

            player_movement(player, scaleFactor, isJumping, jumpForce, gravity);
            tank_movement(tank);

            
            if (IsKeyPressed(KEY_F) && !bullet.active) {
                bullet.active = true;
                bullet.position.x = player.x + player.collider.width;
                bullet.position.y = player.y + (player.collider.height / 2);
            }

            if (bullet.active) {
                bullet.position.x += bullet.speed;
                if (bullet.position.x > screenWidth) bullet.active = false;
            }

           
            if (!rocket.active && (currentTime - lastRocketTime) >= rocketSpawnInterval) {
                rocket.active = true;
                lastRocketTime = currentTime;

                
                int heightOffset = rand() % 30 - 15; 
                rocket.position.x = tank.x;
                rocket.position.y = tank.y + (tank.collider.height / 2) + heightOffset;
            }

            if (rocket.active) {
                rocket.position.x -= rocket.speed;
                if (rocket.position.x < 0) rocket.active = false;
            }

            
            BeginDrawing();
            ClearBackground(RAYWHITE);

            
            DrawTextureEx(background1, { bg1, 0 }, 0.0f, (float)screenWidth / background1.width, WHITE);
            DrawTextureEx(background2, { bg2, 0 }, 0.0f, (float)screenWidth / background2.width, WHITE);

            
            DrawTextureEx(MainPlayerTexture, { (float)player.x, (float)player.y }, 0.0f, scaleFactor, WHITE);
            DrawTextureEx(TankTexture, { (float)tank.x, (float)tank.y }, 0.0f, scaleFactor, WHITE);

           
            if (bullet.active) DrawCircleV(bullet.position, 5, RED);

            
            if (rocket.active) DrawCircleV(rocket.position, 10, BLACK);

            EndDrawing();
        }
    }

    UnloadTexture(Startbg);
    UnloadTexture(background1);
    UnloadTexture(background2);
    UnloadTexture(MainPlayerTexture);
    UnloadTexture(TankTexture);
    CloseWindow();

    return 0;
}
