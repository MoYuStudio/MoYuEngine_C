
#include "raylib.h"

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

    const int mapSize = 9; // 地图尺寸
    const float tileSize = 16.0f; // 格子大小

    //SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // 处理相机移动
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

                // 绘制地图
                for (int y = 0; y < mapSize; y++)
                {
                    for (int x = 0; x < mapSize; x++)
                    {
                        // 计算每个格子的位置
                        float tileX = (x - y) * tileSize / 2.0f;
                        float tileY = (x + y) * tileSize / 4.0f;
                        Vector2 position = { tileX, tileY };
                        DrawTextureEx(texture, position, 0.0f, 1.0f, WHITE);
                    }
                }

            EndMode2D();

            DrawText("MoYuEngine Testing Ver", 550, 10, 20, LIGHTGRAY);
            DrawFPS(10, 10);

        EndDrawing();
    }

    UnloadTexture(texture);

    CloseWindow();
    return 0;
}
