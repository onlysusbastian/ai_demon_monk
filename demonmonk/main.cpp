#include "raylib.h"
#include <vector>
#include <string>
#include <cmath>

#include "graphics.h"

enum GameState {
    START_SCREEN,
    GAMEPLAY
};

GameState gameState = START_SCREEN;

const int screenWidth = 1000;
const int screenHeight = 600;
const int boatCapacity = 2;

std::vector<Character> characters;

Side boatSide = LEFT;
int boatOccupants = 0;

bool gameOver = false;
bool win = false;

int stepCounter = 0;

float boatX;
float boatTargetX;
bool boatMoving = false;

Rectangle moveButton    = {screenWidth/2 - 80, screenHeight - 80, 160, 60};
Rectangle restartButton = {screenWidth/2 - 80, 20, 160, 60};
Rectangle finishButton  = {screenWidth/2 - 80, 100, 160, 60};
Rectangle menuButton    = {20,20,40,40};

float waterTime = 0.0f;

void ResetGame()
{
    characters.clear();

    for (int i = 0; i < 3; i++)
    {
        characters.push_back({"Monk",LEFT});
        characters.push_back({"Demon",LEFT});
    }

    boatSide = LEFT;
    boatOccupants = 0;

    gameOver = false;
    win = false;

    stepCounter = 0;

    boatX = 300;
    boatMoving = false;
}

void CountSide(Side side,int& monks,int& demons)
{
    monks = demons = 0;

    for (auto& c : characters)
    {
        if (c.side == side)
        {
            if (c.type == "Monk") monks++;
            else demons++;
        }
    }
}

void CheckGameState()
{
    int lm,ld,rm,rd;

    CountSide(LEFT,lm,ld);
    CountSide(RIGHT,rm,rd);

    if ((lm > 0 && ld > lm) || (rm > 0 && rd > rm))
        gameOver = true;
}

bool AllOnRight()
{
    for (auto& c : characters)
        if (c.side != RIGHT) return false;

    return true;
}

void UpdatePositions()
{
    int leftY = 100;
    int rightY = 100;
    int boatY = 350;

    for (auto& c : characters)
    {
        if (c.side == LEFT)
        {
            c.bounds = {100,(float)leftY,60,60};
            leftY += 80;
        }
        else if (c.side == RIGHT)
        {
            c.bounds = {800,(float)rightY,60,60};
            rightY += 80;
        }
        else if (c.side == BOAT)
        {
            c.bounds = {boatX + 10,(float)boatY,60,60};
            boatY += 70;
        }
    }
}

int main()
{
    InitWindow(screenWidth,screenHeight,"Demon Monk - Raylib");
    SetTargetFPS(60);

    Texture2D demonTexture = LoadTexture("assets/demon.png");
    Texture2D monkTexture  = LoadTexture("assets/monk.png");
    Texture2D boatTexture =  LoadTexture("assets/boat.png");
    Font gameFont = LoadFontEx("assets/font.ttf", 64, 0, 0);
    SetTextureFilter(gameFont.texture, TEXTURE_FILTER_POINT);
    SetGameFont(gameFont);

    ResetGame();

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        waterTime += dt;

        if (gameState == START_SCREEN)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Rectangle startButton = {screenWidth/2 - 120,screenHeight/2 - 40,240,80};

                if (CheckCollisionPointRec(GetMousePosition(),startButton))
                {
                    gameState = GAMEPLAY;
                    ResetGame();
                }
            }

            BeginDrawing();
            DrawStartScreen(screenWidth,screenHeight);
            EndDrawing();
            continue;
        }

        if (boatMoving)
        {
            boatX += (boatTargetX - boatX) * 5.0f * dt;

            if (fabs(boatX - boatTargetX) < 10.0f)
            {
                boatX = boatTargetX;
                boatMoving = false;
                boatSide = (boatSide == LEFT) ? RIGHT : LEFT;
                CheckGameState();
            }
        }

        if (!boatMoving && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mouse = GetMousePosition();

            if (CheckCollisionPointRec(mouse,menuButton))
                gameState = START_SCREEN;

            if (CheckCollisionPointRec(mouse,restartButton))
                ResetGame();

            if (CheckCollisionPointRec(mouse,finishButton))
                if (AllOnRight()) win = true;

            if (!gameOver && !win)
            {
                for (auto& c : characters)
                {
                    if (CheckCollisionPointRec(mouse,c.bounds))
                    {
                        if (c.side != BOAT && c.side == boatSide)
                        {
                            if (boatOccupants < boatCapacity)
                            {
                                c.side = BOAT;
                                boatOccupants++;
                            }
                        }
                        else if (c.side == BOAT)
                        {
                            c.side = boatSide;
                            boatOccupants--;
                        }

                        break;
                    }
                }

                if (CheckCollisionPointRec(mouse,moveButton))
                {
                    if (boatOccupants > 0)
                    {
                        stepCounter++;
                        boatTargetX = (boatSide == LEFT) ? 600 : 300;
                        boatMoving = true;
                    }
                }
            }
        }

        UpdatePositions();

        BeginDrawing();

        DrawGameScene(
            screenWidth,
            screenHeight,
            waterTime,
            boatX,
            characters,
            demonTexture,
            monkTexture,
            boatTexture
        );

        DrawUI(
            moveButton,
            restartButton,
            finishButton,
            menuButton,
            stepCounter,
            gameOver,
            win
        );

        EndDrawing();
    }

    UnloadTexture(demonTexture);
    UnloadTexture(monkTexture);
    UnloadFont(gameFont);

    CloseWindow();
    return 0;
}
