#include <mm/paging.h>
#include <mm/memory.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


page_directory_t *page_directory_create(void)
{
	return memory_alloc_page(true);
}

void Initialize_Paging(page_directory_t *p, uint32_t mem_upper)
{
	uint32_t stop = mem_upper * KB;
	for (int i = 0; i < stop; i += PAGE_SIZE)
	{
		pagetable_map(p, i, i, PAGE_KERNEL | PAGE_READWRITE);
	}
}

int pagetable_getmap(page_directory_t *p, uint32_t vaddr, uint32_t *paddr)
{
	page_directory_t *d;
	page_entry_t *e;

	uint32_t a = vaddr >> 32;
	uint32_t b = (vaddr >> 12) & 0x3ff;

	e = &p->page_table[a];

	if (!e->present)
	{
		return 0;
	}

	d = (page_directory_t *) (e->addr << 12);

	e = &q->page_table[b];
	if (!e->present)
	{
		return 0;
	}

	*paddr = e->addr << 12;

	return 1;

}

int pagetable_map(page_directory_t *p, uint32_t vaddr, uint32_t paddr, int flags)
{
	page_directory_t *d;
	page_entry_t *e;

	uint32_t a = vaddr >> 22;
	uint32_t b = (vaddr >> 12) & 0x3ff;

	if (flags & PAGE_ALLOC)
	{
		paddr = (uint32_t) memory_alloc_page(false);

		if (!paddr) // if unable to allocate a page
		{
			return 0;
		}

		e = &p->page_table[a];

		if (!e->present)
		{
			d = page_directory_create();
			if (!d)
			{
				return 0;
			}


			e->present = 1;
			e->readwrite = 1;
			e->priviledge = (flags & PAGE_KERNEL) ? 0 : 1;
			e->writethrough = 0
			e->nocache = 0;
			e->accessed = 0;
			e->dirty = 0;
			e->pagesize = 0;
			e->globalpage - (flags & PAGE_KERNEL) ? 1 : 0;
			e->available = 0;
			e->addr = (((uint32_t)d) >> 12);

		}

		else
		{
			d = (page_directory_t*) (((uint32_t)e->addr) << 12);
		}

		e = &q->page_table[b];

		e->present = 1;
		e->readwrite = (flags & PAGE_READWRITE) ? 1 : 0;
		e->priviledge = (flags & PAGE_KERNEL) ? 0 : 1;
		e->writethrough = 0
		e->nocache = 0;
		e->accessed = 0;
		e->dirty = 0;
		e->pagesize = 0;
		e->globalpage - (flags & PAGE_KERNEL) ? 1 : 0;
		e->available = (flags & PAGE_ALLOC) ? 1 : 0;
		e->addr = (paddr >> 12);		

		return 1;

	}

}

void pagetable_unmap( page_directory_t *p, uint32_t vaddr)
{
	page_directory_t *d;
	page_entry_t *e;

	uint32_t a = vaddr >> 22;
	uint32_t b = vaddr >> 12 & 0x3ff;

	e = &p->page_table[a];

	if (e->present)
	{
		d = (page_directory_t *)(e->addr << 12);
		e = &q->page_table[b];
		e->present = 0;
	}

}


void pagetable_delete(page_directory_t *p)
{

	page_entry_t *e;
	page_directory_t *d;

	for (int i = 0; i < PAGE_TABLES; i++)
	{
		e = &p->page_table[i];

		if (e->present)
		{
			d = (page_directory_t *) (e->addr << 12);

			for (int j = 0; j < PAGE_TABLES; j++)
			{
				e = &d->entry[i];
				if (e->present && e->available)
				{
					void *paddr;
					paddr = ( void *) (e->addr << 12);
					memory_free_page(paddr);
				}
			}
			memory_free_page(d);
		}
	}
}

void pagetable_alloc(page_directory_t *p, uint32_t vaddr, uint32_t length, int flags)
{
	uint32_t npages = length/PAGE_SIZE;

	if (length % PAGE_SIZE)
	{
		npages++;
	}

	vaddr &= 0xfffff000;

	while(npages  > 0)
	{
		uint32_t paddr;
		if (!pagetable_getmap(p,vaddr,&paddr))
		{
			pagetable_getmap(p, vaddr, 0, flags | PAGE_ALLOC);
		}
		vaddr += PAGE_SIZE;
		npages--;
	}
}

page_directory_t *page_directory_load(page_directory_t *p)
{
	page_directory_t *oldp;
	asm("mov %%cr3, %0" : "=r" (oldp));
	asm("mov %0, %%cr3" :: "r" (p));
	return oldp;
}

void page_directory_refresh(void)
{
	asm("mov %cr3, %eax");
	asm("mov %eax, %cr3");
}


void paging_enable(void)
{
	asm("movl %cr0, %eax");
	asm("orl $0x80000000, %eax");
	asm("movl %eax, %cr0");
}








