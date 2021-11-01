/** @file Word_List.c
 * @see Word_List.h for description.
 * @author Adrien RICCIARDI
 */
#include <Log.h>
#include <stdlib.h>
#include <string.h>
#include <Word_List.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void WordListInitialize(TWordList *Pointer_Word_List)
{
	Pointer_Word_List->Size = 0;
	Pointer_Word_List->Pointer_Head_Item = NULL;
}

int WordListInsert(TWordList *Pointer_Word_List, char *Pointer_String_Word)
{
	TWordListItem *Pointer_New_Item;

	// Make sure the word is not too long
	if (strlen(Pointer_String_Word) >= CONFIGURATION_WORD_LIST_ITEM_MAXIMUM_STRING_SIZE) return -2;

	// Try to allocate a new item
	Pointer_New_Item = malloc(sizeof(TWordListItem));
	if (Pointer_New_Item == NULL)
	{
		LOG_ERROR("Could not allocate memory for the new list item.");
		return -1;
	}
	strcpy(Pointer_New_Item->String_Word, Pointer_String_Word);

	// Insert the item at the list head (this is simple and fast)
	Pointer_New_Item->Pointer_Next_Item = Pointer_Word_List->Pointer_Head_Item;
	Pointer_Word_List->Pointer_Head_Item = Pointer_New_Item;
	Pointer_Word_List->Size++;

	return 0;
}
