#ifndef _X86_MM_PAGING_H
#define _X86_MM_PAGING_H

#include <stdint.h>
#include <mm/memory.h>

#define PAGE_USER			1
#define PAGE_KERNEL			0

#define PAGE_PRESENT		1
#define PAGE_ABSENT			0

#define PAGE_READWRITE		4
#define PAGE_READONLY		0

#define PAGE_WRITETHROUGH	1
#define PAGE_NOWRITETHROUGH 0

#define PAGE_NOCACHE		1
#define PAGE_CACHE			0

#define PAGE_ALLOC			2

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

typedef page_directory
{
	page_entry_t page_table[PAGE_TABLES];

} page_directory_t;



void Initialize_Paging(uint32_t mem_upper);
int pagetable_getmap(page_directory_t *p, uint32_t vaddr, uint32_t *paddr);
int pagetable_map(page_directory_t *p, uint32_t vaddr, uint32_t paddr, int flags);
void pagetable_unmap( page_directory_t *p, uint32_t vaddr);
void pagetable_delete(page_directory_t *p);
void pagetable_alloc(page_directory_t *p, uint32_t vaddr, uint32_t lenght, int flags);
page_directory_t *page_directory_load(page_directory_t *p);
void page_directory_refresh(void);





#endif