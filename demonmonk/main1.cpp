#include "raylib.h"
#include <vector>
#include <string>
#include <cmath>

enum Side { LEFT, RIGHT, BOAT };

enum GameState {
    START_SCREEN,
    GAMEPLAY
};

GameState gameState = START_SCREEN;

struct Character {
    std::string type;
    Side side;
    Rectangle bounds;
};

const int screenWidth = 1000;
const int screenHeight = 600;
const int boatCapacity = 2;

std::vector<Character> characters;

Side boatSide = LEFT;
int boatOccupants = 0;

bool gameOver = false;
bool win = false;

int stepCounter = 0;

// Boat animation
float boatX;
float boatStartX;
float boatTargetX;
bool boatMoving = false;
float boatSpeed = 300.0f;

// Buttons
Rectangle moveButton    = {screenWidth/2 - 80, screenHeight - 80, 160, 60};
Rectangle restartButton = {screenWidth/2 - 80, 20, 160, 60};
Rectangle finishButton  = {screenWidth/2 - 80, 100, 160, 60};

float waterTime = 0.0f;

void ResetGame() {

    characters.clear();

    for (int i = 0; i < 3; i++) {
        characters.push_back({"Monk", LEFT});
        characters.push_back({"Demon", LEFT});
    }

    boatSide = LEFT;
    boatOccupants = 0;
    gameOver = false;
    win = false;

    stepCounter = 0;

    boatX = 300;
    boatMoving = false;
}

void CountSide(Side side, int& monks, int& demons) {

    monks = demons = 0;

    for (auto& c : characters) {
        if (c.side == side) {
            if (c.type == "Monk") monks++;
            else demons++;
        }
    }
}

void CheckGameState() {

    int lm, ld, rm, rd;

    CountSide(LEFT, lm, ld);
    CountSide(RIGHT, rm, rd);

    if ((lm > 0 && ld > lm) || (rm > 0 && rd > rm)) {
        gameOver = true;
    }
}

bool AllOnRight() {

    for (auto& c : characters)
        if (c.side != RIGHT) return false;

    return true;
}

void DrawInteractiveButton(Rectangle rect, const char* text, Color baseColor) {

    Vector2 mouse = GetMousePosition();

    bool hovered = CheckCollisionPointRec(mouse, rect);
    bool pressed = hovered && IsMouseButtonDown(MOUSE_LEFT_BUTTON);

    Color color = baseColor;

    if (hovered) color = ColorBrightness(baseColor, -0.15f);
    if (pressed) color = ColorBrightness(baseColor, -0.30f);

    float offsetY = pressed ? 3 : 0;

    DrawRectangleRounded(
        {rect.x, rect.y + offsetY, rect.width, rect.height},
        0.3f, 8, color
    );

    int fontSize = 20;
    int textWidth = MeasureText(text, fontSize);

    DrawText(text,
             rect.x + rect.width/2 - textWidth/2,
             rect.y + rect.height/2 - fontSize/2 + offsetY,
             fontSize,
             WHITE);
}

void UpdatePositions() {

    int leftY = 100;
    int rightY = 100;
    int boatY = 350;

    for (auto& c : characters) {

        if (c.side == LEFT) {
            c.bounds = {100, (float)leftY, 60, 60};
            leftY += 80;
        }
        else if (c.side == RIGHT) {
            c.bounds = {800, (float)rightY, 60, 60};
            rightY += 80;
        }
        else if (c.side == BOAT) {
            c.bounds = {boatX + 10, (float)boatY, 60, 60};
            boatY += 70;
        }
    }
}

int main() {

    InitWindow(screenWidth, screenHeight, "Demon Monk - Raylib");
    SetTargetFPS(60);

    ResetGame();

    while (!WindowShouldClose()) {

        float dt = GetFrameTime();
        waterTime += dt;

        // START SCREEN
        if (gameState == START_SCREEN) {

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

                Rectangle startButton = {screenWidth/2 - 120, screenHeight/2 - 40, 240, 80};

                if (CheckCollisionPointRec(GetMousePosition(), startButton)) {
                    gameState = GAMEPLAY;
                    ResetGame();
                }
            }

            BeginDrawing();
            ClearBackground(SKYBLUE);

            DrawText("MONKS AND DEMONS", 300, 200, 50, DARKBLUE);

            Rectangle startButton = {screenWidth/2 - 120, screenHeight/2 - 40, 240, 80};
            DrawInteractiveButton(startButton, "START GAME", DARKGREEN);

            EndDrawing();
            continue;
        }

        // Boat animation
        if (boatMoving) {

            float direction = (boatTargetX > boatStartX) ? 1.0f : -1.0f;
            boatX += direction * boatSpeed * dt;

            if ((direction > 0 && boatX >= boatTargetX) ||
                (direction < 0 && boatX <= boatTargetX)) {

                boatX = boatTargetX;
                boatMoving = false;

                boatSide = (boatSide == LEFT) ? RIGHT : LEFT;
                CheckGameState();
            }
        }

        // Input
        if (!boatMoving && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

            Vector2 mouse = GetMousePosition();

            if (CheckCollisionPointRec(mouse, restartButton))
                ResetGame();

            if (CheckCollisionPointRec(mouse, finishButton))
                if (AllOnRight()) win = true;

            if (!gameOver && !win) {

                for (auto& c : characters) {

                    if (CheckCollisionPointRec(mouse, c.bounds)) {

                        if (c.side != BOAT && c.side == boatSide) {

                            if (boatOccupants < boatCapacity) {
                                c.side = BOAT;
                                boatOccupants++;
                            }
                        }
                        else if (c.side == BOAT) {

                            c.side = boatSide;
                            boatOccupants--;
                        }

                        break;
                    }
                }

                if (CheckCollisionPointRec(mouse, moveButton)) {

                    if (boatOccupants > 0) {

                        stepCounter++;

                        boatStartX = boatX;
                        boatTargetX = (boatSide == LEFT) ? 600 : 300;
                        boatMoving = true;
                    }
                }
            }
        }

        UpdatePositions();

        BeginDrawing();
        ClearBackground(SKYBLUE);

        DrawText("LEFT BANK", 80, 50, 20, DARKGREEN);
        DrawText("RIGHT BANK", 780, 50, 20, DARKGREEN);

        // River
        for (int y = 0; y < screenHeight; y++) {

            float edgeWave = sinf(waterTime * 2.0f + y * 0.04f) * 6.0f;

            float leftEdge  = 250 + edgeWave;
            float rightEdge = 750 + edgeWave;

            DrawLineEx({leftEdge, (float)y},
                       {rightEdge, (float)y},
                       1.0f,
                       BLUE);
        }

        // Shimmer
        for (int y = 0; y < screenHeight; y += 20) {

            float wave = sinf((y + waterTime * 100.0f) * 0.05f) * 10.0f;

            DrawRectangle(
                250 + wave,
                y,
                500,
                20,
                Fade(WHITE, 0.08f)
            );
        }

        // Boat
        DrawRectangleRounded({boatX - 20, 400, 120, 40}, 0.4f, 6, BROWN);

        // Characters
        for (auto& c : characters) {

            DrawCircle(c.bounds.x + 35,
                       c.bounds.y + 55,
                       28,
                       Fade(BLACK, 0.25f));

            Color color = (c.type == "Monk") ? WHITE : RED;

            DrawCircle(c.bounds.x + 30,
                       c.bounds.y + 30,
                       30,
                       color);

            DrawText(c.type.c_str(),
                     c.bounds.x + 5,
                     c.bounds.y + 65,
                     12,
                     BLACK);
        }

        DrawInteractiveButton(moveButton, "Move Boat", DARKGRAY);
        DrawInteractiveButton(finishButton, "Finish", DARKBLUE);

        // Step Counter
        DrawText(TextFormat("Steps: %d", stepCounter), 820, 20, 20, BLACK);

        // Overlay
        if (gameOver || win) {

            Color overlayColor = gameOver ? Fade(RED, 0.5f)
                                          : Fade(GREEN, 0.5f);

            DrawRectangle(0, 90, screenWidth,
                          screenHeight - 90,
                          overlayColor);

            DrawText(gameOver ? "GAME OVER" : "YOU WIN!",
                     360, 260, 50, WHITE);
        }

        DrawInteractiveButton(restartButton, "Restart", MAROON);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
