/* Jan Ras
 * CSCI 4100
 * Assignment 5
 * Test code for virtual address space simulation
 */

#include <stdio.h>
#include "va_space.h"

int main() {
  /* Display the page table */
  puts("TESTING INIT...");
  page_table table;
  pt_init(table);
  pt_display(table);
  puts("");

  /* Display a page table entry */
  puts("TESTING DISPLAY ENTRY...");
  pt_index page_num = 0xb1;
  pt_display_entry(table, page_num);
  puts("");
  
  /* Test set functions */
  puts("TESTING SET FUNCTIONS...");
  pt_set_dirty(table, page_num);
  pt_set_accessed(table, page_num);
  pt_set_present(table, page_num);
  puts("");
  
  /* Test boolean functions */
  puts("TESTING BOOL FUNCTIONS...");
  pt_allocated(table, page_num);
  pt_accessed(table, page_num);
  pt_dirty(table, page_num);
  pt_present(table, page_num);
  puts("");
  
  /* Test clear functions */
  puts("TESTING CLEAR FUNCTIONS...");
  pt_clear_dirty(table, page_num);
  pt_clear_accessed(table, page_num);
  pt_clear_present(table, page_num);
  pt_display_entry(table, page_num);
  puts("");
  
  /* Test map */
  puts("TEST MAPPING...");
  pt_map(table, 0x42, 0xb4, PT_READ + PT_WRITE);
  puts("");
  pt_display(table);
  puts("");
  
  /* Test permissions */
  puts("TESTING PERMISSIONS...");
  pt_not_permitted(table, 0x42, PT_READ + PT_WRITE);
  puts("");
  
  /* Display an address */
  puts("TESTING DISPLAY ADDRESS...");
  pt_address virtual_address = 0x42f3;
  pt_display_address("Virtual address", virtual_address);
  puts("");
  
  /* Testing translate */
  puts("TESTING TRANSLATE - address allocated, in memory, with R + W permissions...");
  pt_translate(table, virtual_address, PT_READ + PT_WRITE);
  puts("");
  
  
  puts("TESTING TRANSLATE - address not allocated...");
  pt_translate(table, 0xb100, PT_READ);
  puts("");
  
  
  puts("TESTING TRANSLATE - address allocated, in memory, without E permission...");
  pt_translate(table, virtual_address, PT_EXECUTE);
  puts("");
  
  
  puts("TESTING TRANSLATE - address allocated, present bit cleared...");
  pt_clear_present(table, 0x42);
  pt_translate(table, virtual_address, PT_EXECUTE);
  puts("");
  
  /* Test unmap */
  puts("TESTING UNMAP");
  pt_unmap(table, 0x42);
  pt_display_entry(table, 0x42);
  
  return 0;
}
