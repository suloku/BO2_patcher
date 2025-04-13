#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <float.h>
#include <math.h>
#include <time.h>

#include "kain.h"
#include "BO2BigFile.h"
#include "Collectables.h"
#include "Enemies.h"
#include "levels.h"
#include "directory.h"
#include "patcher.h"
#include "modvalues.h"
#include "weapons.h"

#include "config.h"

//#define DATA_DUMP_MESSAGES
#define OUTPUT_TUNEDATA_FILES

extern NPC_tunedata defaultNPC;

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

    printf("::::::::::::::::::::::::::::::::::::::::\n");
    printf(": Blood Omen 2 Patcher v0.2            :\n");
    printf(":                          suloku 2025 :\n");
    printf("::::::::::::::::::::::::::::::::::::::::\n\n");

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

    append_directory(runPath, "config.ini", filename, sizeof(filename));
    Config config;
    init_config(&config);

    parse_ini(filename, &config);
    //print_config(&config);

    NPC_tunedata tempNPCconfig;
    init_NPC_config(&tempNPCconfig);

    WEAPON_tunedata tempWEAPONconfig;
    init_WEAPON_config(&tempWEAPONconfig);

    CHEST_tunedata tempCHESTconfig;
    init_CHEST_config(&tempCHESTconfig);

    if (get_config_CHEST(&config, &tempCHESTconfig, "col_lore", "levels1"))
    {
        printf("found");
        printf("\t%s %f %s\n", tempCHESTconfig.chestFile, tempCHESTconfig.lore, tempCHESTconfig.levels);
        printf("\t%s %f %s\n", defaultCHEST.chestFile, defaultCHEST.lore, defaultCHEST.levels);
    }

    if (get_config_WEAPON(&config, &tempWEAPONconfig, "lsword", "level1"))
    {
        printf("found");
        printf("\t%s %f %d %s\n", tempWEAPONconfig.weaponFile, tempWEAPONconfig.HP, tempWEAPONconfig.grabLoops, tempWEAPONconfig.levels);
        printf("\t%s %f %d %s\n", defaultWEAPON.weaponFile, defaultWEAPON.HP, defaultWEAPON.grabLoops, defaultWEAPON.levels);
    }

    if (get_config_NPC(&config, &tempNPCconfig, "VampireHunter", "level1"))
    {
        printf("found");
        printf("\t%s %f\n\t%s\n", tempNPCconfig.npcFile, tempNPCconfig.HitPoints, tempNPCconfig.levels);
    }

    free_config(&config);

    my_exit();

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
                    printf("\t\tWipe Chance successfully replaced at offset 0x%lX to %f\n", replace_offset, wipe_chance);
                } else
                {
                    printf("\t\tFailed to replace wipe chance.\n");
                }
                //Change Lore Per Particle
                replace_offset = current->data.fileOffset+lorePerParticle_offset;
                if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&lorePerParticle, sizeof(lorePerParticle)) == 0) {
                    printf("\t\tLore particles successfully replaced at offset 0x%lX to %f\n", replace_offset, lorePerParticle);
                } else {
                    printf("\t\tFailed to replace Lore particles.\n");
                }

                //Change number of button presses for Kain to get up after knockdown
                replace_offset = current->data.fileOffset+KainHitReactControls_offset;
                if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&kain_get_up_presses, sizeof(kain_get_up_presses)) == 0) {
                    printf("\t\tKnockdown button presses successfully replaced at offset 0x%lX to %d\n", replace_offset, kain_get_up_presses);
                } else {
                    printf("\t\tFailed to replace knockdown button presses.\n");
                }

                //Change Vampire Weapon Damage Multiplier
                replace_offset = current->data.fileOffset+vampireWeaponDamageMultiplier_offset;
                if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&VampireWeaponMultiplier, sizeof(VampireWeaponMultiplier)) == 0) {
                    printf("\t\tVampire Weapon Damage Multiplier successfully replaced at offset 0x%lX to %f\n", replace_offset, VampireWeaponMultiplier);
                } else {
                    printf("\t\tFailed to replace Vampire Weapon Damage Multiplier.\n");
                }

                //Change Max Lore Levels
                replace_offset = current->data.fileOffset+numLoreLevels_offset;
                if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&kain_MaxLoreLevels, sizeof(kain_MaxLoreLevels)) == 0) {
                    printf("\t\tMax Lore Levels successfully replaced at offset 0x%lX to %d\n", replace_offset, kain_MaxLoreLevels);
                } else {
                    printf("\t\tFailed to replace Max Lore Levels.\n");
                }

                //Change Kain's level data
                //Patch Lore
                for (j=0;j<KAIN_TOTAL_LEVELS;j++)
                {
                    int levelToChange = j;
                    float temp = 0;
                    replace_offset = current->data.fileOffset+get_kain_level_lore_offset(levelToChange);
                    read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                    if (temp != kainlevel_lore_values[levelToChange])
                    {
                        if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&kainlevel_lore_values[levelToChange], sizeof(kainlevel_lore_values[levelToChange])) == 0) {
                            if (kainlevel_lore_values[levelToChange] == FLT_MAX)
                                printf("\t\tLevel %d's Lore successfully replaced at offset 0x%lX to FLT_MAX\n", levelToChange, replace_offset);
                            else
                                printf("\t\tLevel %d's Lore successfully replaced at offset 0x%lX to %f\n", levelToChange, replace_offset, kainlevel_lore_values[levelToChange]);
                        } else {
                            printf("\t\tFailed to replace Level %d's Lore.\n", levelToChange);
                        }
                    }
                    //Patch HP
                    replace_offset = current->data.fileOffset+get_kain_level_hp_offset(levelToChange);
                    read_4_bytes_from_file(filename, replace_offset, ( unsigned char *)&temp);
                    if (temp != kainlevel_hp_values[levelToChange])
                    {
                        if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&kainlevel_hp_values[levelToChange], sizeof(kainlevel_hp_values[levelToChange])) == 0) {
                            printf("\t\tLevel %d's HP successfully replaced at offset 0x%lX to %f\n", levelToChange, replace_offset, kainlevel_hp_values[levelToChange]);
                        } else {
                            printf("\t\tFailed to replace Level %d's Lore.\n", levelToChange);
                        }
                    }
                }
                //Since we found the kain.tunedata file and modified it, we are done
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

        //TODO add some checks in this file to allow customization.
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

#ifdef OUTPUT_TUNEDATA_FILES
        // Debug code that dumps all *.tundeta file to "tunedatas.txt"
        FILE *file;
        // Open the file in append mode ("a"). This will create the file if it doesn't exist.
        file = fopen("tunedatas.txt", "a");
        // Check if the file was opened successfully
        if (file == NULL) {
            printf("Error opening file!\n");
            my_exit();
        }
        fprintf(file, "%s\n", get_filename(filename));
        while (current != NULL)
        {
            if (strcmp(current->data.fileType, "tunedata")==0)
            {
                fprintf(file, "\t%s.tunedata\n", current->data.fileName);
            }
            current = current->next;
        }
        // Close the file when done
        fclose(file);
#endif

        while (current != NULL)
        {
            if (strcmp(current->data.fileType, "tunedata")==0)
            {
                //Check for Red lore chests
                if (strcmp(current->data.fileName, "coll_lore")==0)
                {
                    printf("\t\tcoll_lore.tunedata\n");
                    replace_offset = current->data.fileOffset+collectable_lore_offset;
                    if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&new_RedChest_Lore, sizeof(new_RedChest_Lore)) == 0) {
                        printf("\t\t\tRed Chest lore successfully replaced at offset 0x%lX to %f\n", replace_offset, new_RedChest_Lore);
                    } else {
                        printf("\t\t\tFailed to replace Red Chest lore.\n");
                    }
                    current = current->next;
                    continue;
                }
                //Blue lore chests
                else if (strcmp(current->data.fileName, "coll_biglore")==0)
                {
                    printf("\t\tcoll_biglore.tunedata\n");
                    replace_offset = current->data.fileOffset+collectable_lore_offset;
                    if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&new_BlueChest_Lore, sizeof(new_BlueChest_Lore)) == 0) {
                        printf("\t\t\tBlue Chest lore successfully replaced at offset 0x%lX to %f\n", replace_offset, new_BlueChest_Lore);
                    } else {
                        printf("\t\t\tFailed to replace Blue Chest lore.\n");
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
                        printf("\t\t%s.tunedata\n", npcFilesToModNames[x]);

                        float test = 0;
                        float npc_hitpoints = 0;
                    //Get NPCs hitpoints
                        replace_offset = current->data.fileOffset+npc_HitPoints_offset;
                        test = 0;
                        read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&npc_hitpoints);
                        //Print this NPCs HP
#ifdef DATA_DUMP_MESSAGES
                        printf("\t\t\t%s.tunedata's HP: %f\n", current->data.fileName, npc_hitpoints);
#endif
                    //Replace Max Blood
                        replace_offset = current->data.fileOffset+npc_BloodSuckTunedata_normalMaxBlood_offset;
                        float MaxBlood = 0;
                        read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&MaxBlood);
                        //Print this NPCs Max Blood
#ifdef DATA_DUMP_MESSAGES
                        printf("\t\t\t%s.tunedata's Max Blood: %f\n", current->data.fileName, MaxBlood);
                        //Replace code
#endif
                    //Replace Max Stealth Blood
                        replace_offset = current->data.fileOffset+npc_BloodSuckTunedata_stealthKillMaxBlood_offset;
                        test = 0;
                        read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&test);
                        //Print this NPCs Max Stealth Blood
#ifdef DATA_DUMP_MESSAGES
                        printf("\t\t\t%s.tunedata's Max Stealh Blood: %f\n", current->data.fileName, test);
#endif
                    //Replace Bloodsuck Rate
                        replace_offset = current->data.fileOffset+npc_BloodSuckTunedata_healthSuckSpeed_offset;
                        float new_bloodsuck_rate = 25; //Default is 25 HP per second
                        if (MaxBlood < 51) {
                            new_bloodsuck_rate = MaxBlood/npc_bloodsukratio_0_50;
                        } else if (MaxBlood > 50 && MaxBlood < 76) {
                            new_bloodsuck_rate = MaxBlood/npc_bloodsukratio_51_75;
                        } else if (MaxBlood > 75 && MaxBlood < 126) {
                            new_bloodsuck_rate = MaxBlood/npc_bloodsukratio_76_125;
                        } else { //MaxBlood > 125
                            new_bloodsuck_rate = MaxBlood/npc_bloodsukratio_126plus;
                        }
                        test = 0;
                        read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&test);
                        //Print this NPCs Bloodsuck rate
#ifdef DATA_DUMP_MESSAGES
                        printf("\t\t\t%s.tunedata's Bloodsuck rate: %f hp per second\n", current->data.fileName, test);
#endif
                        //Replace code
                        if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&new_bloodsuck_rate, sizeof(new_bloodsuck_rate)) == 0) {
                            printf("\t\t\t%s.tunedata's Bloodsuck rate successfully replaced at offset 0x%lX to %f\n", current->data.fileName, replace_offset, new_bloodsuck_rate);
                        } else {
                            printf("\t\t\tFailed to replace %s.tunedata's Bloodsuck rate.\n", current->data.fileName);
                        }

                    //Replace Lore
                        replace_offset = current->data.fileOffset+npc_BloodSuckTunedata_maxLore_offset;
                        if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&new_npc_lore, sizeof(new_npc_lore)) == 0) {
                            printf("\t\t\t%s.tunedata's lore successfully replaced at offset 0x%lX to %f\n", current->data.fileName, replace_offset, new_npc_lore);
                        } else {
                            printf("\t\t\tFailed to replace %s.tunedata's lore.\n", current->data.fileName);
                        }

                    //Replace Crawl Hitpoints
                        replace_offset = current->data.fileOffset+npc_crawl_away_data_hitpoints_offset;
                        test = 0;
                        read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&test);
                        //Print this NPCs crawl hitpoints
#ifdef DATA_DUMP_MESSAGES
                        printf("\t\t\t%s.tunedata's crawl hitpoints: %f\n", current->data.fileName, test);
#endif
                        if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&new_npc_crawl_hitpoints, sizeof(new_npc_crawl_hitpoints)) == 0) {
                            printf("\t\t\t%s.tunedata's crawl hitpoints successfully replaced at offset 0x%lX to %f\n", current->data.fileName, replace_offset, new_npc_crawl_hitpoints);
                        } else {
                            printf("\t\t\tFailed to replace %s.tunedata's lore.\n", current->data.fileName);
                        }

                        break;//test next file
                    }
                }

                //Check for Weapon files inside the bigfile
                for (x=0;x<dropWeaponFilesToModNamesCount;x++)
                {
                    //If we find an npc file, make the modifications and skip to next file
                    if (strcmp(current->data.fileName, dropWeaponFilesToModNames[x])==0)
                    {
                        printf("\t\t%s.tunedata\n", dropWeaponFilesToModNames[x]);

                        //Update weapon parameters
                        //Change weapon's HP
                        replace_offset = current->data.fileOffset+weaponTune_weaponHP_offset;
                        float test = 0;
                        read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&test);
                        //Print this Weapon's HP
#ifdef DATA_DUMP_MESSAGES
                        printf("\t\t\t%s.tunedata's weapon HP: %f\n", current->data.fileName, test);
#endif
                        if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&new_npc_lore, sizeof(new_npc_lore)) == 0) {
                            printf("\t\t\t%s.tunedata's weapon HP successfully replaced at offset 0x%lX to %f\n", current->data.fileName, replace_offset, new_npc_lore);
                        } else {
                            printf("\t\t\tFailed to replace %s.tunedata's weapon HP.\n", current->data.fileName);
                        }

                        //Change number of grab loops
                        int32_t weaponTune_GrabLoopTimes = 0x40;
                    }
                    break;//test next file
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
    printf("\nPatching finished. Enjoy.");
    my_exit();
}

void my_exit()
{
    printf("\nPress a key and then Enter to continue...\n");
    getchar();  // Waits for a character + Enter
    exit(0);
}
