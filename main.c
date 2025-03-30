#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

#include "kain.h"
#include "BO2BigFile.h"
#include "Collectables.h"
#include "Enemies.h"
#include "levels.h"
#include "directory.h"
#include "patcher.h"
#include "modvalues.h"

void my_exit();

int get_kain_level_lore_offset(int level)
{
    return kain_levels_offset + (level*4*2);
}
int get_kain_level_hp_offset(int level)
{
    return kain_levels_offset + (level*4*2) + 4;
}

int main(int argc, char *argv[]) {

//First get the path the file is run from
    if (argc > 0) {
        printf("Executable path: %s\n", argv[0]);
    } else {
        printf("Could not determine path.\n");
    }

    char runPath [PATH_MAX];
    char filename [PATH_MAX];
    strcpy (runPath, argv[0]);

    char *pos = strrchr(runPath, get_path_separator());
    if (pos != NULL) {
        //*pos = get_path_separator(); //this will put the null terminator here. you can also copy to another string if you want
        *(pos) = '\0'; //this will put the null terminator here. you can also copy to another string if you want
    }
    printf ("RunPath: %s\n", runPath);

    //Check correct path
    append_directory(runPath, "bo2.exe", filename, sizeof(filename));
    if (file_exists(filename)) {
        printf("File '%s' exists.\n", filename);
    } else {
        printf("File '%s' does not exist.\n", filename);
        return 0;
    }

    //Check that lore in chests and lorePerParticle are correctly divisible

    if (fmod(new_RedChest_Lore, lorePerParticle) != 0)
    {
        printf("RedChest Lore amount (%.2f) is not divisible by Lore Particles (%.2f)\n", new_RedChest_Lore, lorePerParticle);
        my_exit();
    }
    if (fmod(new_BlueChest_Lore, lorePerParticle) != 0)
    {
        printf("BlueChest Lore amount (%.2f) is not divisible by Lore Particles (%.2f)\n", new_BlueChest_Lore, lorePerParticle);
        my_exit();
    }

    long current_offset;
    long replace_offset;

    //Kain Bigfiles

    //printf ("kain: %i\n", kainbigfilecount);

    int i = 0;
    int j = 0;
    BigFileEntryList* entryList;
    entryList = createBigFileEntryList();

    for (i=0; i < kainbigfilecount; i++)
    {
        //Clear the entry list
        freeBigFileEntryList(entryList);
        entryList = createBigFileEntryList();

        append_directory(runPath, "Data", filename, sizeof(filename));
        append_directory(filename, kain_big_files[i], filename, sizeof(filename));
        printf("\nPatching file %s\n", filename);
        if (file_exists(filename)) {
            //printf("File '%s' exists.\n", filename);
        } else {
            printf("File '%s' does not exist. Skipping to next file...\n", filename);
            continue;
        }

        //Create the list of files in kainX.big
        printf("\tReading BigFile entries... ");
        int result = BigFile_PopulateEntryList(entryList, filename);
        if (result)
        {
            printf("%d entries read.\n", entryList->size);
        }
        else
        {
            printf("Couldn't populate BigFile entries list. Skipping file %s\n", get_filename(filename));
            continue;
        }

        //Search for kain.tunedata inside the bigfile
        BigFileEntryNode* current = entryList->head;
        while (current != NULL)
        {
            if (strcmp(current->data.fileType, "tunedata")==0 && strcmp(current->data.fileName, "kain")==0)
            {
                //Change wipe chance
                replace_offset = current->data.fileOffset+wipe_chance_offset;
                if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&wipe_chance, sizeof(wipe_chance)) == 0)
                {
                    printf("\t\tWipe Chance successfully replaced at offset 0x%lX\n", replace_offset);
                } else
                {
                    printf("\t\tFailed to replace wipe chance.\n");
                }
                //Change Lore Per Particle
                replace_offset = current->data.fileOffset+lorePerParticle_offset;
                if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&lorePerParticle, sizeof(lorePerParticle)) == 0) {
                    printf("\t\tLore particles successfully replaced at offset 0x%lX\n", replace_offset);
                } else {
                    printf("\t\tFailed to replace Lore particles.\n");
                }

                //Change Kain's level data
                //Patch max lore level for level 16 to prevent overflow bug
                int levelToChange = 16;
                replace_offset = current->data.fileOffset+get_kain_level_lore_offset(levelToChange);
                if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&kainlevel_lore_values[levelToChange], sizeof(kainlevel_lore_values[levelToChange])) == 0) {
                    printf("\t\tLevel %d's Lore successfully replaced at offset 0x%lX\n", levelToChange, replace_offset);
                } else {
                    printf("\t\tFailed to replace Level %d's Lore.\n", levelToChange);
                }
                //Patch level 16's HP to be 500 instead of 485
                levelToChange = 16;
                replace_offset = current->data.fileOffset+get_kain_level_hp_offset(levelToChange);
                if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&kainlevel_hp_values[levelToChange], sizeof(kainlevel_hp_values[levelToChange])) == 0) {
                    printf("\t\tLevel %d's HP successfully replaced at offset 0x%lX\n", levelToChange, replace_offset);
                } else {
                    printf("\t\tFailed to replace Level %d's Lore.\n", levelToChange);
                }
                break;
            }

            current = current->next;
        }

/*
//Old code that searched binary patterns
        //Locate kain.tunedata
        long targetFile_offset = BigFile_FindFile("kain", "tunedata", filename);
        //printf ("File found at: 0x%lX", targetFile_offset);
        if (targetFile_offset < 0)
        {
            printf("\n\tKain.tunedata not found inside %s\n", filename);
            my_exit();
        }
        else
        {
            printf ("   Kain.tunedata found at 0x%lX in %s\n", targetFile_offset, filename);

            //Make current_offset point to the beggining of kain.tunedata file
            current_offset = targetFile_offset;

            //Replace wipe chance
            replace_offset = current_offset+wipe_chance_offset;
            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&wipe_chance, sizeof(wipe_chance)) == 0) {
                printf("\tWipe Chance successfully replaced at offset 0x%lX (%ld bytes)\n", replace_offset, replace_offset);
            } else {
                printf("\tFailed to replace wipe data.\n");
            }

            //Patch max lore level for level 19 to prevent overflow bug when filling the lore bar at level 25
            replace_offset = current_offset+lorelevel_19_lore_offset;
            //No real benefits come after reaching level 19.
            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&max_lore_level, sizeof(max_lore_level)) == 0) {
                printf("\tLevel 19's Lore successfully replaced at offset 0x%lX (%ld bytes)\n", replace_offset, replace_offset);
            } else {
                printf("\tFailed to replace Level 19's Lore.\n");
            }

            //Patch how many particles appear per amount of lore in chests. Both chest's contents must be divisible.
            replace_offset = current_offset+lorePerParticle_offset;
            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&lorePerParticle, sizeof(lorePerParticle)) == 0) {
                printf("\tLore particles successfully replaced at offset 0x%lX (%ld bytes)\n", replace_offset, replace_offset);
            } else {
                printf("\tFailed to replace Lore particles.\n");
            }
*/
    }//end kain bigfiles

    printf("\nKain.tunedata patched in all kainX.big files.\n");

   //Collectables
   //Scan trough every game level, find the coll_lore.tunedata, coll_biglore.tunedata and coll_weapon.tunedata files and change the lore
    i = 0;
    for (i=0; i < levelBigFileCount; i++)
    {
        //Clear the entry list
        freeBigFileEntryList(entryList);
        entryList = createBigFileEntryList();

        append_directory(runPath, "Data", filename, sizeof(filename));
        append_directory(filename, levels_bigfile_names[i], filename, sizeof(filename));

        //sl02.big does not contain lore chest, only two chained prisoners
        //these two chained prisoners are mandatory to be sucked to get
        //6 lore and level up to level 1 as part of the tutorial.
        //Since we don't need to change any file inside sl02.big, skip it.
        if (strcmp(get_filename(filename), "sl02.big")==0)
        {
            printf("\tSkipping file '%s' (no changes needed).\n", get_filename(filename));
            continue;
        }

        printf("\nOpening file %s\n", filename);
        if (file_exists(filename)) {
            //printf("File '%s' exists.\n", filename);
        } else {
            printf("\tFile '%s' does not exist. Skipping to next file...\n", get_filename(filename));
            continue;
        }

        printf("\tReading BigFile entries... ");
        int result = BigFile_PopulateEntryList(entryList, filename);
        if (result)
        {
            printf("%d entries read.\n", entryList->size);
        }
        else
        {
            printf("Couldn't populate BigFile entries list. Skipping file %s\n", get_filename(filename));
            continue;
        }

        BigFileEntryNode* current = entryList->head;
        while (current != NULL)
        {
            if (strcmp(current->data.fileType, "tunedata")==0)
            {
                //Check for Red lore chests
                if (strcmp(current->data.fileName, "coll_lore")==0)
                {
                    replace_offset = current->data.fileOffset+collectable_lore_offset;
                    if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&new_RedChest_Lore, sizeof(new_RedChest_Lore)) == 0) {
                        printf("\t\tRed Chest lore successfully replaced at offset 0x%lX\n", replace_offset);
                    } else {
                        printf("\t\tFailed to replace Red Chest lore.\n");
                    }
                    current = current->next;
                    continue;
                }
                //Blue lore chests
                else if (strcmp(current->data.fileName, "coll_biglore")==0)
                {
                    replace_offset = current->data.fileOffset+collectable_lore_offset;
                    if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&new_BlueChest_Lore, sizeof(new_BlueChest_Lore)) == 0) {
                        printf("\t\tBlue Chest lore successfully replaced at offset 0x%lX\n", replace_offset);
                    } else {
                        printf("\t\tFailed to replace Blue Chest lore.\n");
                    }
                    current = current->next;
                    continue;
                }
                //Check for NPC files inside the bigfile
                int x = 0;
                for (x=0;x<npcFilesToModCount;x++)
                {
                    //If we find an npc file, make the modifications and skip to next file
                    if (strcmp(current->data.fileName, npcFilesToModNames[x])==0)
                    {
                        replace_offset = current->data.fileOffset+npc_lore_offset;
                        if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&new_npc_lore, sizeof(new_npc_lore)) == 0) {
                            printf("\t\t%s.tunedata's lore successfully replaced at offset 0x%lX\n", current->data.fileName, replace_offset);
                        } else {
                            printf("\t\tFailed to replace %s.tunedata's lore.\n", current->data.fileName);
                        }
                        replace_offset = current->data.fileOffset+npc_crawl_away_data_hitpoints_offset;
                        float test = 0;
                        read_4_bytes_from_file(filename, replace_offset, &test);
                        printf("\t\t%s.tunedata's crawl hitpoints: %f\n", current->data.fileName, test);
                        if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&new_npc_crawl_hitpoints, sizeof(new_npc_crawl_hitpoints)) == 0) {
                            printf("\t\t%s.tunedata's crawl hitpoints successfully replaced at offset 0x%lX\n", current->data.fileName, replace_offset);
                        } else {
                            printf("\t\tFailed to replace %s.tunedata's lore.\n", current->data.fileName);
                        }
                        break;
                    }
                }

/*      //This code prints out all .tunedata files bigger than 3000 bytes (npc files are around 6000 bytes)
                if (current->data.fileLength > 3000)
                {
                    float tunedata_lore = 0;
                    read_4_bytes_from_file(filename, current->data.fileOffset+npc_lore_offset, (unsigned char*)&tunedata_lore);
                    printf("\t\t\tFile Name: %s | Type: %s | Size: %d bytes | Lore: %f\n",
                       current->data.fileName ? current->data.fileName : "NULL",
                       current->data.fileType ? current->data.fileType : "NULL",
                       current->data.fileLength, tunedata_lore);
                }
*/
/*      //This code prints out only .tunedata files with filenames not present in the npcFileNames array
                int found = 0;
                for (int x = 0; x < npcFileCount; x++)
                {
                    if (strcmp(current->data.fileName, npcFileNames[x])==0)
                    {
                        found = 1;
                        break;
                    }
                }
                if (!found)
                {
                    float tunedata_lore = 0;
                    read_4_bytes_from_file(filename, current->data.fileOffset+npc_lore_offset, (unsigned char*)&tunedata_lore);
                    printf("\t\tFile Name: %s | Type: %s | Size: %d bytes | Lore: %f\n",
                       current->data.fileName ? current->data.fileName : "NULL",
                       current->data.fileType ? current->data.fileType : "NULL",
                       current->data.fileLength, tunedata_lore);
                    break;
                }
*/
            }

            current = current->next;
        }


/*
//Old binary pattern search code
        //Locate coll_lore.tunedata (Red Chest)
        long targetFile_offset = BigFile_FindFile("coll_lore", "tunedata", filename);
        if (targetFile_offset < 0)
        {
            //printf("\tCouldn't find coll_lore.tunedata inside %s\n", get_filename(filename));
        }
        else
        {
            printf ("   coll_lore.tunedata found at 0x%lX in %s\n", targetFile_offset, get_filename(filename));
            //Make current_offset point to the beggining of coll_lore.tunedata file
            current_offset = targetFile_offset;

            //Replace Red Chest Lore
            replace_offset = current_offset+collectable_lore_offset;
            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&new_RedChest_Lore, sizeof(new_RedChest_Lore)) == 0) {
                printf("\tRed Chest lore successfully replaced at offset 0x%lX (%ld bytes)\n", replace_offset, replace_offset);
            } else {
                printf("\tFailed to replace Red Chest lore.\n");
            }
        }
        //Locate coll_biglore.tunedata (Blue Chest)
        targetFile_offset = BigFile_FindFile("coll_biglore", "tunedata", filename);
        if (targetFile_offset < 0)
        {
            printf("   Couldn't find coll_biglore.tunedata inside %s\n", get_filename(filename));
            //goto NPC_Data;
        }
        else
        {
            printf ("   coll_biglore.tunedata found at 0x%lX in %s\n", targetFile_offset, get_filename(filename));
            //Make current_offset point to the beggining of coll_lore.tunedata file
            current_offset = targetFile_offset;

            //Replace Blue Chest Lore
            replace_offset = current_offset+collectable_lore_offset;
            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&new_BlueChest_Lore, sizeof(new_BlueChest_Lore)) == 0) {
                printf("\tBlue Chest lore successfully replaced at offset 0x%lX (%ld bytes)\n", replace_offset, replace_offset);
            } else {
                printf("\tFailed to replace Blue Chest lore.\n");
            }
        }
        //NPC DATA
        j = 0;
        //Go trough all NPC file
        for (j=0; j < npcFileCount; j++)
        {
            //printf("\tNPCDATA: filecount %d %s\n", npcFileCount, npcFileNames[j]);
            //Locate current npc file .tunedata
            targetFile_offset = BigFile_FindFile((char*)npcFileNames[j], "tunedata", filename);
            if (targetFile_offset < 0)
            {
                printf("   Couldn't find %s.tunedata inside %s\n", npcFileNames[j], get_filename(filename));
                continue;
            }
            else
            {
                printf ("   %s.tunedata found at 0x%lX in %s\n", npcFileNames[j], targetFile_offset, get_filename(filename));
                //Make current_offset point to the beggining of npc .tunedata file
                current_offset = targetFile_offset;

                //Replace Lore value
                replace_offset = current_offset+npc_lore_offset;
                if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&new_npc_lore, sizeof(new_npc_lore)) == 0) {
                    printf("\t%s's lore successfully replaced at offset 0x%lX (%ld bytes)\n",npcFileNames[j], replace_offset, replace_offset);
                } else {
                    printf("\tFailed to replace %s's lore.\n", npcFileNames[j]);
                }
            }
        }
*/
    }//Collectables level loop

    my_exit();
}

void my_exit()
{
    printf("\nPress a key and then Enter to continue...\n");
    getchar();  // Waits for a character + Enter
    exit(0);
}
