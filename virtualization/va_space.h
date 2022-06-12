/* Jan Ras
 * CSCI 4100
 * Assignment 2
 * Header file for virtual address space simulation
 */

#include <stdint.h>

#ifndef __VA_SPACE_H__
#define __VA_SPACE_H__

#define PT_SIZE 256

/* Error codes */
extern const int ERR_PAGE_NOT_ALLOCATED;
extern const int ERR_PAGE_NOT_PRESENT;
extern const int ERR_PERMISSION_DENIED;

/* Data types */
typedef uint16_t pt_entry, pt_address;
typedef uint8_t pt_bits, pt_index, pt_offset;
typedef pt_entry page_table[PT_SIZE];

/* Permissions for page table entries */
extern const pt_bits PT_ALLOC, PT_DIRTY, PT_ACCESSED, PT_PRESENT,
  PT_KERNEL, PT_READ, PT_WRITE, PT_EXECUTE;

/* Initialize an empty page table */
void pt_init(page_table table);

/* Add a page table entry */
void pt_map(page_table table, pt_index page_num, pt_index fram_num, pt_bits perm_page);

/* Remove a page table entry */
void pt_unmap(page_table table, pt_index page_num);

/* Set and clear bits */
void pt_set_dirty(page_table table, pt_index page_num);
void pt_set_accessed(page_table table, pt_index page_num);
void pt_set_present(page_table table, pt_index page_num);
void pt_clear_dirty(page_table table, pt_index page_num);
void pt_clear_accessed(page_table table, pt_index page_num);
void pt_clear_present(page_table table, pt_index page_num);


/* Test bits */
int pt_allocated(page_table table, pt_index page_num);
int pt_dirty(page_table table, pt_index page_num);
int pt_accessed(page_table table, pt_index page_num);
int pt_present(page_table table, pt_index page_num);

/* Test permissions */
int pt_not_permitted(page_table table, pt_index page_num, pt_bits perm_needed);

/* Translate a virtual address to a physical address */
int pt_translate(page_table table, pt_address virtual_address, pt_bits perm_needed);

/* Display page table */
void pt_display(page_table table);

/* Display page table entry */
void pt_display_entry(page_table table, pt_index page_num);

/* Display address */
void pt_display_address(const char *label, pt_address address);
  
#endif
