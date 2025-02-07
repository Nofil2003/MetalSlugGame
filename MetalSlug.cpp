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

const int MAX_BULLETS = 10;
const int MAX_ROCKETS = 5;


Bullet* bullets = new Bullet[MAX_BULLETS];
Rocket* rockets = new Rocket[MAX_ROCKETS];


void initBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i] = { {0, 0}, 5.0f, false };
    }
}


void initRockets() {
    for (int i = 0; i < MAX_ROCKETS; i++) {
        rockets[i] = { {0, 0}, 4.0f, false };
    }
}
void shootBullet(MainPlayer* player) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].active = true;
            bullets[i].position.x = player->x + player->collider.width;
            bullets[i].position.y = player->y + (player->collider.height / 2);
            break;
        }
    }
}


void launchRocket(Tank* tank, float currentTime, float* lastRocketTime) {
    if (currentTime - *lastRocketTime >= 2.5f) {
        *lastRocketTime = currentTime;
        for (int i = 0; i < MAX_ROCKETS; i++) {
            if (!rockets[i].active) {
                rockets[i].active = true;
                int heightOffset = rand() % 30 - 15;
                rockets[i].position.x = tank->x;
                rockets[i].position.y = tank->y + (tank->collider.height / 2) + heightOffset;
                break;
            }
        }
    }
}


void updateBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].position.x += bullets[i].speed;
            if (bullets[i].position.x > screenWidth) 
                bullets[i].active = false;
        }
    }
}

void updateRockets() {
    for (int i = 0; i < MAX_ROCKETS; i++) {
        if (rockets[i].active) {
            rockets[i].position.x -= rockets[i].speed;
            if (rockets[i].position.x < 0)
                rockets[i].active = false;
        }
    }
}


void drawBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active)
            DrawCircleV(bullets[i].position, 5, RED);
    }
}

void drawRockets() {
    for (int i = 0; i < MAX_ROCKETS; i++) {
        if (rockets[i].active)
            DrawCircleV(rockets[i].position, 10, BLACK);
    }
}




//struct Chopper {
//    Texture2D texture;
//    Vector2 position;
//    int frameCount;
//    int currentFrame;
//    float frameSpeed;
//    float frameTimer;
//
//
//    void InitChopper(const char* filePath, Vector2 pos, int frames, float speed) {
//        texture = LoadTexture(filePath);
//        position = pos;
//        frameCount = frames;
//        currentFrame = 0;
//        frameSpeed = speed;
//        frameTimer = 0.0f;
//    }
//
//
//    void UpdateChopper() {
//        frameTimer += GetFrameTime();
//        if (frameTimer >= frameSpeed) {
//            frameTimer = 0.0f;
//            currentFrame = (currentFrame + 1) % frameCount;
//        }
//    }
//
//
//    void DrawChopper() {
//        int frameWidth = texture.width / frameCount;
//        int frameHeight = texture.height;
//
//        Rectangle sourceRect = { currentFrame * frameWidth, 0, frameWidth, frameHeight };
//        Rectangle destRect = { position.x, position.y, frameWidth * 2, frameHeight * 2 }; // Scale the size
//        DrawTexturePro(texture, sourceRect, destRect, { 0, 0 }, 0.0f, WHITE);
//    }
//}


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
   /* Chopper helicopter;*/
    /*helicopter.InitChopper("helicopter.png", { screenWidth / 2.0f, screenHeight / 4.0f }, 4, 0.1f);*/
    Texture2D background1 = LoadTexture("bg1.png");
    Texture2D background2 = LoadTexture("bg2.png");
    Texture2D MainPlayerTexture = LoadTexture("player.png");
    Texture2D Startbg = LoadTexture("Start_Screen.png");
    Texture2D TankTexture = LoadTexture("tank.png");
    Music start_music;
    start_music = LoadMusicStream("start_audio.mp3");
    Sound jump = LoadSound("jump.mp3");
    Music ingame = LoadMusicStream("Ingame.mp3");

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

    initBullets();
    initRockets();

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
            if (IsKeyPressed(KEY_ENTER)) 
                startScreen = false;
        }
        else {

            bg1 -= bgSpeed * GetFrameTime();
            bg2 -= bgSpeed * GetFrameTime();

            if (bg1 <= -screenWidth) bg1 = screenWidth;
            if (bg2 <= -screenWidth) bg2 = screenWidth;

            player_movement(player, scaleFactor, isJumping, jumpForce, gravity);
            tank_movement(tank);

           /* frameTimer += GetFrameTime();
            if (frameTimer >= frameSpeed) {
                frameTimer = 0.0f;
                currentFrame = (currentFrame + 1) % frameCount;
            }*/

            if (IsKeyPressed(KEY_F)) {
                shootBullet(&player);
            }
            launchRocket(&tank, currentTime, &lastRocketTime);


            updateBullets();
            updateRockets();






            BeginDrawing();
            ClearBackground(RAYWHITE);


            DrawTextureEx(background1, { bg1, 0 }, 0.0f, (float)screenWidth / background1.width, WHITE);
            DrawTextureEx(background2, { bg2, 0 }, 0.0f, (float)screenWidth / background2.width, WHITE);

            DrawTextureEx(MainPlayerTexture, { (float)player.x, (float)player.y }, 0.0f, scaleFactor, WHITE);
            DrawTextureEx(TankTexture, { (float)tank.x, (float)tank.y }, 0.0f, scaleFactor, WHITE);
            drawBullets();
            drawRockets();


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
