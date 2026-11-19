#include "Arch/i386/Interrupts/IDT.h"
#include "Arch/i386/Interrupts/PIC.h"
#include "Arch/i386/Interrupts/PIT.h"
#include "Arch/i386/Memory/Multiboot.h"

#include "Input/Input.h"

#include "Library/kStandardLib.h"

#include "Memory/Heap.h"
#include "Memory/Memory.h"
#include "Memory/PhysicalMemoryManager.h"

#include "Kernel.h"

#include <stddef.h>
#include <stdint.h>

#define PIT_FREQUENCY 100

extern "C"
{

extern uint8_t __kernel_physical_start;
extern uint8_t __kernel_physical_end;

}

void initInterrupts()
{
	Kernel::Interrupts::InitializeIDT();
	kprintf("Initialized IDT\n");
	Kernel::Interrupts::InitializePIC();
	kprintf("Initialized PIC\n");

	Kernel::Interrupts::InitializePIT(PIT_FREQUENCY);
	kprintf("Initialized PIT at a frequency of %d\n", PIT_FREQUENCY);

	Kernel::Interrupts::SetIRQMask(0, false);
	if (Kernel::g_GlobalObjects.Input_Keyboard->Ready())
	{
		Kernel::Interrupts::SetIRQMask(1, false);
	}

	asm volatile("sti" ::: "memory");
	kprintf("Enabled interrupts\n");
}

void initMemoryMap(uint32_t multibootMagic, uint32_t multibootInfoAddress)
{
	if (multibootMagic != 0x2BADB002)
	{
		kprintf("Invalid Multiboot magic number!\n");
		while (true)
		{
			asm volatile("cli; hlt");
		}
	}

	auto* multibootInfo = reinterpret_cast<Kernel::Boot::MultibootInfo*>(multibootInfoAddress);
	if (multibootInfo->Flags & (1 << 6))
	{
		kprintf("Memory map address: 0x%X\n", multibootInfo->MemoryMapAddress);
		kprintf("Memory map length: %u bytes\n", multibootInfo->MemoryMapLength);
	}
	else
	{
		kprintf("WARNING: GRUB did not provide a memory map\n");
	}

	constexpr size_t MaxMemoryRegions = 32;
	Kernel::Memory::MemoryRegion memoryRegions[MaxMemoryRegions];
	size_t memoryRegionCount = 0;

	auto* mapAddress = reinterpret_cast<uint8_t*>(multibootInfo->MemoryMapAddress);
	uint8_t* mapEnd = mapAddress + multibootInfo->MemoryMapLength;
	while (mapAddress < mapEnd)
	{
		auto* entry = reinterpret_cast<Kernel::Boot::MultibootMemoryMapEntry*>(mapAddress);

		uint32_t remaining = static_cast<uint32_t>(mapEnd - mapAddress);
		if (remaining < 4 || entry->Size < 20 || entry->Size > remaining - 4)
		{
			kprintf("Invalid memory map entry!\n");
			break;
		}

		auto& region = memoryRegions[memoryRegionCount];
		region.BaseAddress = entry->BaseAddress;
		region.Size = entry->Length;

		switch (entry->Type)
		{
		case 1:
			region.Type = Kernel::Memory::MemoryRegionType::Available;
			++memoryRegionCount;
			break;
		case 2:
			region.Type = Kernel::Memory::MemoryRegionType::Reserved;
			++memoryRegionCount;
			break;
		default:
			kprintf("Unknown memory region type %u\n", entry->Type);
			break;
		}

		mapAddress += entry->Size + 4;
	}

	for (size_t i = 0; i < memoryRegionCount; i++)
	{
		auto& region = memoryRegions[i];
		kprintf("Region at 0x%X: %u bytes\n", region.BaseAddress, region.Size);
	}

	uintptr_t kernelPhysicalStart = reinterpret_cast<uintptr_t>(&__kernel_physical_start);
	uintptr_t kernelPhysicalEnd = reinterpret_cast<uintptr_t>(&__kernel_physical_end);

	Kernel::g_GlobalObjects.Memory_PMM.Initialize(memoryRegions, memoryRegionCount);

	// Reserve the first MiB of physical memory.
	Kernel::g_GlobalObjects.Memory_PMM.ReserveRange(0, 0x100000);

	// Reserve the kernel's physical memory.
	Kernel::g_GlobalObjects.Memory_PMM.ReserveRange(kernelPhysicalStart, kernelPhysicalEnd - kernelPhysicalStart);

	// Reserve the Multiboot information structure.
	Kernel::g_GlobalObjects.Memory_PMM.ReserveRange(multibootInfoAddress, sizeof(Kernel::Boot::MultibootInfo));

	// Reserve the Multiboot memory map.
	if (multibootInfo->Flags & (1 << 6))
	{
		Kernel::g_GlobalObjects.Memory_PMM.ReserveRange(multibootInfo->MemoryMapAddress, multibootInfo->MemoryMapLength);
	}
}

extern "C" void kernelMain(uint32_t multibootMagic, uint32_t multibootInfoAddress)
{
	initMemoryMap(multibootMagic, multibootInfoAddress);

	if (!Kernel::g_GlobalObjects.Memory_Heap.Initialize())
	{
		kprintf("FATAL: Failed to initialise kernel heap!\n");

		while (true)
		{
			asm volatile("cli; hlt");
		}
	}

	kprintf("Initialised kernel heap\n");

	Kernel::Input::Initialize();
	if (Kernel::g_GlobalObjects.Input_Keyboard->Ready())
	{
		kprintf("Initialized keyboard\n");
	}
	else
	{
		kprintf("WARNING: Keyboard initialization failed\n");
	}

	initInterrupts();

	void* a = kmalloc(16);
	void* b = kmalloc(128);
	void* c = kmalloc(4096);

	kprintf("Allocation A: 0x%x\n", static_cast<uint32_t>(reinterpret_cast<uintptr_t>(a)));
	kprintf("Allocation B: 0x%x\n", static_cast<uint32_t>(reinterpret_cast<uintptr_t>(b)));
	kprintf("Allocation C: 0x%x\n", static_cast<uint32_t>(reinterpret_cast<uintptr_t>(c)));

	if (a && b && c)
	{
		// Check that allocated memory is actually writable.
		*static_cast<uint32_t*>(a) = 12345;
		*static_cast<uint32_t*>(b) = 67890;

		kprintf("A contains: %d\n", *static_cast<uint32_t*>(a));
		kprintf("B contains: %d\n", *static_cast<uint32_t*>(b));
	}

	kfree(b);
	kfree(a);
	kfree(c);

	void* d = kmalloc(128);
	kprintf("Reused allocation: 0x%x\n", static_cast<uint32_t>( reinterpret_cast<uintptr_t>(d)));
	kfree(d);

	while (true)
	{
		Kernel::KeyEvent event = { };

		while (Kernel::g_GlobalObjects.Input_KeyboardEvents.Pop(event))
		{
			Kernel::g_GlobalObjects.Input_Keyboard->ProcessKeyEvent(event);
		}

		// Avoid missing an interrupt between checking the queue and halting.
		asm volatile("cli" ::: "memory");

		if (Kernel::g_GlobalObjects.Input_KeyboardEvents.Empty())
		{
			// STI delays interrupt recognition until after the following HLT.
			asm volatile("sti\n\thlt" ::: "memory");
		}
		else
		{
			asm volatile("sti" ::: "memory");
		}
	}
}
