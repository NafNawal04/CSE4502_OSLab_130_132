#include <lib/debug.h>
#include "import.h"

#define PAGESIZE     4096
#define VM_USERLO    0x40000000
#define VM_USERHI    0xF0000000
#define VM_USERLO_PI (VM_USERLO / PAGESIZE)
#define VM_USERHI_PI (VM_USERHI / PAGESIZE)

/**
 * The initialization function for the allocation table AT.
 * It contains two major parts:
 * 1. Calculate the actual physical memory of the machine, and sets the number
 *    of physical pages (NUM_PAGES).
 * 2. Initializes the physical allocation table (AT) implemented in the MATIntro layer
 *    based on the information available in the physical memory map table.
 *    Review import.h in the current directory for the list of available
 *    getter and setter functions.
 */
void pmem_init(unsigned int mbi_addr)
{
    unsigned int nps;
    

    // TODO: Define your local variables here.

    unsigned int row_num;
    unsigned int start_addr;
    unsigned int length;
    unsigned int highest_addr;
    unsigned int page_idx;
    unsigned int perm;



    // Calls the lower layer initialization primitive.
    // The parameter mbi_addr should not be used in the further code.
    devinit(mbi_addr);

    /**
     * Calculate the total number of physical pages provided by the hardware and
     * store it into the local variable nps.
     * Hint: Think of it as the highest address in the ranges of the memory map table,
     *       divided by the page size.
     */
    // TODO

    row_num = get_size();
    if(row_num == 0)
    {
      nps = 0;
    }

    else
    {
      start_addr = get_mms(row_num- 1);  //getting the start address or last row
      length = get_mml(row_num- 1);      
      highest_addr = start_addr + length - 1;    //getting the final address
      nps = (start_addr + length) / PAGESIZE;    
      //final address divided by pagesize will give number of pages 
    }

    


    set_nps(nps);  // Setting the value computed above to NUM_PAGES.

    /**
     * Initialization of the physical allocation table (AT).
     *
     * In CertiKOS, all addresses < VM_USERLO or >= VM_USERHI are reserved by the kernel.
     * That corresponds to the physical pages from 0 to VM_USERLO_PI - 1,
     * and from VM_USERHI_PI to NUM_PAGES - 1.
     * The rest of the pages that correspond to addresses [VM_USERLO, VM_USERHI)
     * can be used freely ONLY IF the entire page falls into one of the ranges in
     * the memory map table with the permission marked as usable.
     *
     * Hint:
     * 1. You have to initialize AT for all the page indices from 0 to NPS - 1.
     * 2. For the pages that are reserved by the kernel, simply set its permission to 1.
     *    Recall that the setter at_set_perm also marks the page as unallocated.
     *    Thus, you don't have to call another function to set the allocation flag.
     * 3. For the rest of the pages, explore the memory map table to set its permission
     *    accordingly. The permission should be set to 2 only if there is a range
     *    containing the entire page that is marked as available in the memory map table.
     *    Otherwise, it should be set to 0. Note that the ranges in the memory map are
     *    not aligned by pages, so it may be possible that for some pages, only some of
     *    the addresses are in a usable range. Currently, we do not utilize partial pages,
     *    so in that case, you should consider those pages as unavailable.
     */
    // TODO

    unsigned int i;
    for(i = 0; i < VM_USERLO_PI; i++)
    {
      at_set_perm(i, 1);   //no permission for kernel
    }
    
    for(i = VM_USERHI_PI; i < nps; i++)
    {
      at_set_perm(i, 1);   //no permission for kernel
    }
   
    for(i = VM_USERLO_PI; i < VM_USERHI_PI; i++)
    {
      at_set_perm(i, 0);    //setting permission default 0 for the usable spaces
    }


    for(i = 0; i < row_num; i++)
    {
        start_addr = get_mms(i);
        length = get_mml(i);
        perm = is_usable(i);
        perm = perm == 1? 2: 0;
        page_idx = start_addr / PAGESIZE;
        
        
        if(page_idx * PAGESIZE < start_addr)
        {
          page_idx++;     //if the page breaks,then it will iterate to the next page index
        }



        while((page_idx + 1) * PAGESIZE <= start_addr + length)   //until there's space of a complete page in a row
        {
            if(page_idx < VM_USERLO_PI)
            {
                page_idx++;     //if the page index is in kernel
                continue;
            }
            if(page_idx >= VM_USERHI_PI)
            {
                break;          //if the page index is in kernel
            }
            at_set_perm(page_idx, perm);
            page_idx++;
        }



    }


    
    
}
