
#include "raylib.h"

float GenerateNoise(int x, int y, int seed)
{
    int n = x + y * 57 + seed * 131;
    n = (n << 13) ^ n;
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7FFFFFFF) / 1073741824.0f);
}

void GenerateMap(Texture2D texture, int mapSize, float tileSize, int seed)
{
    for (int y = 0; y < mapSize; y++)
    {
        for (int x = 0; x < mapSize; x++)
        {
            float noise = GenerateNoise(x, y, seed);
            float tileX = (x - y) * tileSize / 2.0f + noise * tileSize / 2.0f;
            float tileY = (x + y) * tileSize / 4.0f + noise * tileSize / 4.0f;
            Vector2 position = { tileX, tileY };
            DrawTextureEx(texture, position, 0.0f, 1.0f, WHITE);
        }
    }
}
int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "MoYuEngine");

    Image icon = LoadImage("assets/block/1.png");
    SetWindowIcon(icon);
    UnloadImage(icon);

    Image image = LoadImage("assets/block/1.png");
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    Vector2 position = { 0.0f, 0.0f };

    const float speed = 200.0f;

    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0.0f, 0.0f };
    camera.offset = (Vector2){ screenWidth / 2, screenHeight / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 7.0f;

    const int mapSize = 9;
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

                GenerateMap(texture, mapSize, tileSize, seed);

            EndMode2D();

            DrawText("MoYuEngine Testing Ver", 550, 10, 20, LIGHTGRAY);
            DrawFPS(10, 10);

        EndDrawing();
    }

    UnloadTexture(texture);

    CloseWindow();
    return 0;
}
