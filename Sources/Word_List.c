/** @file Word_List.c
 * See Word_List.h for description.
 * @author Adrien RICCIARDI
 */
#include <stdio.h>
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
	if (Pointer_New_Item == NULL) return -1;
	strcpy(Pointer_New_Item->String_Word, Pointer_String_Word);

	// Insert the item at the list head (this is simple and fast)
	Pointer_New_Item->Pointer_Next_Item = Pointer_Word_List->Pointer_Head_Item;
	if (Pointer_Word_List->Pointer_Head_Item != NULL) Pointer_Word_List->Pointer_Head_Item->Pointer_Previous_Item = Pointer_New_Item; // Also link the previous item to make an item removal easy
	Pointer_Word_List->Pointer_Head_Item = Pointer_New_Item;
	Pointer_Word_List->Size++;

	return 0;
}

TWordListItem *WordListRemove(TWordList *Pointer_Word_List, TWordListItem *Pointer_Item)
{
	TWordListItem *Pointer_Previous_Item, *Pointer_Next_Item;

	// Cache some values to make algorithm more understandable
	Pointer_Previous_Item = Pointer_Item->Pointer_Previous_Item;
	Pointer_Next_Item = Pointer_Item->Pointer_Next_Item;

	// If there is a previous item, make it point to the item following the one that will be removed
	if (Pointer_Previous_Item != NULL) Pointer_Previous_Item->Pointer_Next_Item = Pointer_Next_Item;
	// Otherwise, the head item is removed, so update the list head
	else Pointer_Word_List->Pointer_Head_Item = Pointer_Next_Item;

	// If there is a following item, make it point to the item preceding the one that will be removed
	if (Pointer_Next_Item != NULL) Pointer_Next_Item->Pointer_Previous_Item = Pointer_Previous_Item;

	// Release the item memory
	free(Pointer_Item);
	Pointer_Word_List->Size--;

	return Pointer_Next_Item;
}

void WordListDisplay(TWordList *Pointer_Word_List)
{
	int i;
	TWordListItem *Pointer_Item;

	printf("Grid size = %d, head item pointer = %p.\n", Pointer_Word_List->Size, Pointer_Word_List->Pointer_Head_Item);

	Pointer_Item = Pointer_Word_List->Pointer_Head_Item;
	for (i = 0; i < Pointer_Word_List->Size; i++)
	{
		printf("Item %d (address = %p) : word = \"%s\", previous item pointer = %p, next item pointer = %p.\n", i, Pointer_Item, Pointer_Item->String_Word, Pointer_Item->Pointer_Previous_Item, Pointer_Item->Pointer_Next_Item);
		Pointer_Item = Pointer_Item->Pointer_Next_Item;
	}
}
