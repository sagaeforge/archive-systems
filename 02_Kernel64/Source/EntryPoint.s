[BITS 64]

SECTION .text

extern Main

START:
    mov ax, 0x10    ; IA-32e 모드 커널용 데이터 세그먼트 디스크립터를 ax 레지스터에 저장.
    mov ds, ax      ; 세그먼트 설정
    mov es, ax      ; 세그먼트 설정
    mov fs, ax      ; 세그먼트 설정
    mov gs, ax      ; 세그먼트 설정

    mov ss, ax          ; SS 세그먼트 셀렉터에 설정 (6 ~ 7MB 영역 사용하기 위한 목적)
    mov rsp, 0x6FFFF8   ; RSP 레지스터의 어드레스를 0x6FFFF8로 설정
    mov rbp, 0x6FFFF8   ; RBP 레지스터의 어드레스를 0x6FFFF8로 설정

    call Main           ; C언어 엔트리 포인트 함수 호출.

    jmp $