#include "vga.h"
#include "keyboard.h"

#define PROMPT "CastleOS $ "
#define PROMPT_LEN 11

int main() {
    vga_clear(MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
    print_color("=== CastleOS ===\n\n", MAKE_COLOR(COLOR_YELLOW, COLOR_BLACK));
    print_color(PROMPT, MAKE_COLOR(COLOR_GREEN, COLOR_BLACK));
    // Kursor rysowany dopiero TUTAJ, po promptcie
    vga_draw_cursor();

    while (1) {
        char c = keyboard_getchar();
        vga_clear_cursor();

        if (c == '\n') {
            vga_putchar('\n', MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
            print_color(PROMPT, MAKE_COLOR(COLOR_GREEN, COLOR_BLACK));
        } else if (c == '\b') {
            if (cursor_x > PROMPT_LEN) {
                cursor_x--;
                vga_putchar_at(' ', cursor_x, cursor_y, MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
            }
        } else {
            vga_putchar(c, MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
        }

        vga_draw_cursor();
    }
}