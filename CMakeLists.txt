cmake_minimum_required(VERSION 3.20)

project(Rollick
    VERSION 0.1.0
    LANGUAGES C CXX ASM
)

# Language standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Components
add_subdirectory(Kernel)

# GRUB / ISO configuration
set(ROLLICK_GRUB_DIR "C:/Dev/Rollick/grub" CACHE PATH "GRUB installation directory")
set(ROLLICK_MSYS2_DIR "C:/msys64" CACHE PATH "MSYS2 installation directory")

set(GRUB_MKIMAGE "${ROLLICK_GRUB_DIR}/grub-mkimage.exe")
set(GRUB_MODULES "${ROLLICK_GRUB_DIR}/i386-pc")
set(XORRISO "${ROLLICK_MSYS2_DIR}/usr/bin/xorriso.exe")
set(MSYS2_CYGPATH "${ROLLICK_MSYS2_DIR}/usr/bin/cygpath.exe")

set(ISO_ROOT "${CMAKE_BINARY_DIR}/iso")
set(ISO_OUTPUT "${CMAKE_BINARY_DIR}/rollick.iso")

# xorriso requires MSYS2-style paths rather than Windows paths.
execute_process(
        COMMAND "${MSYS2_CYGPATH}" -u "${ISO_ROOT}"
        OUTPUT_VARIABLE ISO_ROOT_MSYS
        OUTPUT_STRIP_TRAILING_WHITESPACE
        COMMAND_ERROR_IS_FATAL ANY
)

execute_process(
        COMMAND "${MSYS2_CYGPATH}" -u "${ISO_OUTPUT}"
        OUTPUT_VARIABLE ISO_OUTPUT_MSYS
        OUTPUT_STRIP_TRAILING_WHITESPACE
        COMMAND_ERROR_IS_FATAL ANY
)

# Build bootable ISO
add_custom_target(rollick_iso
        COMMAND ${CMAKE_COMMAND} -E rm -rf "${ISO_ROOT}"

        COMMAND ${CMAKE_COMMAND} -E make_directory
        "${ISO_ROOT}/boot/grub/i386-pc"

        COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:rollick_kernel>"
        "${ISO_ROOT}/boot/rollick.elf"

        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/Boot/grub/grub.cfg"
        "${ISO_ROOT}/boot/grub/grub.cfg"

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${GRUB_MODULES}"
        "${ISO_ROOT}/boot/grub/i386-pc"

        COMMAND "${GRUB_MKIMAGE}"
        -O i386-pc-eltorito
        -d "${GRUB_MODULES}"
        -p /boot/grub
        -o "${ISO_ROOT}/boot/grub/i386-pc/eltorito.img"
        biosdisk iso9660 normal configfile multiboot

        COMMAND "${XORRISO}"
        -as mkisofs
        -R -J
        -V ROLLICK
        -c boot/grub/boot.cat
        -b boot/grub/i386-pc/eltorito.img
        -no-emul-boot
        -boot-load-size 4
        -boot-info-table
        -o "${ISO_OUTPUT_MSYS}"
        "${ISO_ROOT_MSYS}"

        DEPENDS rollick_kernel
        USES_TERMINAL
        VERBATIM
)

# QEMU (GRUB ISO)
add_custom_target(run_iso
        COMMAND qemu-system-i386
        -cdrom "${ISO_OUTPUT}"
        -boot d
        -serial stdio
        -monitor none
        -d int,cpu_reset
        -D "${CMAKE_BINARY_DIR}/qemu-iso.log"
        DEPENDS rollick_iso
        USES_TERMINAL
        VERBATIM
)
