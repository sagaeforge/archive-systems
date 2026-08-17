# archive-systems

**OS 커널 · 저수준 시스템** 작업 아카이브입니다.

## 구성

| 디렉터리 | 내용 | 커밋 | 기간 |
|---|---|---:|---|
| [`mint64/`](mint64) | 64비트 OS 커널 자작 — 부트로더 / 32비트·64비트 커널 (ASM + C) | 8 | 2025.11 |
| [`ndk/`](ndk) | Nugdev Developer Kit — C++ 공용 모듈 (CMake + vcpkg) | 4 | 2025.04–2026.08 |
| [`ndk-legacy/nugdev-developer-kit/`](ndk-legacy/nugdev-developer-kit) | NDK 초기 설계 — Memory / Structure / Algorithm / String | 1 | 2023.09 |
| [`ndk-legacy/nugunga-develop-kit/`](ndk-legacy/nugunga-develop-kit) | C++ 리플렉션 실험 — Property / Reflection | 1 | 2023.10 |
| [`wine-python/`](wine-python) | Wine 위에서 Python ↔ DLL 브리지 (Docker) | 2 | 2024.09–2026.08 |

## 참고

- `ndk/` 는 vcpkg 를 서브모듈로 씁니다 — `git submodule update --init --recursive`
- `wine-python/docker/install/` 의 wine-mono / wine-gecko 설치본(184MB)은 제외했습니다.
  같은 디렉터리의 `README.md` 에 받는 곳을 적어뒀습니다.

## 히스토리 보존 방식

각 프로젝트는 원본 저장소의 커밋 히스토리를 그대로 유지한 채 `git subtree` 방식으로 편입했습니다.
원본의 모든 브랜치 끝점은 `legacy/<프로젝트>/<브랜치>` 태그로 남아 있습니다.

```bash
git tag -l 'legacy/*'                      # 보존된 원본 브랜치 목록
git log  legacy/<프로젝트>/<브랜치>          # 그 프로젝트의 원본 커밋 히스토리
git show legacy/<프로젝트>/<브랜치>          # 편입 직전 최종 상태
```

> ⚠️ `git log -- <디렉터리>/` 로는 편입 커밋 1개만 보입니다.
> 서브트리 편입에 `-s ours` 병합을 써서 main 쪽 경로 히스토리에는 병합 커밋만 남기 때문입니다.
> **원본 커밋은 전부 이 저장소 안에 그대로 있고, 위의 `legacy/*` 태그로 접근합니다.**

보존된 태그 11개:

```
  legacy/mint64/feature/04/2025-11-24
  legacy/mint64/feature/05/2025-11-25
  legacy/mint64/feature/06
  legacy/mint64/feature/07
  legacy/mint64/feature/08
  legacy/mint64/feature/09
  legacy/mint64/feature/10
  legacy/mint64/feature/11
  legacy/mint64/main
  legacy/ndk/develop
  legacy/ndk/wip/archive-import
```

---

*이 저장소는 아카이브입니다. 유지보수하지 않습니다.*
