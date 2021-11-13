/** @file Main.c
 * Word search solver entry point.
 * @author Adrien RICCIARDI
 */
#include <Grid.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Word_List.h>

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Store a list of words to find for each alphabet letter. */
static TWordList Main_Word_Lists[CONFIGURATION_ALPHABET_LETTERS_COUNT];

/** The loaded grid height. */
static int Main_Grid_Rows_Count;
/** The loaded grid width. */
static int Main_Grid_Columns_Count;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Solve the grid in the smoothest way and print the hidden word.
 * @return -1 if the grid logic is invalid,
 * @return 0 on success.
 */
int MainSolveGrid(void)
{
	int Row, Column, Found_Words_Count = 0;
	char Character;
	TWordList *Pointer_Word_List;
	TWordListItem *Pointer_Word_List_Item;

	// Check each grid character only once
	for (Row = 0; Row < Main_Grid_Rows_Count; Row++)
	{
		for (Column = 0; Column < Main_Grid_Columns_Count; Column++)
		{
			// Cache the character corresponding to the current location
			Character = GridGetLetter(Row, Column);

			// Determine the word list to use according to the letter value
			Pointer_Word_List = &Main_Word_Lists[Character - 'A'];

			// Check all possible words
			Pointer_Word_List_Item = Pointer_Word_List->Pointer_Head_Item;
			while (Pointer_Word_List_Item != NULL)
			{
				if (GridMatchWordWithPosition(Pointer_Word_List_Item->String_Word, Row, Column) == 0)
				{
					Found_Words_Count++;
					printf("Found word \"%s\" at row %d and column %d (%d words have been found).\n", Pointer_Word_List_Item->String_Word, Row + 1, Column + 1, Found_Words_Count);
					Pointer_Word_List_Item = WordListRemove(Pointer_Word_List, Pointer_Word_List_Item); // The function returns the item following the one that has been deleted
				}
				// Proceed to next word
				else Pointer_Word_List_Item = Pointer_Word_List_Item->Pointer_Next_Item;
			}
		}
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	int i;
	char String_Hidden_Word[CONFIGURATION_HIDDEN_WORD_MAXIMUM_STRING_SIZE];

	// Display banner
	printf("+---------------------------+\n"
		"|    Word search solver     |\n"
		"| (C) 2021 Adrien RICCIARDI |\n"
		"+---------------------------+\n");

	// Check arguments
	if (argc != 2)
	{
		printf("Usage : %s Word_Search_File\n"
			"Word search file format consists of the grid content, followed by the separating character '-', then followed by the list of the words to search (one per line).\n"
			"Only ASCII letters (from A to Z), both uppercase of lowercase, are accepted.\n"
			"See test files in the Tests directory for examples.\n", argv[0]);
		return EXIT_FAILURE;
	}

	// Try to load the specified file
	printf("Trying to solve the file \"%s\".\n", argv[1]);
	if (GridLoadFromFile(argv[1], Main_Word_Lists, &Main_Grid_Rows_Count, &Main_Grid_Columns_Count) != 0)
	{
		printf("The specified file \"%s\" is invalid\n.", argv[1]);
		return EXIT_FAILURE;
	}

	// Try to find all searched words and the hidden word
	if (MainSolveGrid() != 0)
	{
		printf("Error : this grid is invalid, no solution could be found.\n");
		return EXIT_FAILURE;
	}

	// Make sure all words have been found
	for (i = 0; i < CONFIGURATION_ALPHABET_LETTERS_COUNT; i++)
	{
		if (Main_Word_Lists[i].Size > 0)
		{
			printf("Error : some words were not found in the grid, the grid seems to be invalid.\n");
			return EXIT_FAILURE;
		}
	}

	// Try to display the hidden word
	if (GridGetHiddenWord(String_Hidden_Word) != 0)
	{
		printf("Error : the hidden word is too long, the grid seems to be invalid.\n");
		return EXIT_FAILURE;
	}
	if (String_Hidden_Word[0] == 0) printf("This grid does not contain a hidden word.\n");
	else printf("Hidden word : \"%s\".\n", String_Hidden_Word);

	return EXIT_SUCCESS;
}
