
#include "stdio.h"

#include "raylib.h"

#include "map_generator.h"

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "MoYuEngine");

    Image icon = LoadImage("assets/block/1.png");
    SetWindowIcon(icon);
    UnloadImage(icon);

    Texture2D textures[6];

    for (int i = 0; i <= 5; i++)
    {
        char filePath[20];
        sprintf(filePath, "assets/block/%d.png", i);
        Image image = LoadImage(filePath);
        textures[i] = LoadTextureFromImage(image);
        UnloadImage(image);
    }

    Vector2 position = { 0.0f, 0.0f };

    const float speed = 300.0f;

    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0.0f, 0.0f };
    camera.offset = (Vector2){ screenWidth / 2, screenHeight / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;

    const int mapSize = 64;
    const float tileSize = 16.0f;
    const int seed = 0; // 设置一个种子值

    //SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_W))
            camera.target.y += tileSize * camera.zoom * GetFrameTime();
        if (IsKeyDown(KEY_S))
            camera.target.y -= tileSize * camera.zoom * GetFrameTime();
        if (IsKeyDown(KEY_A))
            camera.target.x += tileSize * camera.zoom * GetFrameTime();
        if (IsKeyDown(KEY_D))
            camera.target.x -= tileSize * camera.zoom * GetFrameTime();

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode2D(camera);

                GenerateMap(textures, mapSize, tileSize, seed);

            EndMode2D();

            DrawText("MoYuEngine Testing Ver", 550, 10, 20, LIGHTGRAY);
            DrawFPS(10, 10);

        EndDrawing();
    }

    for (int i = 0; i <= 5; i++)
    {
        UnloadTexture(textures[i]);
    }

    CloseWindow();
    return 0;
}
