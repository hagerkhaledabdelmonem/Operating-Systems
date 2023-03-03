# Operating-Systems
## Develop modules in a Real Operating System.

## It's Divided into 3 Parts :
1. Command Prompt & Dynamic Allocator :
   - Autocomplete
   - initialize_MemBlocksList
   - find_block
   - alloc_block_FF
   - alloc_block_BF
   - insert_sorted_allocList
   - insert_sorted_with_merge_freeList
   - alloc_block_NF
2. Paging, Chunk Oper’s & Kernel Heap :
   - Page Table Entries Manipulation :
     - pt_clear_page_table_entry
     - pt_set_page_permissions
     - pt_get_page_permissions
     - virtual_to_physical
   - RAM chunks Manipulation :
     - cut_paste_pages
     - copy_paste_chunk
     - share_chunk
     - allocate_chunk
     - calculate_allocated_space
     - calculate_required_frames
   - Kernel Heap :
     - initialize_dyn_block_system
     - kmalloc (using FIRST & BEST fit)
     - kfree
     - kheap_virtual_address
     - kheap_physical_address
     - Kmalloc (using NEXT fit)
     - Krealloc (using BEST fit)

3. Heap, Fault Handler, Semaphores & Shared Variables
   - Loading Program [env_create]
     - create_user_page_WS
     - create_user_directory
   - Page Fault Handler
     - Placement
     - Replacement
   - USER HEAP
     - initialize_dyn_block_system (User side)
     - malloc [Using First Fit] (User side)
     - free (User side)
     - free_user_mem(Kernel side)
   - Shared Variables
     - smalloc (User side)
     - sget (User side)
     - createSharedObject(Kernel side)
     - getSharedObject(Kernel side)
   - Semaphores
     - createSemaphore
     - waitSemaphore
     - signalSemaphore

## **Note**

Each Part has its README File That defines its Functions .


 			
