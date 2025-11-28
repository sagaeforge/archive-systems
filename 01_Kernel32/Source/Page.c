#include "Page.h"

void kInitializePageTables(void) {
    // -------------------------------------------------------------------------
    // 1. PML4 테이블 생성 (Level 4, 최상위 지도)
    // - 위치: 0x100000 (1MB 지점)
    // - 역할: 512GB 단위의 큰 구역(PDPT)을 관리하는 총괄 본부
    // -------------------------------------------------------------------------
    PML4TENTRY *pstPML4TEntry = (PML4TENTRY *) 0x100000;

    // [Entry 0 설정]
    // 0번 엔트리를 활성화하여 바로 아래 단계인 PDPT(0x101000)로 연결합니다.
    // 0번 엔트리 하나만 있으면 512GB까지 커버되므로, 우리는 이것 하나면 충분합니다.
    kSetPageEntryData(&(pstPML4TEntry[0]), 0x00, 0x101000, PAGE_FLAGS_DEFAULT, 0);

    // [나머지 초기화]
    // 1번~511번 엔트리는 사용하지 않으므로 0으로 깨끗이 비웁니다.
    for (int i = 1; i < PAGE_MAX_ENTRY_COUNT; i++) {
        kSetPageEntryData(&(pstPML4TEntry[i]), 0, 0, 0, 0);
    }

    // -------------------------------------------------------------------------
    // 2. PDPT (Page Directory Pointer Table) 생성 (Level 3)
    // - 위치: 0x101000 (PML4 바로 뒤)
    // - 역할: 1GB 단위의 구역(PD)을 관리하는 지역 본부
    // -------------------------------------------------------------------------
    PDPTENTRY *pstPDPTEntry = (PDPTENTRY *) 0x101000;

    // [0~63번 Entry 설정] -> 총 64GB 매핑 준비
    // i번째 엔트리는 i번째 PD(Page Directory) 테이블을 가리킵니다.
    // PD 테이블 하나가 1GB를 담당하므로, 64개를 만들면 총 64GB 메모리를 관리할 수 있습니다.
    // 구조: 0x102000부터 4KB(0x1000) 간격으로 PD 테이블들이 줄지어 배치됩니다.
    for (int i = 0; i < 64; i++) {
        // 주의: 원본 코드의 0x10200 오타 수정 -> 0x102000 (PD 시작 주소)
        kSetPageEntryData(&(pstPDPTEntry[i]), 0, 0x102000 + (i * PAGE_TABLE_SIZE), PAGE_FLAGS_DEFAULT, 0);
    }

    // [나머지 초기화]
    // 64GB 이후의 영역은 당장 필요 없으므로 비활성화(0) 합니다.
    for (int i = 64; i < PAGE_MAX_ENTRY_COUNT; i++) {
        kSetPageEntryData(&(pstPDPTEntry[i]), 0, 0, 0, 0);
    }

    // -------------------------------------------------------------------------
    // 3. PD (Page Directory) 생성 (Level 2) - 여기가 핵심!
    // - 위치: 0x102000 부터 쭉 이어짐
    // - 역할: 실제 물리 메모리 주소와 연결하는 실무자
    // -------------------------------------------------------------------------
    PDENTRY *pstPDEntry = (PDPTENTRY *) 0x102000;

    // 매핑할 실제 물리 메모리 주소 (0번지부터 시작)
    DWORD dwMappingAddress = 0;

    // 총 엔트리 개수: 512개(한 테이블) * 64개(테이블 개수) = 32,768개의 페이지
    for (int i = 0; i < PAGE_MAX_ENTRY_COUNT * 64; i++) {
        // [상위 주소 계산 마법]
        // 32비트 변수로는 4GB 이상의 주소를 담을 수 없습니다.
        // 따라서 (i * 2MB)로 계산된 주소 중, 32비트를 넘어가는 상위 비트만 추출하는 식입니다.
        // PAGE_DEFAULT_SIZE(2MB) >> 20 = 2
        // (i * 2) >> 12  ==> 결국 (i * 2MB) >> 32 와 같은 의미가 됩니다.
        DWORD dwUpperBaseAddress = (i * (PAGE_DEFAULT_SIZE >> 20)) >> 12;

        // [엔트리 설정]
        // - PAGE_FLAGS_PS (Page Size): 이 비트가 1이면 "밑으로(PTE) 내려가지 말고 여기가 끝(2MB 페이지)이다"라는 뜻.
        // - 3단계 페이징의 핵심입니다.
        kSetPageEntryData(&(pstPDEntry[i]),
                          dwUpperBaseAddress,
                          dwMappingAddress, // 하위 32비트 주소
                          PAGE_FLAGS_DEFAULT | PAGE_FLAGS_PS,
                          0);

        // [주소 증가]
        // 다음 페이지는 2MB 뒤에 있으므로 주소를 2MB(0x200000)만큼 늘려줍니다.
        // (주의: 원본의 0x1000은 4KB이므로 2MB 페이지에 맞게 수정해야 함)
        dwMappingAddress += PAGE_DEFAULT_SIZE;
    }
}

// -------------------------------------------------------------------------
// 유틸리티 함수: 64비트 엔트리를 상위/하위 32비트로 쪼개서 저장
// -------------------------------------------------------------------------
void kSetPageEntryData(PTENTRY *pstEntry, DWORD dwUpperBaseAddress, DWORD dwLowerBaseAddress, DWORD dwLowerFlags,
                       DWORD dwUpperFlags) {
    // 하위 32비트: 주소의 하위 부분 + 기본 속성(P, RW 등)
    pstEntry->dwAttributesAndLowerBaseAddress = dwLowerBaseAddress | dwLowerFlags;

    // 상위 32비트: 주소의 상위 부분(8비트만 씀) + 확장 속성(EXB 등)
    // 0xFF로 마스킹하는 이유는 x86-64에서 물리 주소는 최대 52비트까지만 쓰기 때문입니다.
    pstEntry->dwUpperBaseAddressAndEXB = (dwUpperBaseAddress & 0xFF) | dwUpperFlags;
}
