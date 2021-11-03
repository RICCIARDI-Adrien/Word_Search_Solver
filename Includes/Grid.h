/** @file Grid.h
 * Allow simple access to grid content regardless of its size.
 * @author Adrien RICCIARDI
 */
#ifndef H_GRID_H
#define H_GRID_H

#include <Word_List.h>

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Load a grid content and the words to search from a file.
 * @param Pointer_String_File_Name The file name to load.
 * @param Pointer_Word_Lists On output, fill the word lists (one per alphabet letter).
 * @param Pointer_Rows_Count On output, contain the grid row dimension.
 * @param Pointer_Columns_Count On output, contain the grid column dimension.
 * @return -1 if an error occurred,
 * @return 0 on success.
 */
int GridLoadFromFile(char *Pointer_String_File_Name, TWordList *Pointer_Word_Lists, int *Pointer_Rows_Count, int *Pointer_Columns_Count);

/** Get the letter corresponding to a grid coordinate.
 * @param Row The row coordinate.
 * @param Column The column coordinate.
 * @return The corresponding letter (in range A-Z) or 0 if the provided coordinate is invalid.
 */
char GridGetLetter(int Row, int Column);

/** Try to find the specified word in every allowed direction starting from the provided coordinates.
 * @param Pointer_String_Word The word to search.
 * @param Row The vertical coordinate to start searching from.
 * @param Column The horizontal coordinate to start searching from.
 * @return -1 if the word could not be found,
 * @return 0 if the word has been found.
 */
int GridMatchWordWithPosition(char *Pointer_String_Word, int Row, int Column);

/** Display the grid content on the console, this is a debug function. */
void GridDisplay(void);

#endif
