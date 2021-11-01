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
 * @return -1 if an error occurred,
 * @return 0 on success.
 */
int GridLoadFromFile(char *Pointer_String_File_Name, TWordList *Pointer_Word_Lists);

#endif
