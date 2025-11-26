[ORG 0x00]
[BITS 16]

SECTION .text

START:
    mov ax, 0x1000  ; 보호 모드 엔트리 포인트의 시작 어드레스를 세그먼트 레지스터 값으로 변환.
    mov ds, ax      ; ds 설정
    mov es, ax      ; es 설정

    cli             ; 인터럽트가 발생하지 못하도록 설정
    lgdt [ GDTR ]   ; GDTR 자료 구조를 프로세서에 설정하여 GDT 테이블 로드

    ;
    ; 보호 모드로 진입
    ; Disable: Paging, Cache, Internal FPU, Align Check
    ; Enable: ProtectedMode
    ;
    mov eax, 0x4000003B     ; PG=0, CD=1, NW=0, AM=0, WP=0, NE=1, ET=1, TS=1, EM=0, MP=1, PE=1
    mov cr0, eax            ; CR0 컨트롤 레지스터에 위에서 저장한 플래그를 설정하여, 보호 모드로 전환.

    ; 커널 코드 세그먼트를 0x00을 기준으로 하는 거승로 교체하고 EIP의 값을 0x00 기준으로 재설정
    ; CS 세그먼트 셀렉터 : CIP
    jmp dword 0x08:(PROTECTED_MODE - $$ + 0x10000)

;
; 보호 모드 진입
;
[BITS 32]
PROTECTED_MODE:
    mov ax, 0x10        ; 보호 모드 커널용 데이터 세그먼트 디스크립터를 AX 레지스터에 저장
    mov ds, ax          ; DS 세그먼트 셀렉터에 설정
    mov es, ax          ; ES ...
    mov fs, ax          ; FS ...
    mov gs, ax          ; GS ...

    ; 스택을 0x0000000 ~ 0x0000FFFF 영역에 64KB 크기로 생성
    mov ss, ax          ; SS ...
    mov esp, 0xFFFE     ; ESP 레지스터의 어드레스를 0xFFFE로 설정
    mov ebp, 0xFFFE     ; EBP 레지스터의 어드레스를 0xFFFE로 설정

    ; 화면에 보호 모드로 전환되었다는 메시지를 출력한다.
    push ( SWITCH_SUCCESS_MESSAGE - $$ + 0x10000 ) ; 출력할 메시지의 어드레스를 스택에 삽입.
    push 2
    push 0
    call PRINT_MESSAGE
    add esp, 12

    jmp dword 0x08: 0x10200 ; C 언어 커널이 존재하는 0x10200 어드레스로 이동하여 C언어 커널 수행.

;
;   함수 코드 영역
;
PRINT_MESSAGE: ; 리얼모드에 똑같음, 크기만 바뀜.
    push ebp
    mov ebp, esp
    push esi,
    push edi
    push eax
    push ecx
    push edx

    mov eax, dword [ ebp + 12 ]
    mov esi, 160
    mul esi
    mov edi, eax

    mov eax, dword [ ebp + 8 ]
    mov esi, 2
    mul esi
    add edi, eax

    mov esi, dword [ ebp + 16 ]

.MESSAGE_LOOP:
    mov cl, byte [ esi ]
    cmp cl, 0
    je .MESSAGE_END
    mov byte [ edi + 0xB8000 ], cl
    add esi, 1
    add edi, 2
    jmp .MESSAGE_LOOP

.MESSAGE_END:
    pop edx
    pop ecx
    pop eax
    pop edi
    pop esi
    pop ebp
    ret

;
; 데이터 영역
;
; 아래의 데이터들을 8바이트에 맞춰 정렬하기 위해 추가.
align 8, db 0

; GDTR의 끝을 8byte로 정렬하기 위해 추가.
dw 0x0000
GDTR:
    dw GDT_END - GDT - 1        ; 아래에 위치하는 GDT 테이블의 전체 크기.
    dd ( GDT - $$ + 0x10000 )   ; 아래에 위치하는 GDT 테이블의 시작 어드레스

; GDT 테이블 정의
GDT:
    ; 널 디스크립터 반드시 0으로 초기화해야함.
    NULL_DESCRIPTOR:
        dw 0x0000
        dw 0x0000
        db 0x00
        db 0x00
        db 0x00
        db 0x00

    ; 보호 모드 커널용 코드 세그먼트 디스크립터
    CODE_DESCRIPTOR:
        dw 0xFFFF   ; LIMIT [15:0]
        dw 0x0000   ; Base [15:0]
        db 0x00     ; Base [23:16]
        db 0x9A     ; P=1, DPL=0, Code Segment, Execute/READ
        db 0xCF     ; G=1, D=1, L=0, Limit[19:16]
        db 0x00     ; Base [31:24]

    ; 보호 모드 커널용 데이터 세그먼트 디스크립터
    DATA_DESCROPTOR:
        dw 0xFFFF   ; Limit [15:0]
        dw 0x0000   ; Base [15:0]
        db 0x00     ; Base [23:16]
        db 0x92     ; P=1, DPL=0, Data Segment, READ/Write
        db 0xCF     ; G=1, D=1, L=0, Limit[19:16]
        db 0x00     ; Base [31:24

GDT_END:

SWITCH_SUCCESS_MESSAGE: db "Switch To Protected Mode Success~!!", 0

times 512 - ( $ - $$ ) db 0x00