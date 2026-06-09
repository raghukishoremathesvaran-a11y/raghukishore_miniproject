#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "graphics.h"

// Dynamic array initial capacity
#define INITIAL_CAPACITY 8

Canvas* create_canvas(int width, int height, char bg_char) {
    Canvas* canvas = (Canvas*)malloc(sizeof(Canvas));
    if (!canvas) return NULL;
    canvas->width = width;
    canvas->height = height;
    canvas->bg_char = bg_char;
    canvas->shape_count = 0;
    canvas->shape_capacity = INITIAL_CAPACITY;
    canvas->shapes = (Shape*)malloc(canvas->shape_capacity * sizeof(Shape));
    if (!canvas->shapes) {
        free(canvas);
        return NULL;
    }
    return canvas;
}

void free_canvas(Canvas* canvas) {
    if (canvas) {
        if (canvas->shapes) {
            free(canvas->shapes);
        }
        free(canvas);
    }
}

int add_shape(Canvas* canvas, Shape shape) {
    if (!canvas) return -1;
    if (canvas->shape_count >= canvas->shape_capacity) {
        canvas->shape_capacity *= 2;
        Shape* temp = (Shape*)realloc(canvas->shapes, canvas->shape_capacity * sizeof(Shape));
        if (!temp) return -1;
        canvas->shapes = temp;
    }
    canvas->shapes[canvas->shape_count] = shape;
    return canvas->shape_count++;
}

int delete_shape(Canvas* canvas, int index) {
    if (!canvas || index < 0 || index >= canvas->shape_count) return 0;
    for (int i = index; i < canvas->shape_count - 1; i++) {
        canvas->shapes[i] = canvas->shapes[i + 1];
    }
    canvas->shape_count--;
    return 1;
}

int modify_shape(Canvas* canvas, int index, Shape shape) {
    if (!canvas || index < 0 || index >= canvas->shape_count) return 0;
    canvas->shapes[index] = shape;
    return 1;
}

// Drawing helper functions
static void draw_line(int x1, int y1, int x2, int y2, char draw_char, char** grid, int width, int height) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) {
            grid[y1][x1] = draw_char;
        }
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

static void draw_rectangle(int x, int y, int w, int h, int fill, char draw_char, char** grid, int width, int height) {
    if (w <= 0 || h <= 0) return;
    if (fill) {
        for (int r = y; r < y + h; r++) {
            if (r >= 0 && r < height) {
                for (int c = x; c < x + w; c++) {
                    if (c >= 0 && c < width) {
                        grid[r][c] = draw_char;
                    }
                }
            }
        }
    } else {
        // top and bottom
        for (int c = x; c < x + w; c++) {
            if (c >= 0 && c < width) {
                if (y >= 0 && y < height) grid[y][c] = draw_char;
                if (y + h - 1 >= 0 && y + h - 1 < height) grid[y + h - 1][c] = draw_char;
            }
        }
        // left and right
        for (int r = y; r < y + h; r++) {
            if (r >= 0 && r < height) {
                if (x >= 0 && x < width) grid[r][x] = draw_char;
                if (x + w - 1 >= 0 && x + w - 1 < width) grid[r][x + w - 1] = draw_char;
            }
        }
    }
}

static void plot_circle_points(int cx, int cy, int x, int y, char draw_char, char** grid, int width, int height) {
    int points[8][2] = {
        {cx + x, cy + y}, {cx - x, cy + y},
        {cx + x, cy - y}, {cx - x, cy - y},
        {cx + y, cy + x}, {cx - y, cy + x},
        {cx + y, cy - x}, {cx - y, cy - x}
    };
    for (int i = 0; i < 8; i++) {
        int px = points[i][0];
        int py = points[i][1];
        if (px >= 0 && px < width && py >= 0 && py < height) {
            grid[py][px] = draw_char;
        }
    }
}

static void plot_circle_filled_lines(int cx, int cy, int x, int y, char draw_char, char** grid, int width, int height) {
    for (int px = cx - x; px <= cx + x; px++) {
        if (px >= 0 && px < width) {
            if (cy + y >= 0 && cy + y < height) grid[cy + y][px] = draw_char;
            if (cy - y >= 0 && cy - y < height) grid[cy - y][px] = draw_char;
        }
    }
    for (int px = cx - y; px <= cx + y; px++) {
        if (px >= 0 && px < width) {
            if (cy + x >= 0 && cy + x < height) grid[cy + x][px] = draw_char;
            if (cy - x >= 0 && cy - x < height) grid[cy - x][px] = draw_char;
        }
    }
}

static void draw_circle(int cx, int cy, int r, int fill, char draw_char, char** grid, int width, int height) {
    if (r < 0) return;
    if (r == 0) {
        if (cx >= 0 && cx < width && cy >= 0 && cy < height) {
            grid[cy][cx] = draw_char;
        }
        return;
    }

    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    if (fill) {
        plot_circle_filled_lines(cx, cy, x, y, draw_char, grid, width, height);
    } else {
        plot_circle_points(cx, cy, x, y, draw_char, grid, width, height);
    }

    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        if (fill) {
            plot_circle_filled_lines(cx, cy, x, y, draw_char, grid, width, height);
        } else {
            plot_circle_points(cx, cy, x, y, draw_char, grid, width, height);
        }
    }
}

static void fill_flat_bottom(int x1, int y1, int x2, int y2, int x3, int y3, char draw_char, char** grid, int width, int height) {
    if (y2 == y1) return;
    float invslope1 = (float)(x2 - x1) / (y2 - y1);
    float invslope2 = (float)(x3 - x1) / (y2 - y1);

    float curx1 = x1;
    float curx2 = x1;

    for (int scanline_y = y1; scanline_y <= y2; scanline_y++) {
        int x_start = (int)roundf(curx1);
        int x_end = (int)roundf(curx2);
        if (x_start > x_end) {
            int temp = x_start;
            x_start = x_end;
            x_end = temp;
        }
        if (scanline_y >= 0 && scanline_y < height) {
            for (int px = x_start; px <= x_end; px++) {
                if (px >= 0 && px < width) {
                    grid[scanline_y][px] = draw_char;
                }
            }
        }
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

static void fill_flat_top(int x1, int y1, int x2, int y2, int x3, int y3, char draw_char, char** grid, int width, int height) {
    if (y3 == y1) return;
    float invslope1 = (float)(x3 - x1) / (y3 - y1);
    float invslope2 = (float)(x3 - x2) / (y3 - y1);

    float curx1 = x3;
    float curx2 = x3;

    for (int scanline_y = y3; scanline_y >= y1; scanline_y--) {
        int x_start = (int)roundf(curx1);
        int x_end = (int)roundf(curx2);
        if (x_start > x_end) {
            int temp = x_start;
            x_start = x_end;
            x_end = temp;
        }
        if (scanline_y >= 0 && scanline_y < height) {
            for (int px = x_start; px <= x_end; px++) {
                if (px >= 0 && px < width) {
                    grid[scanline_y][px] = draw_char;
                }
            }
        }
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

typedef struct {
    int x, y;
} Point;

static int compare_points(const void* a, const void* b) {
    return ((Point*)a)->y - ((Point*)b)->y;
}

static void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int fill, char draw_char, char** grid, int width, int height) {
    if (!fill) {
        draw_line(x1, y1, x2, y2, draw_char, grid, width, height);
        draw_line(x2, y2, x3, y3, draw_char, grid, width, height);
        draw_line(x3, y3, x1, y1, draw_char, grid, width, height);
    } else {
        Point pts[3] = {{x1, y1}, {x2, y2}, {x3, y3}};
        qsort(pts, 3, sizeof(Point), compare_points);

        int xa = pts[0].x, ya = pts[0].y;
        int xb = pts[1].x, yb = pts[1].y;
        int xc = pts[2].x, yc = pts[2].y;

        if (ya == yc) {
            int x_min = xa;
            if (xb < x_min) x_min = xb;
            if (xc < x_min) x_min = xc;
            int x_max = xa;
            if (xb > x_max) x_max = xb;
            if (xc > x_max) x_max = xc;
            if (ya >= 0 && ya < height) {
                for (int px = x_min; px <= x_max; px++) {
                    if (px >= 0 && px < width) grid[ya][px] = draw_char;
                }
            }
        } else if (ya == yb) {
            fill_flat_top(xa, ya, xb, yb, xc, yc, draw_char, grid, width, height);
        } else if (yb == yc) {
            fill_flat_bottom(xa, ya, xb, yb, xc, yc, draw_char, grid, width, height);
        } else {
            int xd = (int)roundf(xa + (float)(yb - ya) * (xc - xa) / (yc - ya));
            fill_flat_bottom(xa, ya, xb, yb, xd, yb, draw_char, grid, width, height);
            fill_flat_top(xb, yb, xd, yb, xc, yc, draw_char, grid, width, height);
        }
    }
}

char** get_grid(Canvas* canvas) {
    if (!canvas) return NULL;
    char** grid = (char**)malloc(canvas->height * sizeof(char*));
    if (!grid) return NULL;
    for (int i = 0; i < canvas->height; i++) {
        grid[i] = (char*)malloc(canvas->width * sizeof(char));
        if (!grid[i]) {
            for (int j = 0; j < i; j++) free(grid[j]);
            free(grid);
            return NULL;
        }
        memset(grid[i], canvas->bg_char, canvas->width);
    }

    for (int i = 0; i < canvas->shape_count; i++) {
        Shape s = canvas->shapes[i];
        switch (s.type) {
            case SHAPE_LINE:
                draw_line(s.data.line.x1, s.data.line.y1, s.data.line.x2, s.data.line.y2, s.draw_char, grid, canvas->width, canvas->height);
                break;
            case SHAPE_RECTANGLE:
                draw_rectangle(s.data.rect.x, s.data.rect.y, s.data.rect.w, s.data.rect.h, s.data.rect.fill, s.draw_char, grid, canvas->width, canvas->height);
                break;
            case SHAPE_CIRCLE:
                draw_circle(s.data.circle.cx, s.data.circle.cy, s.data.circle.radius, s.data.circle.fill, s.draw_char, grid, canvas->width, canvas->height);
                break;
            case SHAPE_TRIANGLE:
                draw_triangle(s.data.triangle.x1, s.data.triangle.y1, s.data.triangle.x2, s.data.triangle.y2, s.data.triangle.x3, s.data.triangle.y3, s.data.triangle.fill, s.draw_char, grid, canvas->width, canvas->height);
                break;
        }
    }
    return grid;
}

void free_grid(char** grid, int height) {
    if (grid) {
        for (int i = 0; i < height; i++) {
            free(grid[i]);
        }
        free(grid);
    }
}

void display_canvas(Canvas* canvas) {
    if (!canvas) return;
    char** grid = get_grid(canvas);
    if (!grid) return;

    // Draw top border
    printf("+");
    for (int i = 0; i < canvas->width; i++) printf("-");
    printf("+\n");

    for (int r = 0; r < canvas->height; r++) {
        printf("|");
        for (int c = 0; c < canvas->width; c++) {
            putchar(grid[r][c]);
        }
        printf("|\n");
    }

    // Draw bottom border
    printf("+");
    for (int i = 0; i < canvas->width; i++) printf("-");
    printf("+\n");

    free_grid(grid, canvas->height);
}

void get_shape_description(const Shape* shape, int index, char* buffer, int buffer_size) {
    if (!shape || !buffer || buffer_size <= 0) return;
    switch (shape->type) {
        case SHAPE_LINE:
            snprintf(buffer, buffer_size, "[%d] Line from (%d, %d) to (%d, %d) using '%c'",
                     index, shape->data.line.x1, shape->data.line.y1, shape->data.line.x2, shape->data.line.y2, shape->draw_char);
            break;
        case SHAPE_RECTANGLE:
            snprintf(buffer, buffer_size, "[%d] %s Rectangle at (%d, %d), size %dx%d using '%c'",
                     index, shape->data.rect.fill ? "Filled" : "Outline",
                     shape->data.rect.x, shape->data.rect.y, shape->data.rect.w, shape->data.rect.h, shape->draw_char);
            break;
        case SHAPE_CIRCLE:
            snprintf(buffer, buffer_size, "[%d] %s Circle at (%d, %d), radius %d using '%c'",
                     index, shape->data.circle.fill ? "Filled" : "Outline",
                     shape->data.circle.cx, shape->data.circle.cy, shape->data.circle.radius, shape->draw_char);
            break;
        case SHAPE_TRIANGLE:
            snprintf(buffer, buffer_size, "[%d] %s Triangle vertices (%d, %d), (%d, %d), (%d, %d) using '%c'",
                     index, shape->data.triangle.fill ? "Filled" : "Outline",
                     shape->data.triangle.x1, shape->data.triangle.y1,
                     shape->data.triangle.x2, shape->data.triangle.y2,
                     shape->data.triangle.x3, shape->data.triangle.y3, shape->draw_char);
            break;
        default:
            snprintf(buffer, buffer_size, "[%d] Unknown Shape", index);
            break;
    }
}
