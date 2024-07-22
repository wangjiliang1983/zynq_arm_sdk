CPU link:
ps7_ddr_0 : ORIGIN = 0x100000, LENGTH = 0x0FF00000

generate bootimage:
add elf of cpu1 to the last

CPU1 link:
ps7_ddr_0 : ORIGIN = 0x10000000, LENGTH = 0x0FF00000
CPU1_BSP settings:
extra_compiler_flags
-mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -nostartfiles -g -Wall -Wextra -DUSE_AMP=1