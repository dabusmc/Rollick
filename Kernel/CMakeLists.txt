set(KERNEL_SOURCES
            Input/Keyboard.cpp
            Input/KeyEventQueue.cpp

            Library/kStandardLib.cpp

            Prekernel/init.cpp

            Security/StackSmashingProtector.cpp

            Panic.cpp
            Kernel.cpp
            init.cpp)

set(i386_SOURCES
        Arch/i386/Interrupts/IDT.cpp
        Arch/i386/Interrupts/Interrupts.cpp
        Arch/i386/Interrupts/ISR.s
        Arch/i386/Interrupts/PIC.cpp
        Arch/i386/Interrupts/PIT.cpp

        Arch/i386/Input/PS2Keyboard.cpp
        Arch/i386/Input/Input.cpp

        Arch/i386/Serial/Serial.cpp

        Arch/i386/Terminal.cpp

        Prekernel/Arch/i386/Boot/boot.s)

add_executable(rollick_kernel ${i386_SOURCES} ${KERNEL_SOURCES})

target_include_directories(rollick_kernel PRIVATE .)

set_target_properties(rollick_kernel PROPERTIES
    OUTPUT_NAME "rollick"
    SUFFIX ".elf"
)

target_compile_definitions(rollick_kernel PRIVATE
        ROLLICK_OS
        ROLLICK_KERNEL
)

# Compiler options
target_compile_options(rollick_kernel PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-ffreestanding>
    $<$<COMPILE_LANGUAGE:C>:-O2>
    $<$<COMPILE_LANGUAGE:C>:-Wall>
    $<$<COMPILE_LANGUAGE:C>:-Wextra>
    $<$<COMPILE_LANGUAGE:C>:-fstack-protector-strong>
    $<$<COMPILE_LANGUAGE:C>:-mstack-protector-guard=global>
    $<$<COMPILE_LANGUAGE:C>:-fno-pic>

    $<$<COMPILE_LANGUAGE:CXX>:-ffreestanding>
    $<$<COMPILE_LANGUAGE:CXX>:-O2>
    $<$<COMPILE_LANGUAGE:CXX>:-Wall>
    $<$<COMPILE_LANGUAGE:CXX>:-Wextra>
    $<$<COMPILE_LANGUAGE:CXX>:-fstack-protector-strong>
    $<$<COMPILE_LANGUAGE:CXX>:-mstack-protector-guard=global>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-pic>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
)

# Linker options
target_link_options(rollick_kernel PRIVATE
    -nostdlib
    -Wl,--build-id=none
    "-Wl,-T,${CMAKE_SOURCE_DIR}/linker.ld"
)

# Linker script dependency
set_property(TARGET rollick_kernel APPEND PROPERTY
    LINK_DEPENDS "${CMAKE_SOURCE_DIR}/linker.ld"
)

# GCC runtime support
target_link_libraries(rollick_kernel PRIVATE gcc)

# QEMU
add_custom_target(run
        COMMAND qemu-system-i386
        -kernel "$<TARGET_FILE:rollick_kernel>"
        -serial stdio
        -monitor none
        -d int,cpu_reset
        -D qemu.log
        DEPENDS rollick_kernel
        USES_TERMINAL
)