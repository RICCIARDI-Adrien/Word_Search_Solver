/** @file Word_List.h
 * A statically allocated list containing statically allocated strings.
 * @author Adrien RICCIARDI
 */
#ifndef H_WORD_LIST_H
#define H_WORD_LIST_H

#include <Configuration.h>

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** TODO */
typedef struct TWordListItem
{
	char String_Word[CONFIGURATION_WORD_LIST_ITEM_STRING_SIZE];
	struct TWordListItem *Pointer_Next_Item;
} TWordListItem;

/** TODO */
typedef struct
{
	int Size;
	TWordListItem *Pointer_Head_Item;
} TWordList;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** TODO */
void WordListInitialize(TWordList *Pointer_Word_List);

/** TODO
 * @param PointerStringWord The string to insert into the list. The provided string will be copied into the list internal memory.
 * @return -2 if the provided word string is too long,
 * @return -1 if no more heap memory is available,
 * @return 0 on success.
 */
int WordListInsert(TWordList *Pointer_Word_List, char *Pointer_String_Word);

#endif
