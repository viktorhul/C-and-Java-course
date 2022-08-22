#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
    Obligatoriska:
    [x] 5.1.1
    [x] 5.1.2
    [x] 5.1.3
    [x] 5.1.4
    [/] 5.1.5
    [x] 5.1.6
    [x] 5.1.7
    [ ] 5.1.8
    [ ] 5.1.9

    Frivilliga:
    [ ] 5.1.10
    [ ] 5.1.11
    [ ] 5.1.12
    [ ] 5.1.13
    [ ] 5.1.14


*/

/*
struct point {
    int x;
    int y;
};
typedef struct point point_t;*/
typedef struct {
    int x;
    int y;
} point_t;

// 5.1.3
/*
struct rectangle {
    point_t upper_left;
    point_t lower_right;
};
// 5.1.4
typedef struct rectangle rectangle_t;*/
typedef struct {
    point_t upper_left;
    point_t lower_right;
} rectangle_t;


// 5.1.5
void print_rect(rectangle_t *r) {
    printf("rectangle(upper_left=(%d,%d), lower_right=(%d,%d))\n", 
        r->upper_left.x, 
        r->upper_left.y, 
        r->lower_right.x, 
        r->lower_right.y
    );
}

// 5.1.5 -- BEHÖVER FIXAS
/*
void print_rect(rectangle_t *r) {
    printf("rectangle(");
    print_point(&upper_left);
    printf(", ");
    print_point(r->lower_right);
    printf(")");
}*/

void translate(point_t *p1, point_t *p2) {
    p1->x += p2->x;
    p1->y += p2->y;
}

void print_point(point_t *p) {
    printf("(%d,%d)", p->x, p->y);
}

point_t make_point(int x, int y) {
    point_t p = { .x = x, .y = y};
    return p;
}

// 5.1.6
rectangle_t make_rect(int x1, int y1, int x2, int y2) {
    rectangle_t r = {
        upper_left: { x: x1, y: y1 },
        lower_right: { x: x2, y: y2 }
    };
    return r;
}

// 5.1.7
double area_rect(rectangle_t *r) {
    int base = r->lower_right.x - r->upper_left.x;
    int height = r->upper_left.y - r->lower_right.y;
    return (base * height);
}

// 5.1.8
bool intersects_rect(rectangle_t *r1, rectangle_t *r2) {
    int r1x1 = r1->upper_left.x;
    int r1y1 = r1->upper_left.y;
    int r1x2 = r1->lower_right.x;
    int r1y2 = r1->lower_right.y;

    int r2x1 = r1->upper_left.x;
    int r2y1 = r1->upper_left.y;
    int r2x2 = r1->lower_right.x;
    int r2y2 = r1->lower_right.y;

    if (r1x1 >= r2x2 || r2x1 >= r1x2) return false;
    if (r1y2 >= r2y1 || r2y2 >= r1y1) return false;

    return true;

    //return (r1x1 < r2x2 && r1x2 > r2x1 && r1y1 < r2y2 && r1y2 > r2y1);
}

int main(void) {
    // 5.1.1
    point_t p1 = { .x = 5, .y = 25 };
    print_point(&p1);
    printf("\n");

    // 5.1.2
    point_t p2 = make_point(10, 10);
    print_point(&p2);
    printf("\n");

    // 5.1.5 alt 1
    rectangle_t r1;
    r1.upper_left.x = 5;
    r1.upper_left.y = 10;
    r1.lower_right.x = 10;
    r1.lower_right.y = 5;
    print_rect(&r1);

    // 5.1.5 alt 2
    rectangle_t r2 = { -4, 4, 0, 0 };
    print_rect(&r2);

    // 5.1.5 alt 3
    rectangle_t r3 = {
        upper_left: { x: 2, y: -3 },
        lower_right: { x: 7, y: -6 }
    };
    print_rect(&r3);

    // 5.1.6
    rectangle_t r4 = make_rect(-10, -2, -4, -8);
    print_rect(&r4);

    // 5.1.7
    double r1_area = area_rect(&r1);
    printf("%f\n", r1_area);
    double r2_area = area_rect(&r2);
    printf("%f\n", r2_area);
    double r3_area = area_rect(&r3);
    printf("%f\n", r3_area);
    double r4_area = area_rect(&r4);
    printf("%f\n", r4_area);

    // 5.1.8
    rectangle_t r5 = make_rect(6, -2, 8, -5);
    bool r1r2_bool = intersects_rect(&r4, &r5);
    if (r1r2_bool) printf("yes");

    return 0;
}