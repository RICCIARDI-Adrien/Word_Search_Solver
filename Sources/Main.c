/** @file Main.c
 * Word search solver entry point.
 * @author Adrien RICCIARDI
 */
#include <Grid.h>
#include <stdio.h>
#include <stdlib.h>
#include <Word_List.h>

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Store a list of words to find for each alphabet letter. */
static TWordList Main_Word_Lists[CONFIGURATION_ALPHABET_LETTERS_COUNT];

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
	if (GridLoadFromFile(argv[1], Main_Word_Lists) != 0)
	{
		printf("The specified file \"%s\" is invalid\n.", argv[1]);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
