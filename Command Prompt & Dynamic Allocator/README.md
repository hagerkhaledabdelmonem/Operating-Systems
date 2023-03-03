# **Functions definitions in dynamic_allocator.c**
## Command Prompt :
   - Add an "Autocomplete" feature to command prompt, which allow the user to list all commands that start with a given set of characters.
   - The user should write set of characters then press enter:
     - If the set of characters are complete and represent an existing command, then execute it (Already implemented). 
     - Else, if there’s one (or more) command that start with the given characters, print their names (ONE PER LINE)
     - Else, print the “unknown command” message that is already exist in the given code.


## Dynamic Allocator :
   - Objective:
        Handle memory blocks dynamically by using LISTS to allocate or free any required space either by the OS or any user program   
   - The main functions are :
     1. initialize_MemBlocksList
       - Function Role: 
         - Currently, the “MemBlockNodes” array is already created with empty “MAX_MEM_BLOCK_CNT” block elements.
         - “AvailableMemBlocksList” is empty without any pointers or nodes.
         - This function shall:
            - ZEROing the size & pointers of “AvailableMemBlocksList”.
            - Then, fill the list by making it points to the first n=numOfBlocks empty elements exist in “MemBlockNodes” array.
            - At the end of this function, the “AvailableMemBlocksList” shall be initialized as a linked list of “numOfBlocks” elements from those that already exist in the “MemBlockNodes” array.

     2. find_block
        - Function Role: 
          -	It shall search for the given start virtual address (va) in the given list (blockList).
          - If a block found with sva equal to given va, return this block.
          - If not found, return NULL.

3.	alloc_block_FF
Function Role: 
•	This function searches for a free memory block in the FreeMemBlocksList with a size greater than or equal input size using FIRST FIT STRATEGY to be allocated later on in the memory.
•	The possible CASES results from the search using FF strategy are:
•	Case 1: no suitable block is found  return NULL.
•	Case 2: a block is found with EXACT size  remove it from the FreeMemBlocksList and return it.
•	Case 3: a block is found with GREATER size  divide it into 2 blocks:
o	A new block with the required size, shall be RETURNED by the end of the function.
o	An updated block with the remaining space, shall be kept in the list

3.	alloc_block_BF
Function Role: 
•	This function searches for a free memory block in the FreeMemBlocksList with a size greater than or equal input size using BEST FIT STRATEGY to be allocated later on in the memory.
•	The possible CASES results from the search using BF strategy are the same that can be occurred by applying FF.

4.	insert_sorted_allocList
Function Role: 
•	This function takes a block and insert it in the AllocMemBlocksList.
•	All blocks inserted in AllocMemBlocksList are placed in a sorted ascending order based on each block’s sva.
•	It has two cases:
o	Case 1: If the AllocMemBlocksList is empty  insert the given block at the head of the list.
o	Case 2: Otherwise  insert the given block in its correct location according to its sva (simply by iterate on the AllocMemBlocksList).

5.	insert_sorted_with_merge_freeList
Function Role: 
•	It takes a block and insert it in the FreeMemBlocksList.
•	All blocks inserted in FreeMemBlocksList are placed in a sorted ascending order based on each block’s sva.
•	However, if the inserted block is DIRECTLY adjacent to another free block (either before or after it)  they shall be merged as ONE block.
•	It has the following cases:
o	Case 1: If the FreeMemBlocksList is empty  insert the given block at the head of the list.
o	Otherwise  insert the given block in its correct location according to its sva (simply by iterate on the FreeMemBlocksList). Here, one of the following four cases can be happened:
	Case 2: No merge
	Case 3: Merge with previous
	Case 4: Merge with next
	Case 5: Merge with previous and next

6.	alloc_block_NF
Function Role: 
•	This function searches for a free memory block in the FreeMemBlocksList with a size greater than or equal input size using NEXT FIT STRATEGY to be allocated later on in the memory.
•	The possible CASES results from the search using NF strategy are the same as FF & BF.
