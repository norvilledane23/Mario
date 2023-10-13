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

struct platform {
    int x1, y1, x2, y2;
};

// Structure for player (add points for their left and right foot)
// Make the screen a box

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900
#define BOTTOM_OF_SCREEN 900
bool mouse;
bool inside_platform;
bool player_on_ground;

// Keyboard variables
int btn_left = 0;
int btn_right = 0;
int btn_up = 0;

// Coordinate variables
float mouse_x;
float mouse_y;
float player_x = 24;
float player_y = BOTTOM_OF_SCREEN;
float vx = 0;
float vy = 0;

uint64_t frame_start_time;
gx_sprite player;
gx_sprite platform;
struct platform platform_1;

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
    //sapp_show_mouse(false); // Remove original computer mouse cursor

    // Entity sprites
    player = gx_make_sprite("player.png");
    platform = gx_make_sprite("platform_1.png");

    platform_1.x1 = 120;
    platform_1.y1 = BOTTOM_OF_SCREEN - 60 - 16;
    platform_1.x2 = 120 + 96;
    platform_1.y2 = BOTTOM_OF_SCREEN - 60;
    player_on_ground = true;
    inside_platform = false;
}

void tick(void) {
    player_on_ground = false;

    if (btn_right == 1) {
        vx += 0.1;
    }

    if (btn_left == 1) {
        vx -= 0.1;
    }

    if (btn_up == 1) {
        if (player_on_ground)
        {
            vy -= 20;
            //player_on_ground = false;
        }
    }

    // Screen borders
    // Horizontal border
    if (player_x < 24) {
        player_x = 24;
        vx = 0;
    }

    if (player_x > (SCREEN_WIDTH - 24)) {
        player_x = SCREEN_WIDTH - 24;
        vx = 0;
    }

    // Vertical border
    if (player_y > BOTTOM_OF_SCREEN) {
        player_y = BOTTOM_OF_SCREEN;
        player_on_ground = true;
        if (vy > 0) {
            vy = 0;
        }
    }

    // Gravity
    if (!player_on_ground) {
        vy += 0.8;
    }

    vx *= 0.6;
}

void platform_collision(void) {
    //player_on_ground = false;

    if (player_x > platform_1.x1 && player_x < platform_1.x2 && player_y > platform_1.y1 && player_y < platform_1.y2) {
        inside_platform = true;
    }

    if (inside_platform) {
        player_y = platform_1.y1;
        //player_on_ground = true;
    }
}

void frame(void) {
    // Animation FPS Calculations
    int target_fps = 30;
    uint64_t frame_duration = stm_since(frame_start_time);
    int frame_duration_ms = stm_ms(frame_duration);
    int target_frame_duration_ms = 1000 / target_fps;
    int need_to_sleep_ms = target_frame_duration_ms - frame_duration_ms;

    tick();
    platform_collision();
    

    player_x += vx;
    player_y += vy;

    gx_begin_drawing();

    gx_draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (sg_color) { 0.4f, 1.0f, 0.6f, 0.75f });
    gx_draw_sprite(platform_1.x1, platform_1.y1, &platform);
    gx_draw_sprite(player_x - (player.width / 2), player_y - 120, &player);

    sdtx_draw();
    gx_end_drawing();
}

void cleanup(void) {
    gx_shutdown();
    sg_shutdown();
}

static void event(const sapp_event* ev) {
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
            btn_right = 1;
            break;

        case SAPP_KEYCODE_LEFT:
            btn_left = 1;
            break;

        case SAPP_KEYCODE_UP:
            btn_up = 1;
            break;

        default:
            break;
        }
        break;

    case SAPP_EVENTTYPE_KEY_UP:
        switch (ev->key_code) {
        case SAPP_KEYCODE_RIGHT:
            btn_right = 0;
            break;

        case SAPP_KEYCODE_LEFT:
            btn_left = 0;
            break;

        case SAPP_KEYCODE_UP:
            btn_up = 0;
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
