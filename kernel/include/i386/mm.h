#ifndef _I386_MM_H
#define _I386_MM_H

#include <i386/multiboot.h>

typedef struct mm_free_block {
	size_t size;
	struct mm_free_block* next;
} mm_free_block;

typedef struct {
	unsigned long size;
	unsigned long base_addr_low;
	unsigned long base_addr_high;
	unsigned long length_low;
	unsigned long length_high;
	unsigned long type;
} memory_map_t;

extern void* mm_kernel_end_palign;
extern void* mm_heap_end;
extern uint32_t kernel_end;
extern unsigned int mm_heap_cap;

void mm_clear(void);
void mm_create_mmap(multiboot_info_t* multiboot);
void mm_dump_phys_mmap(void);

void* sbrk(size_t size, bool page_align);
void* malloc(size_t size);
void* calloc(size_t number, size_t size);
void free(void* ptr);

#endif