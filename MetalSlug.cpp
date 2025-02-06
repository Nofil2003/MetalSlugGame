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
    SetTargetFPS(60);
    Bullet bullet_mp;
    Texture2D background1 = LoadTexture("bg1.png");
    Texture2D background2 = LoadTexture("bg2.png");
    Texture2D MainPlayerTexture = LoadTexture("player.png");
    bullet_mp.bullet_texture = LoadTexture("bullet.png");
   
    float bg1 = 0;
    float bg2 = screenWidth;
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
    
    const float gravity = 0.5f;
    const float jumpForce = -12.5f;
    bool isJumping = false;

    while (!WindowShouldClose()) {
        bg1 -= bgSpeed * GetFrameTime();
        bg2 -= bgSpeed * GetFrameTime();
        

        if (bg1 <= -screenWidth) bg1 = screenWidth;
        if (bg2 <= -screenWidth) bg2 = screenWidth;

        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT) ){
            player.x -= 7;
            player.collider.x = (float)player.x;
        }
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            player.x += 7;
            player.collider.x = (float)player.x;
        }
        
        if ((IsKeyPressed(KEY_SPACE) && !isJumping) || (IsKeyPressed(KEY_W) && !isJumping)){
            player.velocityY = jumpForce;
            isJumping = true;
        }
        if (IsKeyPressed(MOUSE_BUTTON_LEFT)) {
            Texture2D mp_bullet = LoadTexture("bullet.png");
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

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTextureEx(background1, { bg1, 0 }, 0.0f, (float)screenWidth / background1.width, WHITE);
        DrawTextureEx(background2, { bg2, 0 }, 0.0f, (float)screenWidth / background2.width, WHITE);
        DrawTextureEx(MainPlayerTexture, { (float)player.x, (float)player.y }, 0.0f, scaleFactor, WHITE);
        DrawTextureEx(bullet_mp.bullet_texture, { (float)bullet_mp.x, (float)bullet_mp.y }, 0.0f, 0.05f, ORANGE);

        EndDrawing();
    }

    UnloadTexture(background1);
    UnloadTexture(MainPlayerTexture);
    CloseWindow();

    return 0;
}
