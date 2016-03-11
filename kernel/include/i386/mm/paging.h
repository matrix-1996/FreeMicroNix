#ifndef _I386_MM_PAGING_H
#define _I386_MM_PAGING_H

#include <stdint.h>

#define PAGE_PRESENT	0x01
#define PAGE_READWRITE  0x02
#define PAGE_USER       0x04
#define PAGE_ACCESSED   0x08
#define PAGE_DIRTY      0x10

typedef uint32_t page_entry_t;
typedef page_entry_t page_table_t;

typedef struct page_directory
{
	page_table_t tables[1024];
	uint32_t tables_physical[1024];
	uint32_t physical_address;

} __attribute__((packed)) page_directory_t;

extern void enable_paging();
extern void load_page_directory(uint32_t * page_dir);

void Initialize_Paging();


#endif