#include <kernel/panic.h>
#include <mm/memory.h>
#include <x86/video/vga.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t pages_free = 0;
static uint32_t pages_total = 0;

static uint32_t *freemap = 0;
static uint32_t freemap_bits = 0;
static uint32_t freemap_bytes = 0;
static uint32_t freemap_cells = 0;
static uint32_t freemap_pages = 0;

static void *allocatable_memory_start = (void *) kernel_physical_end;

#define CELL_BITS (8*sizeof(*freemap))


void Initialize_Memory(uint32_t mem_upper, uint32_t kernel_physical_end)
{
	pages_total = mem_upper / (PAGE_SIZE / KB)
	pages_free = pages_total;

	freemap = allocatable_memory_start;
	freemap_bits = pages_total;
	freemap_bytes = 1 + (freemap_bits / 8);
	freemap_cells = 1 + (freemap_bits/CELL_BITS);
	freemap_pages = 1 + (freemap_bytes/PAGE_SIZE);

	kprintf("Memory: %d bits %d bytes %d cells %d pages\n", freemap_bits, freemap_bytes, freemap_cells, freemap_pages);

	memset(freemap, 0xff, freemap_bytes);

	for (int i = 0; i < freemap_pages; i++)
	{
		memory_alloc_page(false);
	}

	freemap[0] = 0x0; // Fix the VMware bug that doesn't allow ues of the page near 1MB

}


uint32_t memory_pages_free(void)
{
	return pages_free;
}

uint32_t memory_pages_total(void)
{
	return pages_total;
}

void *memory_alloc_page(bool zeroit)
{
	uint32_t cell_mask;
	uint32_t page_number;
	void *page_addr;

	if (!freemap)
	{
		kprintf("Memory not yet initialized!\n");
		return 0;
	}

	for (uint32_t i = 0; i < freemap_cells; i++)
	{
		if ( freemap[i] != 0 )
		{
			for (uint32_t j = 0; j < CELL_BITS; j++)
			{
				cellmask = (1 << j);

				if ( freemap[i] & cell_mask )
				{
					freemap[i] &= ~cell_mask;
					page_number = i * CELL_BITS + j;
					page_addr = (page_number << PAGE_BITS) + allocatable_memory_start;
					if (zeroit)
					{
						memset(page_addr, 0, PAGE_SIZE);
					}

					pages_free--;
					return page_addr;
				}
			}
		}

	}

	// If this point in execution is reached, an Out of Memory condition has been reached

	Terminal_Set_Color(COLOR_LIGHT_RED);
	kprintf("Out of memory!!!\n");
	panic();

	return 0;
}



void memory_free_page(void *page_addr)
{
	uint32_t page_number = (page_addr - allocatable_memory_start) >> PAGE_BITS;
	uint32_t cell_number = page_number/CELL_BITS;
	uint32_t cell_offset = page_number%CELL_BITS;
	uint32_t cell_mask = ( 1 << cell_offset);

	freemap[cell_number] |= cellmask;
	pages_free++;
}
