/*
 * dyn_block_management.c
 *
 *  Created on: Sep 21, 2022
 *      Author: HP
 */
#include <inc/assert.h>
#include <inc/string.h>
#include "../inc/dynamic_allocator.h"


//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//

//===========================
// PRINT MEM BLOCK LISTS:
//===========================

void print_mem_block_lists()
{
	cprintf("\n=========================================\n");
	struct MemBlock* blk ;
	struct MemBlock* lastBlk = NULL ;
	cprintf("\nFreeMemBlocksList:\n");
	uint8 sorted = 1 ;
	LIST_FOREACH(blk, &FreeMemBlocksList)
	{
		if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
			sorted = 0 ;
		cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size) ;
		lastBlk = blk;
	}
	if (!sorted)	cprintf("\nFreeMemBlocksList is NOT SORTED!!\n") ;

	lastBlk = NULL ;
	cprintf("\nAllocMemBlocksList:\n");
	sorted = 1 ;
	LIST_FOREACH(blk, &AllocMemBlocksList)
	{
		if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
			sorted = 0 ;
		cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size) ;
		lastBlk = blk;
	}
	if (!sorted)	cprintf("\nAllocMemBlocksList is NOT SORTED!!\n") ;
	cprintf("\n=========================================\n");

}

//********************************************************************************//
//********************************************************************************//

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

//===============================
// [1] INITIALIZE AVAILABLE LIST:
//===============================
void initialize_MemBlocksList(uint32 numOfBlocks)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] initialize_MemBlocksList
	// Write your code here, remove the panic and write your code
	//panic("initialize_MemBlocksList() is not implemented yet...!!");
	    LIST_INIT(&AvailableMemBlocksList);
		LIST_INSERT_HEAD(&AvailableMemBlocksList,&(MemBlockNodes[0]));
		struct MemBlock *ptr = MemBlockNodes;
		LIST_FOREACH(ptr,&(AvailableMemBlocksList))
		{
			for(uint32 i = 1 ; i < numOfBlocks ; i++)
			{
				ptr++;
				LIST_INSERT_TAIL(&AvailableMemBlocksList,ptr);
			}
		}
}

//===============================
// [2] FIND BLOCK:
//===============================
struct MemBlock *find_block(struct MemBlock_List *blockList, uint32 va)
{
	struct MemBlock *FoundBlock=LIST_FIRST(blockList);
	int flag= 0;
	for(;FoundBlock !=NULL;){
			 if(va==FoundBlock->sva){
				  flag=1;
				break;

			  }
			 flag=0;
				 FoundBlock= FoundBlock->prev_next_info.le_next;
 		}
	if(flag==1)
		return FoundBlock;
	else
		return NULL;
}
//=========================================
// [3] INSERT BLOCK IN ALLOC LIST [SORTED]:
//=========================================
void insert_sorted_allocList(struct MemBlock *blockToInsert)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_allocList
	// Write your code here, remove the panic and write your code
	if(LIST_FIRST(&AllocMemBlocksList)==NULL){

		LIST_INSERT_HEAD(&AllocMemBlocksList,blockToInsert);
	}
	else if(LIST_FIRST(&AllocMemBlocksList)->sva >= blockToInsert->sva)
		LIST_INSERT_BEFORE(&AllocMemBlocksList,LIST_FIRST(&AllocMemBlocksList),blockToInsert);
	else {
		struct  MemBlock *temp;
temp=LIST_FIRST(&AllocMemBlocksList);

	while(temp->prev_next_info.le_next!=NULL &&
			temp->prev_next_info.le_next->sva <= blockToInsert->sva){
		temp=temp->prev_next_info.le_next;
	}
	LIST_INSERT_AFTER(&AllocMemBlocksList,temp,blockToInsert);

	}



	//panic("insert_sorted_allocList() is not implemented yet...!!");
}

//=========================================
// [4] ALLOCATE BLOCK BY FIRST FIT:
//=========================================
struct MemBlock *ptr = NULL;
struct MemBlock* alloc_block_FF(uint32 size) {
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_FF
	// Write your code here, remove the panic and write your code
	//	panic("alloc_block_FF() is not implemented yet...!!");

	struct MemBlock *blk = LIST_FIRST(&FreeMemBlocksList), *result;
		int isFound = 0;

		LIST_FOREACH(blk, &FreeMemBlocksList) {
			if (blk->size == size) {
				result = blk;
				LIST_REMOVE(&FreeMemBlocksList, blk);
				isFound = 1;
				break;
			} else if (blk->size > size) {
				// pulling new block from the availableBlocks list
				struct MemBlock* newBlk = LIST_FIRST(&AvailableMemBlocksList);
				LIST_REMOVE(&AvailableMemBlocksList, newBlk);

				// setting it's values
				newBlk->sva = blk->sva;
				newBlk->size = size;

				// updating the current block data after splitting it
				blk->sva += size;
				blk->size -= size;


				result = newBlk;
				isFound = 1;
				break;
			}
		}
		if (isFound == 0)
			return NULL;

		return result;
}

//=========================================
// [5] ALLOCATE BLOCK BY BEST FIT:
//=========================================
struct MemBlock *alloc_block_BF(uint32 size)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_BF
	// Write your code here, remove the panic and write your code
	//panic("alloc_block_BF() is not implemented yet...!!");
    struct MemBlock *ptr , *min =  NULL , *result = NULL ;
	LIST_FOREACH(ptr,&FreeMemBlocksList)
	{
		if(ptr->size >= size)
		{
			if(min == NULL)
			{
				min = ptr;
			}
			if(ptr->size < min->size)
				min = ptr;
		}
	}
	if(min == NULL)
		return NULL;
	if(min->size == size)
	{
		result = min;
		LIST_REMOVE(&FreeMemBlocksList,min);
	}
	else if(min->size > size)
	{
		struct MemBlock *NewBlock = LIST_FIRST(&AvailableMemBlocksList);
		LIST_REMOVE(&AvailableMemBlocksList, NewBlock);
		NewBlock->sva = min->sva;
		NewBlock->size = size;

		min->sva += size;
		min->size -= size;

		result = NewBlock;
	}
	return result;
}


//=========================================
// [7] ALLOCATE BLOCK BY NEXT FIT:
//=========================================
struct MemBlock* next_to_alloc = NULL;
struct MemBlock *alloc_block_NF(uint32 size)
{
	//TODO: [PROJECT MS1 - BONUS] [DYNAMIC ALLOCATOR] alloc_block_NF
	// Write your code here, remove the panic and write your code
//	panic("alloc_block_NF() is not implemented yet...!!");

	if (next_to_alloc == NULL)
		next_to_alloc = LIST_FIRST(&FreeMemBlocksList);

	struct MemBlock* result = NULL;
	int isFound = 0;
	while (next_to_alloc != NULL) {
		if (next_to_alloc->size == size) {
			result = next_to_alloc;
			next_to_alloc = next_to_alloc->prev_next_info.le_next;
			LIST_REMOVE(&FreeMemBlocksList, result);
			isFound = 1;
			break;
		}
		else if (next_to_alloc->size > size) {
			// pulling new block from the availableBlocks list
			struct MemBlock* newBlk = LIST_FIRST(&AvailableMemBlocksList);
			LIST_REMOVE(&AvailableMemBlocksList, newBlk);

			// setting it's values
			newBlk->sva = next_to_alloc->sva;
			newBlk->size = size;

			// updating the current block data after splitting it
			next_to_alloc->sva += size;
			next_to_alloc->size -= size;

			result = newBlk;

			isFound = 1;
			break;
		}
		next_to_alloc = next_to_alloc->prev_next_info.le_next;
	}

	if (isFound == 0) {
		struct MemBlock* blk = LIST_FIRST(&FreeMemBlocksList);
		LIST_FOREACH(blk, &FreeMemBlocksList) {
				if (blk->size == size) {
					result = blk;
					next_to_alloc = blk->prev_next_info.le_next;
					LIST_REMOVE(&FreeMemBlocksList, blk);
					isFound = 1;
					break;
				} else if (blk->size > size) {
					// pulling new block from the availableBlocks list
					struct MemBlock* newBlk = LIST_FIRST(&AvailableMemBlocksList);
					LIST_REMOVE(&AvailableMemBlocksList, newBlk);

					// setting it's values
					newBlk->sva = blk->sva;
					newBlk->size = size;

					// updating the current block data after splitting it
					blk->sva += size;
					blk->size -= size;

					next_to_alloc = blk;
					result = newBlk;
					isFound = 1;
					break;
				}
			}
			if (isFound == 0)
				return NULL;
	}

	return result;
}

//===================================================
// [8] INSERT BLOCK (SORTED WITH MERGE) IN FREE LIST:
//===================================================
void insert_sorted_with_merge_freeList(struct MemBlock *blockToInsert)
{
//	cprintf("BEFORE INSERT with MERGE: insert [%x, %x)\n=====================\n", blockToInsert->sva, blockToInsert->sva + blockToInsert->size);
//	print_mem_block_lists() ;

	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_with_merge_freeList
	// Write your code here, remove the panic and write your code
	//panic("insert_sorted_with_merge_freeList() is not implemented yet...!!");

	struct MemBlock *point_free =LIST_FIRST(&FreeMemBlocksList);

	if(LIST_FIRST(&FreeMemBlocksList) == NULL){
		LIST_INSERT_HEAD(&FreeMemBlocksList , blockToInsert);
		point_free = FreeMemBlocksList.lh_first;
	}

	else if(point_free->sva > blockToInsert->sva){
			LIST_INSERT_BEFORE(&FreeMemBlocksList , point_free , blockToInsert);

			uint32 after_address = blockToInsert->sva + blockToInsert->size;
			if(after_address == (blockToInsert->prev_next_info.le_next)->sva){
				(blockToInsert->prev_next_info.le_next)->sva = blockToInsert->sva;
				(blockToInsert->prev_next_info.le_next)->size += blockToInsert->size;
				 blockToInsert->size = 0;
				 blockToInsert->sva = 0;
				 LIST_REMOVE(&FreeMemBlocksList,blockToInsert);
				 LIST_INSERT_TAIL(&AvailableMemBlocksList ,blockToInsert);
			}
	}

	else{
		while(point_free != NULL){

			 if(point_free->sva < blockToInsert->sva && (point_free->prev_next_info.le_next) == NULL) {
					LIST_INSERT_TAIL(&FreeMemBlocksList , blockToInsert);

					uint32 before_address = point_free->sva + point_free->size;

					if(before_address == blockToInsert->sva){
						(blockToInsert->prev_next_info.le_prev)->size += blockToInsert->size;
						 blockToInsert->size = 0;
						 blockToInsert->sva = 0;
						 LIST_REMOVE(&FreeMemBlocksList,blockToInsert);
						 LIST_INSERT_TAIL(&AvailableMemBlocksList , blockToInsert);
					   }
			 }

			 else if(point_free->sva < blockToInsert->sva &&
				(point_free->prev_next_info.le_next)->sva > blockToInsert->sva){
				LIST_INSERT_AFTER(&FreeMemBlocksList , point_free , blockToInsert);

				uint32 before_address = point_free->sva + point_free->size;
				uint32 after_address = blockToInsert->sva + blockToInsert->size;

				 if(before_address == blockToInsert->sva)
				 {
					(blockToInsert->prev_next_info.le_prev)->size += blockToInsert->size;
					 blockToInsert->size = 0;
					 blockToInsert->sva = 0;
					 LIST_REMOVE(&FreeMemBlocksList,blockToInsert);
					 LIST_INSERT_TAIL(&AvailableMemBlocksList , blockToInsert);

				uint32 final_address =	point_free->sva + point_free->size;

				 if(final_address == (point_free->prev_next_info.le_next)->sva){
					point_free->size +=  (point_free->prev_next_info.le_next)->size ;
					struct MemBlock *ptr = point_free->prev_next_info.le_next;
					point_free->prev_next_info.le_next = ptr->prev_next_info.le_next;
					ptr->size = 0;
					ptr->sva =0;
					LIST_REMOVE(&FreeMemBlocksList, ptr);
					LIST_INSERT_TAIL(&AvailableMemBlocksList , ptr);

				 }
				 }

				else if(after_address == (blockToInsert->prev_next_info.le_next)->sva){
						(blockToInsert->prev_next_info.le_next)->sva = blockToInsert->sva;
						(blockToInsert->prev_next_info.le_next)->size += blockToInsert->size;
						 blockToInsert->size = 0;
						 blockToInsert->sva = 0;
						 LIST_REMOVE(&FreeMemBlocksList,blockToInsert);
						 LIST_INSERT_TAIL(&AvailableMemBlocksList ,blockToInsert);
					}

				}
			 point_free = point_free->prev_next_info.le_next;
			 }

		}
//	cprintf("\nAFTER INSERT with MERGE:\n=====================\n");
//	print_mem_block_lists();
}
