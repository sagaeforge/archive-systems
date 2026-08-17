//
// Created by lambda on 11/26/25.
//

#include "Types.h"
#include "Page.h"
#include "ModeSwitch.h"

void kPrintString(int iX, int iY, const char *pcString);
BOOL kInitializeKernel64Area(void);
BOOL kIsMemoryEnough(void);
void kCopyKernel64ImageTo2MByte(void);

void Main(void) {
	kPrintString(0, 3, "Protected Mode C Language Kernel Start................[Pass]");

	// 최소 메모리 크기를 만족하는 지 검사.
	kPrintString(0, 4, "Minimum Memory Size Check.............................[    ]");
	if (kIsMemoryEnough() == FALSE) {
		kPrintString(55, 4, "FAIL");
		kPrintString(0, 5, "Not Enough Memory~!! MINT64 OS Requires Over 64MB Memory");

		while (1);
	} else {
		kPrintString(55, 4, "Pass");
	}

	// IA-32e 모드의 커널 영역을 초기화
	kPrintString(0, 5, "IA-32e Kernel Area Initialize.........................[    ]");
	if (kInitializeKernel64Area() == FALSE) {
		kPrintString(55, 5, "FAIL");
		kPrintString(0, 6, "Kernel Area Initialization failed");

		while (1);
	}

	kPrintString(55, 5, "Pass");

	// IA-32e 모드 커널을 위한 페이지 테이블 생성
	kPrintString(0, 6, "IA-32e Page Tables Initialize.........................[    ]");
	kInitializePageTables();
	kPrintString(55, 6, "Pass");

	// 프로세서 제조사 정보 읽기
	DWORD dwEAX, dwEBX, dwECX, dwEDX;
	char vcVendorString[13];
	for (int i = 0; i < 13; i++) {
		vcVendorString[i] = 0;
	}

	kReadCPUID(0x00, &dwEAX, &dwEBX, &dwECX, &dwEDX);
	*(DWORD *) vcVendorString = dwEBX;
	*((DWORD *) vcVendorString + 1) = dwEDX;
	*((DWORD *) vcVendorString + 2) = dwECX;
	kPrintString(0, 7, "Processor Vendor String...............................[            ]");
	kPrintString(55, 7, vcVendorString);

	// 64비트 지원 유무 확인
	kReadCPUID(0x80000001, &dwEAX, &dwEBX, &dwECX, &dwEDX);
	kPrintString(0, 8, "64bit Mode Support Check..............................[    ]");
	if (dwEDX & (1 << 29)) {
		kPrintString(55, 8, "Pass");
	} else {
		kPrintString(55, 8, "Fail");
		kPrintString(0, 9, "This processor does not support 64bit mode~!!");
		while (1);
	}

	// IA-32e 모드 커널을 0x20_0000(2MB) 주소로 이동.
	kPrintString(0, 9, "Copy IA-32e Kernel To 2M Addresss.....................[    ]");
	kCopyKernel64ImageTo2MByte();
	kPrintString(55, 9, "Pass");

	// [검증 코드] 0x200000 주소(커널 시작점) 앞부분 4바이트를 확인
	DWORD *pCheck = (DWORD *) 0x200000;

	// 만약 0이라면 복사가 안 된 것임 -> 에러 메시지 출력하고 무한 루프
	if (*pCheck == 0) {
		kPrintString(0, 10, "Copy Failed! 0x200000 is EMPTY!");
		while (1);
	} else {
		// 값이 있다면 뭔가 들어있다는 뜻 (정확한 값 확인은 어렵지만 일단 0은 아님)
		kPrintString(0, 10, "Copy Check OK. Jumping...");
	}

	// IA-32e 모드로 전환
	kPrintString(0, 10, "Switch To IA-32e Moded");
	kSwitchAndExecute64bitKernel();

	while (1);
}

void kPrintString(int iX, int iY, const char *pcString) {
	CHARACTOR *pstScreen = (CHARACTOR *) 0xB8000;
	int i;

	pstScreen += (iY * 80) + iX;
	for (i = 0; pcString[i] != 0; i++) {
		pstScreen[i].bCharacter = pcString[i];
	}
}

BOOL kInitializeKernel64Area(void) {
	// 초기화를 시작할 어드레스인 0x100000(1MB)을 설정
	DWORD *pdwCurrentAddress = (DWORD *) 0x100000;

	// 마지막 어드레스인 0x600000(6MB)까지 루프를 돌면서 4바이트씩 0으로 채움.
	while ((DWORD) pdwCurrentAddress < (DWORD) 0x600000) {
		*pdwCurrentAddress = 0x00;

		// 0으로 저장한 후, 다시 읽었을 때, 0이 나오지 않는 다면 해당 어드레스를 사용하는 데 문제가 생긴것으므로 더 이상 진행하지 않고 종료
		if (*pdwCurrentAddress != 0x00) {
			return FALSE;
		}

		// 다음 어드레스로 이동.
		pdwCurrentAddress++;
	}

	return TRUE;
}

BOOL kIsMemoryEnough(void) {
	// 0x100000(1MB)부터 검사 시작
	DWORD *pdwCurrentAddress = (DWORD *) 0x100000;

	// 0x4000000(64MB)까지 루프를 돌면서 확인
	while ((DWORD) pdwCurrentAddress < (DWORD) 0x4000000) {
		*pdwCurrentAddress = 0x12345678;

		// 0x12345678로 저장한 후, 다시 읽었을 때, 0x12345678이 나오지 않으면, 해당 어드레스를 사용하는 데 문제가 생긴 것이므로 더 이상 진행하지 않고 종료.
		if (*pdwCurrentAddress != 0x12345678) {
			return FALSE;
		}

		pdwCurrentAddress += (0x100000 / 4);
	}

	return TRUE;
}

void kCopyKernel64ImageTo2MByte(void) {
	WORD wTotalKernelSectorCount = *((WORD *) 0x7c05);
	WORD wKernel32SectorCount = *((WORD *) 0x7c07);
	DWORD *pdwSourceAddress = (DWORD *) (0x10000 + (wKernel32SectorCount * 512));
	DWORD *pdwDestinationAddress = (DWORD *) 0x200000;

	for (int i = 0; i < 512 * (wTotalKernelSectorCount - wKernel32SectorCount) / 4; i++) {
		*pdwDestinationAddress = *pdwSourceAddress;
		pdwDestinationAddress++;
		pdwSourceAddress++;
	}
}
