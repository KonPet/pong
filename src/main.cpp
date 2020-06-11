#include "raylib.h"
#include <math.h>
#include <time.h>
#include <unistd.h>

#define player_width 30
#define radius 20
#define PI 3.14159265

int main (void)
{
    srand(time(NULL));
    
    const int screenWidth = 800.0f;
    const int screenHeight = 450.0f;
    
    InitWindow(screenWidth, screenHeight, "PONG");
    InitAudioDevice();
    
    Rectangle Player1 = {20, 185, player_width, 80};
    Rectangle Player2 = {screenWidth - 20 - player_width, 185, player_width, 80};
    Vector2 Ball = {screenWidth / 2, screenHeight / 2};
    Sound bounce = LoadSound("sounds/bounce.wav");
    Sound wallbounce = LoadSound("sounds/wallbounce.wav");
    Sound point = LoadSound("sounds/point.wav");
    
    float angle = 55.0f;
    int vel = 10;
    int scoreP1 = 0;
    int scoreP2 = 0;
    int frames = 0;


    SetTargetFPS(60);
    
    // Draw title-screen

    // Game
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("PONG", (screenWidth/2 - MeasureText("PONG", 80)/2), (screenHeight/2 - 40), 80, BLACK);
            DrawText("Use W/S for the left player and I/K for the right player.", (screenWidth/2 - MeasureText("Use W/S for the left player and Up/Down for the right player.", 20)/2), (screenHeight/2 + 135), 20, LIGHTGRAY);
        EndDrawing();
        
        if (frames >= 40) {
            ClearBackground(RAYWHITE);
            DrawText("Press Space to start", (screenWidth/2 - MeasureText("Press Space to start", 20)/2), (screenHeight/2 + 50), 20, GRAY);
        }
        if (frames == 80) {
            ClearBackground(RAYWHITE);
            frames = 0;
        }
        frames++;
        if (IsKeyDown(KEY_SPACE)) {
            break;
        }
    }
    
    while (!WindowShouldClose())
    {
        Ball.x += cos(angle * PI / 180.0f) * (vel * 0.5);
        Ball.y += sin(angle * PI / 180.0f) * (vel * 0.5);
        if ((CheckCollisionCircleRec(Ball, radius, Player1) && ((angle > 90 || angle < -90) && (-270 < angle && angle < 270))) || (CheckCollisionCircleRec(Ball, radius, Player2) && ((-90 < angle && angle < 90) || (270 < angle || angle < -270)))) {
            angle = 180 - angle;
            angle += rand() % 20 - 10;
            if (angle >= 360) {
                angle -= 360;
            } else if (angle <= -360) {
                angle += 360;
            }
            if ((130 > abs(angle) && abs(angle) > 50) || (230 < abs(angle) && abs(angle) < 310)) {
                if (angle > 0) {
                    if ((angle + 10 > 130 && angle + 10 < 230) || angle + 10 > 310) {
                        angle += 10;
                    } else {
                        angle -= 10;
                    }
                } else {
                    if ((angle - 10 < -130 && angle - 10 > -230) || angle - 10 < -310) {
                        angle -= 10;
                    } else {
                        angle += 10;
                    }
                }
            }
            if (rand() % 2 == 1) {
                vel += 1;
            }
            PlaySound(bounce);
        }
        if (Ball.y + radius > screenHeight || Ball.y - radius < 0) {
            angle = -angle;
            PlaySound(wallbounce);
        }
        
        if (IsKeyDown(KEY_S) && Player1.y + Player1.height < screenHeight) {
            Player1.y += 5;
            while (Player1.y + Player1.height > screenHeight) {
                Player1.y--;
            }
        }
        if (IsKeyDown(KEY_W) && Player1.y > 0) {
            Player1.y -= 5;
            while (Player1.y < 0) {
                Player1.y++;
            }
        }
        
        if (IsKeyDown(KEY_K) && Player2.y + Player2.height < screenHeight) {
            Player2.y += 5;
            while (Player2.y + Player2.height > screenHeight) {
                Player2.y--;
            }
        }
        if (IsKeyDown(KEY_I) && Player2.y > 0) {
            Player2.y -= 5;
            while (Player2.y < 0) {
                Player2.y++;
            }
        }
        
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText(FormatText("%i", (int) scoreP1), 60, 10, 60, RED);
            DrawText(FormatText("%i", (int) scoreP2), (screenWidth - MeasureText(FormatText("%i", (int) scoreP2), 60) - 60), 10, 60, BLUE);
            DrawRectangleRec(Player1, RED);
            DrawRectangleRec(Player2, BLUE);
            DrawCircleV(Ball, radius, GREEN);
        EndDrawing();
        if (Ball.x + radius + 3 - cos(angle * PI / 180.0f) * (vel * 0.5) < 0) {
            scoreP2++;
            BeginDrawing();
                DrawText("BLUE SCORED!", (GetScreenWidth()/2 - MeasureText("BLUE SCORED!", 40) / 2), 10, 40, BLUE);
            EndDrawing();
            PlaySound(point);
            usleep(1000000);
            Ball = {screenWidth / 2, screenHeight / 2};
            angle = 135.0f;
            vel = 10;
        } else if (Ball.x - radius - 3 - cos(angle * PI / 180.0f) * (vel * 0.5) > screenWidth) {
            scoreP1++;
            BeginDrawing();
                DrawText("RED SCORED!", (GetScreenWidth()/2 - MeasureText("RED SCORED!", 40) / 2), 10, 40, RED);
            EndDrawing();
            PlaySound(point);
            usleep(1000000);
            Ball = {screenWidth / 2, screenHeight / 2};
            angle = 45.0f;
            vel = 10;
        }
    }
    UnloadSound(bounce);
    UnloadSound(wallbounce);
    UnloadSound(point);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
