set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR i686)

# Cross-compiler location
set(TOOLCHAIN_BIN "C:/Dev/Rollick/toolchain/tools/bin")

# Compilers
set(CMAKE_C_COMPILER "${TOOLCHAIN_BIN}/i686-elf-gcc.exe")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_BIN}/i686-elf-g++.exe")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_BIN}/i686-elf-gcc.exe")

# Binary utilities
set(CMAKE_AR "${TOOLCHAIN_BIN}/i686-elf-ar.exe")
set(CMAKE_RANLIB "${TOOLCHAIN_BIN}/i686-elf-ranlib.exe")
set(CMAKE_OBJCOPY "${TOOLCHAIN_BIN}/i686-elf-objcopy.exe")

# Don't attempt to link a hosted executable during compiler checks
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)