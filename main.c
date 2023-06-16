
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

    //SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_W))
            camera.target.y += speed * GetFrameTime();
        if (IsKeyDown(KEY_S))
            camera.target.y -= speed * GetFrameTime();
        if (IsKeyDown(KEY_A))
            camera.target.x += speed * GetFrameTime();
        if (IsKeyDown(KEY_D))
            camera.target.x -= speed * GetFrameTime();

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode2D(camera);

                DrawTextureEx(texture, position, 0.0f, 1.0f, WHITE);

            EndMode2D();

            DrawText("MoYuEngine Testing Ver", 550, 10, 20, LIGHTGRAY);
            DrawFPS(10, 10);

        EndDrawing();
    }

    UnloadTexture(texture);

    CloseWindow();
    return 0;
}
