#ifndef GRAPHICS_H
#define GRAPHICS_H

typedef enum {
    SHAPE_LINE,
    SHAPE_RECTANGLE,
    SHAPE_CIRCLE,
    SHAPE_TRIANGLE
} ShapeType;

typedef struct {
    int x1, y1, x2, y2;
} LineData;

typedef struct {
    int x, y, w, h;
    int fill;
} RectangleData;

typedef struct {
    int cx, cy, radius;
    int fill;
} CircleData;

typedef struct {
    int x1, y1, x2, y2, x3, y3;
    int fill;
} TriangleData;

typedef struct {
    ShapeType type;
    char draw_char;
    union {
        LineData line;
        RectangleData rect;
        CircleData circle;
        TriangleData triangle;
    } data;
} Shape;

typedef struct {
    int width;
    int height;
    char bg_char;
    Shape* shapes;
    int shape_count;
    int shape_capacity;
} Canvas;

Canvas* create_canvas(int width, int height, char bg_char);
void free_canvas(Canvas* canvas);
int add_shape(Canvas* canvas, Shape shape);
int delete_shape(Canvas* canvas, int index);
int modify_shape(Canvas* canvas, int index, Shape shape);
char** get_grid(Canvas* canvas);
void free_grid(char** grid, int height);
void display_canvas(Canvas* canvas);
void get_shape_description(const Shape* shape, int index, char* buffer, int buffer_size);

#endif /* GRAPHICS_H */
