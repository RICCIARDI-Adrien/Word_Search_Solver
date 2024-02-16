/** @file Grid.c
 * See Grid.h for description.
 * @author Adrien RICCIARDI
 */
#include <errno.h>
#include <Grid.h>
#include <stdio.h>
#include <string.h>

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** A grid cell, containing a letter and a flag telling whether the letter is part of a searched word. */
typedef struct
{
	char Letter;
	int Is_Letter_Present_In_Word;
} TGridCell;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The grid containing the letters to search words from. */
static TGridCell Grid_Cells[CONFIGURATION_GRID_MAXIMUM_SIZE * CONFIGURATION_GRID_MAXIMUM_SIZE];

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

/** Tell that the letter at the specified location is part of a word.
 * @param Row The vertical coordinate.
 * @param Column The horizontal coordinate.
 */
void GridSetLetterFound(int Row, int Column)
{
	// Make sure provided coordinates are valid
	if ((Row < 0) || (Row >= Grid_Rows_Count) || (Column < 0) || (Column >= Grid_Columns_Count))
	{
		printf("Error : the invalid coordinates %dx%d have been specified, returning 0.\n", Column, Row);
		return;
	}

	Grid_Cells[Row * Grid_Columns_Count + Column].Is_Letter_Present_In_Word = 1;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int GridLoadFromFile(char *Pointer_String_File_Name, TWordList *Pointer_Word_Lists, int *Pointer_Rows_Count, int *Pointer_Columns_Count, int *Pointer_Words_Count)
{
	FILE *Pointer_File = NULL;
	int i, Return_Value = -1, Words_Count = 0;
	char String_Temporary[256];
	size_t Length;
	TGridCell *Pointer_Grid_Cell = Grid_Cells;

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

		// Fill the current grid row with the read content
		for (i = 0; i < Grid_Columns_Count; i++)
		{
			Pointer_Grid_Cell->Letter = String_Temporary[i];
			Pointer_Grid_Cell++;
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

	*Pointer_Rows_Count = Grid_Rows_Count;
	*Pointer_Columns_Count = Grid_Columns_Count;
	*Pointer_Words_Count = Words_Count;

	// Everything went fine, display some statistics
	printf("Successfully loaded a %dx%d grid and %d words to search.\n", Grid_Columns_Count, Grid_Rows_Count, Words_Count);
	Return_Value = 0;

Exit:
	if (Pointer_File != NULL) fclose(Pointer_File);
	return Return_Value;
}

char GridGetLetter(int Row, int Column)
{
	// Make sure provided coordinates are valid
	if ((Row < 0) || (Row >= Grid_Rows_Count) || (Column < 0) || (Column >= Grid_Columns_Count))
	{
		printf("Error : the invalid coordinates %dx%d have been specified, returning 0.\n", Column, Row);
		return 0;
	}

	return Grid_Cells[Row * Grid_Columns_Count + Column].Letter;
}

int GridMatchWordWithPosition(char *Pointer_String_Word, int Row, int Column)
{
	int Word_Length, i;
	char String_Temporary[CONFIGURATION_WORD_LIST_ITEM_MAXIMUM_STRING_SIZE], *Pointer_String_Temporary;

	// Cache word to search length
	Word_Length = (int) strlen(Pointer_String_Word);

	// Can the word be compared to the location's north ?
	if ((Row + 1) - Word_Length >= 0)
	{
		// Retrieve the characters of the word to compare
		Pointer_String_Temporary = String_Temporary;
		for (i = 0; i < Word_Length; i++)
		{
			*Pointer_String_Temporary = GridGetLetter(Row - i, Column);
			Pointer_String_Temporary++;
		}
		*Pointer_String_Temporary = 0;

		// Does the searched word match ?
		if (strncmp(Pointer_String_Word, String_Temporary, Word_Length) == 0)
		{
			for (i = 0; i < Word_Length; i++) GridSetLetterFound(Row - i, Column);
			return 0;
		}
	}

	// Can the word be compared to the location's north east ?
	if ((Row + 1 >= Word_Length) && (Grid_Columns_Count - Column >= Word_Length)) // There is enough room vertically and vertically for the whole word to fit
	{
		// Retrieve the characters of the word to compare
		Pointer_String_Temporary = String_Temporary;
		for (i = 0; i < Word_Length; i++)
		{
			*Pointer_String_Temporary = GridGetLetter(Row - i, Column + i);
			Pointer_String_Temporary++;
		}
		*Pointer_String_Temporary = 0;

		// Does the searched word match ?
		if (strncmp(Pointer_String_Word, String_Temporary, Word_Length) == 0)
		{
			for (i = 0; i < Word_Length; i++) GridSetLetterFound(Row - i, Column + i);
			return 0;
		}
	}

	// Can the word be compared to the location's east ?
	if (Column + Word_Length <= Grid_Columns_Count)
	{
		// Retrieve the characters of the word to compare
		Pointer_String_Temporary = String_Temporary;
		for (i = 0; i < Word_Length; i++)
		{
			*Pointer_String_Temporary = GridGetLetter(Row, Column + i);
			Pointer_String_Temporary++;
		}
		*Pointer_String_Temporary = 0;

		// Does the searched word match ?
		if (strncmp(Pointer_String_Word, String_Temporary, Word_Length) == 0)
		{
			for (i = 0; i < Word_Length; i++) GridSetLetterFound(Row, Column + i);
			return 0;
		}
	}

	// Can the word be compared to the location's south east ?
	if ((Grid_Rows_Count - Row >= Word_Length) && (Grid_Columns_Count - Column >= Word_Length)) // There is enough room vertically and vertically for the whole word to fit
	{
		// Retrieve the characters of the word to compare
		Pointer_String_Temporary = String_Temporary;
		for (i = 0; i < Word_Length; i++)
		{
			*Pointer_String_Temporary = GridGetLetter(Row + i, Column + i);
			Pointer_String_Temporary++;
		}
		*Pointer_String_Temporary = 0;

		// Does the searched word match ?
		if (strncmp(Pointer_String_Word, String_Temporary, Word_Length) == 0)
		{
			for (i = 0; i < Word_Length; i++) GridSetLetterFound(Row + i, Column + i);
			return 0;
		}
	}

	// Can the word be compared to the location's south ?
	if (Grid_Rows_Count - Row >= Word_Length)
	{
		// Retrieve the characters of the word to compare
		Pointer_String_Temporary = String_Temporary;
		for (i = 0; i < Word_Length; i++)
		{
			*Pointer_String_Temporary = GridGetLetter(Row + i, Column);
			Pointer_String_Temporary++;
		}
		*Pointer_String_Temporary = 0;

		// Does the searched word match ?
		if (strncmp(Pointer_String_Word, String_Temporary, Word_Length) == 0)
		{
			for (i = 0; i < Word_Length; i++) GridSetLetterFound(Row + i, Column);
			return 0;
		}
	}

	// Can the word be compared to the location's south west
	if ((Grid_Rows_Count - Row >= Word_Length) && (Column + 1 >= Word_Length))
	{
		// Retrieve the characters of the word to compare
		Pointer_String_Temporary = String_Temporary;
		for (i = 0; i < Word_Length; i++)
		{
			*Pointer_String_Temporary = GridGetLetter(Row + i, Column - i);
			Pointer_String_Temporary++;
		}
		*Pointer_String_Temporary = 0;

		// Does the searched word match ?
		if (strncmp(Pointer_String_Word, String_Temporary, Word_Length) == 0)
		{
			for (i = 0; i < Word_Length; i++) GridSetLetterFound(Row + i, Column - i);
			return 0;
		}
	}

	// Can the word be compared to the location's west
	if (Column + 1 >= Word_Length)
	{
		// Retrieve the characters of the word to compare
		Pointer_String_Temporary = String_Temporary;
		for (i = 0; i < Word_Length; i++)
		{
			*Pointer_String_Temporary = GridGetLetter(Row, Column - i);
			Pointer_String_Temporary++;
		}
		*Pointer_String_Temporary = 0;

		// Does the searched word match ?
		if (strncmp(Pointer_String_Word, String_Temporary, Word_Length) == 0)
		{
			for (i = 0; i < Word_Length; i++) GridSetLetterFound(Row, Column - i);
			return 0;
		}
	}

	// Can the word be compared to the location's north west
	if ((Row + 1 >= Word_Length) && (Column + 1 >= Word_Length))
	{
		// Retrieve the characters of the word to compare
		Pointer_String_Temporary = String_Temporary;
		for (i = 0; i < Word_Length; i++)
		{
			*Pointer_String_Temporary = GridGetLetter(Row - i, Column - i);
			Pointer_String_Temporary++;
		}
		*Pointer_String_Temporary = 0;

		// Does the searched word match ?
		if (strncmp(Pointer_String_Word, String_Temporary, Word_Length) == 0)
		{
			for (i = 0; i < Word_Length; i++) GridSetLetterFound(Row - i, Column - i);
			return 0;
		}
	}

	return -1;
}

int GridGetHiddenWord(char *Pointer_String_Hidden_Word)
{
	int Row, Column, Size = 0;
	TGridCell *Pointer_Cell;

	// Make sure to not exceed output string size
	for (Row = 0; Row < Grid_Rows_Count; Row++)
	{
		for (Column = 0; Column < Grid_Columns_Count; Column++)
		{
			// Cache cell access
			Pointer_Cell = &Grid_Cells[Row * Grid_Columns_Count + Column];

			// Is this letter part of the hidden word ?
			if (!Pointer_Cell->Is_Letter_Present_In_Word)
			{
				// Make sure there is enough room in the string buffer
				if (Size >= CONFIGURATION_HIDDEN_WORD_MAXIMUM_STRING_SIZE - 1) return -1;

				// Append the character to the string
				*Pointer_String_Hidden_Word = Pointer_Cell->Letter;
				Pointer_String_Hidden_Word++;
				Size++;
			}
		}
	}

	// Terminate string
	*Pointer_String_Hidden_Word = 0;

	return 0;
}

void GridDisplay(void)
{
	int Row, Column;

	for (Row = 0; Row < Grid_Rows_Count; Row++)
	{
		for (Column = 0; Column < Grid_Columns_Count; Column++) putchar(Grid_Cells[Row * Grid_Columns_Count + Column].Letter);
		putchar('\n');
	}
}
