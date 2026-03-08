#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"
#include <vector>
#include <string>

enum Side { LEFT, RIGHT, BOAT };

struct Character {
    std::string type;
    Side side;
    Rectangle bounds;
};

void SetGameFont(Font font);

void DrawStartScreen(int screenWidth, int screenHeight);

void DrawGameScene(
    int screenWidth,
    int screenHeight,
    float waterTime,
    float boatX,
    std::vector<Character>& characters,
    Texture2D demonTexture,
    Texture2D monkTexture,
    Texture2D boatTexture
);

void DrawUI(
    Rectangle moveButton,
    Rectangle restartButton,
    Rectangle finishButton,
    Rectangle menuButton,
    int stepCounter,
    bool gameOver,
    bool win
);

void DrawInteractiveButton(Rectangle rect, const char* text, Color baseColor);

#endif
