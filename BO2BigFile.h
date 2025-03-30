#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct BO2BigFileHeader {
    char fileType[8]; //"goefile\0"
    uint32_t mdrFileType1;
    uint32_t mdrFileType2;
    char mainsymlist[8]; //"symlist\0"
    int32_t mainSymListLength;
    uint32_t unknown1;
    int32_t total_symbols;
};

typedef struct BigFileEntry {
    char* fileType;
    int32_t entryLength;
    uint32_t unknown1;
    int32_t nameIndex;
    char* symlist; //"symlist\0"
    int32_t symListLength;
    int32_t fileLength;
    int32_t fileOffset;
    char* fileName;
}BigFileEntry;

// Define the structure for data in each entry in the BigFile
/*typedef struct {
    char* name;  // Pointer for variable-length string
    int age;
} BigFileEntry;
*/

// Define the structure for the doubly linked list node
typedef struct BigFileEntryNode {
    BigFileEntry data;       // The structure stored in each node
    struct BigFileEntryNode* next; // Pointer to the next node
    struct BigFileEntryNode* prev; // Pointer to the previous node
} BigFileEntryNode;

// Define the structure for the doubly linked list
typedef struct BigFileEntryList {
    BigFileEntryNode* head;        // Pointer to the head node
    BigFileEntryNode* tail;        // Pointer to the tail node
    int size;          // Number of elements in the list
} BigFileEntryList;

long BigFile_FindFile(char* FileToFind, char* FileTypeToFind, char* BigFilePath);

BigFileEntryList* createBigFileEntryList();
BigFileEntryNode* getBigFileEntryAt(BigFileEntryList* list, int index);
void printBigFileEntryList(BigFileEntryList* list);
void freeBigFileEntryList(BigFileEntryList* list);
int BigFile_PopulateEntryList (BigFileEntryList* list, char* BigFilePath);
BigFileEntryNode* BigFile_findFileInList(BigFileEntryList* list, const char* filename, const char* filetype);
