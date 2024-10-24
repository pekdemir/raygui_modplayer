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
    
    InitWindow(408, 360, "raygui - controls test suite");
    SetTargetFPS(60);

    InitAudioDevice();

    ctx = xmp_create_context();

    if (xmp_load_module(ctx, "test.it") < 0) {
            fprintf(stderr, "%s: error loading %s\n", "test", "test.it");
            return 1;
    }

    struct xmp_module_info mi;
    struct xmp_frame_info fi;
    xmp_get_module_info(ctx, &mi);

    AudioStream stream = LoadAudioStream(44100, 16, NUM_CHANNEL);
    SetAudioStreamCallback(stream, fillBuffer);
    xmp_start_player(ctx, 44100, 0);



    bool ButtonPrevPressed = false;
    bool ButtonPlayPausePressed = false;
    bool ButtonStopPressed = false;
    bool ButtonNextPressed = false;
    float SliderBarValue = 0.0f;
    Rectangle ScrollPanelScrollView = { 0, 0, 0, 0 };
    Vector2 ScrollPanelScrollOffset = { 0, 0 };
    Vector2 ScrollPanelBoundsOffset = { 0, 0 };
    //GuiLoadStyleCyber();

    while (!WindowShouldClose())
    {
        xmp_get_frame_info(ctx, &fi);
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            GuiGroupBox((Rectangle){ 0, 8, 216, 104 }, "Module Info");
            GuiGroupBox((Rectangle){ 216, 8, 192, 104 }, "Player");
            ButtonPrevPressed = GuiButton((Rectangle){ 240, 16, 24, 40 }, "#129#"); 
            GuiLabel((Rectangle){ 8, 16, 120, 24 }, "SAMPLE TEXT");
            GuiLabel((Rectangle){ 8, 40, 120, 24 }, "SAMPLE TEXT");
            GuiLabel((Rectangle){ 8, 64, 120, 24 }, "SAMPLE TEXT");
            GuiLabel((Rectangle){ 8, 88, 120, 24 }, "SAMPLE TEXT");
            ButtonPlayPausePressed = GuiButton((Rectangle){ 264, 16, 48, 40 }, playing ? "#132#" : "#131#"); 
            ButtonStopPressed = GuiButton((Rectangle){ 312, 16, 48, 40 }, "#133#"); 
            ButtonNextPressed = GuiButton((Rectangle){ 360, 16, 24, 40 }, "#134#"); 
            GuiSliderBar((Rectangle){ 240, 64, 144, 40 }, NULL, NULL, &SliderBarValue, 0, 100);
            GuiGroupBox((Rectangle){ 0, 120, 408, 240 }, "Playlist");
            GuiScrollPanel((Rectangle){ 8, 128, 392 - ScrollPanelBoundsOffset.x, 200 - ScrollPanelBoundsOffset.y }, NULL, (Rectangle){ 8, 128, 392, 200 }, &ScrollPanelScrollOffset, &ScrollPanelScrollView);
            GuiStatusBar((Rectangle){ 0, 336, 408, 24 }, "Status");


            if (ButtonPlayPausePressed) 
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

            if (ButtonStopPressed)
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