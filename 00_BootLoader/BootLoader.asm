[ORG 0x00] ; 코드의 시작 어드레스를 0x00으로 설정
[BITS 16] ; 이하의 코드는 16비트 코드로 설정

SECTION .text ; text 섹션(세그먼트)을 정의

jmp 0x07c0:START ; CS 세그먼트 레지스터에 0x07C0을 복사하면서, START 레이블로 이동.

;
;   MINT64 OS에 관련된 환경 설정 값
;
TOTAL_SECTOR_COUNT: dw 2 ; 부트 로더를 제외한 MINT OS 이미지의 크기, 최대 1152 섹터(0x9000Byte)까지 가능.

;
;   코드 영역
;
START:
    mov ax, 0x07c0          ; AX 레지스터에 0x07c0 복사
    mov ds, ax              ; DS 세그먼트 레지스터에 AS 레지스터의 값 복사.
    mov ax, 0xB800          ; 비디어 메모리의 시작 어드레스(0xB800)를 세그먼트 레지스터 값으로 변환.
    mov es, ax              ; ES 세그먼트 레지스터에 설정.

    mov ax, 0x0000          ; 스택 세그멘트의 시작 어드레스를 세그먼트 레지스터 값으로 변환.
    mov ss, ax              ; SS 세그먼트 레지스터의 설정.
    mov sp, 0xFFFE          ; SP 레지스터의 어드레스를 0xFFFE로 설정.
    mov bp, 0xFFFE          ; BP 레지스터의 어드레스를 0xFFFE로 설정.

    ;
    ;   화면 초기화(모두 지우고, 속성 값을 녹색으로 설정.)
    ;
    mov si, 0               ; SI 레지스터 초기화.

.SCREEN_CLEAR_LOOP:             ; 화면을 지우는 루프
    mov byte [es:si], 0         ; 비디오 메모리의 문자가 위치하는 어드레스에 0을 복사하여 문자 삭재.
    mov byte [es:si + 1], 0x0a  ; 검은 바탕에 밝은 녹색 복사.

    add si, 2                   ; 다음 위치로 이동.

    cmp si, 80 * 25 * 2         ; 현재 크기가 전체인지 비교.
    jl .SCREEN_CLEAR_LOOP       ; 반복문으로 점프

    ;
    ;   화면 상단에 시작 메시지 출력
    ;
    push SHOW_MESSAGE                   ; 출력할 메시지의 어드레스를 스택에 삽입.
    push 0                              ; 화면 Y 좌표(0)를 스택에 삽입.
    push 0                              ; 화면 X 좌표(0)를 스택에 삽입.
    call PRINT_MESSAGE                  ; PRINT_MESSAGE 함수 호출.
    add sp, 6                           ; 삽입한 파라미터 제거.

    ;
    ;   OS 이미지를 로딩한다는 메시지 출력
    ;
    push IMAGE_LOADING_MESSAGE          ; 출력할 메시지의 어드레스를 스택에 삽입.
    push 1                              ; 화면 Y 좌표(1)를 스택에 삽입.
    push 0                              ; 화면 X 좌표(0)를 스택에 삽입.
    call PRINT_MESSAGE                  ; PRINT_MESSAGE 함수 호출.
    add sp, 6                           ; 삽입한 파라미터 제거.

    ;
    ;   디스크에서 OS 이미지 로딩
    ;

    ;
    ;   디스크를 읽기 전에 먼저 리셋
    ;
RESET_DISK:                             ; 디스크를 리셋하는 코드의 시작.
    ;
    ;   BIOS Reset Function 호출
    ;
    mov ax, 0                           ; 서비스 번호 0
    mov dl, 0                           ; 드라이브 번호(0 = Floppy)
    int 0x13                            ; BIOS 서비스 인터럽트(0x13 = Disk I/O Service)

    jc HANDLE_DISK_ERROR                ; 문제가 발생하면, 에러 처리로 이동

    ;
    ;   디스크에서 섹터를 읽음.
    ;
    mov si, 0x1000                      ; 디스크의 내용을 메모리로 복사할 어드레스를(ES:BX)를 0x10000으로 설정
    mov es, si
    mov bx, 0x0000

    mov di, word [ TOTAL_SECTOR_COUNT ] ; 복사할 OS 이미지의 섹터 수를 DI 레지스터에 저장.

READ_DATA:
    cmp di, 0                           ; 복사할 OS 이미지의 섹터 수를 0과 비교.
    je READ_END                         ; 다 읽었으면 이동(READ_END)
    sub di, 0x1                         ; 복사할 섹터 수를 1 감소

    ;
    ;   BIOS Read Function 호출
    ;
    mov ah, 0x02                        ; BIOS 서비스 번호 2(Read Sector)
    mov al, 0x1                         ; 읽을 섹터의 수 설정(1)
    mov ch, byte [ TRACK_NUMBER ]       ; 읽을 트랙 번호 설정.
    mov cl, byte [ SECTOR_NUMBER ]      ; 읽을 섹터 번호 설정.
    mov dh, byte [ HEAD_NUMBER ]        ; 읽을 헤드 번호 설정.
    mov dl, 0x00                        ; 읽을 드라이브 번호 설정(0 = Floppy) 설정
    int 0x13                            ; BIOS 서비스 인터럽트(0x13 = Disk I/O Service)
    jc HANDLE_DISK_ERROR                ; 문제가 발생하면, 애러 처리로 이동.

    ;
    ;   복사할 어드레스와 트랙, 헤드, 섹터 주소 계산
    ;
    add si, 0x20                        ; 512(0x20) 바이트 만큼 읽었으므로, 세그먼트 레지스터 값으로 변환.
                                        ; +추가 si에 바로 오프셋을 바로 더할 수 있으나, 세그먼트 이동 방식이기에, 512바이트를 세그먼트 단위로 표현시, 512 / 16 = 32, 32 = 0x20(16진수) 그렇기에, si를 0x20으로 설정.
                                        ; +추가 bs를 사용하지 않는 이유는 bx가 0xffff를 넘으면 오버플로우인데, 여러 섹터를 읽을 때 많이 복잡해짐.
    mov es, si                          ; es 세그먼트 레지스터에 더해서 어드레스를 한 섹터 만큼 증가.

    ; 한섹터를 읽었으므로, 섹터 번호를 증가 시키고, 마지막 섹터(18)까지 읽었는 지 판단.
    ; 마지막 섹터가 아니라면, 섹터 읽기로 이동해서 다시 섹터 읽기 수행.
    mov al, byte [ SECTOR_NUMBER ]      ; 섹터 번호를 AL 레지스터에 설정.
    add al, 0x01                        ; 섹터 번호를 1 추가.
    mov byte [ SECTOR_NUMBER ], al      ; al 값을 섹터 번호에 적용.
    cmp al, 37                          ; 증가시킨 섹터 번호를 37와 비교.
                                        ; + 18개가 아니라, 36개라고 합니다.
    jl READ_DATA                        ; 아니라면 읽도록 이동.

    ; 마지막 섹터까지 읽었다면, 헤드를 토클하고, 섹터 번호를 1로 설정.
    ; 토글 이유는 플로피가 양면 디스크라, 앞 뒤로 읽기 위함.
    xor byte [ HEAD_NUMBER ], 0x01      ; 헤드 번호를 토글
    mov byte [ SECTOR_NUMBER ], 0x01    ; 섹터 번호를 1로 설정.

    ; 만약 헤드가 1 -> 0으로 갔으면 모두 읽었기에, 트랙 번호를 1 증가.
    cmp byte [ HEAD_NUMBER ], 0x00      ; 헤드 번호를 0과 비교.
    jne READ_DATA                       ; 다시 읽으십쇼ㅋㅋ.

    ; 트랙 1 증가 시킨 후 다시 섹터 읽기로 이동
    add byte [ TRACK_NUMBER ], 0x01     ; 트랙 번호 1 증가.
    jmp READ_DATA                       ; 읽으십쇼.

READ_END:
    ;
    ;   OS 이미지 로드 완료되었다는 메시지 출력
    ;
    push LOADING_COMPLETE_MESSAGE       ; 출력할 메시지의 어드레스를 스택에 삽입.
    push 1                              ; 화면 Y 좌표(1)를 스택에 삽입.
    push 20                             ; 화면 X 좌표(20)를 스택에 삽입.
    call PRINT_MESSAGE                  ; PRINT_MESSAGE 함수 호출.
    add sp, 6                           ; 삽입한 파라미터 제거.

    jmp 0x1000:0x0000                   ; 로딩한 가상 OS 이미지 실행.

HANDLE_DISK_ERROR:
    push DISK_ERROR_MESSAGE             ; 출력할 메시지의 어드레스를 스택에 삽입.
    push 1                              ; 화면 Y 좌표(1)를 스택에 삽입.
    push 20                             ; 화면 X 좌표(20)를 스택에 삽입.
    call PRINT_MESSAGE                  ; PRINT_MESSAGE 함수 호출.
    add sp, 6                           ; 삽입한 파라미터 제거.

    jmp $                               ; 현재 위치에서 무한 루프 수행.

PRINT_MESSAGE: ; 메시지를 출력하는 함수.
    push bp
    mov bp, sp

    push es
    push si
    push di
    push ax
    push cx
    push dx

    ;
    ; 위에는 이전 함수 호출 스택을 저장하는 거임.
    ;

    mov ax, 0xB800                      ; 비디오 메모리 시작 어드레스를 세그먼트 레지스터 값으로 변환.
    mov es, ax                          ; es에 설정.

    ; y 좌표 처리.
    mov ax, word [ bp + 6 ]             ; 파리머티 2(bp + 6, 6 = word_size(2) * param_len(3))
    mov si, 160                         ; 한 라인의 최대 바이트 수를 SI 레지스터에 설정
    mul si                              ; ax * si를 통해서 y 어드레스 구함.
    mov di, ax                          ; ax는 y좌표가 있기에, di 레지스터에 옮김.

    ; x 좌표 처리
    mov ax, word [ bp + 4 ]             ; 파라미터 1(bp + 4, 4 = word_size(2) * 2)
    mov si, 2                           ; 한 문자를 나타내는 바이트 수(2)를 SI 레지스터에 대입.
    mul si                              ; 곱해, x 위치를 구함.
    add di, ax                          ; y좌표에 x 좌표를 더함.

    mov si, word [ bp + 8 ]             ; 파리미터 3

.MESSAGE_LOOP:                          ; 메시지를 출력하는 루프.
    mov cl, byte[ si ]                  ; 루프의 끝(cl) 정의, cl은 cx 레지스터의 하위 1바이트를 의미하는데, 문자열은 1바이트로 충분.

    cmp cl, 0                           ; cl이 0인지 비교.
    je .MESSAGE_END                     ; 복사한 문자의 값이 0(NULL)이면, 종료되었기에, .MESSAGE_END로 이동

    mov byte[es:di], cl                 ; 0이 아니라면, 비디오 메모리 어드레스 0xB800:di에 문자 출력

    add si, 1                           ; SI 레지스터에 1 추가.
    add di, 2                           ; DI 레지스터에 2를 더해서, 다음 문자 위치로 이동. 왜 2냐면, 상위 1바이트는 문자, 하위 1바이트는 속성이라 그럼.

    jmp .MESSAGE_LOOP                   ; 반복

.MESSAGE_END:
    pop dx
    pop cx
    pop ax
    pop di
    pop si
    pop es
    pop bp
    ret

SHOW_MESSAGE:               db 'MINT64 OS Boot Loader Start~!!', 0

DISK_ERROR_MESSAGE:         db 'DISK Error~!!', 0
IMAGE_LOADING_MESSAGE:      db 'OS Image Loading...', 0
LOADING_COMPLETE_MESSAGE:   db 'Load Complete~!!', 0

SECTOR_NUMBER:              db 0x02 ; 0x00, 0x01이 아닌 이유, 0x00 시스템 섹터, 0x01 부트 섹터임.
HEAD_NUMBER:                db 0x00
TRACK_NUMBER:               db 0x00

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