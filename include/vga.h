#ifndef VGA_H
#define VGA_H

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH   80
#define VGA_HEIGHT  25

// Kolory tekstu
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

// Tworzy bajt koloru: tekst na tle
#define MAKE_COLOR(fg, bg) ((bg << 4) | fg)

static int cursor_x = 0;
static int cursor_y = 0;

// Czyści ekran wybranym kolorem tła
void vga_clear(unsigned char color) {
    unsigned short *vga = (unsigned short *)VGA_ADDRESS;
    unsigned short blank = (unsigned short)(' ') | ((unsigned short)color << 8);
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga[i] = blank;
    }
    cursor_x = 0;
    cursor_y = 0;
}

// Wypisuje pojedynczy znak na pozycji (x, y) z kolorem
void vga_putchar_at(char c, int x, int y, unsigned char color) {
    unsigned short *vga = (unsigned short *)VGA_ADDRESS;
    vga[y * VGA_WIDTH + x] = (unsigned short)c | ((unsigned short)color << 8);
}

// Wypisuje znak w miejscu kursora (obsługuje \n)
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
    // Prosty scroll — gdy wyjdziemy za ekran, wróć na górę
    if (cursor_y >= VGA_HEIGHT) {
        cursor_y = 0;
    }
}

// Wypisuje string z wybranym kolorem
void print_color(const char *str, unsigned char color) {
    for (int i = 0; str[i] != '\0'; i++) {
        vga_putchar(str[i], color);
    }
}

// Wypisuje string — biały tekst na czarnym tle (domyślny)
void print(const char *str) {
    print_color(str, MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
}

// Wypisuje string i przechodzi do nowej linii
void println(const char *str) {
    print(str);
    vga_putchar('\n', MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
}

#endif