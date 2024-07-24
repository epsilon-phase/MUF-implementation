#pragma once
#include "trie.h"
#include <stddef.h>
/**
 * Parse a string with protomuck style ansi markers. 
 *
 * Supported types:
 * 16 color strings such as
 * * ^red^
 * * ^blue^ etc
 * 
 * 256 color strings such as
 * * ^25^
 * * ^b25^
 * 
 * 24 bit color strings
 * * ^25:25:25^
 * * ^b25:25:25^
 * */
size_t parse_ansi(const char* input, char** output, size_t *written);
