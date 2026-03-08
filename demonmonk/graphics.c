#include "graphics.h"
#include <cmath>

static Font gameFont;

void SetGameFont(Font font)
{
    gameFont = font;
}

void DrawRiver(int screenHeight, float waterTime)
{
    for (int y = 0; y < screenHeight; y++)
    {
        float edgeWave = sinf(waterTime * 2.0f + y * 0.04f) * 6.0f;

        float leftEdge  = 250.0f + edgeWave;
        float rightEdge = 750.0f + edgeWave;

        DrawLineEx({leftEdge,(float)y},{rightEdge,(float)y},1.0f,BLUE);
    }

    for (int y = 0; y < screenHeight; y += 20)
    {
        float wave = sinf((y + waterTime * 100.0f) * 0.05f) * 10.0f;

        DrawRectangle(
            250 + wave,
            y,
            500,
            20,
            Fade(WHITE,0.08f)
        );
    }
}

void DrawBoat(float boatX, Texture2D boatTexture)
{
    DrawTextureEx(
        boatTexture,
        {boatX - 20.0f, 400.0f},
        0.0f,
        1.0f,
        WHITE
    );
}

void DrawCharacters(std::vector<Character>& characters,
                    Texture2D demonTexture,
                    Texture2D monkTexture)
{
    for (auto& c : characters)
    {
        DrawCircle(
            c.bounds.x + 35,
            c.bounds.y + 55,
            28,
            Fade(BLACK,0.25f)
        );

        if (c.type == "Demon")
        {
            DrawTextureEx(
                demonTexture,
                {c.bounds.x, c.bounds.y},
                0,
                1.0f,
                WHITE
            );
        }
        else
        {
            DrawTextureEx(
                monkTexture,
                {c.bounds.x, c.bounds.y},
                0,
                1.0f,
                WHITE
            );
        }

       
        
    }
}

void DrawInteractiveButton(Rectangle rect, const char* text, Color baseColor)
{
    Vector2 mouse = GetMousePosition();

    bool hovered = CheckCollisionPointRec(mouse, rect);
    bool pressed = hovered && IsMouseButtonDown(MOUSE_LEFT_BUTTON);

    Color color = baseColor;

    if (hovered) color = ColorBrightness(baseColor, -0.15f);
    if (pressed) color = ColorBrightness(baseColor, -0.30f);

    float offsetY = pressed ? 3.0f : 0.0f;

    DrawRectangleRounded(
        {rect.x, rect.y + offsetY, rect.width, rect.height},
        0.3f,
        8,
        color
    );

    int fontSize = 20;

    Vector2 textSize = MeasureTextEx(gameFont, text, fontSize, 1);

    DrawTextEx(
        gameFont,
        text,
        {
            rect.x + rect.width/2 - textSize.x/2,
            rect.y + rect.height/2 - fontSize/2 + offsetY
        },
        fontSize,
        1,
        WHITE
    );
}

void DrawStartScreen(int screenWidth, int screenHeight)
{
    ClearBackground(SKYBLUE);

    DrawTextEx(
        gameFont,
        "MONKS AND DEMONS",
        {300,200},
        50,
        2,
        DARKBLUE
    );

    Rectangle startButton =
    {
        screenWidth * 0.5f - 120.0f,
        screenHeight * 0.5f - 40.0f,
        240.0f,
        80.0f
    };

    DrawInteractiveButton(startButton,"START GAME",DARKGREEN);
}

void DrawGameScene(
    int screenWidth,
    int screenHeight,
    float waterTime,
    float boatX,
    std::vector<Character>& characters,
    Texture2D demonTexture,
    Texture2D monkTexture,
    Texture2D boatTexture
)
{
    ClearBackground({139, 90, 43, 255});

   
    DrawRiver(screenHeight,waterTime);

    DrawBoat(boatX, boatTexture);

    DrawCharacters(characters,demonTexture,monkTexture);
}

void DrawUI(
    Rectangle moveButton,
    Rectangle restartButton,
    Rectangle finishButton,
    Rectangle menuButton,
    int stepCounter,
    bool gameOver,
    bool win
)
{
    DrawInteractiveButton(moveButton,"Move Boat",DARKGRAY);
    DrawInteractiveButton(finishButton,"Finish",DARKBLUE);
    DrawInteractiveButton(menuButton,"<-",DARKPURPLE);

    DrawTextEx(
        gameFont,
        TextFormat("Steps: %d",stepCounter),
        {820,20},
        20,
        1,
        BLACK
    );

    if (gameOver || win)
    {
        Color overlayColor = gameOver ? Fade(RED,0.5f)
                                      : Fade(GREEN,0.5f);

        DrawRectangle(0,90,1000,510,overlayColor);

        DrawTextEx(
            gameFont,
            gameOver ? "GAME OVER" : "YOU WIN!",
            {360,260},
            50,
            2,
            WHITE
        );
    }

    DrawInteractiveButton(restartButton,"Restart",MAROON);
}
