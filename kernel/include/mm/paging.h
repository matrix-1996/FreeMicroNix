#ifndef _X86_MM_PAGING_H
#define _X86_MM_PAGING_H

#include <stdint.h>
#include <mm/memory.h>

#define PAGE_USER			1
#define PAGE_KERNEL			0

#define PAGE_PRESENT		1
#define PAGE_ABSENT			0

#define PAGE_READWRITE		1
#define PAGE_READONLY		0

#define PAGE_WRITETHROUGH	1
#define PAGE_NOWRITETHROUGH 0

#define PAGE_NOCACHE		1
#define PAGE_CACHE			0

#define PAGE_ACCESSED		1
#define PAGE_NOTACCESSED	0

#define PAGE_DIRTY 			1
#define PAGE_CLEAN			0

#define PAGE_FLUSH			1
#define PAGE_NOFLUSH		0

#define PAGE_TABLES 1024

typedef struct page_entry
{
	unsigned present   	  : 1;	// 1 = present, 0 = absent
	unsigned readwrite 	  : 1; 	// 1 = readwrite , 0 = readonly 
	unsigned privilege 	  : 1;	// 1 = user, 0 = kernel
	unsigned writethrough : 1;	// 1 = writethrough, 0 = nowritethrough

	unsigned nocache 	  : 1;	// 1 = nocache, 0 = cache
	unsigned accessed	  : 1;	// 1 = accessed, 0 = notaccessed
	unsigned dirty 		  : 1;	// 1 = dirty, 0 = clean
	unsigned pagesize 	  : 1;	// leave to 0

	unsigned globalpage	  : 1;	// 1 = flush, 0 = noflush
	unsigned available	  : 3;

	unsigned addr 		  : 20;

} page_entry_t;

typedef page_entry_t page_table_t;

typdef struct page_directory
{
	page_entry_t tables[PAGE_TABLES];
	uint32_t tables_physical[PAGE_TABLES];
	uint32_t phys_addr;

} page_directory_t;

extern void page_directory_loader(uint32_t *page_dir);
extern void paging_enable(void);

void Initialize_Paging(void);
void paging_map_address(void *physical_address, void *virtual_address, uint16_t flags);
void paging_unmap_address(void *physical_address);


#endif