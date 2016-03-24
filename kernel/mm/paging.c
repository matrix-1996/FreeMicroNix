#include <mm/paging.h>
#include <stdint.h>


page_table_t page_dir[PAGE_TABLES] __attribute__((aligned(4096)));
page_entry_t page_table_first[PAGE_TABLES] __attribute__((aligned(4096)));


void Initialize_Paging(void)
{
	
}