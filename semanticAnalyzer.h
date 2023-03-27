#include "semanticAnalyzerDef.h"

// Function to add key into the table along with the entry number
extern int sym_tab_entry_add(char* key,sym_tab_entry* table[],type_exp temp);
// Checks if the key is there in the table
extern int sym_tab_entry_contains(char* key,sym_tab_entry* table[]);