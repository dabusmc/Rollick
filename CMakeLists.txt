cmake_minimum_required(VERSION 3.20)

project(Rollick LANGUAGES C CXX ASM)

# Language standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Kernel executable
add_executable(rollick_kernel
    src/arch/i386/boot/boot.s
    src/kernel/kernel.c
)

set_target_properties(rollick_kernel PROPERTIES
    OUTPUT_NAME "rollick"
    SUFFIX ".elf"
    LINKER_LANGUAGE CXX
)

# Compiler options: C
target_compile_options(rollick_kernel PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-ffreestanding>
    $<$<COMPILE_LANGUAGE:C>:-O2>
    $<$<COMPILE_LANGUAGE:C>:-Wall>
    $<$<COMPILE_LANGUAGE:C>:-Wextra>
    $<$<COMPILE_LANGUAGE:C>:-fno-stack-protector>
    $<$<COMPILE_LANGUAGE:C>:-fno-pic>
)

# Compiler options: C++
target_compile_options(rollick_kernel PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-ffreestanding>
    $<$<COMPILE_LANGUAGE:CXX>:-O2>
    $<$<COMPILE_LANGUAGE:CXX>:-Wall>
    $<$<COMPILE_LANGUAGE:CXX>:-Wextra>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-stack-protector>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-pic>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
)

# Linker options
target_link_options(rollick_kernel PRIVATE
    -ffreestanding
    -nostdlib
    -Wl,--build-id=none
    "-Wl,-T,${CMAKE_CURRENT_SOURCE_DIR}/linker.ld"
)

# GCC runtime support
target_link_libraries(rollick_kernel PRIVATE gcc)

# QEMU
add_custom_target(run
    COMMAND qemu-system-i386 -kernel "$<TARGET_FILE:rollick_kernel>"
    DEPENDS rollick_kernel
    USES_TERMINAL
)