/* Jan Ras
 * CSCI 4100
 * Assignment 2
 * Source file for virtual address space simulation
 */

#include "va_space.h"
#include <stdio.h>
#include <string.h>

/* Error codes */
const int ERR_PAGE_NOT_ALLOCATED = -1;
const int ERR_PAGE_NOT_PRESENT = -2;
const int ERR_PERMISSION_DENIED = -3;

/* Bits for page table entries */
const pt_bits PT_ALLOC = 0x80, PT_DIRTY = 0x40, PT_ACCESSED = 0x20, PT_PRESENT = 0x10,
  PT_KERNEL = 0x08, PT_READ = 0x04, PT_WRITE = 0x02, PT_EXECUTE = 0x01;

/* Initialize an empty page table */
void pt_init(page_table table) {
  puts("Initializing table...");
  int i;
  for (i = 0; i < 256; i++) {
    table[i] = 0;
  }
}

/* Add a page table entry */
void pt_map(page_table table, pt_index page_num, pt_index frame_num, pt_bits perm_page) {
  pt_address e = frame_num; // new entry with frame_num as first 8 bits
  e = frame_num << 8 | perm_page | PT_ALLOC; // shift bits to left and add pt_bits and alloc bit
  table[page_num] = e; // add entry to table
  
  char req[50] = "";
  if (perm_page & PT_READ)
    strcat(req, " READ");
  if (perm_page & PT_WRITE)
    strcat(req, " WRITE");
  if (perm_page & PT_EXECUTE)
    strcat(req, " EXECUTE");
  
   
  
  // check allocated bit
  pt_allocated(table, page_num);
  
  // set present bit
  pt_set_present(table, page_num); 
  
  // clear accessed and dirty bit
  pt_clear_accessed(table, page_num); 
  pt_clear_dirty(table, page_num);
  
  printf("Mapping page 0x%02x to frame 0x%02x with%s permissions...\n", page_num, frame_num, req);
  pt_display_entry(table, page_num);
  
}

/* Remove a page table entry */
void pt_unmap(page_table table, pt_index page_num) {
  table[page_num] = 0;
}

/* Set and clear bits */
void pt_set_dirty(page_table table, pt_index page_num){
  puts("Setting dirty bit:");
  pt_address e = table[page_num];
  e = e | PT_DIRTY;
  table[page_num] = e;
  puts("Dirty bit is set");
}

void pt_set_accessed(page_table table, pt_index page_num){
  puts("\nSetting accessed bit:");
  pt_address e = table[page_num];
  e = e | PT_ACCESSED;
  table[page_num] = e;
  puts("Accessed bit is set");
}

void pt_set_present(page_table table, pt_index page_num){
  puts("\nSetting present bit:");
  pt_address e = table[page_num];
  e = e | PT_PRESENT;
  table[page_num] = e;
  puts("Present bit is set");
}

void pt_clear_dirty(page_table table, pt_index page_num){
  puts("Clearing dirty bit:");
  pt_entry e = table[page_num];
  e = e & ~PT_DIRTY;
  table[page_num] = e;
  puts("Dirty bit is not set");
}

void pt_clear_accessed(page_table table, pt_index page_num){
  puts("\nClearing accessed bit:");
  pt_entry e = table[page_num];
  e = e & ~PT_ACCESSED;
  table[page_num] = e;
  puts("Accessed bit is not set");
}

void pt_clear_present(page_table table, pt_index page_num){
  puts("\nClearing present bit:");
  pt_entry e = table[page_num];
  e = e & ~PT_PRESENT;
  table[page_num] = e;
  puts("Present bit is not set");
}

/* Test bits */
int pt_allocated(page_table table, pt_index page_num) {
  pt_entry entry = table[page_num];
  if(entry & PT_ALLOC) {
    puts("The allocated bit is set");
    return 1;
  }
  else {
    puts("The allocated bit is not set");
    return 0;
  }
}

int pt_dirty(page_table table, pt_index page_num) {
  pt_entry entry = table[page_num];
  if(entry & PT_DIRTY) {
    puts("The dirty bit is set");
    return 1;
  }
  else {
    puts("The dirty bit is not set");
    return 0;
  }
}

int pt_accessed(page_table table, pt_index page_num) {
  pt_entry entry = table[page_num];
  if(entry & PT_ACCESSED) {
    puts("The accessed bit is set");
    return 1;
  }
  else {
    puts("The accessed bit is not set");
    return 0;
  }
}

int pt_present(page_table table, pt_index page_num) {
  pt_entry entry = table[page_num];
  if(entry & PT_PRESENT) {
    puts("The present bit is set");
    return 1;
  }
  else {
    puts("The present bit is not set");
    return 0;
  }
}

/* Test permissions */
int pt_not_permitted(page_table table, pt_index page_num, pt_bits perm_needed) {
  pt_entry e = table[page_num]; 
  int perms = e & 0x000F;
  
  char req[50] = "";
  if (perm_needed & PT_READ)
    strcat(req, " READ");
  if (perm_needed & PT_WRITE)
    strcat(req, " WRITE");
  if (perm_needed & PT_EXECUTE)
    strcat(req, " EXECUTE");
  
  printf("Testing%s permissions\n", req);
  
  if(perm_needed == perms) {
    printf("Permitted to%s \n", req);
    return 0;
  }
  else {
    printf("Not permitted to%s \n", req);
    return 1;
  }
}

/* Translate a virtual address to a physical address */
int pt_translate(page_table table, pt_address virtual_address, pt_bits perm_needed) {
  pt_display_address("Virtual address: ", virtual_address);
  
  pt_address physical = 0;
  
  pt_index page_num = virtual_address >> 8;
  pt_bits perms = table[page_num] & 0x00FF;
  pt_display_entry(table, page_num);
  
  if (!pt_allocated(table, page_num)) {
    puts("Error: page not allocated");
    return ERR_PAGE_NOT_ALLOCATED;
  }
  else if (!pt_present(table, page_num)) {
    puts("Error: page not in memory");
    return ERR_PAGE_NOT_PRESENT;
  }
  else if (pt_not_permitted(table, page_num, perm_needed)) {
    puts("Error: permission denied");
    return ERR_PERMISSION_DENIED;
  }
  else {
    pt_offset offset = virtual_address & 0x00FF;

    pt_address padd = table[page_num];
    pt_index frame = padd >> 8;

    physical = frame << 8 | offset;
    pt_display_address("Physical address: ", physical);
    return physical;
  }
  
}

/* Display page table */
void pt_display(page_table table) {
  int row, col;
  const int ROWS = 16, COLS = 16;
  printf("   ");
  for(col = 0; col < COLS; col++)
    printf(" %x    ", col);
  puts("");
  for(row = 0; row < ROWS; row++) {
    printf("%x  ", row);
    for(col = 0; col < COLS; col++)
      printf("%04x  ", table[row * 16 + col]);
    puts("");
  }
}

/* Display page table entry */
void pt_display_entry(page_table table, pt_index page_num) {
  printf("%02x: 0x%04x\n", page_num, table[page_num]);
}

/* Display address */
void pt_display_address(const char *label, pt_address address) {
  printf("%s: 0x%04x\n", label, address);
}


