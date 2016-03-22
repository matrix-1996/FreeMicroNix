extern gp

global gdt_loader
gdt_loader:
    lgdt [gp]        ; Load the new GDT pointer
    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax        ; Load all data segment selectors
    mov es, ax
    mov ss, ax
    ;mov ax, 0x33
    ;mov fs, ax
    ;mov ax, 0x3B
    ;mov gs, ax
    jmp 0x08:.flush   ; 0x08 is the offset to our code segment: Far jump!
.flush:
    ret
