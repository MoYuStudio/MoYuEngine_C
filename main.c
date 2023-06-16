
#include "stdio.h"

#include "raylib.h"

float GenerateNoise(int x, int y, int seed)
{
    int freq = 12;
    int octaves = 2;

    float noise = 0.0f;

    for (int i = 0; i < octaves; i++)
    {
        int n = x + y * 57 + seed * 131;
        n = (n << 13) ^ n;
        float octaveNoise = (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7FFFFFFF) / 1073741824.0f);

        noise += octaveNoise * freq;
        freq *= 2; // 增加频率
    }

    // 将噪声值缩放到[0, 1000]范围并减缓过度
    int scaledNoise = (int)(noise * 1000 + 1000);

    return scaledNoise;
}


void GenerateMap(Texture2D* textures, int mapSize, float tileSize, int seed)
{
    for (int y = 0; y < mapSize; y++)
    {
        for (int x = 0; x < mapSize; x++)
        {
            float noise = GenerateNoise(x, y, seed);
            int imageIndex = 0;
            if (noise <= 500.0f)
            {
                imageIndex = 3;
            }
            else if (noise <= 700.0f)
            {
                imageIndex = 1;
            }
            else if (noise <= 1000.0f)
            {
                imageIndex = 2;
            }
            else if (noise <= 2000.0f)
            {
                imageIndex = 4;
            }
            else
            {
                imageIndex = 5;
            }
            
            Texture2D texture = textures[imageIndex];
            float tileX = (x - y) * tileSize / 2.0f;
            float tileY = (x + y) * tileSize / 4.0f;
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
