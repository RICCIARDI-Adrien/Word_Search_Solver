/** @file Word_List.h
 * A dynamically allocated list containing statically allocated strings.
 * @author Adrien RICCIARDI
 */
#ifndef H_WORD_LIST_H
#define H_WORD_LIST_H

#include <Configuration.h>

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** A word in a list. */
typedef struct TWordListItem
{
	char String_Word[CONFIGURATION_WORD_LIST_ITEM_MAXIMUM_STRING_SIZE];
	struct TWordListItem *Pointer_Previous_Item;
	struct TWordListItem *Pointer_Next_Item;
} TWordListItem;

/** The list object. */
typedef struct
{
	int Size;
	TWordListItem *Pointer_Head_Item;
} TWordList;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Clear the list to make it ready to use.
 * @param Pointer_Word_List The list to initialize.
 * @note Initializing a non-empty list will result in a memory leak.
 */
void WordListInitialize(TWordList *Pointer_Word_List);

/** Add a new word item at the head of the list.
 * @param Pointer_Word_List The list to insert the word to.
 * @param Pointer_String_Word The string to insert into the list. The provided string will be copied into the list internal memory.
 * @return -2 if the provided word string is too long,
 * @return -1 if no more heap memory is available,
 * @return 0 on success.
 */
int WordListInsert(TWordList *Pointer_Word_List, char *Pointer_String_Word);

/** Remove a specific item from a list.
 * @param Pointer_Word_List The list to remove an item from.
 * @param Pointer_Item The item to remove. If the provided item does not belong to the list, the result will be unpredictable.
 * @return The item following the one that has been removed (this can be NULL if the last list item was removed).
 */
TWordListItem *WordListRemove(TWordList *Pointer_Word_List, TWordListItem *Pointer_Item);

/** Print a list content on the console, this is a debug function.
 * @param Pointer_Word_List The list to print.
 */
void WordListDisplay(TWordList *Pointer_Word_List);

#endif
