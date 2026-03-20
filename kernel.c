#include "include/vga.h"

int main() {
    vga_clear(MAKE_COLOR(COLOR_WHITE, COLOR_BLACK));
    print_color("CastleOS $", MAKE_COLOR(COLOR_GREEN, COLOR_BLACK));
    while(1);
}