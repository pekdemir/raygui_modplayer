#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "../examples/styles/style_cyber.h"

#include <xmp.h>

#define NUM_CHANNEL 2

xmp_context ctx = NULL;

static int playing = 0;

//AudioCallback
void fillBuffer(void *bufferData, unsigned int frames)
{
    if(xmp_play_buffer(ctx, bufferData, frames * sizeof(short) * NUM_CHANNEL, 0) < 0)
    {
        playing = 0;
    }
}

int main()
{
    
    InitWindow(400, 200, "raygui - controls test suite");
    SetTargetFPS(60);

    InitAudioDevice();

    ctx = xmp_create_context();

    if (xmp_load_module(ctx, "test.it") < 0) {
            fprintf(stderr, "%s: error loading %s\n", "test", "test.it");
            return 1;
    }

    struct xmp_module_info mi;
    xmp_get_module_info(ctx, &mi);

    AudioStream stream = LoadAudioStream(44100, 16, NUM_CHANNEL);
    SetAudioStreamCallback(stream, fillBuffer);
    xmp_start_player(ctx, 44100, 0);


    GuiLoadStyleCyber();

    while (!WindowShouldClose())
    {
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            if (GuiButton((Rectangle){ 30, 30, 50, 50 }, playing ? "#132#" : "#131#")) 
            {
                // int result = GuiMessageBox((Rectangle){ 85, 70, 250, 100 },
                //     "#191#Message Box", "Hi! This is a message!", "Nice;Cool");

                // if (result >= 0) showMessageBox = false;
                playing = !playing;
                if (playing)
                {
                    PlayAudioStream(stream);
                }
                else
                {
                    PauseAudioStream(stream);
                }
            }

            if (GuiButton((Rectangle){ 85, 30, 50, 50 }, "#133#"))
            {
                StopAudioStream(stream);
                xmp_stop_module(ctx);
                xmp_restart_module(ctx);
                xmp_start_player(ctx, 44100, 0);
                playing = 0;
            }
        EndDrawing();
    
    }

    xmp_release_module(ctx);
    xmp_free_context(ctx);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}