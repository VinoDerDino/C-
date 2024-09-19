#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define SCREEN_WIDTH 854
#define SCREEN_HEIGHT 480
#define PI 3.1415926
#define FOV 20

typedef float f32;
typedef double f64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef size_t usize;

#define ASSERT(_e, ...)               \
    if (!(_e))                        \
    {                                 \
        fprintf(stderr, __VA_ARGS__); \
        exit(1);                      \
    }
#define FOV_TO_RAD(_e) (_e * (PI / 180))

typedef struct v2_s
{
    int x, y;
} v2;
typedef struct v2_f_s
{
    float x, y;
} v2_f;

struct wall
{
    v2 a, b;
    int portal;
};

struct sector
{
    int id;
    size_t firstwall, nwalls;
    f32 zfloor, zceil;
};

static struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture, *debug;
    uint32_t *pixels;

    struct
    {
        v2_f pos;
        f32 angle, anglecos, anglesin;
        int sector;
    } camera;

    struct
    {
        struct wall arr[128];
        int n;
    } walls;
    struct
    {
        struct sector arr[24];
        int n;
    } sectors;

    bool quit;

} state;

void calculateEndPoint(int startX, int startY, int length, float angle)
{
    double radians = angle * M_PI / 180.0;

    int endX = startX + length * cos(radians),
        endY = startY + length * sin(radians);

    SDL_RenderDrawLine(state.renderer, (int)(startX), (int)(startY), (int)(endX), (int)(endY));
}

void ver_line(int x, float distance, int wall)
{
    const int height = (int)(SCREEN_HEIGHT / distance);

    if (distance > 10)
        distance = 10.0;

    const float factor = 1.0 - (distance / 10.0);
    const int red = (int)(255 * factor),
              choice = wall % 3;

    if (choice == 0)
    {
        SDL_SetRenderDrawColor(state.renderer, 255, 0, 0, 255);
    }
    else if (choice == 1)
    {
        SDL_SetRenderDrawColor(state.renderer, 0, 255, 0, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(state.renderer, 0, 0, 255, 255);
    }
    SDL_RenderDrawLine(state.renderer, x, (int)(SCREEN_HEIGHT / 2) - height, x, (int)(SCREEN_HEIGHT / 2) + height); // wall
    SDL_SetRenderDrawColor(state.renderer, 190, 190, 190, 255);
    SDL_RenderDrawLine(state.renderer, x, 0, x, (int)(SCREEN_HEIGHT / 2) - height - 1); // ceil
    SDL_SetRenderDrawColor(state.renderer, 255, 170, 0, 255);
    SDL_RenderDrawLine(state.renderer, x, (int)(SCREEN_HEIGHT / 2) + height + 1, x, SCREEN_HEIGHT - 1); // floor
}

void allocate_pixels(int x, int y1, int y2, int color)
{
    for (int i = y1; i <= y2; i++)
    {
        state.pixels[i * SCREEN_WIDTH + x] = color;
    }
}

int print_map(float angle)
{
    SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
    for (int i = 0; i < 50; i++)
    {
        SDL_RenderDrawLine(state.renderer, i, 0, i, 50);
    }
    for (size_t i = state.sectors.arr[state.camera.sector].firstwall; i <= state.sectors.arr[state.camera.sector].firstwall + state.sectors.arr[state.camera.sector].nwalls; i++)
    {
        struct wall *w = &state.walls.arr[i];
        SDL_SetRenderDrawColor(state.renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(state.renderer, w->a.x * 5 + 10, w->a.y * 5 + 10, w->b.x * 5 + 10, w->b.y * 5 + 10);
    }
    SDL_SetRenderDrawColor(state.renderer, 0, 0, 255, 255);
    SDL_RenderDrawPoint(state.renderer, state.camera.pos.x * 5 + 10, state.camera.pos.y * 5 + 10);
    return 0;
}

int render()
{
    SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
    SDL_RenderClear(state.renderer);
    SDL_SetRenderDrawColor(state.renderer, 255, 0, 0, 255);

    struct sector *curr_sector = &state.sectors.arr[state.camera.sector];

    for (size_t y = 0; y < SCREEN_WIDTH; y++)
    {

        // float ti = state.camera.angle - (FOV_TO_RAD(FOV) / 2) + ((float)y / (float)SCREEN_WIDTH) * FOV_TO_RAD(FOV);

        float screen_half_width = SCREEN_WIDTH / 2;
        float ti = state.camera.angle + atan((y - screen_half_width) / screen_half_width / tan(FOV_TO_RAD(90) / 2));

        float cos_ti = cos(ti);
        float sin_ti = sin(ti);

        float last_t = -1.0;
        int last_wall = 0, portal = false;

        for (size_t i = curr_sector->firstwall; i < curr_sector->firstwall + curr_sector->nwalls; i++)
        {
            struct wall *w = &state.walls.arr[i];

            float detA = -cos_ti * (float)(w->b.y - w->a.y) + sin_ti * (float)(w->b.x - w->a.x);

            if (fabs(detA) < 1e-4) // Ist der ray parallel zur Wand?
                continue;

            float detAt = -(w->a.x - state.camera.pos.x) * (w->b.y - w->a.y) + (w->a.y - state.camera.pos.y) * (w->b.x - w->a.x);
            float detAu = cos_ti * (w->a.y - state.camera.pos.y) - sin_ti * (w->a.x - state.camera.pos.x);

            float t = detAt / detA;
            float u = detAu / detA;

            if (t >= 0 && u >= 0 && u <= 1)
            {
                int fx = state.camera.pos.x + (int)(t * cos_ti);
                int fy = state.camera.pos.y + (int)(t * sin_ti);

                if (last_t == -1.0)
                {
                    last_t = t;
                    last_wall = (int)i;

                    portal = w->portal;
                }

                if (t < last_t)
                {
                    last_t = t;
                    last_wall = (int)i;
                    portal = w->portal;
                }
            }
        }

        if (last_t == -1.0)
            continue;

        if (portal)
        {
            ver_line(y, last_t * cos(state.camera.angle - ti), last_wall);
        }
        else
        {
            ver_line(y, last_t * cos(state.camera.angle - ti), last_wall);
        }
    }
    // print_map(0.0);
}

static int load_sectors(const char *path)
{
    state.sectors.n = 1;

    FILE *f = fopen(path, "r");
    if (!f)
    {
        return -1;
    }

    int retval = 0;
    enum
    {
        SCAN_SECTOR,
        SCAN_WALL,
        SCAN_NONE
    } ss = SCAN_NONE;

    char line[1024], buf[64];
    while (fgets(line, sizeof(line), f))
    {
        const char *p = line;
        while (isspace(*p))
        {
            p++;
        }

        if (!*p || *p == '#')
        {
            continue;
        }
        else if (*p == '[')
        {
            strncpy(buf, p + 1, sizeof(buf));
            const char *section = strtok(buf, "]");
            if (!section)
            {
                retval = -2;
                goto done;
            }

            if (!strcmp(section, "SECTOR"))
            {
                ss = SCAN_SECTOR;
            }
            else if (!strcmp(section, "WALL"))
            {
                ss = SCAN_WALL;
            }
            else
            {
                retval = -3;
                goto done;
            }
        }
        else
        {
            switch (ss)
            {
            case SCAN_WALL:
            {
                struct wall *wall = &state.walls.arr[state.walls.n++];
                if (sscanf(
                        p,
                        "%d %d %d %d %d",
                        &wall->a.x,
                        &wall->a.y,
                        &wall->b.x,
                        &wall->b.y,
                        &wall->portal) != 5)
                {
                    retval = -4;
                    goto done;
                }
            };
            break;
            case SCAN_SECTOR:
            {
                struct sector *sector = &state.sectors.arr[state.sectors.n++];
                if (sscanf(
                        p,
                        "%d %zu %zu %f %f",
                        &sector->id,
                        &sector->firstwall,
                        &sector->nwalls,
                        &sector->zfloor,
                        &sector->zceil) != 5)
                {
                    retval = -5;
                    goto done;
                }
            };
            break;
            default:
                retval = -6;
                goto done;
            }
        }
    }

    if (ferror(f))
    {
        retval = -128;
        goto done;
    }
done:
    fclose(f);
    return retval;
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    state.window = SDL_CreateWindow(
        "Doom Renderer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    state.renderer = SDL_CreateRenderer(
        state.window,
        -1,
        SDL_RENDERER_ACCELERATED);

    state.texture = SDL_CreateTexture(
        state.renderer,
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH,
        SCREEN_HEIGHT);

    state.pixels = malloc(SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));

    state.camera.sector = 1;
    state.camera.pos.x = 3.0;
    state.camera.pos.y = 3.0;
    state.camera.angle = 0.0;

    int ret = 0;
    ASSERT(
        !(ret = load_sectors("res/level.txt")),
        "error while loading sectors: %d",
        ret);
    printf(
        "loaded %zu sectors with %zu walls",
        state.sectors.n,
        state.walls.n);

    while (!state.quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                state.quit = true;
            }
        }

        if (state.quit)
        {
            break;
        }

        const f32 rot_speed = 1.0f * 0.0016f;
        const f32 move_speed = 1.0f * 0.0016f;
        const u8 *keystate = SDL_GetKeyboardState(NULL);

        if (keystate[SDLK_RIGHT & 0xFFFF])
        {
            state.camera.angle += rot_speed;
        }

        if (keystate[SDLK_LEFT & 0xFFFF])
        {
            state.camera.angle -= rot_speed;
        }

        state.camera.anglecos = cos(state.camera.angle);
        state.camera.anglesin = sin(state.camera.angle);

        if (keystate[SDLK_UP & 0xFFFF])
        {
            state.camera.pos = (v2_f){
                state.camera.pos.x + (move_speed * state.camera.anglecos),
                state.camera.pos.y + (move_speed * state.camera.anglesin),
            };
        }

        if (keystate[SDLK_DOWN & 0xFFFF])
        {
            state.camera.pos = (v2_f){
                state.camera.pos.x - (move_speed * state.camera.anglecos),
                state.camera.pos.y - (move_speed * state.camera.anglesin),
            };
        }

        render();
        SDL_RenderPresent(state.renderer);
    }

    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyTexture(state.texture);
    SDL_DestroyWindow(state.window);
    SDL_Quit();

    return 0;
}
