/**
 * file    Main.c (Refactored)
 * date    2009/01/09
 * author  kkamagui
 * Copyright(c)2008 All rights reserved by kkamagui
 * brief   키보드 디바이스 드라이버 소스 파일 (리팩토링 버전)
 */

#include "Types.h"
#include "AssemblyUtility.h"
#include "Keyboard.h"

////////////////////////////////////////////////////////////////////////////////
//
//  내부 전용 헬퍼 함수 (반복되는 대기 로직 제거)
//
////////////////////////////////////////////////////////////////////////////////

/**
 * 입력 버퍼(포트 0x60)가 빌 때까지 대기
 */
static BOOL kWaitForInputBuffer(void) {
    for (int i = 0; i < 0xFFFF; i++) {
        if (kIsInputBufferFull() == FALSE) {
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * 출력 버퍼(포트 0x60)가 찰 때까지 대기
 */
static BOOL kWaitForOutputBuffer(void) {
    for (int i = 0; i < 0xFFFF; i++) {
        if (kIsOutputBufferFull() == TRUE) {
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * 키보드로부터 ACK(0xFA)가 올 때까지 대기
 */
static BOOL kWaitForACK(void) {
    // ACK가 오기 전에 버퍼에 다른 키 데이터가 쌓일 수 있으므로 최대 100번 시도
    for (int j = 0; j < 100; j++) {
        // 출력 버퍼에 데이터가 들어올 때까지 대기
        if (kWaitForOutputBuffer() == FALSE) {
            break;
        }

        // 출력 버퍼(포트 0x60)에서 읽은 데이터가 ACK(0xFA)이면 성공
        if (kInPortByte(0x60) == 0xFA) {
            return TRUE;
        }
    }
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
//
// 키보드 컨트롤러 및 키보드 제어에 관련된 함수들
//
////////////////////////////////////////////////////////////////////////////////

BOOL kIsOutputBufferFull(void) {
    if (kInPortByte(0x64) & 0x01) {
        return TRUE;
    }
    return FALSE;
}

BOOL kIsInputBufferFull(void) {
    if (kInPortByte(0x64) & 0x02) {
        return TRUE;
    }
    return FALSE;
}

/**
 * 키보드를 활성화
 */
BOOL kActivateKeyboard(void) {
    // 1. 키보드 활성화 커맨드 전송 (컨트롤러 제어)
    kOutPortByte(0x64, 0xAE);

    // 2. 입력 버퍼 대기 후 키보드 활성화 커맨드 전송 (키보드 제어)
    if (kWaitForInputBuffer() == FALSE) {
        return FALSE;
    }
    kOutPortByte(0x60, 0xF4);

    // 3. ACK 대기
    return kWaitForACK();
}

/**
 * 출력 버퍼(포트 0x60)에서 키를 읽음
 */
BYTE kGetKeyboardScanCode(void) {
    while (kIsOutputBufferFull() == FALSE) {
        ;
    }
    return kInPortByte(0x60);
}

/**
 * 키보드 LED의 ON/OFF를 변경
 */
BOOL kChangeKeyboardLED(BOOL bCapsLockOn, BOOL bNumLockOn, BOOL bScrollLockOn) {
    // 1. LED 변경 커맨드(0xED) 전송
    if (kWaitForInputBuffer() == FALSE) {
        return FALSE;
    }
    kOutPortByte(0x60, 0xED);

    // 2. 키보드가 커맨드를 가져갈 때까지 대기 (입력 버퍼 비움 확인)
    if (kWaitForInputBuffer() == FALSE) {
        return FALSE;
    }

    // 3. ACK 확인
    if (kWaitForACK() == FALSE) {
        return FALSE;
    }

    // 4. LED 데이터 전송
    kOutPortByte(0x60, (bCapsLockOn << 2) | (bNumLockOn << 1) | bScrollLockOn);

    // 5. 키보드가 데이터를 가져갈 때까지 대기
    if (kWaitForInputBuffer() == FALSE) {
        return FALSE;
    }

    // 6. ACK 확인
    return kWaitForACK();
}

/**
 * A20 게이트를 활성화
 */
void kEnableA20Gate(void) {
    BYTE bOutputPortData;

    // 1. 출력 포트 읽기 커맨드(0xD0) 전송
    kOutPortByte(0x64, 0xD0);

    // 2. 출력 포트 데이터 수신 대기
    if (kWaitForOutputBuffer() == FALSE) {
        return; // 에러 처리 필요 시 추가
    }

    // 3. 출력 포트 데이터 읽기 및 A20 비트 설정
    bOutputPortData = kInPortByte(0x60);
    bOutputPortData |= 0x01;

    // 4. 출력 포트 설정 커맨드(0xD1) 전송을 위해 입력 버퍼 대기
    if (kWaitForInputBuffer() == FALSE) {
        return;
    }
    kOutPortByte(0x64, 0xD1);

    // 5. 변경된 데이터 전송
    kOutPortByte(0x60, bOutputPortData);
}

/**
 * 프로세서를 리셋(Reset)
 */
void kReboot(void) {
    // 1. 입력 버퍼 대기
    if (kWaitForInputBuffer() == FALSE) {
        return;
    }

    // 2. 출력 포트 설정 커맨드 전송
    kOutPortByte(0x64, 0xD1);

    // 3. 0x00을 전송하여 리셋 (Bit 0=0: Reset)
    kOutPortByte(0x60, 0x00);

    while (1) {
        ;
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// 스캔 코드를 ASCII 코드로 변환하는 기능에 관련된 함수들
//
////////////////////////////////////////////////////////////////////////////////
static KEYBOARDMANAGER gs_stKeyboardManager = {0,};

// (매핑 테이블은 데이터이므로 변경 없음 - 생략 가능하나 컴파일을 위해 유지)
static KEYMAPPINGENTRY gs_vstKeyMappingTable[KEY_MAPPINGTABLEMAXCOUNT] =
{
    /* 0   */ {KEY_NONE, KEY_NONE},
    /* 1   */ {KEY_ESC, KEY_ESC},
    /* 2   */ {'1', '!'},
    /* 3   */ {'2', '@'},
    /* 4   */ {'3', '#'},
    /* 5   */ {'4', '$'},
    /* 6   */ {'5', '%'},
    /* 7   */ {'6', '^'},
    /* 8   */ {'7', '&'},
    /* 9   */ {'8', '*'},
    /* 10  */ {'9', '('},
    /* 11  */ {'0', ')'},
    /* 12  */ {'-', '_'},
    /* 13  */ {'=', '+'},
    /* 14  */ {KEY_BACKSPACE, KEY_BACKSPACE},
    /* 15  */ {KEY_TAB, KEY_TAB},
    /* 16  */ {'q', 'Q'},
    /* 17  */ {'w', 'W'},
    /* 18  */ {'e', 'E'},
    /* 19  */ {'r', 'R'},
    /* 20  */ {'t', 'T'},
    /* 21  */ {'y', 'Y'},
    /* 22  */ {'u', 'U'},
    /* 23  */ {'i', 'I'},
    /* 24  */ {'o', 'O'},
    /* 25  */ {'p', 'P'},
    /* 26  */ {'[', '{'},
    /* 27  */ {']', '}'},
    /* 28  */ {'\n', '\n'},
    /* 29  */ {KEY_CTRL, KEY_CTRL},
    /* 30  */ {'a', 'A'},
    /* 31  */ {'s', 'S'},
    /* 32  */ {'d', 'D'},
    /* 33  */ {'f', 'F'},
    /* 34  */ {'g', 'G'},
    /* 35  */ {'h', 'H'},
    /* 36  */ {'j', 'J'},
    /* 37  */ {'k', 'K'},
    /* 38  */ {'l', 'L'},
    /* 39  */ {';', ':'},
    /* 40  */ {'\'', '\"'},
    /* 41  */ {'`', '~'},
    /* 42  */ {KEY_LSHIFT, KEY_LSHIFT},
    /* 43  */ {'\\', '|'},
    /* 44  */ {'z', 'Z'},
    /* 45  */ {'x', 'X'},
    /* 46  */ {'c', 'C'},
    /* 47  */ {'v', 'V'},
    /* 48  */ {'b', 'B'},
    /* 49  */ {'n', 'N'},
    /* 50  */ {'m', 'M'},
    /* 51  */ {',', '<'},
    /* 52  */ {'.', '>'},
    /* 53  */ {'/', '?'},
    /* 54  */ {KEY_RSHIFT, KEY_RSHIFT},
    /* 55  */ {'*', '*'},
    /* 56  */ {KEY_LALT, KEY_LALT},
    /* 57  */ {' ', ' '},
    /* 58  */ {KEY_CAPSLOCK, KEY_CAPSLOCK},
    /* 59  */ {KEY_F1, KEY_F1},
    /* 60  */ {KEY_F2, KEY_F2},
    /* 61  */ {KEY_F3, KEY_F3},
    /* 62  */ {KEY_F4, KEY_F4},
    /* 63  */ {KEY_F5, KEY_F5},
    /* 64  */ {KEY_F6, KEY_F6},
    /* 65  */ {KEY_F7, KEY_F7},
    /* 66  */ {KEY_F8, KEY_F8},
    /* 67  */ {KEY_F9, KEY_F9},
    /* 68  */ {KEY_F10, KEY_F10},
    /* 69  */ {KEY_NUMLOCK, KEY_NUMLOCK},
    /* 70  */ {KEY_SCROLLLOCK, KEY_SCROLLLOCK},

    /* 71  */ {KEY_HOME, '7'},
    /* 72  */ {KEY_UP, '8'},
    /* 73  */ {KEY_PAGEUP, '9'},
    /* 74  */ {'-', '-'},
    /* 75  */ {KEY_LEFT, '4'},
    /* 76  */ {KEY_CENTER, '5'},
    /* 77  */ {KEY_RIGHT, '6'},
    /* 78  */ {'+', '+'},
    /* 79  */ {KEY_END, '1'},
    /* 80  */ {KEY_DOWN, '2'},
    /* 81  */ {KEY_PAGEDOWN, '3'},
    /* 82  */ {KEY_INS, '0'},
    /* 83  */ {KEY_DEL, '.'},
    /* 84  */ {KEY_NONE, KEY_NONE},
    /* 85  */ {KEY_NONE, KEY_NONE},
    /* 86  */ {KEY_NONE, KEY_NONE},
    /* 87  */ {KEY_F11, KEY_F11},
    /* 88  */ {KEY_F12, KEY_F12}
};

BOOL kIsAlphabetScanCode(BYTE bScanCode) {
    if (('a' <= gs_vstKeyMappingTable[bScanCode].bNormalCode) &&
        (gs_vstKeyMappingTable[bScanCode].bNormalCode <= 'z')) {
        return TRUE;
    }
    return FALSE;
}

BOOL kIsNumberOrSymbolScanCode(BYTE bScanCode) {
    if ((2 <= bScanCode) && (bScanCode <= 53) &&
        (kIsAlphabetScanCode(bScanCode) == FALSE)) {
        return TRUE;
    }
    return FALSE;
}

BOOL kIsNumberPadScanCode(BYTE bScanCode) {
    if ((71 <= bScanCode) && (bScanCode <= 83)) {
        return TRUE;
    }
    return FALSE;
}

/**
 * 조합된 키 값을 사용해야 하는지 여부를 반환 (리팩토링: 로직 단순화)
 */
BOOL kIsUseCombinedCode(BOOL bScanCode) {
    BYTE bDownScanCode;
    BOOL bUseCombinedKey = FALSE;

    bDownScanCode = bScanCode & 0x7F;

    // 1. 알파벳 키: Shift XOR CapsLock
    if (kIsAlphabetScanCode(bDownScanCode) == TRUE) {
        bUseCombinedKey = (gs_stKeyboardManager.bShiftDown ^ gs_stKeyboardManager.bCapsLockOn);
    }
    // 2. 숫자/기호 키: Shift
    else if (kIsNumberOrSymbolScanCode(bDownScanCode) == TRUE) {
        bUseCombinedKey = gs_stKeyboardManager.bShiftDown;
    }
    // 3. 숫자 패드: NumLock (확장 키 아닐 때만)
    else if ((kIsNumberPadScanCode(bDownScanCode) == TRUE) &&
             (gs_stKeyboardManager.bExtendedCodeIn == FALSE)) {
        bUseCombinedKey = gs_stKeyboardManager.bNumLockOn;
    }

    return bUseCombinedKey;
}

void UpdateCombinationKeyStatusAndLED(BYTE bScanCode) {
    BOOL bDown;
    BYTE bDownScanCode;
    BOOL bLEDStatusChanged = FALSE;

    if (bScanCode & 0x80) {
        bDown = FALSE;
        bDownScanCode = bScanCode & 0x7F;
    } else {
        bDown = TRUE;
        bDownScanCode = bScanCode;
    }

    // 조합 키 검색 및 LED 상태 갱신
    if ((bDownScanCode == 42) || (bDownScanCode == 54)) {
        gs_stKeyboardManager.bShiftDown = bDown;
    } else if ((bDownScanCode == 58) && (bDown == TRUE)) {
        gs_stKeyboardManager.bCapsLockOn ^= TRUE;
        bLEDStatusChanged = TRUE;
    } else if ((bDownScanCode == 69) && (bDown == TRUE)) {
        gs_stKeyboardManager.bNumLockOn ^= TRUE;
        bLEDStatusChanged = TRUE;
    } else if ((bDownScanCode == 70) && (bDown == TRUE)) {
        gs_stKeyboardManager.bScrollLockOn ^= TRUE;
        bLEDStatusChanged = TRUE;
    }

    if (bLEDStatusChanged == TRUE) {
        kChangeKeyboardLED(gs_stKeyboardManager.bCapsLockOn,
                           gs_stKeyboardManager.bNumLockOn, gs_stKeyboardManager.bScrollLockOn);
    }
}

BOOL kConvertScanCodeToASCIICode(BYTE bScanCode, BYTE *pbASCIICode, BOOL *pbFlags) {
    BOOL bUseCombinedKey;

    if (gs_stKeyboardManager.iSkipCountForPause > 0) {
        gs_stKeyboardManager.iSkipCountForPause--;
        return FALSE;
    }

    if (bScanCode == 0xE1) {
        *pbASCIICode = KEY_PAUSE;
        *pbFlags = KEY_FLAGS_DOWN;
        gs_stKeyboardManager.iSkipCountForPause = KEY_SKIPCOUNTFORPAUSE;
        return TRUE;
    } else if (bScanCode == 0xE0) {
        gs_stKeyboardManager.bExtendedCodeIn = TRUE;
        return FALSE;
    }

    bUseCombinedKey = kIsUseCombinedCode(bScanCode);

    if (bUseCombinedKey == TRUE) {
        *pbASCIICode = gs_vstKeyMappingTable[bScanCode & 0x7F].bCombinedCode;
    } else {
        *pbASCIICode = gs_vstKeyMappingTable[bScanCode & 0x7F].bNormalCode;
    }

    if (gs_stKeyboardManager.bExtendedCodeIn == TRUE) {
        *pbFlags = KEY_FLAGS_EXTENDEDKEY;
        gs_stKeyboardManager.bExtendedCodeIn = FALSE;
    } else {
        *pbFlags = 0;
    }

    if ((bScanCode & 0x80) == 0) {
        *pbFlags |= KEY_FLAGS_DOWN;
    }

    UpdateCombinationKeyStatusAndLED(bScanCode);
    return TRUE;
}
