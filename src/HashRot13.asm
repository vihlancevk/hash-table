section .text

global HashRot13

; rdi - const void *elem
; rsi - size_t size

HashRot13:
        test    rsi, rsi
        je      .return
        
        xor     ecx, ecx ; ecx = i    = 0
        xor     eax, eax ; eax = hash = 0
        
.for:
        movsx   edx, byte [rdi + rcx] ; hash += (int)buffer[i]
        add     eax, edx              ;
        
        mov     edx, eax ; hash -= ( hash << 13 ) | ( hash >> 19 )
        rol     edx, 13  ;
        sub     eax, edx ;
        
        add     rcx, 1   ; for
        cmp     rsi, rcx ;
        jne     .for     ;
        
        mov     ecx, eax            ; hash = hash % 2000
        imul    rcx, rcx, 274877907 ;
        shr     rcx, 39             ;
        imul    ecx, ecx, 2000      ;
        sub     eax, ecx            ;
        
        ret
        
.return:
        xor     eax, eax
        ret
