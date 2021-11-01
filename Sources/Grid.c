/** @file Grid.c
 * See Grid.h for description.
 * @author Adrien RICCIARDI
 */
#include <errno.h>
#include <Grid.h>
#include <stdio.h>
#include <string.h>

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The grid containing the letters to search words from. */
static char Grid_Content[CONFIGURATION_GRID_MAXIMUM_SIZE * CONFIGURATION_GRID_MAXIMUM_SIZE];

/** How many rows have the loaded grid. */
static int Grid_Rows_Count = 0;
/** How many columns have the loaded grid. */
static int Grid_Columns_Count = 0;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Make sure the provided string does contain only uppercase letters (convert lowercase ones if needed).
 * @param Pointer_String The string to check, it can be modified on output if some lowercase letters were converted to uppercase.
 * @return 0 if the string is fine,
 * @return 1 if the string contains invalid characters.
 */
static int GridValidateString(char *Pointer_String)
{
	char Character;

	// Only uppercase letters are allowed in the string
	while (*Pointer_String != 0)
	{
		// Cache the next character
		Character = *Pointer_String;

		// Convert lowercase character to uppercase if needed
		if ((Character >= 'a') && (Character <= 'z'))
		{
			Character -= 32;
			*Pointer_String = Character;
		}

		// Is this a valid character ?
		if ((Character < 'A') || (Character > 'Z')) return 1;
		Pointer_String++;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int GridLoadFromFile(char *Pointer_String_File_Name, TWordList *Pointer_Word_Lists)
{
	FILE *Pointer_File = NULL;
	int Return_Value = -1, Words_Count = 0;
	char String_Temporary[256];
	size_t i, Length;

	// Try to open the file
	Pointer_File = fopen(Pointer_String_File_Name, "r");
	if (Pointer_File == NULL)
	{
		printf("Error : could not open the file \"%s\" (%s).\n", Pointer_String_File_Name, strerror(errno));
		return -1;
	}

	// Load grid content
	while (1)
	{
		// Retrieve the next grid row
		errno = 0;
		if (fscanf(Pointer_File, "%s", String_Temporary) != 1)
		{
			if (errno != 0)
			{
				printf("Error : could not read the file \"%s\" (%s).\n", Pointer_String_File_Name, strerror(errno));
				goto Exit;
			}
			// The end of the file is reached
			else
			{
				printf("Error : end of file has been reached without reaching the list of words to find.\n");
				goto Exit;
			}
		}

		// Go to the next step if the separator has been found
		if (strcmp(String_Temporary, "-") == 0) break;

		// Determine the column size
		Length = strlen(String_Temporary);
		if (Length > CONFIGURATION_GRID_MAXIMUM_SIZE)
		{
			printf("Error : grid row size can't be greater than %d characters.\n", CONFIGURATION_GRID_MAXIMUM_SIZE);
			goto Exit;
		}

		// Does the line contain only the allowed characters ?
		if (GridValidateString(String_Temporary) != 0)
		{
			printf("Error : the grid line \"%s\" contains invalid characters (only letters are allowed).\n", String_Temporary);
			goto Exit;
		}

		// Set the grid width if this is the first row read
		if (Grid_Columns_Count == 0) Grid_Columns_Count = Length;
		// Grid width has already been found, make sure this line has the correct length
		else if ((int) Length != Grid_Columns_Count)
		{
			printf("Error : all grid lines must be of the same length.\n");
			goto Exit;
		}
		Grid_Rows_Count++;
	}

	// Initialize all word lists before adding words to them
	for (i = 0; i < CONFIGURATION_ALPHABET_LETTERS_COUNT; i++) WordListInitialize(&Pointer_Word_Lists[i]);

	// Read one word at a time
	while (1)
	{
		// Retrieve the next word
		errno = 0;
		if (fscanf(Pointer_File, "%s", String_Temporary) != 1)
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
		Length = strlen(String_Temporary);
		if (Length == 0)
		{
			printf("Error : a searched word can't be empty.\n");
			goto Exit;
		}
		if (Length >= CONFIGURATION_WORD_LIST_ITEM_MAXIMUM_STRING_SIZE)
		{
			printf("Error : the searched word \"%s\" is too long.\n", String_Temporary);
			goto Exit;
		}

		// Does the word contain only the allowed characters ?
		if (GridValidateString(String_Temporary) != 0)
		{
			printf("Error : the word \"%s\" contains invalid characters (only letters are allowed).\n", String_Temporary);
			goto Exit;
		}

		// The word is valid, insert it to the correct list
		i = String_Temporary[0] - 'A'; // Recycle 'i' variable to compute the destination list
		if (WordListInsert(&Pointer_Word_Lists[i], String_Temporary) != 0)
		{
			printf("Error : failed to insert the word in the list of the letter '%c'.\n", String_Temporary[0]);
			goto Exit;
		}
		Words_Count++;
	}

	// Everything went fine, display some statistics
	printf("Successfully loaded a %dx%d grid and %d words to search.\n", Grid_Columns_Count, Grid_Rows_Count, Words_Count);
	Return_Value = 0;

Exit:
	if (Pointer_File != NULL) fclose(Pointer_File);
	return Return_Value;
}