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

struct Tank_Bullet {
    int x;
    int y;
    Rectangle collider;
    Texture2D rocket_texture;
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
    Texture2D bullet_texture;
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

void player_Collision(Rectangle* collider1, Rectangle* collider2) {
    if (CheckCollisionRecs(*collider1, *collider2)) {
        //code for player collision
    }
}
void explosion_Collision(Rectangle* collider1, Rectangle* collider2) {
    if (CheckCollisionRecs(*collider1, *collider2)) {
        //code for explosion
    }
}


int main() {
    InitWindow(screenWidth, screenHeight, "Metal Slug Prototype");
    InitAudioDevice();
    SetTargetFPS(60);
    Bullet bullet_mp;
    Texture2D background1 = LoadTexture("bg1.png");
    Texture2D background2 = LoadTexture("bg2.png");
    Texture2D MainPlayerTexture = LoadTexture("player.png");
    Texture2D Startbg = LoadTexture("Start_Screen.png");
    bullet_mp.bullet_texture = LoadTexture("bullet.png"); // loaded
    Music start_music;
    start_music = LoadMusicStream("start_audio.mp3");
    Sound jump = LoadSound("jump.mp3");
    Music ingame = LoadMusicStream("Ingame.mp3");
    float bg1 = 0;
    float bg2 = screenWidth;
    float sbg = 0;
    float bgSpeed = 500;

    
    if (background1.id == 0) cout << "Failed to load background texture!" << endl;
    if (MainPlayerTexture.id == 0) cout << "Failed to load player texture!" << endl;

    const float scaleFactor = 0.1f;

    MainPlayer player;
    player.x = screenWidth / 2 - (MainPlayerTexture.width * scaleFactor) / 2;
    player.y = screenHeight - (MainPlayerTexture.height * scaleFactor);
    player.collider = {
        (float)player.x,
        (float)player.y,
        MainPlayerTexture.width * scaleFactor,
        MainPlayerTexture.height * scaleFactor
    }; 
    
    const float gravity = 0.7f;
    const float jumpForce = -12.5f;
    bool isJumping = false;
    PlayMusicStream(start_music);
    bool startScreen = true;
    while (!WindowShouldClose()) {
        if (startScreen) {
            BeginDrawing();
            
            ClearBackground(RAYWHITE);
            UpdateMusicStream(start_music);
            DrawTexturePro(
                Startbg,
                { 0, 0, (float)Startbg.width, (float)Startbg.height },  // Source rectangle
                { 0, 0, (float)screenWidth, (float)screenHeight },      // Destination rectangle
                { 0, 0 },  // Origin
                0.0f,      // Rotation
                WHITE
            );
            EndDrawing();
            bullet_mp.x = player.x + 75;
            bullet_mp.y = player.y + 70;

            if (IsKeyPressed(KEY_ENTER)) {
                startScreen = false;
                UnloadMusicStream(start_music);
                PlayMusicStream(ingame);
            }
            
        }
        else 
        {
            if (!startScreen) {
                UpdateMusicStream(ingame);
            }

            bg1 -= bgSpeed * GetFrameTime();
            bg2 -= bgSpeed * GetFrameTime();

            if (bg1 <= -screenWidth) bg1 = screenWidth;
            if (bg2 <= -screenWidth) bg2 = screenWidth;

            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
                player.x -= 7;
                player.collider.x = (float)player.x;
            }
            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
                player.x += 7;
                player.collider.x = (float)player.x;
            }

            if ((IsKeyPressed(KEY_SPACE) && !isJumping) || (IsKeyPressed(KEY_W) && !isJumping)) {
                PlaySound(jump);
                player.velocityY = jumpForce;
                isJumping = true;
            }

            player.velocityY += gravity;
            player.y += player.velocityY;
            player.collider.y = (float)player.y;

            float groundY = screenHeight - (MainPlayerTexture.height * scaleFactor);
            if (player.y > groundY) {
                player.y = groundY;
                player.velocityY = 0;
                isJumping = false;
            }

            if (player.x < 0) player.x = 0;
            if (player.x > screenWidth - 150) player.x = screenWidth - 150;
            bullet_mp.bullet_texture.height = 100;
            bullet_mp.bullet_texture.width = 100;
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTextureEx(background1, { bg1, 0 }, 0.0f, (float)screenWidth / background1.width, WHITE);
            DrawTextureEx(background2, { bg2, 0 }, 0.0f, (float)screenWidth / background2.width, WHITE);
            DrawTextureEx(MainPlayerTexture, { (float)player.x, (float)player.y }, 0.0f, scaleFactor, WHITE);
            
            EndDrawing();
        }
    }
    UnloadMusicStream(ingame);
    UnloadTexture(Startbg);
    UnloadTexture(background1);
    UnloadTexture(MainPlayerTexture);
    CloseWindow();

    return 0;
}
