#pragma once

#include <stdint.h>

namespace Kernel
{
	namespace Boot
	{
		struct MultibootInfo
		{
			uint32_t Flags;
			uint32_t MemoryLower;
			uint32_t MemoryUpper;
			uint32_t BootDevice;
			uint32_t CommandLine;
			uint32_t ModulesCount;
			uint32_t ModulesAddress;
			uint32_t Symbols[4];
			uint32_t MemoryMapLength;
			uint32_t MemoryMapAddress;
		} __attribute__((packed));

		struct MultibootMemoryMapEntry
		{
			uint32_t Size;
			uint64_t BaseAddress;
			uint64_t Length;
			uint32_t Type;
		} __attribute__((packed));
	}
}