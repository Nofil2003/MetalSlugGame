// https://github.com/Nofil2003/MetalSlugGame

#include <ctime>
#include <cstdlib>
#include <raylib.h>
#include <iostream>

using namespace std;

const int screenWidth = 1200;
const int screenHeight = 500;
int health = 100;
int max_distance = 1000;
int dis = 0;
int level = 1;
int enemiesDefeated = 0;
const int ENEMIES_TO_WIN = 10;

//---------------------------------------------------------------------------------------------------------------------
//Structs
//---------------------------------------------------------------------------------------------------------------------
struct MainPlayer {
    int x;
    int y;
    Rectangle collider;
    Texture2D texture_mp;
    float velocityY = 0;
};

struct Samurai {
    int x;
    int y;
    Rectangle collider;
    Texture2D textureSamurai;
    bool active;
};

struct Tank {
    int x;
    int y;
    Rectangle collider;
    Texture2D texture;
    bool active;
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

//---------------------------------------------------------------------------------------------------------------------
// Function pointer type for movement
//---------------------------------------------------------------------------------------------------------------------
typedef void (*MovementFunction)(MainPlayer&);
typedef void (*JumpFunction)(MainPlayer&, bool&, Sound&);
typedef void (*VerticalMoveFunction)(MainPlayer&, Sound&);

//---------------------------------------------------------------------------------------------------------------------
// Movement System for main player / main character
//---------------------------------------------------------------------------------------------------------------------
struct MovementSystem {
    MovementFunction horizontalMove;
    JumpFunction jumpFunction;
    VerticalMoveFunction verticalMove;

    MovementSystem(int level);
};

//---------------------------------------------------------------------------------------------------------------------
// Generic Functions
//---------------------------------------------------------------------------------------------------------------------


Texture2D LoadTextureFromFile(const char* filename) {
    Texture2D texture = LoadTexture(filename);
    if (texture.id == 0) {
        cout << "Could not load " << filename << endl;
    }
    return texture;
}


void UpdateScrollingBackground(float& bg1, float& bg2, float bgSpeed) {
    bg1 -= bgSpeed * GetFrameTime();
    bg2 -= bgSpeed * GetFrameTime();
    if (bg1 <= -screenWidth) bg1 = screenWidth;
    if (bg2 <= -screenWidth) bg2 = screenWidth;
}


void DrawScrollingBackground(Texture2D background, float bg1, float bg2) {
    DrawTextureEx(background, { bg1, 0 }, 0.0f, (float)screenWidth / background.width, WHITE);
    DrawTextureEx(background, { bg2, 0 }, 0.0f, (float)screenWidth / background.width, WHITE);
}


void DrawRectangleCollider(Rectangle collider, Color color) {
#if _DEBUG
    DrawRectangleLinesEx(collider, 1, color);
#endif
}


void UpdateEntityPosition(int& x, int& y, int speedX, int speedY) {
    x += speedX;
    y += speedY;
}


void KeepEntityWithinBounds(int& x, int minX, int maxX) {
    if (x < minX) x = minX;
    if (x > maxX) x = maxX;
}


void DrawBar(int x, int y, int width, int height, float value, int maxValue, Color backgroundColor, Color foregroundColor, Color textColor, const char* label) {
    float barWidth = (float)width * (value / (float)maxValue);
    DrawRectangle(x, y, width, height, backgroundColor);
    DrawRectangle(x, y, barWidth, height, foregroundColor);
    DrawRectangleLines(x, y, width, height, BLACK);
    const char* text = TextFormat(label, (int)value, maxValue);
    DrawText(text, x + width + 10, y, 20, textColor);
}

//---------------------------------------------------------------------------------------------------------------------
// Level 1 Functions
//---------------------------------------------------------------------------------------------------------------------

void movePlayerLevel1(MainPlayer& player) {
    int speed = 7;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player.x -= speed;
        player.collider.x = (float)player.x;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player.x += speed;
        player.collider.x = (float)player.x;
    }

    KeepEntityWithinBounds(player.x, 0, screenWidth - 150);
}

void jumpLevel1(MainPlayer& player, bool& isJumping, Sound& jumpSound) {
    const float gravity = 0.7f;
    const float jumpForce = -17.0f;

    if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W)) && !isJumping) {
        PlaySound(jumpSound);
        player.velocityY = jumpForce;
        isJumping = true;
    }

    player.velocityY += gravity;
    player.y += player.velocityY;
    player.collider.y = (float)player.y;

    float groundY = screenHeight - player.collider.height;
    if (player.y >= groundY) {
        player.y = groundY;
        player.velocityY = 0;
        isJumping = false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
// Level 2 Functions
//---------------------------------------------------------------------------------------------------------------------
void movePlayerLevel2(MainPlayer& player) {
    int speed = 7;  // Reduced speed for better control
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player.x -= speed;
        player.collider.x = (float)player.x;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player.x += speed;
        player.collider.x = (float)player.x;
    }

    // Update collider position
    player.collider.x = (float)player.x;
    player.collider.y = (float)player.y;

    KeepEntityWithinBounds(player.x, 0, screenWidth - 70);  // Adjusted boundary for character width
}

void jumpLevel2(MainPlayer& player, bool& isJumping, Sound& jumpSound) {
    const int GROUND_Y = 390;
    const int PLATFORM1_Y = 282;
    const int PLATFORM2_Y = 174;
    const int JUMP_HEIGHT = 108;

    if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W)) && !isJumping) {
        PlaySound(jumpSound);
        isJumping = true;


        if (player.y == GROUND_Y) {
            player.y = PLATFORM1_Y;
        }
        else if (player.y == PLATFORM1_Y) {
            player.y = PLATFORM2_Y;
        }
        else if (player.y == PLATFORM2_Y) {

            player.velocityY = -12.0f;
        }
    }


    if (player.y < PLATFORM2_Y) {
        const float gravity = 0.7f;
        player.velocityY += gravity;
        player.y += player.velocityY;

        if (player.y > PLATFORM2_Y) {
            player.y = PLATFORM2_Y;
            player.velocityY = 0;
            isJumping = false;
        }
    }
    else {
        isJumping = false;
    }


    player.collider.y = (float)player.y;
}

void handleVerticalMovementLevel2(MainPlayer& player, Sound& stompSound) {
    const int GROUND_Y = 390;
    const int PLATFORM1_Y = 282;
    const int PLATFORM2_Y = 174;

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        PlaySound(stompSound);


        if (player.y == PLATFORM2_Y) {
            player.y = PLATFORM1_Y;
        }
        else if (player.y == PLATFORM1_Y) {
            player.y = GROUND_Y;
        }


        player.collider.y = (float)player.y;
    }
}

//---------------------------------------------------------------------------------------------------------------------
// Movement System Constructor
//---------------------------------------------------------------------------------------------------------------------

MovementSystem::MovementSystem(int level) {
    if (level == 1) {
        horizontalMove = movePlayerLevel1;
        jumpFunction = jumpLevel1;
        verticalMove = nullptr;
    }
    else {
        horizontalMove = movePlayerLevel2;
        jumpFunction = jumpLevel2;
        verticalMove = handleVerticalMovementLevel2;
    }
}

//---------------------------------------------------------------------------------------------------------------------
// Bullet and Rocket Functions
//---------------------------------------------------------------------------------------------------------------------

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
        rockets[i] = { {0, 0}, 8.0f, false };
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
    if (currentTime - *lastRocketTime >= 1.5f) {
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
            DrawCircleV(rockets[i].position, 10, GREEN);
    }
}

//---------------------------------------------------------------------------------------------------------------------
// Collision Functions
//---------------------------------------------------------------------------------------------------------------------

void checkBulletCollisions(MainPlayer& player, Tank& tank) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active && tank.active) {
            if (CheckCollisionPointRec(bullets[i].position, tank.collider)) {
                bullets[i].active = false;
                tank.active = false;
            }
        }
    }
}

void checkRocketPlayerCollision(MainPlayer& player) {
    for (int i = 0; i < MAX_ROCKETS; i++) {
        if (rockets[i].active && CheckCollisionPointRec(rockets[i].position, player.collider)) {
            rockets[i].active = false;
            health -= 20;
            if (health < 0) health = 0;
        }
    }
}

void checkCollision(MainPlayer& player, Tank& tank) {
    if (tank.active && CheckCollisionRecs(player.collider, tank.collider)) {
        if (player.y + player.collider.height < tank.y + 10) {
            tank.active = false;
        }
        else {
            health -= 20;
            if (health < 0) health = 0;
        }
    }
}

void checkSamuraiBulletCollision(Samurai& samurai, Bullet& bullet) {
    if (CheckCollisionPointRec(bullet.position, samurai.collider)) {
        bullet.active = false;
        samurai.active = false;
    }
}
void checkSamuraiCollision(MainPlayer& player, Samurai& samurai) {
    if (CheckCollisionRecs(player.collider, samurai.collider)) {
        samurai.active = false;
        health -= 50;
        if (health < 0) health = 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
// Enemy Movements
//---------------------------------------------------------------------------------------------------------------------

void tank_movement(Tank& tank) {
    tank.x -= 5;
    tank.collider.x = (float)tank.x;
    if (tank.x < -tank.texture.width) {
        tank.x = screenWidth;
        tank.collider.x = (float)tank.x;
        tank.active = true;
    }
}

void samurai_movement(Samurai& samurai) {
    samurai.x -= 5;
    samurai.collider.x = (float)samurai.x;

    if (samurai.x < -samurai.textureSamurai.width) {
        const int yPositions[] = { 390, 282, 174 };
        int randomIndex = rand() % 3; 

        samurai.x = screenWidth;
        samurai.y = yPositions[randomIndex];

        samurai.collider.x = (float)samurai.x;
        samurai.collider.y = (float)samurai.y;

        samurai.active = true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
// Main Function
//---------------------------------------------------------------------------------------------------------------------

int main() {
    InitWindow(screenWidth, screenHeight, "Metal Slug Prototype");
    InitAudioDevice();
    SetTargetFPS(60);
    srand(time(0));


    Texture2D background1 = LoadTextureFromFile("bg1.png");
    Texture2D background2 = LoadTextureFromFile("bg2.png");
    Texture2D MainPlayerTexture = LoadTextureFromFile("player.png");
    Texture2D Startbg = LoadTextureFromFile("Start_Screen.png");
    Texture2D Game_over = LoadTextureFromFile("Game_Over.png");
    Texture2D loading_lvl2 = LoadTextureFromFile("lvl2_loading.jpg");
    Texture2D TankTexture = LoadTextureFromFile("tank.png");
    Texture2D textureSamurai = LoadTextureFromFile("samurai.png");

    Texture2D lvl2_bg = LoadTextureFromFile("lvl2_bg.jpg");

    lvl2_bg.height = screenHeight;
    lvl2_bg.width = screenWidth;


    Music lost = LoadMusicStream("lose.mp3");
    Music start_music = LoadMusicStream("start_audio.mp3");
    Sound jump = LoadSound("jump.mp3");
    Music ingame = LoadMusicStream("Ingame.mp3");
    Sound stomp = LoadSound("stomp.mp3");
    Music lvl2_theme = LoadMusicStream("forest_theme.mp3");


    MainPlayer lvl2_mc, lvl2_mc_inverse;
    lvl2_mc.texture_mp = LoadTextureFromFile("lvl2_character.png");
    lvl2_mc.texture_mp.height = 60;
    lvl2_mc.texture_mp.width = 70;
    lvl2_mc.x = 10;
    lvl2_mc.y = 390;

    lvl2_mc_inverse.texture_mp = LoadTextureFromFile("lvl2_invert_character.png");
    lvl2_mc_inverse.texture_mp.height = 60;
    lvl2_mc_inverse.texture_mp.width = 70;
    lvl2_mc_inverse.x = 10;
    lvl2_mc_inverse.y = 390;


    MainPlayer player;
    const float scaleFactor = 0.1f;
    player.x = screenWidth / 2 - (MainPlayerTexture.width * scaleFactor) / 2;
    player.y = screenHeight - (MainPlayerTexture.height * scaleFactor);
    player.collider = { (float)player.x, (float)player.y, MainPlayerTexture.width * scaleFactor, MainPlayerTexture.height * scaleFactor };


    Tank tank;
    tank.x = screenWidth - (TankTexture.width * scaleFactor) / 2;
    tank.y = screenHeight - (TankTexture.height * scaleFactor);
    tank.texture = TankTexture;
    tank.collider = { (float)tank.x, (float)tank.y, TankTexture.width * scaleFactor, TankTexture.height * scaleFactor };
    tank.active = true;


    Samurai samurai;
    samurai.x = screenWidth - (textureSamurai.width * scaleFactor) / 2;
    samurai.y = screenHeight - (textureSamurai.height * scaleFactor);
    samurai.textureSamurai = textureSamurai;
    samurai.collider = { (float)tank.x, (float)tank.y, textureSamurai.width * scaleFactor, textureSamurai.height * scaleFactor };
    tank.active = true;


    initBullets();
    initRockets();
    float lastRocketTime = 0.0f;


    float bg1 = 0, bg2 = screenWidth, bgSpeed = 500;
    bool isJumping = false;
    bool startScreen = true;
    bool startScreen2 = true;

    MovementSystem movementSystem(level);


    lvl2_mc.collider = { (float)lvl2_mc.x, (float)lvl2_mc.y, 70, 60 };
    lvl2_mc_inverse.collider = { (float)lvl2_mc_inverse.x, (float)lvl2_mc_inverse.y, 70, 60 };


    loading_lvl2.height = screenHeight;
    loading_lvl2.width = screenWidth;
    Game_over.height = screenHeight;
    Game_over.width = screenWidth;

    const float gravity2 = 0.7f;
    const float jumpForce2 = -12.0f;
    bool isJumping2 = false;

    PlayMusicStream(start_music);

    while (!WindowShouldClose()) {
        float currentTime = GetTime();

        if (level == 1) {
            if (startScreen) {
                BeginDrawing();
                ClearBackground(RAYWHITE);
                UpdateMusicStream(start_music);
                DrawTexturePro(Startbg,
                    { 0, 0, (float)Startbg.width, (float)Startbg.height },
                    { 0, 0, (float)screenWidth, (float)screenHeight },
                    { 0, 0 }, 0.0f, WHITE);
                DrawText("LEVEL 1", (screenWidth / 2) - 80, 0, 35, YELLOW);
                EndDrawing();

                if (IsKeyPressed(KEY_ENTER)) {
                    startScreen = false;
                    PauseMusicStream(start_music);
                    PlayMusicStream(ingame);
                }
            }
            else {
                UpdateMusicStream(ingame);


                UpdateScrollingBackground(bg1, bg2, bgSpeed);

                dis++;


                movementSystem.horizontalMove(player);
                movementSystem.jumpFunction(player, isJumping, jump);


                tank_movement(tank);
                checkCollision(player, tank);
                checkBulletCollisions(player, tank);
                checkRocketPlayerCollision(player);



                if (IsKeyPressed(KEY_F)) {
                    shootBullet(&player);
                }
                launchRocket(&tank, currentTime, &lastRocketTime);

                updateBullets();
                updateRockets();

                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawScrollingBackground(background1, bg1, bg2);
                DrawTextureEx(MainPlayerTexture, { (float)player.x, (float)player.y }, 0.0f, scaleFactor, WHITE);

                if (tank.active) {
                    DrawTextureEx(TankTexture, { (float)tank.x, (float)tank.y }, 0.0f, scaleFactor, WHITE);
                }

                drawBullets();
                drawRockets();

                DrawBar(20, 20, 200, 20, health, 100, GRAY, RED, BLACK, "HP: %d/%d");
                DrawBar(20, 50, 200, 20, dis, max_distance, GRAY, GREEN, BLACK, "Distance: %d/%d");


                if (health <= 0) {
                    StopMusicStream(ingame);
                    PlayMusicStream(lost);

                    BeginDrawing();
                    ClearBackground(RAYWHITE);
                    DrawTexture(Game_over, 0, 0, WHITE);
                    DrawText("GAME OVER", screenWidth / 2 - 100, screenHeight / 2 - 30, 40, RED);
                    DrawText("Press ESC to exit", screenWidth / 2 - 100, screenHeight / 2 + 30, 20, WHITE);
                    EndDrawing();

                    if (IsKeyPressed(KEY_ESCAPE)) {
                        break;
                    }
                    continue;
                }

                if (dis >= max_distance) {
                    bool waitingForEnter = true;
                    while (waitingForEnter) {
                        UpdateMusicStream(start_music);
                        BeginDrawing();
                        ResumeMusicStream(start_music);
                        ClearBackground(RAYWHITE);
                        DrawTexture(loading_lvl2, 0, 0, WHITE);
                        DrawText("Objective: Eliminate 20 enemies", 50, 100, 30, GREEN);
                        DrawText("Press ENTER to continue", screenWidth / 2 - 200, screenHeight - 100, 30, GREEN);
                        EndDrawing();

                        if (IsKeyPressed(KEY_ENTER)) {
                            UnloadMusicStream(start_music);
                            level++;
                            startScreen2 = false;

                            PlayMusicStream(lvl2_theme);
                            movementSystem = MovementSystem(level);
                            Texture2D lvl2_bg = LoadTexture("lvl2_bg.jpg");
                            lvl2_bg.height = screenHeight;
                            lvl2_bg.width = screenWidth;
                            break;
                        }
                    }
                }

                EndDrawing();
            }
        }
        if (level == 2) {
            UpdateMusicStream(lvl2_theme);

            movementSystem.horizontalMove(lvl2_mc);
            movementSystem.jumpFunction(lvl2_mc, isJumping, jump);
            if (movementSystem.verticalMove) {
                movementSystem.verticalMove(lvl2_mc, stomp);
            }

            lvl2_mc_inverse.x = lvl2_mc.x;
            lvl2_mc_inverse.y = lvl2_mc.y;
            lvl2_mc_inverse.collider = lvl2_mc.collider;

            
            samurai_movement(samurai);

          
            if (IsKeyPressed(KEY_F)) {
            
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (!bullets[i].active) {
                        bullets[i].active = true;
                        bullets[i].position.x = lvl2_mc.x + lvl2_mc.texture_mp.width;
                        bullets[i].position.y = lvl2_mc.y + (lvl2_mc.texture_mp.height / 2);
                        break;
                    }
                }
            }

      
            updateBullets();

            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullets[i].active && samurai.active) {
                    if (CheckCollisionPointRec(bullets[i].position, samurai.collider)) {
                        bullets[i].active = false;
                        samurai.active = false;
                        enemiesDefeated++;
                    }
                }
            }

            
            if (samurai.active) {
                checkSamuraiCollision(lvl2_mc, samurai);
            }

           
            if (health <= 0) {
                StopMusicStream(lvl2_theme);
                PlayMusicStream(lost);

                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawTexture(Game_over, 0, 0, WHITE);
                DrawText("GAME OVER", screenWidth / 2 - 100, screenHeight / 2 - 30, 40, RED);
                DrawText("Press ESC to exit", screenWidth / 2 - 100, screenHeight / 2 + 30, 20, WHITE);
                EndDrawing();

                if (IsKeyPressed(KEY_ESCAPE)) {
                    break;
                }
                continue;
            }

            
            if (enemiesDefeated >= ENEMIES_TO_WIN) {
                StopMusicStream(lvl2_theme);
                PlayMusicStream(start_music);

                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                DrawText("CONGRATULATIONS!", screenWidth / 2 - 200, screenHeight / 2 - 60, 40, GOLD);
                DrawText("You have completed the game!", screenWidth / 2 - 250, screenHeight / 2, 30, GREEN);
                DrawText("Press ESC to exit", screenWidth / 2 - 100, screenHeight / 2 + 60, 20, WHITE);
                EndDrawing();

                if (IsKeyPressed(KEY_ESCAPE)) {
                    break;
                }
                continue;
            }

           
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTexture(lvl2_bg, 0, 0, WHITE);

            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
                DrawTextureEx(lvl2_mc_inverse.texture_mp, { (float)lvl2_mc.x, (float)lvl2_mc.y }, 0.0f, 1.0f, WHITE);
            }
            else {
                DrawTextureEx(lvl2_mc.texture_mp, { (float)lvl2_mc.x, (float)lvl2_mc.y }, 0.0f, 1.0f, WHITE);
            }

            
            if (samurai.active) {
                DrawTextureEx(samurai.textureSamurai, { (float)samurai.x, (float)samurai.y }, 0.0f, scaleFactor, WHITE);
            }

            
            drawBullets();

            
            DrawBar(20, 20, 200, 20, health, 100, GRAY, RED, BLACK, "HP: %d/%d");
            DrawBar(20, 50, 200, 20, enemiesDefeated, ENEMIES_TO_WIN, GRAY, BLUE, BLACK, "Enemies: %d/%d");

            EndDrawing();
        }

    }

    UnloadTexture(background1);
    UnloadTexture(background2);
    UnloadTexture(MainPlayerTexture);
    UnloadTexture(Startbg);
    UnloadTexture(Game_over);
    UnloadTexture(loading_lvl2);
    UnloadTexture(TankTexture);
    UnloadTexture(textureSamurai);
    UnloadTexture(lvl2_bg);
    UnloadTexture(lvl2_mc.texture_mp);
    UnloadTexture(lvl2_mc_inverse.texture_mp);


    UnloadMusicStream(lost);
    UnloadMusicStream(start_music);
    UnloadSound(jump);
    UnloadMusicStream(ingame);
    UnloadSound(stomp);
    UnloadMusicStream(lvl2_theme);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
