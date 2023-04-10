
section .data
    mybuffer db "hello",10,0
    len db $-mybuffer


readInput : 
    push ecx
    push edx
    push esi
    mov ecx, mybuffer
    mov edx , myBuffsize
    call readString
    mov esi, eax
    mov byte [mybuffer + esi],0
    mov eax , mybuffer
    call atoi  
    pop esi 
    pop edx
    pop ecx
    
