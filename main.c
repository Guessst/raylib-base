#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ACTION_TIMER_IN_MS 150

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define SCREEN_WIDTH_IN_BLOCKS 12
#define SCREEN_HEIGHT_IN_BLOCKS 10

#define BLOCK_WIDTH (SCREEN_WIDTH / SCREEN_WIDTH_IN_BLOCKS)
#define BLOCK_HEIGHT (SCREEN_HEIGHT / SCREEN_HEIGHT_IN_BLOCKS)
#define PADDING 0.2f

#define PALETTE_COLOR_1 CLITERAL(Color){0x0f, 0x82, 0x8c, 0xff} // #0F828C
#define PALETTE_COLOR_2 CLITERAL(Color){0x78, 0xB9, 0xB5, 0xff} // #78B9B5
#define PALETTE_COLOR_3 CLITERAL(Color){0x00, 0x40, 0x30, 0xff} // #004030
#define PALETTE_COLOR_4 CLITERAL(Color){0x4E, 0x36, 0x36, 0xff} // #4E3636
#define PALETTE_COLOR_5 CLITERAL(Color){0xCD, 0x18, 0x18, 0xff} // #CD1818
#define PALETTE_COLOR_6 CLITERAL(Color){0x32, 0x1E, 0x1E, 0xff} // #321E1E

#define PLAYER_HEAD_COLOR PALETTE_COLOR_1
#define PLAYER_TAIL_COLOR PALETTE_COLOR_2
#define PLAYER_DEAD_COLOR PALETTE_COLOR_3
#define BLOCK_COLOR PALETTE_COLOR_4
#define FRUIT_COLOR PALETTE_COLOR_5
#define BACKGROUND_COLOR PALETTE_COLOR_6

typedef struct
{
    int x;
    int y;
} Vector2_int;

typedef struct
{
    Vector2_int position;
    int curr_tail_size;
    Vector2_int tail[SCREEN_HEIGHT_IN_BLOCKS * SCREEN_WIDTH_IN_BLOCKS];
} Player_t;

typedef struct
{
    Vector2_int position;
} Fruit_t;

void draw_screen(Player_t player, Fruit_t fruit, bool gameover)
{
    int padding_w_px = (int)(0.5f * PADDING * BLOCK_WIDTH);
    int padding_h_px = (int)(0.5f * PADDING * BLOCK_HEIGHT);

    int screen_block_w = (int)((1.0f - PADDING) * BLOCK_WIDTH);
    int screen_block_h = (int)((1.0f - PADDING) * BLOCK_HEIGHT);

    // draw blocks
    for (int i = 0; i < SCREEN_WIDTH_IN_BLOCKS; ++i)
    {
        for (int j = 0; j < SCREEN_WIDTH_IN_BLOCKS; ++j)
        {
            Vector2_int screen_block = {
                .x = j * BLOCK_WIDTH,
                .y = i * BLOCK_HEIGHT};

            DrawRectangle(
                screen_block.x + padding_w_px,
                screen_block.y + padding_h_px,
                screen_block_w,
                screen_block_h,
                BLOCK_COLOR);
        }
    }

    // draw tail blocks
    for (int i = 0; i < player.curr_tail_size; ++i)
    {
        Vector2_int tail_rect = {
            .x = player.tail[i].x * BLOCK_WIDTH,
            .y = player.tail[i].y * BLOCK_HEIGHT};
        DrawRectangle(
            tail_rect.x + padding_w_px,
            tail_rect.y + padding_h_px,
            screen_block_w,
            screen_block_h,
            PLAYER_TAIL_COLOR);
    }

    // draw player block
    Vector2_int player_rect = {
        .x = player.position.x * BLOCK_WIDTH,
        .y = player.position.y * BLOCK_HEIGHT};

    DrawRectangle(
        player_rect.x + padding_w_px,
        player_rect.y + padding_h_px,
        screen_block_w,
        screen_block_h,
        (gameover ? PLAYER_DEAD_COLOR : PLAYER_HEAD_COLOR));

    // draw fruit block
    Vector2_int fruit_rect = {
        .x = fruit.position.x * BLOCK_WIDTH,
        .y = fruit.position.y * BLOCK_HEIGHT};

    DrawRectangle(
        fruit_rect.x + padding_w_px,
        fruit_rect.y + padding_h_px,
        screen_block_w,
        screen_block_h,
        FRUIT_COLOR);
}

// input and game behaviour
void process_input(float frametime, Player_t *player, Fruit_t *fruit, float *action_timer, int *key_pressed, long long unsigned *score, bool *gameover)
{
    int just_pressed = GetKeyPressed();
    if (just_pressed == KEY_UP || just_pressed == KEY_RIGHT || just_pressed == KEY_DOWN || just_pressed == KEY_LEFT)
    {
        *key_pressed = just_pressed;
    }

    if (*action_timer > 0)
        *action_timer -= frametime;
    else
    {
        Vector2_int dir;
        switch (*key_pressed)
        {
        case KEY_UP:
        {
            dir.x = 0;
            dir.y = -1;
            break;
        }

        case KEY_RIGHT:
        {
            dir.x = 1;
            dir.y = 0;
            break;
        }

        case KEY_LEFT:
        {
            dir.x = -1;
            dir.y = 0;
            break;
        }

        case KEY_DOWN:
        {
            dir.x = 0;
            dir.y = 1;
            break;
        }

        default:
            exit(-1);
            break;
        }
        // will collect the fruit
        if (player->position.x + dir.x == fruit->position.x && player->position.y + dir.y == fruit->position.y)
        {
            player->curr_tail_size++;

            // move tail
            for (int i = player->curr_tail_size; i > 0; --i)
            {
                player->tail[i].x = player->tail[i - 1].x;
                player->tail[i].y = player->tail[i - 1].y;
            }
            player->tail[0].x = player->position.x;
            player->tail[0].y = player->position.y;

            player->position.x += dir.x;
            player->position.y += dir.y;

            int randx;
            int randy;
            while (true)
            {
                bool valid = true;
                randx = rand() % SCREEN_WIDTH_IN_BLOCKS;
                randy = rand() % SCREEN_HEIGHT_IN_BLOCKS;

                valid = !(player->position.x == randx && player->position.y == randy);
                for (int i = 0; i < player->curr_tail_size; ++i)
                {
                    valid = valid && !(player->tail[i].x == randx && player->tail[i].y == randy);
                }
                if (valid)
                    break;
            }

            fruit->position.x = randx;
            fruit->position.y = randy;
            *score += 1;
        }
        else
        {
            // move tail
            for (int i = player->curr_tail_size; i > 0; --i)
            {
                player->tail[i].x = player->tail[i - 1].x;
                player->tail[i].y = player->tail[i - 1].y;
            }
            player->tail[0].x = player->position.x;
            player->tail[0].y = player->position.y;

            // move head
            player->position.x += dir.x;
            if (player->position.x == SCREEN_WIDTH_IN_BLOCKS)
                player->position.x = 0;
            else if (player->position.x < 0)
                player->position.x = SCREEN_WIDTH_IN_BLOCKS - 1;

            player->position.y += dir.y;
            if (player->position.y == SCREEN_HEIGHT_IN_BLOCKS)
                player->position.y = 0;
            else if (player->position.y < 0)
                player->position.y = SCREEN_HEIGHT_IN_BLOCKS - 1;

            bool collided_with_tail = false;
            for (int i = 0; i < player->curr_tail_size; ++i)
            {
                int px = player->position.x;
                int py = player->position.y;
                if (px == player->tail[i].x && py == player->tail[i].y)
                {
                    collided_with_tail = true;
                    break;
                }
            }

            if (collided_with_tail)
            {
                *gameover = true;
                // printf("gameover\n");
                return;
            }
        }

        *action_timer = (MAX_ACTION_TIMER_IN_MS / 1000.0f) - (frametime - *action_timer);
    }
}

int main()
{
    srand((unsigned int)time(NULL));
    float action_timer = MAX_ACTION_TIMER_IN_MS / 1000.0f;
    int key_pressed = KEY_UP;
    bool gameover = false;
    long long unsigned score = 0;
    long long unsigned max_score = 0;

    Player_t player = {
        .position = {.x = 5, .y = 5},
        .curr_tail_size = 0,
    };

    Fruit_t fruit = {
        .position = {.x = 2, .y = 2},
    };

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SNAKE");

    SetTargetFPS(0);

    // Main game loop
    while (!WindowShouldClose())
    {
        float frametime = GetFrameTime();
        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);
        if (!gameover)
        {
            process_input(frametime, &player, &fruit, &action_timer, &key_pressed, &score, &gameover);
        }

        // printf("%s", fruit ? "collected\n" : "");
        draw_screen(player, fruit, gameover);

        int font_size = 20;
        const char *score_txt = TextFormat("SCORE: %llu", score);
        const char *max_score_txt = TextFormat("MAX: %llu", max_score);
        DrawFPS(0, 0);
        DrawText(score_txt, 0, 20, font_size, YELLOW);
        DrawText(max_score_txt, 0, 40, font_size, YELLOW);

        if (gameover)
        {
            const char *txt = "Press R to restart";
            int txt_size = MeasureText(txt, font_size);
            DrawText(txt, SCREEN_WIDTH / 2 - txt_size / 2, SCREEN_HEIGHT / 2, font_size, RAYWHITE);

            if (GetKeyPressed() == KEY_R)
            {
                player.curr_tail_size = 0;
                player.position.x = 5;
                player.position.y = 5;
                fruit.position.x = 2;
                fruit.position.y = 2;
                key_pressed = KEY_UP;
                if (score > max_score)
                    max_score = score;
                score = 0;
                gameover = false;
            }
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}