#ifndef VGA_H
#define VGA_H

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH   80
#define VGA_HEIGHT  25

#define COLOR_BLACK         0
#define COLOR_BLUE          1
#define COLOR_GREEN         2
#define COLOR_CYAN          3
#define COLOR_RED           4
#define COLOR_MAGENTA       5
#define COLOR_BROWN         6
#define COLOR_LIGHT_GREY    7
#define COLOR_DARK_GREY     8
#define COLOR_LIGHT_BLUE    9
#define COLOR_LIGHT_GREEN   10
#define COLOR_LIGHT_CYAN    11
#define COLOR_LIGHT_RED     12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_YELLOW        14
#define COLOR_WHITE         15

#define MAKE_COLOR(fg, bg) ((bg << 4) | fg)

int cursor_x = 0;
int cursor_y = 0;

void vga_clear(unsigned char color) {
    unsigned short *vga = (unsigned short *)VGA_ADDRESS;
    unsigned short blank = (unsigned short)(' ') | ((unsigned short)color << 8);
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        vga[i] = blank;
    cursor_x = 0;
    cursor_y = 0;
}

void vga_putchar_at(char c, int x, int y, unsigned char color) {
    unsigned short *vga = (unsigned short *)VGA_ADDRESS;
    vga[y * VGA_WIDTH + x] = (unsigned short)c | ((unsigned short)color << 8);
}

// Rysuje kursor _ w aktualnej pozycji
void vga_draw_cursor() {
    vga_putchar_at('_', cursor_x, cursor_y, MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
}

// Usuwa kursor (zastępuje spacją)
void vga_clear_cursor() {
    vga_putchar_at(' ', cursor_x, cursor_y, MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
}

void vga_putchar(char c, unsigned char color) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        vga_putchar_at(c, cursor_x, cursor_y, color);
        cursor_x++;
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }
    if (cursor_y >= VGA_HEIGHT)
        cursor_y = 0;
}

void print_color(const char *str, unsigned char color) {
    for (int i = 0; str[i] != '\0'; i++)
        vga_putchar(str[i], color);
}

void print(const char *str) {
    print_color(str, MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
}

void println(const char *str) {
    print(str);
    vga_putchar('\n', MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
}

#endif