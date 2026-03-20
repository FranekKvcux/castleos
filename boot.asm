org 0x7c00
global _start

_start:
    ; Clear screen
    mov ax, 0x0003
    int 0x10

    mov ah, 0x0e
    mov bx, hello

    printHello:
        mov al, [bx]
        cmp al, 0
        je end
        int 0x10
        inc bx
        jmp printHello

    end:

    ; Load kernel from disk
    mov ax, 0x1000
    mov es, ax
    mov bx, 0
    mov ah, 0x02
    mov al, 1      ; Load 1 sector
    mov ch, 0      ; Cylinder 0
    mov cl, 2      ; Sector 2
    mov dh, 0      ; Head 0
    mov dl, 0x80   ; Drive 0
    int 0x13

    ; Jump to kernel
    jmp 0x1000:0

    ; Start Hello
    hello:
        db "Hello from CastleOS!", 0

    ; Boot signature
    times 510-($-$$) db 0
    db 0x55, 0xaa