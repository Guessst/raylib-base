#include "raylib.h"

int main(void)
{
    const int screen_width = 800;
    const int screen_h = 450;

    InitWindow(screen_width, screen_h, "GAME");

    SetTargetFPS(0);
    
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}