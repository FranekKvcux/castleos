org 0x7c00
bits 16

_start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    ; Wypisz "BOOT" przez BIOS żeby wiedzieć że bootloader działa
    mov ah, 0x0e
    mov al, 'B'
    int 0x10
    mov al, 'O'
    int 0x10
    mov al, 'O'
    int 0x10
    mov al, 'T'
    int 0x10

    ; Załaduj kernel pod 0x10000
    mov ax, 0x1000
    mov es, ax
    xor bx, bx
    mov ah, 0x02
    mov al, 10      ; 10 sektorów (bezpiecznie poniżej limitu floppy)
    mov ch, 0       ; cylinder 0
    mov cl, 2       ; sektor 2 (zaraz po bootloaderze)
    mov dh, 0       ; głowica 0
    mov dl, 0x00    ; 0x00 = floppy
    int 0x13

    ; Sprawdź czy wczytało się OK (carry flag)
    jc disk_error

    ; Wypisz "OK"
    mov ah, 0x0e
    mov al, 'O'
    int 0x10
    mov al, 'K'
    int 0x10

    ; Załaduj GDT
    lgdt [gdt_descriptor]

    ; Wejdź w Protected Mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:protected_mode

disk_error:
    mov ah, 0x0e
    mov al, 'E'
    int 0x10
    mov al, 'R'
    int 0x10
    mov al, 'R'
    int 0x10
    jmp $

bits 32
protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    jmp 0x10000

; ===== GDT =====
gdt_start:
gdt_null:
    dd 0x00000000
    dd 0x00000000
gdt_code:
    dw 0xffff
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00
gdt_data:
    dw 0xffff
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510-($-$$) db 0
db 0x55, 0xaa