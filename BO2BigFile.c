#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "directory.h"
#include "BO2BigFile.h"

//#define DEBUG

// Function to search for a string and return its position in the symlist
// Returns position (0-based index) if found, -1 if not found or on error
// Params
//      file: opened BigFile
//      start_offset: offset in the BigFile where the symlist starts
//      search_string: string to search for
long find_string_position(FILE *file, long start_offset, const char *search_string) {

    // Seek to the start of the string list
    if (fseek(file, start_offset, SEEK_SET) != 0) {
        perror("Error seeking to offset");
        return -1;
    }

    const size_t BUFFER_SIZE = 1024;
    unsigned char *buffer = malloc(BUFFER_SIZE);
    if (buffer == NULL) {
        perror("Error allocating buffer");
        return -1;
    }

    size_t search_len = strlen(search_string);
    size_t current_string_start = 0;
    size_t bytes_read_total = 0;
    long current_position = 0;  // Tracks position in terms of string index
    long found_position = -1;

    while (!feof(file)) {
        size_t bytes_read = fread(buffer, 1, BUFFER_SIZE, file);
        bytes_read_total += bytes_read;

        for (size_t i = 0; i < bytes_read; i++) {
            if (buffer[i] == 0x00) {
                // Compare the current string
                size_t current_string_len = i - current_string_start;
                if (current_string_len == search_len) {
                    long file_pos = start_offset + bytes_read_total - bytes_read + current_string_start;
                    fseek(file, file_pos, SEEK_SET);
                    char *temp = malloc(current_string_len + 1);
                    if (temp) {
                        fread(temp, 1, current_string_len, file);
                        temp[current_string_len] = '\0';
                        if (strcmp(temp, search_string) == 0) {
                            found_position = current_position;
                            free(temp);
                            break;
                        }
                        free(temp);
                    }
                }
                current_string_start = i + 1;
                current_position++;  // Increment string position
            }
        }

        if (found_position != -1) {
            break;
        }

        if (bytes_read < BUFFER_SIZE) {
            break;
        }

        // Move back to process potential string split across buffer boundary
        fseek(file, start_offset + bytes_read_total - search_len - 1, SEEK_SET);
        current_string_start = 0;
    }

    free(buffer);
    return found_position;
}

// Function to get string at specific position
// Returns allocated string (must be freed by caller) or NULL if not found/error
// Expects FILE *file to be set at the start of the string list
char *read_string_at_position(FILE *file, size_t position) {
    if (!file) return NULL;

    int index = 0;
    int ch;

    long start_pos = ftell(file);  // Save initial position

    fseek(file, 0x24, SEEK_SET);

    // Move through strings to find the correct position
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\0') {
            index++;  // Move to the next string
            if (index == position) {
                break;
            }
        }
    }


    if (ch == EOF || index != position) return NULL;  // Position out of bounds

    //save string start position
    long str_start = ftell(file);
    int length = 0;
    // Find the length of the string
    while ((ch = fgetc(file)) != EOF) {
        length++;
        if (ch == '\0') {
            break;
        }
    }

    // Now, read the actual string
    // Allocate memory for the string
    char *result = (char *)malloc(length);
    if (!result) return NULL;

    // Read the string from the file
    fseek(file, str_start, SEEK_SET);
    fread(result, 1, length, file);
    result[length-1] = '\0';  // Null-terminate

    // Restore file position
    fseek(file, start_pos, SEEK_SET);

    return result;
}

// Function to create a new node with a specific name and age
BigFileEntryNode* createBigFileEntry(const char* fileType, uint32_t entryLength, uint32_t unknown1, int32_t nameIndex, const char * symlist, int32_t symListLength, int32_t fileLength, int32_t fileOffset, const char * fileName) {
    BigFileEntryNode* newNode = (BigFileEntryNode*)malloc(sizeof(BigFileEntryNode));
    if (!newNode) {
        printf("CreateBigFileEntry: Memory allocation for node failed!\n");
        exit(1);
    }

    // Allocate memory for the fileType string and copy it
    newNode->data.fileType = (char*)malloc(strlen(fileType) + 1);
    if (!newNode->data.fileType) {
        printf("CreateBigFileEntry: Memory allocation for fileType failed!\n");
        free(newNode);
        exit(1);
    }
    strcpy(newNode->data.fileType, fileType);

    //symlist
    newNode->data.symlist = (char*)malloc(strlen(symlist) + 1);
    if (!newNode->data.symlist) {
        printf("CreateBigFileEntry: Memory allocation for symlist failed!\n");
        free(newNode);
        exit(1);
    }
    strcpy(newNode->data.symlist, symlist);

    //fileName
    newNode->data.fileName = (char*)malloc(strlen(fileName) + 1);
    if (!newNode->data.fileName) {
        printf("CreateBigFileEntry: Memory allocation for symlist failed!\n");
        free(newNode);
        exit(1);
    }
    strcpy(newNode->data.fileName, fileName);

    //Save other variables
    newNode->data.entryLength = entryLength;
    newNode->data.unknown1 = unknown1;
    newNode->data.nameIndex = nameIndex;
    newNode->data.symListLength = symListLength;
    newNode->data.entryLength = entryLength;
    newNode->data.fileLength = fileLength;
    newNode->data.fileOffset = fileOffset;

    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

// Function to initialize the doubly linked list
BigFileEntryList* createBigFileEntryList() {
    BigFileEntryList* list = (BigFileEntryList*)malloc(sizeof(BigFileEntryList));
    if (!list) {
        printf("Memory allocation for BigFileEntryList failed!\n");
        exit(1);
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

// Function to append a node to the list
void appendBigFileEntry(BigFileEntryList* list, const char* fileType, uint32_t entryLength, uint32_t unknown1, int32_t nameIndex, const char * symlist, int32_t symListLenght, int32_t filelength, int32_t fileOffset, const char * fileName) {
    BigFileEntryNode* newNode = createBigFileEntry(fileType, entryLength, unknown1, nameIndex, symlist, symListLenght, filelength, fileOffset, fileName);

    if (list->head == NULL) {  // If list is empty
        list->head = list->tail = newNode;
    } else {
        list->tail->next = newNode;
        newNode->prev = list->tail;
        list->tail = newNode;
    }
    list->size++;  // Increment the size of the list
}

// Function to get the N-th node (0-based index)
BigFileEntryNode* getBigFileEntryAt(BigFileEntryList* list, int index) {
    // Check if the index is valid
    if (index < 0 || index >= list->size) {
        return NULL;  // Index is out of bounds
    }

    BigFileEntryNode* current = NULL;

    // Start from head if index is in the first half of the list
    if (index < list->size / 2) {
        current = list->head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
    } else {
        // Otherwise, start from tail if index is in the second half of the list
        current = list->tail;
        for (int i = list->size - 1; i > index; i--) {
            current = current->prev;
        }
    }

    return current;
}

// Function to print the list
void printBigFileEntryList(BigFileEntryList* list) {
/*    BigFileEntryNode* current = list->head;
    while (current != NULL) {
        printf("Name: %s, Type: %s, Size: %d bytes, Offset: 0x%X\n", current->data.fileName, current->data.fileType, current->data.fileLength, current->data.fileOffset);
        current = current->next;
    }
*/
    if (!list || !list->head) {
        printf("The list is empty.\n");
        return;
    }

    BigFileEntryNode* current = list->head;
    while (current != NULL) {
        printf("File Name: %s | Type: %s | Size: %d bytes\n",
               current->data.fileName ? current->data.fileName : "NULL",
               current->data.fileType ? current->data.fileType : "NULL",
               current->data.fileLength);
        current = current->next;
    }
}

// Function to free allocated memory
void freeBigFileEntryList(BigFileEntryList* list) {
    BigFileEntryNode* current = list->head;
    while (current != NULL) {
        BigFileEntryNode* temp = current;
        current = current->next;
        free(temp->data.fileName);
        free(temp->data.fileType);
        free(temp->data.symlist);
        free(temp);
    }
    free(list);
}

int BigFile_PopulateEntryList (BigFileEntryList* list, char* BigFilePath)
{
    struct BO2BigFileHeader BigFileHeader;
    const char goefile[] = "goefile";
    const char symlist[] = "symlist";

    FILE *file = fopen(BigFilePath, "rb");  // Open in binary read mode
    if (file == NULL) {
        printf("\nError opening %s", BigFilePath);
        freeBigFileEntryList(list);
        return -1;
    }

#ifdef DEBUG
    printf("\tOpened %s\n", BigFilePath);
#endif

    fseek(file, 0, SEEK_END); // seek to end of file
    long bigfilesize = ftell(file); // get current file pointer
    fseek(file, 0, SEEK_SET); // seek back to beginning of file
    // proceed with allocating memory and reading the file

    // Read the entire structure in one go
    size_t bytes_read = fread(&BigFileHeader, sizeof(struct BO2BigFileHeader), 1, file);
    if (bytes_read != 1) {
        fclose(file);
        if (feof(file)) {
            printf("\nError: File too small\n");
        } else {
            perror("\nError reading file");
        }
        freeBigFileEntryList(list);
        return -1;
    }

#ifdef DEBUG
    printf("\tHeader read\n");
#endif

    //Sanity checks
    if (strcmp(BigFileHeader.fileType, goefile) != 0)
    {
        // Could also mean there are symbols. Not 100% sure yet.
        printf("\nBigfile does not contain the 'goefile' header at expected position, and therefore cannot be read as a BO2 BigFile.");
        fclose(file);
        freeBigFileEntryList(list);
        return -1;
    }
    if (strcmp(BigFileHeader.mainsymlist, symlist) != 0)
    {
        // Could also mean there are symbols. Not 100% sure yet.
        printf("\nBigfile does not contain the 'symlist' header at expected position, and therefore cannot be read as a BO2 BigFile.");
        fclose(file);
        freeBigFileEntryList(list);
        return -1;
    }

#ifdef DEBUG
    printf("\tHeader checks OK. Total symbols: %d\n", BigFileHeader.total_symbols);
#endif

    fseek(file, 0, SEEK_SET);
    int mainSymListOffset  = 0x10;
    long nextEntryOffset = mainSymListOffset + BigFileHeader.mainSymListLength;
    long symListOffset = 0;

    char fileType[9];
    int32_t entryLength = 0;
    uint32_t unknown1 = 0;
    int32_t nameIndex = 0;
    char file_symlist[9]; //"symlist\0"
    int32_t symListLength = 0;
    int32_t fileLength = 0;
    long file_offset = 0;

    long entrycount = 0;

    //Go trough all files and locate the target file
    while (1)
    {
        if (nextEntryOffset >= bigfilesize)
        {
#ifdef DEBUG
            printf("\tReached end of file (Entry %ld, offset 0x%04lX)\n", entrycount, nextEntryOffset);
#endif
            break;
        }
#ifdef DEBUG
        printf("\tEntry %ld, offset 0x%04lX\n", entrycount, nextEntryOffset);
#endif
        fileType[0] = '\0';
        file_symlist[0] = '\0';

        fseek(file, nextEntryOffset, SEEK_SET);

        fread(&fileType, 1, 8, file);
        fileType[8] = '\0';
        fread(&entryLength, 1, 4, file);
        //Some files (i.e kain.big) are padded, so this detects when last entry was reached
        if (fileType[0]==0){
            entrycount--;
            break;
        }
        if (entryLength <= 0)
        {
            printf ("\n\nWhen parsing this file as the selected file type, one of the files contained has a negative length (Header offset 0x%lX). This generally indicates that the file is not of the selected type.\n\n", nextEntryOffset);
            fclose(file);
            freeBigFileEntryList(list);
            return -1;

        }

        nextEntryOffset = nextEntryOffset+entryLength;
#ifdef DEBUG
    printf("\t\tFiletype %s\n", fileType);
#endif

        fread(&unknown1, 1, 4, file);
        fread(&nameIndex, 1, 4, file);

        symListOffset = ftell(file);
        fread(&file_symlist, 1, 8, file);
        file_symlist[8] = '\0';
        if (strcmp(file_symlist, symlist) != 0)
        {
            //This also happens when reaching the last file, since the next entry may be empty
            //TODO check the next file and see if there's a valid entry next or this might be the last one.
 //           if (!entrycount) //first entry
 //           {
#ifdef DEBUG
                // Could also mean there are symbols. Not 100% sure yet.
                printf("\n\tCurrent entry (%ld) does not contain the 'symlist' header at expected position, and therefore cannot be read.\n", entrycount);
#endif
                fclose(file);
                freeBigFileEntryList(list);
                return -1;
 /*           }
            else
            {
#ifdef DEBUG
                // Assume previous entry was the last entry
                printf("\nFinished reading big file. Total entries: %d.\n", entrycount);
#endif
                break;
            }*/
        }
        fread(&symListLength, 1, 4, file);
        fseek(file, symListOffset+symListLength, SEEK_SET);
        fread(&fileLength, 1, 4, file);

        file_offset = ftell(file);
        file_offset += 15;
        file_offset &= 0xFFFFFF0;

        //fileOffset now points at the beggining of the current file and fileLength holds its size

        //Get the fileName
        char Lipsync_str[] = "Lipsync";
        if (strcmp(fileType, "lipsync")== 0)
        {
            appendBigFileEntry(list, fileType, entryLength, unknown1, nameIndex, symlist, symListLength, fileLength, file_offset, Lipsync_str);
#ifdef DEBUG
            printf("\t\tFilename %s\n", Lipsync_str);
#endif
        }
        else
        {
            char *str = read_string_at_position(file, nameIndex);
#ifdef DEBUG
            printf("\t\tFilename %s\n", str);
#endif
            //Append data to list
            if (str)
            {
                appendBigFileEntry(list, fileType, entryLength, unknown1, nameIndex, symlist, symListLength, fileLength, file_offset, str);
                free (str);
            }
        }

        entrycount++;
    }

    //printf("Scan finished. %ld\n", file_offset);
    fclose(file);
    return 1;
}

BigFileEntryNode* BigFile_findFileInList(BigFileEntryList* list, const char* filename, const char* filetype) {
    if (list->size == 0) return NULL; // If list is empty, return NULL

    BigFileEntryNode* curNode;

   // Standard forward search
    curNode = list->head;
    while (curNode != NULL)
    {
        if (strcmp(curNode->data.fileType, filetype) == 0)
        {
            if (strcmp(curNode->data.fileName, filename) == 0)
            {
                return curNode;
            }
        }

        curNode = curNode->next;
    }

    return NULL;  // If name is not found
}

long BigFile_FindFile(char* FileToFind, char* FileTypeToFind, char* BigFilePath)
{
    struct BO2BigFileHeader BigFileHeader;
    const char goefile[] = "goefile";
    const char symlist[] = "symlist";

    FILE *file = fopen(BigFilePath, "rb");  // Open in binary read mode
    if (file == NULL) {
        printf("\nError opening %s", BigFilePath);
        return -1;
    }

#ifdef DEBUG
    printf("\tOpened %s\n", BigFilePath);
#endif

    fseek(file, 0, SEEK_END); // seek to end of file
    long bigfilesize = ftell(file); // get current file pointer
    fseek(file, 0, SEEK_SET); // seek back to beginning of file
    // proceed with allocating memory and reading the file

    // Read the entire structure in one go
    size_t bytes_read = fread(&BigFileHeader, sizeof(struct BO2BigFileHeader), 1, file);
    if (bytes_read != 1) {
        fclose(file);
        if (feof(file)) {
            printf("\nError: File too small\n");
        } else {
            perror("\nError reading file");
        }
        return -1;
    }

#ifdef DEBUG
    printf("\tHeader read\n");
#endif

    //Sanity checks
    if (strcmp(BigFileHeader.fileType, goefile) != 0)
    {
        // Could also mean there are symbols. Not 100% sure yet.
        printf("\nBigfile does not contain the 'goefile' header at expected position, and therefore cannot be read as a BO2 BigFile.");
        fclose(file);
        return -1;
    }
    if (strcmp(BigFileHeader.mainsymlist, symlist) != 0)
    {
        // Could also mean there are symbols. Not 100% sure yet.
        printf("\nBigfile does not contain the 'symlist' header at expected position, and therefore cannot be read as a BO2 BigFile.");
        fclose(file);
        return -1;
    }

#ifdef DEBUG
    printf("\tHeader checks OK. Total symbols: %d\n", BigFileHeader.total_symbols);
#endif

    fseek(file, 0, SEEK_SET);
    int mainSymListOffset  = 0x10;
    long nextEntryOffset = mainSymListOffset + BigFileHeader.mainSymListLength;
    long symListOffset = 0;

    char fileType[9];
    int32_t entryLength = 0;
    uint32_t unknown1 = 0;
    int32_t nameIndex = 0;
    char file_symlist[9]; //"symlist\0"
    int32_t symListLength = 0;
    int32_t fileLength = 0;
    long file_offset = 0;

    long entrycount = 0;

    //Go trough all files and locate the target file
    while (1)
    {
#ifdef DEBUG
        printf("\tEntry %ld, offset 0x%04lX\n", entrycount, nextEntryOffset);
#endif
        fileType[0] = '\0';
        file_symlist[0] = '\0';

        fseek(file, nextEntryOffset, SEEK_SET);

        fread(&fileType, 1, 8, file);
        fileType[8] = '\0';
        fread(&entryLength, 1, 4, file);
        if (entryLength <= 0)
        {
            printf ("When parsing this file as the selected file type, one of the files contained has a negative length (Header offset 0x%lX). This generally indicates that the file is not of the selected type.", nextEntryOffset);
            break;

        }

        nextEntryOffset = nextEntryOffset+entryLength;
#ifdef DEBUG
    printf("\t\tFiletype %s\n", fileType);
#endif

        fread(&unknown1, 1, 4, file);
        fread(&nameIndex, 1, 4, file);

        symListOffset = ftell(file);
        fread(&file_symlist, 1, 8, file);
        file_symlist[8] = '\0';
        if (strcmp(file_symlist, symlist) != 0)
        {
            //This also happens when reaching the last file, since the next one may be empty
#ifdef DEBUG
            // Could also mean there are symbols. Not 100% sure yet.
            printf("\n\tCurrent entry (%ld) does not contain the 'symlist' header at expected position, and therefore cannot be read.\n", entrycount);
#endif
            fclose(file);
            return -1;
        }
        fread(&symListLength, 1, 4, file);
        fseek(file, symListOffset+symListLength, SEEK_SET);
        fread(&fileLength, 1, 4, file);

        file_offset = ftell(file);
        file_offset += 15;
        file_offset &= 0xFFFFFF0;

        //fileOffset now points at the beggining of the current file and fileLength holds its size

        if (nameIndex >= BigFileHeader.total_symbols)
        {
#ifdef DEBUG
            printf("\t\tCurrent entry symbol index (%d) is bigger than the total symbols (%d).", nameIndex, BigFileHeader.total_symbols);
#endif
            goto wrongType;
        }

        //Check if the file is the one we are looking for
        fseek(file, 0x24, SEEK_SET);
        char *str = read_string_at_position(file, nameIndex);
        if (str)
        {
#ifdef DEBUG
            printf("\t\tFound %s at position %d\n", str, nameIndex);
#endif
            if (strcmp(str, FileToFind) == 0)
            {
                free(str);  // Important: free the returned string
                if (strcmp(fileType, FileTypeToFind) != 0)
                    goto wrongType;
#ifdef DEBUG
                printf("\t\tFound file: offset 0x%lX, size %d\n", file_offset, fileLength);
#endif
                break;
            }
                free(str);  // Important: free the returned string

        }
        else
        {
            printf("\t\tFailed to get string at position %d\n", nameIndex);
        }
wrongType:
        if (file_offset+fileLength >= bigfilesize)
        {
            file_offset = -1;
            //printf ("\tCouldn't find %s.%s inside %s\n", FileToFind, FileTypeToFind, get_filename(BigFilePath));
            break;
        }
        entrycount++;
    }

    //printf("Scan finished. %ld\n", file_offset);
    fclose(file);
    return file_offset;
}
