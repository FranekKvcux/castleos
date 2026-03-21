#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_DATA_PORT    0x60
#define KEYBOARD_STATUS_PORT  0x64

#define SCANCODE_SHIFT_LEFT          0x2A
#define SCANCODE_SHIFT_RIGHT         0x36
#define SCANCODE_SHIFT_LEFT_RELEASE  0xAA
#define SCANCODE_SHIFT_RIGHT_RELEASE 0xB6
#define SCANCODE_CAPSLOCK            0x3A

static char scancode_map[] = {
    0,   0,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t','q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,   '\\','z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0,  ' '
};

static char scancode_map_shift[] = {
    0,   0,  '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t','Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0,   'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0,   '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0,  ' '
};

static int shift_pressed = 0;
static int capslock_on   = 0;

static inline unsigned char inb(unsigned short port) {
    unsigned char val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

// Zamienia małą literę na wielką i odwrotnie
static char toggle_case(char c) {
    if (c >= 'a' && c <= 'z') return c - 32;
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}

char keyboard_getchar() {
    while (1) {
        unsigned char status = inb(KEYBOARD_STATUS_PORT);
        if (!(status & 0x01)) continue;

        unsigned char scancode = inb(KEYBOARD_DATA_PORT);

        // Shift wciśnięty
        if (scancode == SCANCODE_SHIFT_LEFT || scancode == SCANCODE_SHIFT_RIGHT) {
            shift_pressed = 1;
            continue;
        }
        // Shift puszczony
        if (scancode == SCANCODE_SHIFT_LEFT_RELEASE || scancode == SCANCODE_SHIFT_RIGHT_RELEASE) {
            shift_pressed = 0;
            continue;
        }
        // Caps Lock - przełącz stan przy wciśnięciu
        if (scancode == SCANCODE_CAPSLOCK) {
            capslock_on = !capslock_on;
            continue;
        }

        // Ignoruj key release
        if (scancode & 0x80) continue;

        if (scancode >= sizeof(scancode_map)) continue;

        char c = shift_pressed ? scancode_map_shift[scancode] : scancode_map[scancode];
        if (c == 0) continue;

        // Caps Lock odwraca wielkość liter (tylko litery, nie symbole)
        if (capslock_on) c = toggle_case(c);

        return c;
    }
}

#endif