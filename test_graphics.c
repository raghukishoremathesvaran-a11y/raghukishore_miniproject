#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "graphics.h"

void test_canvas_init() {
    Canvas* c = create_canvas(10, 5, '.');
    assert(c != NULL);
    assert(c->width == 10);
    assert(c->height == 5);
    assert(c->bg_char == '.');
    assert(c->shape_count == 0);

    char** grid = get_grid(c);
    for (int r = 0; r < 5; r++) {
        for (int col = 0; col < 10; col++) {
            assert(grid[r][col] == '.');
        }
    }
    free_grid(grid, 5);
    free_canvas(c);
    printf("test_canvas_init passed.\n");
}

void test_add_delete_modify_shapes() {
    Canvas* c = create_canvas(10, 5, '.');
    
    // Add shape
    Shape s1;
    s1.type = SHAPE_LINE;
    s1.draw_char = '#';
    s1.data.line.x1 = 0;
    s1.data.line.y1 = 0;
    s1.data.line.x2 = 9;
    s1.data.line.y2 = 0;
    int idx = add_shape(c, s1);
    assert(idx == 0);
    assert(c->shape_count == 1);
    
    char** grid = get_grid(c);
    for (int col = 0; col < 10; col++) {
        assert(grid[0][col] == '#');
    }
    free_grid(grid, 5);

    // Modify shape
    Shape s2;
    s2.type = SHAPE_LINE;
    s2.draw_char = '$';
    s2.data.line.x1 = 0;
    s2.data.line.y1 = 0;
    s2.data.line.x2 = 4;
    s2.data.line.y2 = 0;
    int success = modify_shape(c, 0, s2);
    assert(success == 1);

    grid = get_grid(c);
    for (int col = 0; col < 5; col++) {
        assert(grid[0][col] == '$');
    }
    for (int col = 5; col < 10; col++) {
        assert(grid[0][col] == '.');
    }
    free_grid(grid, 5);

    // Delete shape
    success = delete_shape(c, 0);
    assert(success == 1);
    assert(c->shape_count == 0);

    grid = get_grid(c);
    for (int col = 0; col < 10; col++) {
        assert(grid[0][col] == '.');
    }
    free_grid(grid, 5);
    free_canvas(c);

    printf("test_add_delete_modify_shapes passed.\n");
}

void test_line_drawing() {
    Canvas* c = create_canvas(5, 5, '.');
    
    Shape s;
    s.type = SHAPE_LINE;
    s.draw_char = 'L';
    s.data.line.x1 = 1;
    s.data.line.y1 = 2;
    s.data.line.x2 = 3;
    s.data.line.y2 = 2;
    add_shape(c, s);

    char** grid = get_grid(c);
    assert(grid[2][0] == '.');
    assert(grid[2][1] == 'L');
    assert(grid[2][2] == 'L');
    assert(grid[2][3] == 'L');
    assert(grid[2][4] == '.');

    free_grid(grid, 5);
    free_canvas(c);
    printf("test_line_drawing passed.\n");
}

void test_rectangle_drawing() {
    Canvas* c = create_canvas(5, 5, '.');
    
    Shape s;
    s.type = SHAPE_RECTANGLE;
    s.draw_char = 'R';
    s.data.rect.x = 1;
    s.data.rect.y = 1;
    s.data.rect.w = 3;
    s.data.rect.h = 3;
    s.data.rect.fill = 0;
    add_shape(c, s);

    char** grid = get_grid(c);
    // Row 1: .RRR.
    assert(grid[1][0] == '.'); assert(grid[1][1] == 'R'); assert(grid[1][2] == 'R'); assert(grid[1][3] == 'R'); assert(grid[1][4] == '.');
    // Row 2: .R.R.
    assert(grid[2][0] == '.'); assert(grid[2][1] == 'R'); assert(grid[2][2] == '.'); assert(grid[2][3] == 'R'); assert(grid[2][4] == '.');
    // Row 3: .RRR.
    assert(grid[3][0] == '.'); assert(grid[3][1] == 'R'); assert(grid[3][2] == 'R'); assert(grid[3][3] == 'R'); assert(grid[3][4] == '.');

    free_grid(grid, 5);
    free_canvas(c);
    printf("test_rectangle_drawing passed.\n");
}

void test_circle_drawing() {
    Canvas* c = create_canvas(5, 5, '.');
    
    Shape s;
    s.type = SHAPE_CIRCLE;
    s.draw_char = 'O';
    s.data.circle.cx = 2;
    s.data.circle.cy = 2;
    s.data.circle.radius = 1;
    s.data.circle.fill = 0;
    add_shape(c, s);

    char** grid = get_grid(c);
    // With radius 1, center (2,2):
    // Coordinates (2,1), (2,3), (1,2), (3,2) should be 'O'
    assert(grid[1][2] == 'O'); // Top
    assert(grid[3][2] == 'O'); // Bottom
    assert(grid[2][1] == 'O'); // Left
    assert(grid[2][3] == 'O'); // Right

    free_grid(grid, 5);
    free_canvas(c);
    printf("test_circle_drawing passed.\n");
}

void test_triangle_drawing() {
    Canvas* c = create_canvas(5, 5, '.');
    
    Shape s;
    s.type = SHAPE_TRIANGLE;
    s.draw_char = 'T';
    s.data.triangle.x1 = 2;
    s.data.triangle.y1 = 0;
    s.data.triangle.x2 = 0;
    s.data.triangle.y2 = 4;
    s.data.triangle.x3 = 4;
    s.data.triangle.y3 = 4;
    s.data.triangle.fill = 1;
    add_shape(c, s);

    char** grid = get_grid(c);
    // Row 0: ..T..
    assert(grid[0][2] == 'T');
    // Row 4 should be completely T: TTTTT
    for (int col = 0; col < 5; col++) {
        assert(grid[4][col] == 'T');
    }

    free_grid(grid, 5);
    free_canvas(c);
    printf("test_triangle_drawing passed.\n");
}

int main() {
    printf("Running C automated unit tests...\n");
    test_canvas_init();
    test_add_delete_modify_shapes();
    test_line_drawing();
    test_rectangle_drawing();
    test_circle_drawing();
    test_triangle_drawing();
    printf("All C tests passed successfully!\n");
    return 0;
}
