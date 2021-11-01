/** @file Main.c
 * Word search solver entry point.
 * @author Adrien RICCIARDI
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Word_List.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** How many letters in the latin alphabet. */
#define MAIN_ALPHABET_LETTERS_COUNT 26

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Store a list of words to find for each alphabet letter. */
static TWordList Main_Word_Lists[MAIN_ALPHABET_LETTERS_COUNT];

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** TODO */
static int MainLoadFile(char *Pointer_String_File_Name)
{
	FILE *Pointer_File = NULL;
	int Return_Value = -1;
	char String_Word[256], *Pointer_String_Word, Character;
	size_t i, Word_Length;

	// Try to open the file
	Pointer_File = fopen(Pointer_String_File_Name, "r");
	if (Pointer_File == NULL)
	{
		printf("Error : could not open the file \"%s\" (%s).\n", Pointer_String_File_Name, strerror(errno));
		return -1;
	}

	// TODO load grid

	// Initialize all word lists before adding words to them
	for (i = 0; i < MAIN_ALPHABET_LETTERS_COUNT; i++) WordListInitialize(&Main_Word_Lists[i]);

	// Read one word at a time
	while (1)
	{
		// Retrieve the next word
		errno = 0;
		if (fscanf(Pointer_File, "%s", String_Word) != 1)
		{
			if (errno != 0)
			{
				printf("Error : could not read the file \"%s\" (%s).\n", Pointer_String_File_Name, strerror(errno));
				goto Exit;
			}
			// The end of the file is reached
			else break;
		}

		// Is the next word size valid ?
		Word_Length = strlen(String_Word);
		if (Word_Length == 0)
		{
			printf("Error : a searched word can't be empty.\n");
			goto Exit;
		}
		if (Word_Length >= CONFIGURATION_WORD_LIST_ITEM_STRING_SIZE)
		{
			printf("Error : the searched word \"%s\" is too long.\n", String_Word);
			goto Exit;
		}

		// Only uppercase letters are allowed in the word
		Pointer_String_Word = String_Word;
		while (*Pointer_String_Word != 0)
		{
			// Cache next character
			Character = *Pointer_String_Word;

			// Convert lowercase character to uppercase if needed
			if ((Character >= 'a') && (Character <= 'z'))
			{
				Character -= 32;
				*Pointer_String_Word = Character;
			}

			// Is this a valid character ?
			if ((Character < 'A') || (Character > 'Z'))
			{
				printf("Error : the word \"%s\" contains invalid characters (only letters are allowed).\n", String_Word);
				goto Exit;
			}
			Pointer_String_Word++;
		}

		// The word is valid, insert it to the correct list
		i = String_Word[0] - 'A'; // Recycle 'i' variable to compute the destination list
		if (WordListInsert(&Main_Word_Lists[i], String_Word) != 0)
		{
			printf("Error : failed to insert the word in the list of the letter '%c'.\n", String_Word[0]);
			goto Exit;
		}
	}

	// Everything went fine
	Return_Value = 0;

Exit:
	if (Pointer_File != NULL) fclose(Pointer_File);
	return Return_Value;
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	// Check arguments
	if (argc != 2)
	{
		printf("Usage : %s Word_Search_File\n", argv[0]);
		// TODO add file format
		return EXIT_FAILURE;
	}

	// Try to load the specified file
	if (MainLoadFile(argv[1]) != 0)
	{
		printf("The specified file \"%s\" is invalid\n.", argv[1]);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
