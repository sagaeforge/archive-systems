[ORG 0x00] ; 코드의 시작 어드레스를 0x00으로 설정
[BITS 16] ; 이하의 코드는 16비트 코드로 설정

SECTION .text ; text 섹션(세그먼트)을 정의

jmp 0x07c0:START ; CS 세그먼트 레지스터에 0x07C0을 복사하면서, START 레이블로 이동.

START:
    mov ax, 0x07c0          ; AX 레지스터에 0x07c0 복사
    mov ds, ax              ; DS 세그먼트 레지스터에 AS 레지스터의 값 복사.
    mov ax, 0xB800          ; 비디어 메모리의 시작 어드레스(0xB800)를 세그먼트 레지스터 값으로 변환.
    mov es, ax              ; ES 세그먼트 레지스터에 설정.

    mov si, 0               ; SI 레지스터를 초기화.

.SCREEN_CLEAR_LOOP:             ; 화면을 지우는 루프
    mov byte [es:si], 0         ; 비디오 메모리의 문자가 위치하는 어드레스에 0을 복사하여 문자 삭재.
    mov byte [es:si + 1], 0x0a  ; 검은 바탕에 밝은 녹색 복사.

    add si, 2                   ; 다음 위치로 이동.

    cmp si, 80 * 25 * 2         ; 현재 크기가 전체인지 비교.
    jl .SCREEN_CLEAR_LOOP       ; 반복문으로 점프

    mov si, 0                   ; 반복문 프롤로그(si를 썼으니, 0으로 초기화)
    mov di, 0                   ; 반복문 프롤로그(si를 썼으니, 0으로 초기화)

.MESSAGE_LOOP:                          ; 메시지를 출력하는 루프.
    mov cl, byte[si + SHOW_MESSAGE ]    ; 루프의 끝(cl) 정의, cl은 cx 레지스터의 하위 1바이트를 의미하는데, 문자열은 1바이트로 충분.

    cmp cl, 0                           ; cl이 0인지 비교.
    je .MESSAGE_END                     ; 복사한 문자의 값이 0(NULL)이면, 종료되었기에, .MESSAGE_END로 이동

    mov byte[es:di], cl                 ; 0이 아니라면, 비디오 메모리 어드레스 0xB800:di에 문자 출력

    add si, 1                           ; SI 레지스터에 1 추가.
    add di, 2                           ; DI 레지스터에 2를 더해서, 다음 문자 위치로 이동. 왜 2냐면, 상위 1바이트는 문자, 하위 1바이트는 속성이라 그럼.

    jmp .MESSAGE_LOOP                   ; 반복

.MESSAGE_END:
    jmp $           ; 현재 위치에서 무한 루프 수행
SHOW_MESSAGE: db 'MINT64 OS Boot Loader Start~!!', 0

times 510 - ( $ - $$ ) db 0x00  ; $: 현재 라인의 어드레스
                                ; $$: 현재 섹션의 시작 어드레스
                                ; $ - $$: 현 섹션을 기준으로 하는 오프셋
                                ; 510 - ($ - $$): 현재부터 어드레스 510까지
                                ; db 0x00: 1바이트를 선언하고 값은 0x00
                                ; time: 반복 수행
                                ; 현재 위치에서 어드레스 510까지 0x00으로 채움.

db 0x55     ; 1 바이트를 선언하고 값은 0x55
db 0xAA     ; 1 바이트를 선언하고 값은 0xAA
            ; 어드레스 511, 512에 0x55, 0xAA를 써서 부트 섹터로 표기함.