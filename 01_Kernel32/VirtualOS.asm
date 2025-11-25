[ORG 0x00]
[BITS 16]

SECTION .text

jmp 0x1000:START

SECTOR_COUNT:       dw 0x00     ; 현재 실행중인 섹터 번호 지정
TOTAL_SECTOR_COUNT: equ 1024    ; 가상 OS의 총 섹터 수, equ는 상수를 정의 하는 명령.

;
;   코드 섹션
;
START:
    mov ax, cs
    mov ds, ax
    mov ax, 0xb800

    mov es, ax

    %assign i 0
    %rep TOTAL_SECTOR_COUNT
        %assign i i + 1

        mov ax, 2

        mul word [ SECTOR_COUNT ]
        mov si, ax

        mov byte [ es: si + (160 * 2)], '0' + (i % 10)
        add word [ SECTOR_COUNT ], 1

        %if i == TOTAL_SECTOR_COUNT
            jmp $
        %else
            jmp ( 0x1000 + i * 0x20 ): 0x0000   ; 다음 섹털로 이동
        %endif

        times (512 - ( $ - $$ ) % 512) db 0x00  ; $: 현재 라인의 어드레스
                                                ; $$: 현재 섹션의 시작 어드레스
                                                ; $ - $$: 현 섹션을 기준으로 하는 오프셋
                                                ; 512 - ($ - $$): 현재부터 어드레스 512까지
                                                ; db 0x00: 1바이트를 선언하고 값은 0x00
                                                ; time: 반복 수행
                                                ; 현재 위치에서 어드레스 512까지 0x00으로 채움.
    %endrep