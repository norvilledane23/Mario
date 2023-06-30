#define SOKOL_D3D11
#define SOKOL_IMPL
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define GX_IMPL
#include "gx.h"
#include "sokol_time.h"
#include "sokol_debugtext.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900
bool mouse;

int left_pressed = 0;
int right_pressed = 0;
int up_pressed = 0;
int down_pressed = 0;

// Coordinate variables
float mouse_x;
float mouse_y;
float player_x = 60;
float player_y = 180;

uint64_t frame_start_time;

void init(void) {
    sg_setup(&(sg_desc) {
        .context = sapp_sgcontext()
    });

    // Text setup
    sdtx_setup(&(sdtx_desc_t) {
        .context_pool_size = 8,
        .printf_buf_size = 4096,
        .fonts = {
            [0] = sdtx_font_kc853(),
            [1] = sdtx_font_kc854(),
            [2] = sdtx_font_z1013(),
            [3] = sdtx_font_cpc(),
            [4] = sdtx_font_c64(),
            [5] = sdtx_font_oric()
        },
        .context = {
            .char_buf_size = 4096,
            .canvas_width = SCREEN_WIDTH,
            .canvas_height = SCREEN_HEIGHT,
            .tab_width = 4,
            .color_format = 0,
            .depth_format = 0,
            .sample_count = 0
        },
    });

    gx_setup();
    stm_setup();
    sapp_show_mouse(false); // Remove original computer mouse cursor

    // Entity sprites
    player = gx_make_sprite("player.png");
}

void frame(void) {
    // Animation FPS Calculations
    int target_fps = 48;
    uint64_t frame_duration = stm_since(frame_start_time); // Duration of the frame since sprite animation started
    int frame_duration_ms = stm_ms(frame_duration); // Converting frame duration to ms
    int target_frame_duration_ms = 1000 / target_fps;
    int need_to_sleep_ms = target_frame_duration_ms - frame_duration_ms; // For determining whether the frame played too fast
    
    if (right_pressed == 1) { // right
        player_x += right_border_check(player_x, player.width / 2);
    }

    if (left_pressed == 1) { // left
        player_x -= left_border_check(player_x, player.width / 2);;
    }

    if (down_pressed == 1) { // down
        player_y += bottom_border_check(player_y, player.height / 2);
    }

    if (up_pressed == 1) { // up
        player_y -= top_border_check(player_y, player.height / 2);
    }

    gx_begin_drawing();

    gx_draw_sprite(player_x - (player.width / 2), player_y - (player.height / 2), &player);

    sdtx_draw();
    gx_end_drawing();
}

void cleanup(void) {
    gx_shutdown();
    sg_shutdown();
}

static void event(const sapp_event* ev) {
    int j;
    switch (ev->type) {
    case SAPP_EVENTTYPE_MOUSE_DOWN:
        break;

    case SAPP_EVENTTYPE_MOUSE_UP:
        mouse = 0;
        break;

    case SAPP_EVENTTYPE_MOUSE_MOVE:
        mouse_x = ev->mouse_x;
        mouse_y = ev->mouse_y;
        break;

    case SAPP_EVENTTYPE_KEY_DOWN:
        switch (ev->key_code) {
        case SAPP_KEYCODE_RIGHT:
            right_pressed = 1;
            break;

        case SAPP_KEYCODE_LEFT:
            left_pressed = 1; 
            break;

        case SAPP_KEYCODE_DOWN:
            down_pressed = 1;
            break;

        case SAPP_KEYCODE_UP:
            up_pressed = 1;
            break;
        
        default:
            break;
        }
        break;

    default:
        break;
    }

    case SAPP_EVENTTYPE_KEY_UP:
        switch (ev->key_code) {
        case SAPP_KEYCODE_RIGHT:
            right_pressed = 0;
            break;

        case SAPP_KEYCODE_LEFT:
            left_pressed = 0; 
            break;

        case SAPP_KEYCODE_DOWN:
            down_pressed = 0;
            break;

        case SAPP_KEYCODE_UP:
            up_pressed = 0;
            break;
        
        default:
            break;
        }
        break;

    default:
        break;
    }
}

sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    return (sapp_desc) {
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .width = SCREEN_WIDTH,
        .height = SCREEN_HEIGHT,
        .window_title = "test1",
    };
}
