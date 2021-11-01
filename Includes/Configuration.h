/** @file Configuration.h
 * Gather all program configurable parameters.
 * @author Adrien RICCIARDI
 */
#ifndef H_CONFIGURATION_H
#define H_CONFIGURATION_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** The maximum size (including the terminating zero) of a word to search. */
#define CONFIGURATION_WORD_LIST_ITEM_MAXIMUM_STRING_SIZE 32

/** The grid containing the letters to search the words from won't be greater than this dimension. */
#define CONFIGURATION_GRID_MAXIMUM_SIZE CONFIGURATION_WORD_LIST_ITEM_MAXIMUM_STRING_SIZE

/** How many letters in the latin alphabet. */
#define CONFIGURATION_ALPHABET_LETTERS_COUNT 26

#endif
