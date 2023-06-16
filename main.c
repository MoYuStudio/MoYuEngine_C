
#include "raylib.h"

int main(void)
{
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "MoYuEngine");

    //SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("test", 190, 200, 20, LIGHTGRAY);

            DrawFPS(10, 10);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}

