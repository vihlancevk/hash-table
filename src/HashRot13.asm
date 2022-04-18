section .text

global HashRot13

; rdi - const void *elem
; rsi - size_t size

HashRot13:

xor eax, eax ; int hash = 0
xor rbx, rbx ; size_t i = 0

.for:
    xor edx, edx

    cmp rbx, rsi
    jae .return

    mov dl, byte [rdi + rbx] ; int edx = (int)(char*)elem[i]
    add eax, edx             ; hash  += edx
    
    mov ecx, eax ; (hash << 13)
    shl ecx, 13  ;
    
    mov edx, eax ; (hash << 19)
    shr edx, 19  ;
    
    or ecx, edx ; (hash << 13) | (hash << 19)
    
    sub eax, ecx ; hash -= (hash << 13) | (hash << 19)
    
    inc rbx
    jmp .for

.return:
    mov ecx, 700
    div ecx
    
    mov eax, edx
    ret
