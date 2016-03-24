#ifndef _X86_MM_MEMORY_H
#define _X86_MM_MEMORY_H

#include <stdint.h>
#include <stdbool.h>

#define KB 1024
#define MB (KB*KB)
#define GB (KB*KB*KB)

#define PAGE_SIZE 4096
#define PAGE_BITS 12

void Initialize_Memory(uint32_t mem_upper, uint32_t kernel_physical_end);
uint32_t memory_pages_free(void);
uint32_t memory_pages_total(void);
void *memory_alloc_page(bool zeroit);
void memory_free_page(void *page_addr);

#endif