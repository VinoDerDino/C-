#ifndef RENDERER_h
#define RENDERER_h

struct ivec2 {
    int x,
        y;
};

struct fvec2 {
    float x,
          y;
};

struct line {
    int x1,
        y1,
        x2,
        y2;
};

struct wall {
    struct line l1,
                l2,
                l3,
                l4;
};

struct sector {
    int id;
    size_t firstwall, nwalls;
    float zfloor, zceil;
};

#endif