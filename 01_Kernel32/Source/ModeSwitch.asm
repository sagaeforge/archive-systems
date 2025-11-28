[BITS 32]           ; 이하 코드는 32비트 코드로 설정

global kReadCPUID, kSwitchAndExecute64bitKernel

SECTION .text

; CPU ID를 반환
; PARAM: DWORD dwEAX, DWORD* pdwEAX, pdwEBx, pdwECX, pdwEDX
kReadCPUID:
    push ebp
    mov ebp, esp
    push eax
    push ebx
    push ecx
    push edx
    push esi

    ;
    ; EAX 레지스터의 값으로 CPU ID 명령어 실행
    ;
    mov eax, dword [ ebp + 8 ]  ; 파라미터 1를 EAX 레지스터에 저장
    cpuid                       ; cpuid 명령 실행

    ; pdwEAX
    mov esi, dword [ ebp + 12 ] ; 파라미터 2를 ESI 레지스터에 저장
    mov dword [ esi ], eax      ; pdwEAX가 포인터이므로 포인터가 가리키는 어드레스에 EBX 레지스터의 값을 저장.

    ; pdwEBX
    mov esi, dword [ ebp + 16 ] ; 파라미터 3를 ESI 레지스터에 저장
    mov dword [ esi ], ebx      ; pdwEBX가 포인터이므로 포인터가 가리키는 어드레스에 EBX 레지스터의 값을 저장

    ; pdwECX
    mov esi, dword [ ebp + 20 ] ; 파라미터 4를 ESI 레지스터에 저장
    mov dword [ esi ], ecx      ; pdwECX가 포인터이므로 포인터가 가리키는 어드레스에 ECX 레지스터의 값을 저장

    ; pdwEDX
    mov esi, dword [ ebp + 24 ] ; 파라미터 5를 ESI 레지스터에 저장
    mov dword [ esi ], edx      ; pdwEDX가 포인터이므로 포인터가 가리키는 어드레스에 EDX 레지스터의 값을 저장

    pop esi                     ; 함수에서 사용이 끝난ㄴ ESI 레지스터부터 EBP 레지스터까지 스택에서 삽입된 값을 이용해서 복구
    pop edx
    pop ecx
    pop ebx
    pop eax
    pop ebp
    ret

; IA-32e 모드로 전환하고 64비트 커널을 수행
; PARAM: 없음.
kSwitchAndExecute64bitKernel:
    ;
    ;   CR4 컨트롤 레지스터의 PAE 비트를 1로 설정
    ;
    mov eax, cr4    ; CR4 컨트롤 레지스터의 값을 EAX 레지스터에 저장
    or eax, 0x20    ; PAE 비트(비트 5)를 1로 설정
    mov cr4, eax    ; cr4 = eax

    ;
    ;   CR3 컨트롤 레지스터에 PML4 테이블의 어드레스와 캐시 활성화.
    ;
    mov eax, 0x100000   ; EAX 레지스터에 PML4 테이블이 존재하는 0x100000(1MB)를 저장
    mov cr3, eax        ; CR3 컨트롤 레지스터에 0x100000(1MB)를 저장

    ;
    ;   IA32_EFER.LME를 1로 설정하여 IA-32e 모드를 활성화
    ;
    mov ecx, 0xC0000080 ; IA32_EFER MSR 레지스터의 어드레스를 저장
    rdmsr               ; MSR 레지스터 읽기

    or eax, 0x0100      ; EAX 레지스터에 저장된 IA32_EFER MSR의 하위 32비트에서 LME비트(8번)을 1로 설정
    wrmsr               ; MSR 레지스터에 쓰기

    ;
    ;   CR0 컨트롤 레지스터를 NW 비트(비트 29) = 0, CD 비트(비트 30) = 0, PG 비트(비트 31) = 1로 설정하여 캐시 기능과 페이징 기능을 활성화
    ;
    mov eax, cr0            ; EAX 레지스터에 CR0 컨트롤 레지스터를 저장
    or eax, 0xE0000000      ; NW 비트, CD 비트, PG 비트 모두 1로 설정
    xor eax, 0x60000000     ; NW 비트, CD 비트를 XOR하여 0으로 설정
    mov cr0, eax            ; cr0 = eax

    jmp 0x08:0x200000       ; CS 세그먼트 셀렉터를 IA32e 모드용 코드 세그먼트 디스크립터로 교체하고, 0x200000(2MB) 어드레스로 이동.