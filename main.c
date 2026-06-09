#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.h"

// Input helper function to read a line from stdin safely
void get_input_line(char* buffer, int size) {
    if (fgets(buffer, size, stdin)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    } else {
        buffer[0] = '\0';
    }
}

// Prompt for an integer with a default fallback
int prompt_int(const char* prompt, int default_val) {
    char buf[128];
    while (1) {
        printf("%s (default %d): ", prompt, default_val);
        get_input_line(buf, sizeof(buf));
        if (strlen(buf) == 0) {
            return default_val;
        }
        int val;
        if (sscanf(buf, "%d", &val) == 1) {
            return val;
        }
        printf("Invalid input. Please enter an integer.\n");
    }
}

// Prompt for an integer (required, no default)
int prompt_int_required(const char* prompt) {
    char buf[128];
    while (1) {
        printf("%s: ", prompt);
        get_input_line(buf, sizeof(buf));
        if (strlen(buf) > 0) {
            int val;
            if (sscanf(buf, "%d", &val) == 1) {
                return val;
            }
        }
        printf("Input required. Please enter an integer.\n");
    }
}

// Prompt for a character with a default fallback
char prompt_char(const char* prompt, char default_val) {
    char buf[128];
    printf("%s (default '%c'): ", prompt, default_val);
    get_input_line(buf, sizeof(buf));
    if (strlen(buf) == 0) {
        return default_val;
    }
    return buf[0];
}

// Prompt for a boolean (yes/no) with a default fallback
int prompt_bool(const char* prompt, int default_val) {
    char buf[128];
    while (1) {
        printf("%s (y/n, default %s): ", prompt, default_val ? "y" : "n");
        get_input_line(buf, sizeof(buf));
        if (strlen(buf) == 0) {
            return default_val;
        }
        char choice = buf[0];
        if (choice == 'y' || choice == 'Y') return 1;
        if (choice == 'n' || choice == 'N') return 0;
        printf("Invalid input. Please enter y or n.\n");
    }
}

int list_shapes(Canvas* canvas) {
    if (!canvas || canvas->shape_count == 0) {
        printf("No shapes on the canvas.\n");
        return 0;
    }
    printf("\n--- Current Shapes ---\n");
    char desc[256];
    for (int i = 0; i < canvas->shape_count; i++) {
        get_shape_description(&canvas->shapes[i], i, desc, sizeof(desc));
        printf("%s\n", desc);
    }
    return 1;
}

void add_shape_menu(Canvas* canvas) {
    printf("\n--- Add a Shape ---\n");
    printf("1. Line\n");
    printf("2. Rectangle\n");
    printf("3. Circle\n");
    printf("4. Triangle\n");
    printf("Select shape type (1-4): ");
    
    char choice_buf[128];
    get_input_line(choice_buf, sizeof(choice_buf));
    int choice = 0;
    sscanf(choice_buf, "%d", &choice);

    Shape shape;
    if (choice == 1) {
        shape.type = SHAPE_LINE;
        shape.data.line.x1 = prompt_int_required("Enter x1");
        shape.data.line.y1 = prompt_int_required("Enter y1");
        shape.data.line.x2 = prompt_int_required("Enter x2");
        shape.data.line.y2 = prompt_int_required("Enter y2");
        shape.draw_char = prompt_char("Enter drawing character", '*');
    } else if (choice == 2) {
        shape.type = SHAPE_RECTANGLE;
        shape.data.rect.x = prompt_int_required("Enter top-left x");
        shape.data.rect.y = prompt_int_required("Enter top-left y");
        shape.data.rect.w = prompt_int_required("Enter width");
        shape.data.rect.h = prompt_int_required("Enter height");
        shape.draw_char = prompt_char("Enter drawing character", '*');
        shape.data.rect.fill = prompt_bool("Fill rectangle?", 0);
    } else if (choice == 3) {
        shape.type = SHAPE_CIRCLE;
        shape.data.circle.cx = prompt_int_required("Enter center x");
        shape.data.circle.cy = prompt_int_required("Enter center y");
        shape.data.circle.radius = prompt_int_required("Enter radius");
        shape.draw_char = prompt_char("Enter drawing character", '*');
        shape.data.circle.fill = prompt_bool("Fill circle?", 0);
    } else if (choice == 4) {
        shape.type = SHAPE_TRIANGLE;
        shape.data.triangle.x1 = prompt_int_required("Enter x1");
        shape.data.triangle.y1 = prompt_int_required("Enter y1");
        shape.data.triangle.x2 = prompt_int_required("Enter x2");
        shape.data.triangle.y2 = prompt_int_required("Enter y2");
        shape.data.triangle.x3 = prompt_int_required("Enter x3");
        shape.data.triangle.y3 = prompt_int_required("Enter y3");
        shape.draw_char = prompt_char("Enter drawing character", '*');
        shape.data.triangle.fill = prompt_bool("Fill triangle?", 0);
    } else {
        printf("Invalid choice.\n");
        return;
    }

    int idx = add_shape(canvas, shape);
    char desc[256];
    get_shape_description(&shape, idx, desc, sizeof(desc));
    printf("Added shape successfully: %s\n", desc);
}

void modify_shape_menu(Canvas* canvas) {
    printf("\n--- Modify a Shape ---\n");
    if (!list_shapes(canvas)) return;

    int idx = prompt_int_required("Enter the index of the shape to modify");
    if (idx < 0 || idx >= canvas->shape_count) {
        printf("Invalid index.\n");
        return;
    }

    Shape shape = canvas->shapes[idx];
    char desc[256];
    get_shape_description(&shape, idx, desc, sizeof(desc));
    printf("Modifying: %s\n", desc);
    printf("Press Enter to keep current values.\n");

    if (shape.type == SHAPE_LINE) {
        shape.data.line.x1 = prompt_int("Enter new x1", shape.data.line.x1);
        shape.data.line.y1 = prompt_int("Enter new y1", shape.data.line.y1);
        shape.data.line.x2 = prompt_int("Enter new x2", shape.data.line.x2);
        shape.data.line.y2 = prompt_int("Enter new y2", shape.data.line.y2);
        shape.draw_char = prompt_char("Enter drawing character", shape.draw_char);
    } else if (shape.type == SHAPE_RECTANGLE) {
        shape.data.rect.x = prompt_int("Enter top-left x", shape.data.rect.x);
        shape.data.rect.y = prompt_int("Enter top-left y", shape.data.rect.y);
        shape.data.rect.w = prompt_int("Enter width", shape.data.rect.w);
        shape.data.rect.h = prompt_int("Enter height", shape.data.rect.h);
        shape.draw_char = prompt_char("Enter drawing character", shape.draw_char);
        shape.data.rect.fill = prompt_bool("Fill rectangle?", shape.data.rect.fill);
    } else if (shape.type == SHAPE_CIRCLE) {
        shape.data.circle.cx = prompt_int("Enter center x", shape.data.circle.cx);
        shape.data.circle.cy = prompt_int("Enter center y", shape.data.circle.cy);
        shape.data.circle.radius = prompt_int("Enter radius", shape.data.circle.radius);
        shape.draw_char = prompt_char("Enter drawing character", shape.draw_char);
        shape.data.circle.fill = prompt_bool("Fill circle?", shape.data.circle.fill);
    } else if (shape.type == SHAPE_TRIANGLE) {
        shape.data.triangle.x1 = prompt_int("Enter new x1", shape.data.triangle.x1);
        shape.data.triangle.y1 = prompt_int("Enter new y1", shape.data.triangle.y1);
        shape.data.triangle.x2 = prompt_int("Enter new x2", shape.data.triangle.x2);
        shape.data.triangle.y2 = prompt_int("Enter new y2", shape.data.triangle.y2);
        shape.data.triangle.x3 = prompt_int("Enter new x3", shape.data.triangle.x3);
        shape.data.triangle.y3 = prompt_int("Enter new y3", shape.data.triangle.y3);
        shape.draw_char = prompt_char("Enter drawing character", shape.draw_char);
        shape.data.triangle.fill = prompt_bool("Fill triangle?", shape.data.triangle.fill);
    }

    modify_shape(canvas, idx, shape);
    get_shape_description(&shape, idx, desc, sizeof(desc));
    printf("Shape at index %d modified successfully: %s\n", idx, desc);
}

void delete_shape_menu(Canvas* canvas) {
    printf("\n--- Delete a Shape ---\n");
    if (!list_shapes(canvas)) return;

    int idx = prompt_int_required("Enter the index of the shape to delete");
    if (delete_shape(canvas, idx)) {
        printf("Shape at index %d deleted successfully.\n", idx);
    } else {
        printf("Invalid index.\n");
    }
}

int main() {
    printf("=========================================\n");
    printf("Welcome to the ASCII Graphics Drawing App\n");
    printf("=========================================\n");

    int width = prompt_int("Enter canvas width", 40);
    int height = prompt_int("Enter canvas height", 20);
    
    char bg_prompt_buf[128];
    printf("Enter background character (default space ' '): ");
    get_input_line(bg_prompt_buf, sizeof(bg_prompt_buf));
    char bg_char = ' ';
    if (strlen(bg_prompt_buf) > 0) {
        bg_char = bg_prompt_buf[0];
    }

    Canvas* canvas = create_canvas(width, height, bg_char);
    if (!canvas) {
        printf("Failed to create canvas. Exiting.\n");
        return 1;
    }
    printf("Canvas created with size %dx%d and background '%c'\n", width, height, bg_char);

    while (1) {
        printf("\n--- Main Menu ---\n");
        printf("1. Display Canvas\n");
        printf("2. Add a Shape\n");
        printf("3. Modify a Shape\n");
        printf("4. Delete a Shape\n");
        printf("5. List Shapes\n");
        printf("6. Exit\n");
        printf("Enter choice (1-6): ");

        char choice_buf[128];
        get_input_line(choice_buf, sizeof(choice_buf));
        int choice = 0;
        sscanf(choice_buf, "%d", &choice);

        if (choice == 1) {
            display_canvas(canvas);
        } else if (choice == 2) {
            add_shape_menu(canvas);
        } else if (choice == 3) {
            modify_shape_menu(canvas);
        } else if (choice == 4) {
            delete_shape_menu(canvas);
        } else if (choice == 5) {
            list_shapes(canvas);
        } else if (choice == 6) {
            printf("Goodbye!\n");
            break;
        } else {
            printf("Invalid choice, please select 1-6.\n");
        }
    }

    free_canvas(canvas);
    return 0;
}
