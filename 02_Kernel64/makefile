# file      makefile
# date      2024/11/28 (Updated)
# author    kkamagui (Modified for 64bit)
# brief     IA-32e(64비트) 모드 커널을 빌드하기 위한 make 파일

################################################################################
# 빌드 환경 및 규칙 설정
################################################################################
# 컴파일러 및 링커 정의
NASM64 = nasm
GCC64 = clang -m64 -ffreestanding -mno-red-zone -target x86_64-pc-none-elf
LD64 = ld.lld -m elf_x86_64 -T ../elf_x86_64.x -nostdlib -e Main -Ttext 0x200000

# 바이너리 이미지 생성을 위한 OBJCOPY 옵션 정의
OBJCOPY64 = llvm-objcopy -j .text -j .data -j .rodata -j .bss -S -O binary

# 디렉터리 정의
OBJECTDIRECTORY = Temp
SOURCEDIRECTORY = Source

################################################################################
# 빌드 항목 및 빌드 방법 설정
################################################################################
# 기본적으로 빌드를 수행할 목록
all: prepare Kernel64.bin

# 오브젝트 파일이 위치할 디렉터리를 생성
prepare:
	mkdir -p $(OBJECTDIRECTORY)

# [수정] EntryPoint를 bin이 아닌 elf64 오브젝트(.o)로 컴파일해야 함
# 이 부분은 아래 내부 빌드 규칙(Internal dependency)에서 처리하므로 여기선 제거하거나
# 명시적으로 .o를 만드는 규칙을 추가할 수 있습니다.
# 하지만 아래 패턴 규칙(%.o)으로 처리되므로 별도 타겟 불필요.

# 커널의 C 소스 파일에 대한 의존성 정보 생성
dep:
	@echo === Make Dependancy File ===
	$(MAKE) -C $(OBJECTDIRECTORY) -f ../makefile InternalDependency
	@echo === Dependancy Search Complete ===

# 디렉터리를 오브젝트 파일 디렉터리로 이동해서 의존성 파일 및 실행 파일을 생성
ExecuteInternalBuild: dep
	$(MAKE) -C $(OBJECTDIRECTORY) -f ../makefile Kernel64.elf

# 커널 이미지를 바이너리 파일로 변환
$(OBJECTDIRECTORY)/Kernel64.elf.bin: ExecuteInternalBuild
	$(OBJCOPY64) $(OBJECTDIRECTORY)/Kernel64.elf $@

# [수정] EntryPoint.bin을 따로 붙이는 과정 제거 (이미 링킹되어 포함됨)
Kernel64.bin: $(OBJECTDIRECTORY)/Kernel64.elf.bin
	cp $< $@

# 소스 파일을 제외한 나머지 파일 정리
clean:
	rm -f *.bin
	rm -f $(OBJECTDIRECTORY)/*.*

################################################################################
# Make에 의해 다시 호출되는 부분, Temp 디렉터리를 기준으로 수행됨
################################################################################
# 빌드할 C 소스 파일 정의, Temp 디렉터리를 기준으로 설정

# [수정] EntryPoint.o를 가장 먼저 링크되도록 설정 (0x200000 위치 점유)
CENTRYPOINTOBJECTFILE = EntryPoint.o

# C 소스 파일 목록
CSOURCEFILES = $(wildcard ../$(SOURCEDIRECTORY)/*.c)
ASSEMBLYSOURCEFILES = $(wildcard ../$(SOURCEDIRECTORY)/*.asm)

# [수정] Main.o를 제외하지 않고 포함시킴 (EntryPoint -> Main 순서로 링크됨)
COBJECTFILES = $(notdir $(patsubst %.c,%.o,$(CSOURCEFILES)))
ASSEMBLYOBJECTFILES = $(notdir $(patsubst %.asm,%.o,$(ASSEMBLYSOURCEFILES)))

# .c 파일을 .o 파일로 바꾸는 규칙 정의
%.o: ../$(SOURCEDIRECTORY)/%.c
	$(GCC64) -c $<

# .asm 파일을 .o 파일로 바꾸는 규칙 정의 (64비트 ELF 포맷)
%.o: ../$(SOURCEDIRECTORY)/%.asm
	$(NASM64) -f elf64 -o $@ $<

# [추가] EntryPoint.s를 .o로 만드는 규칙 (확장자가 .s이므로 별도 처리)
EntryPoint.o: ../$(SOURCEDIRECTORY)/EntryPoint.s
	$(NASM64) -f elf64 -o $@ $<

# 실제 의존성에 관련된 파일을 생성
InternalDependency:
	$(GCC64) -MM $(CSOURCEFILES) > Dependency.dep

# 실제 커널 이미지를 빌드
# [수정] CENTRYPOINTOBJECTFILE(EntryPoint.o)가 가장 앞에 와야 함
Kernel64.elf: $(CENTRYPOINTOBJECTFILE) $(COBJECTFILES) $(ASSEMBLYOBJECTFILES)
	$(LD64) -o $@ $^

# 현재 디릭토리의 파일 중, dependency 파일이 있으면 make에 포함
ifeq (Dependency.dep, $(wildcard Dependency.dep))
include Dependency.dep
endif