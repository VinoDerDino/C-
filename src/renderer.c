#include "renderer.h"
#include <SDL2/SDL.h>
#include "renderer.h"

void rend_wall(struct wall w, SDL_Renderer* renderer) {
    SDL_RenderDrawLine(renderer, w.l1.x1, w.l1.y1, w.l1.x2, w.l1.y2);
    SDL_RenderDrawLine(renderer, w.l2.x1, w.l2.y1, w.l2.x2, w.l2.y2);
    SDL_RenderDrawLine(renderer, w.l3.x1, w.l3.y1, w.l3.x2, w.l3.y2);
    SDL_RenderDrawLine(renderer, w.l4.x1, w.l4.y1, w.l4.x2, w.l4.y2);
}