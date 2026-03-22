#include "vga.h"
#include "keyboard.h"

#define PROMPT     "CastleOS $ "
#define PROMPT_LEN 11
#define VERSION "1.0\n"
#define KERNEL "KK x86\n"

int strcmp(const char *a, const char *b) {
    while (*a && *b && *a == *b) { a++; b++; }
    return *a - *b;
}

int strncmp(const char *a, const char *b, int n) {
    while (n-- && *a && *b && *a == *b) { a++; b++; }
    return n < 0 ? 0 : *a - *b;
}

char buffer[256]; // characters typed by the user go here
int buf_len = 0;  // number of characters in the buffer

int main() {
    vga_clear(MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
    print_color("=== CastleOS ===\n\n", MAKE_COLOR(COLOR_YELLOW, COLOR_BLACK));
    print_color(PROMPT, MAKE_COLOR(COLOR_GREEN, COLOR_BLACK));

    // Draw cursor here, after the prompt
    vga_draw_cursor();

    while (1) {
        char c = keyboard_getchar();
        vga_clear_cursor();

        if (c != '\n' && c != '\b') buffer[buf_len++] = c;

        if (c == '\n') {
            // End string with zero
            buffer[buf_len] = '\0';

            // Commands:
            // help
            if (strcmp(buffer, "help") == 0) {
                print_color("\nCommands:\n", MAKE_COLOR(COLOR_CYAN, COLOR_BLACK));
                // - help
                print_color(" - help:", MAKE_COLOR(COLOR_CYAN, COLOR_BLACK));
                print_color(" list of commands and what they do\n", MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
                // - clear
                print_color(" - clear:", MAKE_COLOR(COLOR_CYAN, COLOR_BLACK));
                print_color(" clears screen\n", MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
                // - echo
                print_color(" - echo:", MAKE_COLOR(COLOR_CYAN, COLOR_BLACK));
                print_color(" prints expresion after 'echo'\n", MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
                // - castlefetch
                print_color(" - castlefetch:", MAKE_COLOR(COLOR_CYAN, COLOR_BLACK));
                print_color(" show info about system", MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
            // clear
            } else if (strcmp(buffer, "clear") == 0) {
                vga_clear(MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
            // echo
            } else if (strncmp(buffer, "echo ", 5) == 0) {
                print_color("\n", MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
                print_color(buffer + 5, MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
            } else if (strcmp(buffer, "castlefetch") == 0) {
                print_color("\n>=CastleOS=<\n", MAKE_COLOR(COLOR_YELLOW, COLOR_BLACK));
                print_color("# # # # # # # # #      ", MAKE_COLOR(COLOR_GREEN, COLOR_BLACK)); print_color("OS: ", MAKE_COLOR(COLOR_MAGENTA, COLOR_BLACK)); print("CastleOS\n");
                print_color("#################      ", MAKE_COLOR(COLOR_GREEN, COLOR_BLACK)); print_color("Version: ", MAKE_COLOR(COLOR_MAGENTA, COLOR_BLACK)); print(VERSION);
                print_color("#CCCCCCCCCCCCCCC#      ", MAKE_COLOR(COLOR_GREEN, COLOR_BLACK)); print_color("Kernel: ", MAKE_COLOR(COLOR_MAGENTA, COLOR_BLACK)); print(KERNEL);
                print_color("#CCCCCC/ \\CCCCCC#    ", MAKE_COLOR(COLOR_GREEN, COLOR_BLACK)); print("\n");
                print_color("#CCCCC|   |CCCCC#    ", MAKE_COLOR(COLOR_GREEN, COLOR_BLACK)); print("\n");
                print_color("#CCCCC|   |CCCCC#", MAKE_COLOR(COLOR_GREEN, COLOR_BLACK)); print("\n");
                print_color("#################", MAKE_COLOR(COLOR_GREEN, COLOR_BLACK)); print("\n");
            } else if (buf_len > 0) {
                print_color("\nERROR: Command not found", MAKE_COLOR(COLOR_RED, COLOR_BLACK));
            }

            vga_putchar('\n', MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
            print_color(PROMPT, MAKE_COLOR(COLOR_GREEN, COLOR_BLACK));

            // Clear buffer for next command
            buf_len = 0;
            buffer[0] = '\0';
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