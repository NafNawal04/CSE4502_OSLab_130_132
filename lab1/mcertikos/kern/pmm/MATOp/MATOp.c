#include <lib/debug.h>
#include "import.h"

#define PAGESIZE     4096
#define VM_USERLO    0x40000000
#define VM_USERHI    0xF0000000
#define VM_USERLO_PI (VM_USERLO / PAGESIZE)
#define VM_USERHI_PI (VM_USERHI / PAGESIZE)

/**
 * Allocate a physical page.
 *
 * 1. First, implement a naive page allocator that scans the allocation table (AT)
 *    using the functions defined in import.h to find the first unallocated page
 *    with normal permissions.
 *    (Q: Do you have to scan the allocation table from index 0? Recall how you have
 *    initialized the table in pmem_init.)
 *    Then mark the page as allocated in the allocation table and return the page
 *    index of the page found. In the case when there is no available page found,
 *    return 0.
 * 2. Optimize the code using memoization so that you do not have to
 *    scan the allocation table from scratch every time.
 */

static unsigned int start = VM_USERLO_PI;


unsigned int palloc()
{
    // TODO

    if (get_nps() == 0) 
    {
      return 0;    //no pages
    }

    unsigned int i = start;
    while(1)
    {
        if (at_is_norm(i) && at_is_allocated(i) == 0)   //if it has normal permission and free space
        {
            at_set_allocated(i, 1);    //allocate it
            start = i + 1;
            return i;
            
        }
        i++;    //next page index
        
        if (i == VM_USERHI_PI) 
        {
            i = VM_USERLO_PI;      //if start moves to the end, we set start to the beginning
        }
        if (i == start)
        {
            return 0;
        }

    }
 
    return 0;
}

/**
 * Free a physical page.
 *
 * This function marks the page with given index as unallocated
 * in the allocation table.
 *
 * Hint: Simple.
 */
void pfree(unsigned int pfree_index)
{
    // TODO
    at_set_allocated(pfree_index,0);
}
