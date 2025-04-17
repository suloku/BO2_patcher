#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <dirent.h>

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

#define MAX_FILES 100
#define MAX_FILENAME PATH_MAX

char runPath [PATH_MAX];
char filename [PATH_MAX];
char csvFilePath [PATH_MAX];
char configFilePath [PATH_MAX];

//#define DATA_DUMP_MESSAGES
//#define OUTPUT_TUNEDATA_FILES

Config config;
char *kainFilenameNoExtension;
char *levelFilenameNoExtension;

#define DUMP_MODE 0
#define PATCH_MODE 1

bool dumpDataCSV = true;
FILE *KainCsvFileptr;
FILE *npcCsvFileptr;
FILE *weaponCsvFileptr;
FILE *chestCsvFileptr;
#define CSV_FILE "bo2tuner_dump.csv"

int program_mode = PATCH_MODE;

void my_exit();

int get_kain_level_lore_offset(int level)
{
    return kain_levels_offset + (level*4*2);
}
int get_kain_level_hp_offset(int level)
{
    return kain_levels_offset + (level*4*2) + 4;
}

void clearScreen()
{
#ifdef _WIN32
        system("cls"); // Windows
    #else
        system("clear"); // Linux/Unix
    #endif
    return;
}

void displayMenu_1() {

    printf("::::::::::::::::::::::::::::::::::::::::\n");
    printf(": Blood Omen 2 Tuner v0.2              :\n");
    printf(":                          suloku 2025 :\n");
    printf("::::::::::::::::::::::::::::::::::::::::\n\n");

    printf("===== Menu =====\n");
    printf("1. Patch Mode\n");
    printf("2. Data Dump Mode\n");
    printf("3. Exit\n");
    printf("\nEnter your choice (1-3): ");
}

void displayMenu_2(char files[][MAX_FILENAME], int count) {

    printf("::::::::::::::::::::::::::::::::::::::::\n");
    printf(": Blood Omen 2 Tuner v0.2              :\n");
    printf(":                          suloku 2025 :\n");
    printf("::::::::::::::::::::::::::::::::::::::::\n\n");

    printf("===== Select .ini File =====\n");
    if (count == 0) {
        printf("\nNo .ini files found in bo2tuner_cfg directory.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, files[i]);
    }
    printf("%d. Exit\n", count + 1);
    printf("\nEnter your choice (1-%d): ", count + 1);
}

int get_valid_input(int max_choice) {
    int choice;
    char buffer[100];

    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &choice) == 1 && choice >= 1 && choice <= max_choice) {
                return choice;
            }
        }
        printf("Invalid input. Please enter a number between 1 and %d: ", max_choice);
    }
}

int list_ini_files(char files[][MAX_FILENAME], const char *dir_path) {
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    dir = opendir(dir_path);
    if (dir == NULL) {
        printf("Error: Could not open directory '%s'\n", dir_path);
        return 0;
    }

    while ((entry = readdir(dir)) != NULL && count < MAX_FILES) {
        // Check if the file ends with .ini
        if (strstr(entry->d_name, ".ini") != NULL) {
            strncpy(files[count], entry->d_name, MAX_FILENAME - 1);
            files[count][MAX_FILENAME - 1] = '\0';
            count++;
        }
    }

    closedir(dir);
    return count;
}



int select_ini_file(char *selected_file) {
    char files[MAX_FILES][MAX_FILENAME];
    char dir_path[MAX_FILENAME];
    int file_count;

    append_directory(runPath, "bo2tuner_cfg", dir_path, sizeof(dir_path));

    // List .ini files
    file_count = list_ini_files(files, dir_path);

    while (1) {
        clearScreen();
        displayMenu_2(files, file_count);

        if (file_count == 0) {
            printf("\nPress Enter to exit...");
            getchar();
            return 0; // No files found
        }

        int choice = get_valid_input(file_count + 1);

        clearScreen();

        if (choice == file_count + 1) {
            //printf("Exiting selection. No file selected.\n");
            return 0; // User chose to exit
        }

        // Store the selected filename
        strncpy(selected_file, files[choice - 1], MAX_FILENAME - 1);
        selected_file[MAX_FILENAME - 1] = '\0';
        printf("Selected file: %s\n", selected_file);
        return 1; // Successful selection
    }
}

int main(int argc, char *argv[]) {

    printf("::::::::::::::::::::::::::::::::::::::::\n");
    printf(": Blood Omen 2 Tuner v0.2              :\n");
    printf(":                          suloku 2025 :\n");
    printf("::::::::::::::::::::::::::::::::::::::::\n\n");

//First get the path the file is run from
    if (argc > 0) {
        printf("Executable path: %s\n", argv[0]);
    } else {
        printf("Could not determine path.\n");
    }

    strcpy (runPath, argv[0]);

    char *pos = strrchr(runPath, get_path_separator());
    if (pos != NULL) {
        //*pos = get_path_separator(); //this will put the null terminator here. you can also copy to another string if you want
        *(pos) = '\0'; //this will put the null terminator here. you can also copy to another string if you want
    }
    printf ("RunPath: %s\n", runPath);

/*
    MENU 1
*/
    int choice = 0;
    while (1) {
        clearScreen();
        displayMenu_1();
        choice = get_valid_input(3);

        clearScreen();

        switch (choice) {
            case 1:
                printf("Patch Mode selected.\n");
                program_mode = PATCH_MODE;
                break;
            case 2:
                printf("Dump Mode selected.\n");
                program_mode = DUMP_MODE;
                break;
            case 3:
                printf("Exiting program. Goodbye!\n");
                my_exit();
        }

        //printf("\nPress Enter to continue...");
        //getchar(); // Wait for user to press Enter

        if (choice) break;
    }

/*
    MENU 2
*/
    char selected_file[MAX_FILENAME] = {0};
    if (program_mode == PATCH_MODE)
    {
        if (select_ini_file(selected_file)) {
            printf("Tunning BO2 with %s\n", selected_file);
        } else {
            printf("No file was selected.\n");
            my_exit();
        }
    }

    clearScreen();
    printf("::::::::::::::::::::::::::::::::::::::::\n");
    printf(": Blood Omen 2 Tuner v0.2              :\n");
    printf(":                          suloku 2025 :\n");
    printf("::::::::::::::::::::::::::::::::::::::::\n\n");

    //Open csv file for data dump
    if (dumpDataCSV && program_mode == DUMP_MODE)
    {
        append_directory(runPath, "dump_kaindata.csv", csvFilePath, sizeof(csvFilePath));
        KainCsvFileptr = fopen(csvFilePath, "w");
        if (KainCsvFileptr == NULL)
        {
            printf("Could not open %s", csvFilePath);
            my_exit();
        }
        append_directory(runPath, "dump_chestdata.csv", csvFilePath, sizeof(csvFilePath));
        chestCsvFileptr = fopen(csvFilePath, "w");
        if (chestCsvFileptr == NULL)
        {
            printf("Could not open %s", csvFilePath);
            my_exit();
        }
        append_directory(runPath, "dump_npcdata.csv", csvFilePath, sizeof(csvFilePath));
        npcCsvFileptr = fopen(csvFilePath, "w");
        if (npcCsvFileptr == NULL)
        {
            printf("Could not open %s", csvFilePath);
            my_exit();
        }
        append_directory(runPath, "dump_weapondata.csv", csvFilePath, sizeof(csvFilePath));
        weaponCsvFileptr = fopen(csvFilePath, "w");
        if (weaponCsvFileptr == NULL)
        {
            printf("Could not open %s", csvFilePath);
            my_exit();
        }
    }

    if (program_mode == PATCH_MODE)
    {
        printf("Tunning BO2 with %s\n", selected_file);
        printf("\n   Press enter to start patching...");
        getchar();
    }

    //Load configuration file
    init_config(&config);
    if (program_mode == PATCH_MODE)
    {
        append_directory(runPath, "bo2tuner_cfg", configFilePath, sizeof(configFilePath));
        append_directory(configFilePath, selected_file, configFilePath, sizeof(configFilePath));
        printf("Loading %s\n", configFilePath);
        //getchar();

        if (parse_ini(configFilePath, &config) == -1)
        {
            my_exit();
        }
    //print_config(&config);
    }

    EXTRA_config EXTRA_cfg;
    init_EXTRA_config(&EXTRA_cfg);
    if (program_mode == PATCH_MODE) get_config_EXTRA(&config, &EXTRA_cfg);

    KAIN_tunedata tempKAINconfig;
    init_KAIN_config(&tempKAINconfig);

    NPC_tunedata tempNPCconfig;
    init_NPC_config(&tempNPCconfig);

    WEAPON_tunedata tempWEAPONconfig;
    init_WEAPON_config(&tempWEAPONconfig);

    CHEST_tunedata tempCHESTconfig;
    init_CHEST_config(&tempCHESTconfig);

    /*
    //Some test code for config loading
    if (get_config_KAIN(&config, &tempKAINconfig, "default"))
    {
        printf("\tKain: %s Presses:%d Multi:%06.2f Level 5 HP:%06.2f ReaverDamage:%06.2f\n", tempKAINconfig.kainFile, tempKAINconfig.get_up_presses, tempKAINconfig.vampireWeaponMultiplier, tempKAINconfig.levels[5].hp, tempKAINconfig.weapons[KAINWEAPON_SOULREAVER].lastberserk_damage);
    }

    if (get_config_CHEST(&config, &tempCHESTconfig, "col_lore", "levels1"))
    {
        printf("\tChest: %s %06.2f %s\n", tempCHESTconfig.chestFile, tempCHESTconfig.lore, tempCHESTconfig.levels);
        //printf("\t%s %06.2f %s\n", defaultCHEST.chestFile, defaultCHEST.lore, defaultCHEST.levels);
    }
        if (get_config_CHEST(&config, &tempCHESTconfig, "col_lore", "levels1"))
    {
        printf("\tChest: %s %06.2f %s\n", tempCHESTconfig.chestFile, tempCHESTconfig.lore, tempCHESTconfig.levels);
        //printf("\t%s %06.2f %s\n", defaultCHEST.chestFile, defaultCHEST.lore, defaultCHEST.levels);
    }

    if (get_config_WEAPON(&config, &tempWEAPONconfig, "lsword", "level1"))
    {
        printf("\tWeapon: %s %06.2f %d %s\n", tempWEAPONconfig.weaponFile, tempWEAPONconfig.HP, tempWEAPONconfig.grabLoops, tempWEAPONconfig.levels);
        //printf("\t%s %06.2f %d %s\n", defaultWEAPON.weaponFile, defaultWEAPON.HP, defaultWEAPON.grabLoops, defaultWEAPON.levels);
    }

    if (get_config_NPC(&config, &tempNPCconfig, "VampireHunter", "level2"))
    {
        printf("\tNPC: %s %06.2f\n\t%s\n", tempNPCconfig.npcFile, tempNPCconfig.HitPoints, tempNPCconfig.levels);
    }

        if (get_config_NPC(&config, &tempNPCconfig, "VampireHunter", "level6"))
    {
        printf("\tNPC: %s %06.2f\n\t%s\n", tempNPCconfig.npcFile, tempNPCconfig.HitPoints, tempNPCconfig.levels);
    }

    if (get_config_NPC(&config, &tempNPCconfig, "VampireHunter", "level5"))
    {
        printf("\tNPC: %s %06.2f\n\t%s\n", tempNPCconfig.npcFile, tempNPCconfig.HitPoints, tempNPCconfig.levels);
    }

    my_exit();

    */

    //Check correct path
    append_directory(runPath, "bo2.exe", filename, sizeof(filename));
    if (file_exists(filename)) {
        printf("File '%s' exists.\n", filename);
    } else {
        printf("File '%s' does not exist.\n", filename);
        return 0;
    }

    //TODO: somehow reimplement lore per particle checks, currently relies on user.
    //Idea: go trough the config file searching for the key values and try to find some incompatibility
    /*
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
    */

    long replace_offset;

    //Kain Bigfiles

    //printf ("kain: %i\n", kainbigfilecount);

    int i = 0;
    int j = 0;
    BigFileEntryList* entryList;
    entryList = createBigFileEntryList();

    //Comment to skip kain*.big files for debugging
    //goto skipKain;

    if (dumpDataCSV && program_mode == DUMP_MODE)
    {
        fprintf(KainCsvFileptr, "File;Wipe Chance;Lore Particles;Get Up Presses;Vampire Weapon Multiplier;MaxLoreLevels;Lvl_00_Lore;Lvl_00_HP;Lvl_01_Lore;Lvl_01_HP;Lvl_02_Lore;Lvl_02_HP;Lvl_03_Lore;Lvl_03_HP;Lvl_04_Lore;Lvl_04_HP;Lvl_05_Lore;Lvl_05_HP;Lvl_06_Lore;Lvl_06_HP;Lvl_07_Lore;Lvl_07_HP;Lvl_08_Lore;Lvl_08_HP;Lvl_09_Lore;Lvl_09_HP;Lvl_10_Lore;Lvl_10_HP;Lvl_11_Lore;Lvl_11_HP;Lvl_12_Lore;Lvl_12_HP;Lvl_13_Lore;Lvl_13_HP;Lvl_14_Lore;Lvl_14_HP;Lvl_15_Lore;Lvl_15_HP;Lvl_16_Lore;Lvl_16_HP;Lvl_17_Lore;Lvl_17_HP;Lvl_18_Lore;Lvl_18_HP;Lvl_19_Lore;Lvl_19_HP;Lvl_20_Lore;Lvl_20_HP;Lvl_21_Lore;Lvl_21_HP;Lvl_22_Lore;Lvl_22_HP;Lvl_23_Lore;Lvl_23_HP;Lvl_24_Lore;Lvl_24_HP;Lvl_25_Lore;Lvl_25_HP;Claws 1st Attack;Claws 2nd Attack;Claws 3rd Attack;Claws Ground Attack;Claws Grab Loop;Claws Grab Final;Claws Grab Throw;Claws Fury Attack;Claws Jump Attack;Claws Berserk Attack;Claws Last Berserk Attack;Long Swords 1st Attack;Long Swords 2nd Attack;Long Swords 3rd Attack;Long Swords Ground Attack;Long Swords Grab Loop;Long Swords Grab Final;Long Swords Grab Throw;Long Swords Fury Attack;Long Swords Jump Attack;Long Swords Berserk Attack;Long Swords Last Berserk Attack;Daggers 1st Attack;Daggers 2nd Attack;Daggers 3rd Attack;Daggers Ground Attack;Daggers Grab Loop;Daggers Grab Final;Daggers Grab Throw;Daggers Fury Attack;Daggers Jump Attack;Daggers Berserk Attack;Daggers Last Berserk Attack;Short Axes 1st Attack;Short Axes 2nd Attack;Short Axes 3rd Attack;Short Axes Ground Attack;Short Axes Grab Loop;Short Axes Grab Final;Short Axes Grab Throw;Short Axes Fury Attack;Short Axes Jump Attack;Short Axes Berserk Attack;Short Axes Last Berserk Attack;Crossbow 1st Attack;Crossbow 2nd Attack;Crossbow 3rd Attack;Crossbow Ground Attack;Crossbow Grab Loop;Crossbow Grab Final;Crossbow Grab Throw;Crossbow Fury Attack;Crossbow Jump Attack;Crossbow Berserk Attack;Crossbow Last Berserk Attack;Double-Bladed Sword 1st Attack;Double-Bladed Sword 2nd Attack;Double-Bladed Sword 3rd Attack;Double-Bladed Sword Ground Attack;Double-Bladed Sword Grab Loop;Double-Bladed Sword Grab Final;Double-Bladed Sword Grab Throw;Double-Bladed Sword Fury Attack;Double-Bladed Sword Jump Attack;Double-Bladed Sword Berserk Attack;Double-Bladed Sword Last Berserk Attack;Broadsword/Long Axe 1st Attack;Broadsword/Long Axe 2nd Attack;Broadsword/Long Axe 3rd Attack;Broadsword/Long Axe Ground Attack;Broadsword/Long Axe Grab Loop;Broadsword/Long Axe Grab Final;Broadsword/Long Axe Grab Throw;Broadsword/Long Axe Fury Attack;Broadsword/Long Axe Jump Attack;Broadsword/Long Axe Berserk Attack;Broadsword/Long Axe Last Berserk Attack;Spiked Club/Scepter 1st Attack;Spiked Club/Scepter 2nd Attack;Spiked Club/Scepter 3rd Attack;Spiked Club/Scepter Ground Attack;Spiked Club/Scepter Grab Loop;Spiked Club/Scepter Grab Final;Spiked Club/Scepter Grab Throw;Spiked Club/Scepter Fury Attack;Spiked Club/Scepter Jump Attack;Spiked Club/Scepter Berserk Attack;Spiked Club/Scepter Last Berserk Attack;Mace 1st Attack;Mace 2nd Attack;Mace 3rd Attack;Mace Ground Attack;Mace Grab Loop;Mace Grab Final;Mace Grab Throw;Mace Fury Attack;Mace Jump Attack;Mace Berserk Attack;Mace Last Berserk Attack;Soul Reaver 1st Attack;Soul Reaver 2nd Attack;Soul Reaver 3rd Attack;Soul Reaver Ground Attack;Soul Reaver Grab Loop;Soul Reaver Grab Final;Soul Reaver Grab Throw;Soul Reaver Fury Attack;Soul Reaver Jump Attack;Soul Reaver Berserk Attack;Soul Reaver Last Berserk Attack;");
    }

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
                //We found a kain*.tunedata file. Load the config for it
                //note: missing values will be loaded from the "default" entry if found
                if (kainFilenameNoExtension) free (kainFilenameNoExtension);
                kainFilenameNoExtension = remove_extension(get_filename(filename));
                get_config_KAIN(&config, &tempKAINconfig, kainFilenameNoExtension);

                printf("\tFound %s.tunedata\n", current->data.fileName);
                if (dumpDataCSV && program_mode == DUMP_MODE)
                {
                    fprintf(KainCsvFileptr, "\n%s;", kain_big_files[i]);
                }

                //Change wipe chance
                replace_offset = current->data.fileOffset+wipe_chance_offset;
                if (tempKAINconfig.wipe_chance != -1 && program_mode == PATCH_MODE)
                {
                    if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempKAINconfig.wipe_chance, sizeof(tempKAINconfig.wipe_chance)) == 0)
                    {
                        printf("\t\tWipe Chance: replaced at offset 0x%lX to %06.2f\n", replace_offset, tempKAINconfig.wipe_chance);
                    } else
                    {
                        printf("\t\tWipe Chance: Failed to replace.\n");
                    }
                }
                else
                {
                    float temp = 0;
                    read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                    if (program_mode == PATCH_MODE) printf("\t\tWipe Chance:  keeping value in file (no setting found) %06.2f\n", temp);
                    else printf("\t\tWipe Chance:\t\t %06.2f\n", temp);
                    if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(KainCsvFileptr, "%f;", temp);
                }

                //Change Lore Per Particle
                replace_offset = current->data.fileOffset+lorePerParticle_offset;
                if (tempKAINconfig.lorePerParticle != -1 && program_mode == PATCH_MODE)
                {
                    if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempKAINconfig.lorePerParticle, sizeof(tempKAINconfig.lorePerParticle)) == 0)
                    {
                        printf("\t\tLore particles: replaced at offset 0x%lX to %06.2f\n", replace_offset, tempKAINconfig.lorePerParticle);
                    } else
                    {
                        printf("\t\tLore particles: Failed to replace.\n");
                    }
                }
                else
                {
                    float temp = 0;
                    read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                    if (program_mode == PATCH_MODE) printf("\t\tLore particles:  keeping value in file (no setting found) %06.2f\n", temp);
                    else printf("\t\tLore particles:\t\t %06.2f\n", temp);
                    if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(KainCsvFileptr, "%f;", temp);
                }

                //Change number of button presses for Kain to get up after knockdown
                replace_offset = current->data.fileOffset+KainHitReactControls_offset;
                if (tempKAINconfig.get_up_presses != -1 && program_mode == PATCH_MODE)
                {
                    if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempKAINconfig.get_up_presses, sizeof(tempKAINconfig.get_up_presses)) == 0)
                    {
                        printf("\t\tKnockdown button presses: replaced at offset 0x%lX to %03d\n", replace_offset, tempKAINconfig.get_up_presses);
                    } else
                    {
                        printf("\t\tKnockdown button presses: Failed to replace.\n");
                    }
                }
                else
                {
                    int32_t temp = 0;
                    read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                    if (program_mode == PATCH_MODE) printf("\t\tKnockdown button presses:  keeping value in file (no setting found) %03d\n", temp);
                    else printf("\t\tKnockdown button presses: %03d\n", temp);
                    if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(KainCsvFileptr, "%d;", temp);
                }

                //Change Vampire Weapon Damage Multiplier
                replace_offset = current->data.fileOffset+vampireWeaponDamageMultiplier_offset;
                if (tempKAINconfig.vampireWeaponMultiplier != -1 && program_mode == PATCH_MODE)
                {
                    if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempKAINconfig.vampireWeaponMultiplier, sizeof(tempKAINconfig.vampireWeaponMultiplier)) == 0)
                    {
                        printf("\t\tVampire Weapon Damage Multiplier: replaced at offset 0x%lX to %06.2f\n", replace_offset, tempKAINconfig.vampireWeaponMultiplier);
                    } else
                    {
                        printf("\t\tVampire Weapon Damage Multiplier: Failed to replace.\n");
                    }
                }
                else
                {
                    float temp = 0;
                    read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                    if (program_mode == PATCH_MODE) printf("\t\tVampire Weapon Damage Multiplier: keeping value in file (no setting found) %06.2f\n", temp);
                    else printf("\t\tVampire Weapon Damage Multiplier: %06.2f\n", temp);
                    if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(KainCsvFileptr, "%f;", temp);
                }

                //Change Max Lore Levels
                replace_offset = current->data.fileOffset+numLoreLevels_offset;
                if (tempKAINconfig.maxLoreLevels != -1 && program_mode == PATCH_MODE)
                {
                    if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempKAINconfig.maxLoreLevels, sizeof(tempKAINconfig.maxLoreLevels)) == 0)
                    {
                        printf("\t\tMax Lore Levels: replaced at offset 0x%lX to %03d\n", replace_offset, tempKAINconfig.maxLoreLevels);
                    } else
                    {
                        printf("\t\tMax Lore Levels: Failed to replace.\n");
                    }
                }
                else
                {
                    int32_t temp = 0;
                    read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                    if (program_mode == PATCH_MODE) printf("\t\tMax Lore Levels: keeping value in file (no setting found) %03d\n", temp);
                    else printf("\t\tMax Lore Levels:\t %03d\n", temp);
                    if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(KainCsvFileptr, "%d;", temp);
                }

                //Change Kain's level data
                //Patch Lore
                for (j=0;j<KAIN_TOTAL_LEVELS;j++)
                {
                    replace_offset = current->data.fileOffset+get_kain_level_lore_offset(j);
                    if (tempKAINconfig.levels[j].lore != -1 && program_mode == PATCH_MODE)
                    {
                        if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempKAINconfig.levels[j].lore, sizeof(tempKAINconfig.levels[j].lore)) == 0)
                        {
                            printf("\t\t\tKain's Level %d Lore:\treplaced at offset 0x%lX to %06.2f\n", j, replace_offset, tempKAINconfig.levels[j].lore);
                        } else
                        {
                            printf("\t\t\tKain's Level %d Lore:\tFailed to replace.\n", j);
                        }
                    }
                    else
                    {
                        float temp = 0;
                        read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                        if (program_mode == PATCH_MODE) printf("\t\t\tKain's Level %d Lore:\tkeeping value in file (no setting found) %06.2f\n", j, temp);
                        else printf("\t\t\tKain's Level %d Lore:\t %06.2f\n", j, temp);
                        if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(KainCsvFileptr, "%f;", temp);
                    }


                    //Patch HP
                    replace_offset = current->data.fileOffset+get_kain_level_hp_offset(j);
                    if (tempKAINconfig.levels[j].hp != -1 && program_mode == PATCH_MODE)
                    {
                        if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempKAINconfig.levels[j].hp, sizeof(tempKAINconfig.levels[j].hp)) == 0)
                        {
                            printf("\t\t\tKain's Level %d HP:\treplaced at offset 0x%lX to %06.2f\n", j, replace_offset, tempKAINconfig.levels[j].hp);
                        } else
                        {
                            printf("\t\t\tKain's Level %d HP:\tFailed to replace.\n", j);
                        }
                    }
                    else
                    {
                        float temp = 0;
                        read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                        if (program_mode == PATCH_MODE) printf("\t\t\tKain's Level %d HP:\tkeeping value in file (no setting found) %06.2f\n", j, temp);
                        else printf("\t\t\tKain's Level %d HP:\t %06.2f\n", j, temp);
                        if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(KainCsvFileptr, "%f;", temp);
                    }
                }

                //Kain Weapon Damage
                printf("\t\tKain Weapon Damage:\n");
                for (j=0; j<KAIN_TOTAL_WEAPONS; j++)
                {
                    printf("\t\t    %s\n", KainWeaponNames[j]);
                    //Go trough all weapons
                    //The offset being 0x00 indicates that weapon doesn't have this specific damage in Kain's damage table

                    //1st attack
                    if (weapon_offsets[j].first_attack != 0)
                    {
                        replace_offset = current->data.fileOffset+weapon_offsets[j].first_attack;
                        if (tempKAINconfig.weapons[j].first_attack_damage != -1 && program_mode == PATCH_MODE)
                        {
                            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempKAINconfig.weapons[j].first_attack_damage , sizeof(tempKAINconfig.weapons[j].first_attack_damage )) == 0)
                            {
                                printf("\t\t\t1st attack damage:\t\treplaced at offset 0x%lX to %06.2f\n", replace_offset, tempKAINconfig.weapons[j].first_attack_damage );
                            } else
                            {
                                printf("\t\t\t1st attack damage:\t\tFailed to replace.\n");
                            }
                        }
                        else
                        {
                            float temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            if (program_mode == PATCH_MODE) printf("\t\t\t1st attack damage:\t\tkeeping value in file (no setting found) %06.2f\n", temp);
                            else printf("\t\t\t1st attack damage:\t\t%06.2f\n", temp);
                            if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(KainCsvFileptr, "%f;", temp);

                        }
                    }else if (dumpDataCSV && program_mode == DUMP_MODE)
                    {
                        fprintf(KainCsvFileptr, "0;");
                    }

                    //2nd attack
                    if (weapon_offsets[j].second_attack != 0)
                    {
                       replace_offset = current->data.fileOffset+weapon_offsets[j].second_attack;
                        if (tempKAINconfig.weapons[j].second_attack_damage != -1 && program_mode == PATCH_MODE)
                        {
                            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempKAINconfig.weapons[j].second_attack_damage , sizeof(tempKAINconfig.weapons[j].second_attack_damage )) == 0)
                            {
                                printf("\t\t\t2nd attack damage:\t\treplaced at offset 0x%lX to %06.2f\n", replace_offset, tempKAINconfig.weapons[j].second_attack_damage );
                            } else
                            {
                                printf("\t\t\t2nd attack damage:\t\tFailed to replace.\n");
                            }
                        }
                        else
                        {
                            float temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            if (program_mode == PATCH_MODE) printf("\t\t\t2nd attack damage:\t\tkeeping value in file (no setting found) %06.2f\n", temp);
                            else printf("\t\t\t2nd attack damage:\t\t%06.2f\n", temp);
                            if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(KainCsvFileptr, "%f;", temp);
                        }
                    }else if (dumpDataCSV && program_mode == DUMP_MODE)
                    {
                        fprintf(KainCsvFileptr, "0;");
                    }

                    //3rd attack
                    if (weapon_offsets[j].third_attack != 0)
                    {
                       replace_offset = current->data.fileOffset+weapon_offsets[j].third_attack;
                        if (tempKAINconfig.weapons[j].third_attack_damage != -1 && program_mode == PATCH_MODE)
                        {
                            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempKAINconfig.weapons[j].third_attack_damage , sizeof(tempKAINconfig.weapons[j].third_attack_damage )) == 0)
                            {
                                printf("\t\t\t3rd attack damage:\t\treplaced at offset 0x%lX to %06.2f\n", replace_offset, tempKAINconfig.weapons[j].third_attack_damage );
                            } else
                            {
                                printf("\t\t\t3rd attack damage:\t\tFailed to replace.\n");
                            }
                        }
                        else
                        {
                            float temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            if (program_mode == PATCH_MODE) printf("\t\t\t3rd attack damage:\t\tkeeping value in file (no setting found) %06.2f\n", temp);
                            else printf("\t\t\t3rd attack damage:\t\t%06.2f\n", temp);
                            if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(KainCsvFileptr, "%f;", temp);
                        }
                    }else if (dumpDataCSV && program_mode == DUMP_MODE)
                    {
                        fprintf(KainCsvFileptr, "0;");
                    }

                    //Ground Damage
                    if (weapon_offsets[j].ground != 0)
                    {
                       replace_offset = current->data.fileOffset+weapon_offsets[j].ground;
                        if (tempKAINconfig.weapons[j].ground_damage != -1 && program_mode == PATCH_MODE)
                        {
                            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempKAINconfig.weapons[j].ground_damage , sizeof(tempKAINconfig.weapons[j].ground_damage )) == 0)
                            {
                                printf("\t\t\tGround attack damage:\t\treplaced at offset 0x%lX to %06.2f\n", replace_offset, tempKAINconfig.weapons[j].ground_damage );
                            } else
                            {
                                printf("\t\t\tGround attack damage:\t\tFailed to replace.\n");
                            }
                        }
                        else
                        {
                            float temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            if (program_mode == PATCH_MODE) printf("\t\t\tGround attack damage:\t\tkeeping value in file (no setting found) %06.2f\n", temp);
                            else printf("\t\t\tGround attack damage:\t\t%06.2f\n", temp);
                            if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(KainCsvFileptr, "%f;", temp);
                        }
                    } else if (dumpDataCSV && program_mode == DUMP_MODE)
                    {
                        fprintf(KainCsvFileptr, "0;");
                    }

                    //Grab Loop Damage
                    if (weapon_offsets[j].grab_loop != 0)
                    {
                       replace_offset = current->data.fileOffset+weapon_offsets[j].grab_loop;
                        if (tempKAINconfig.weapons[j].grab_loop_damage != -1 && program_mode == PATCH_MODE)
                        {
                            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempKAINconfig.weapons[j].grab_loop_damage , sizeof(tempKAINconfig.weapons[j].grab_loop_damage )) == 0)
                            {
                                printf("\t\t\tGrab Loop attack damage:\treplaced at offset 0x%lX to %06.2f\n", replace_offset, tempKAINconfig.weapons[j].grab_loop_damage );
                            } else
                            {
                                printf("\t\t\tGrab Loop attack damage:\tFailed to replace.\n");
                            }
                        }
                        else
                        {
                            float temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            if (program_mode == PATCH_MODE) printf("\t\t\tGrab Loop attack damage:\tkeeping value in file (no setting found) %06.2f\n", temp);
                            else printf("\t\t\tGrab Loop attack damage:\t%06.2f\n", temp);
                            if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(KainCsvFileptr, "%f;", temp);
                        }
                    } else if (dumpDataCSV && program_mode == DUMP_MODE)
                    {
                        fprintf(KainCsvFileptr, "0;");
                    }

                    //Grab Final Damage
                    if (weapon_offsets[j].grab_final != 0)
                    {
                       replace_offset = current->data.fileOffset+weapon_offsets[j].grab_final;
                        if (tempKAINconfig.weapons[j].grab_final_damage != -1 && program_mode == PATCH_MODE)
                        {
                            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempKAINconfig.weapons[j].grab_final_damage , sizeof(tempKAINconfig.weapons[j].grab_final_damage )) == 0)
                            {
                                printf("\t\t\tGrab Final attack damage:\treplaced at offset 0x%lX to %06.2f\n", replace_offset, tempKAINconfig.weapons[j].grab_final_damage );
                            } else
                            {
                                printf("\t\t\tGrab Final attack damage:\tFailed to replace.\n");
                            }
                        }
                        else
                        {
                            float temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            if (program_mode == PATCH_MODE) printf("\t\t\tGrab Final attack damage:\tkeeping value in file (no setting found) %06.2f\n", temp);
                            else printf("\t\t\tGrab Final attack damage:\t%06.2f\n", temp);
                            if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(KainCsvFileptr, "%f;", temp);
                        }
                    } else if (dumpDataCSV && program_mode == DUMP_MODE)
                    {
                        fprintf(KainCsvFileptr, "0;");
                    }

                    //Grab Throw Damage
                    if (weapon_offsets[j].grab_throw != 0)
                    {
                       replace_offset = current->data.fileOffset+weapon_offsets[j].grab_throw;
                        if (tempKAINconfig.weapons[j].grab_throw_damage != -1 && program_mode == PATCH_MODE)
                        {
                            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempKAINconfig.weapons[j].grab_throw_damage , sizeof(tempKAINconfig.weapons[j].grab_throw_damage )) == 0)
                            {
                                printf("\t\t\tGrab Throw attack damage:\treplaced at offset 0x%lX to %06.2f\n", replace_offset, tempKAINconfig.weapons[j].grab_throw_damage );
                            } else
                            {
                                printf("\t\t\tGrab Throw attack damage:\tFailed to replace.\n");
                            }
                        }
                        else
                        {
                            float temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            if (program_mode == PATCH_MODE) printf("\t\t\tGrab Throw attack damage:\tkeeping value in file (no setting found) %06.2f\n", temp);
                            else printf("\t\t\tGrab Throw attack damage:\t%06.2f\n", temp);
                            if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(KainCsvFileptr, "%f;", temp);
                        }
                    } else if (dumpDataCSV && program_mode == DUMP_MODE)
                    {
                        fprintf(KainCsvFileptr, "0;");
                    }

                    //Fury Damage
                    if (weapon_offsets[j].charge_move != 0)
                    {
                       replace_offset = current->data.fileOffset+weapon_offsets[j].charge_move;
                        if (tempKAINconfig.weapons[j].fury_damage != -1 && program_mode == PATCH_MODE)
                        {
                            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempKAINconfig.weapons[j].fury_damage , sizeof(tempKAINconfig.weapons[j].fury_damage )) == 0)
                            {
                                printf("\t\t\tFury attack damage:\t\treplaced at offset 0x%lX to %06.2f\n", replace_offset, tempKAINconfig.weapons[j].fury_damage );
                            } else
                            {
                                printf("\t\t\tFury attack damage:\t\tFailed to replace.\n");
                            }
                        }
                        else
                        {
                            float temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            if (program_mode == PATCH_MODE) printf("\t\t\tFury attack damage:\t\tkeeping value in file (no setting found) %06.2f\n", temp);
                            else printf("\t\t\tFury attack damage:\t\t%06.2f\n", temp);
                            if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(KainCsvFileptr, "%f;", temp);
                        }
                    } else if (dumpDataCSV && program_mode == DUMP_MODE)
                    {
                        fprintf(KainCsvFileptr, "0;");
                    }

                    //Jump Damage
                    if (weapon_offsets[j].super_jump != 0)
                    {
                       replace_offset = current->data.fileOffset+weapon_offsets[j].super_jump;
                        if (tempKAINconfig.weapons[j].jump_damage != -1 && program_mode == PATCH_MODE)
                        {
                            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempKAINconfig.weapons[j].jump_damage , sizeof(tempKAINconfig.weapons[j].jump_damage )) == 0)
                            {
                                printf("\t\t\tJump attack damage:\t\treplaced at offset 0x%lX to %06.2f\n", replace_offset, tempKAINconfig.weapons[j].jump_damage );
                            } else
                            {
                                printf("\t\t\tJump attack damage:\t\tFailed to replace.\n");
                            }
                        }
                        else
                        {
                            float temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            if (program_mode == PATCH_MODE) printf("\t\t\tJump attack damage:\t\tkeeping value in file (no setting found) %06.2f\n", temp);
                            else printf("\t\t\tJump attack damage:\t\t%06.2f\n", temp);
                            if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(KainCsvFileptr, "%f;", temp);
                        }
                    } else if (dumpDataCSV && program_mode == DUMP_MODE)
                    {
                        fprintf(KainCsvFileptr, "0;");
                    }

                    //Berserk Damage
                    if (weapon_offsets[j].speed_attack != 0)
                    {
                       replace_offset = current->data.fileOffset+weapon_offsets[j].speed_attack;
                        if (tempKAINconfig.weapons[j].berserk_damage != -1 && program_mode == PATCH_MODE)
                        {
                            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempKAINconfig.weapons[j].berserk_damage , sizeof(tempKAINconfig.weapons[j].berserk_damage )) == 0)
                            {
                                printf("\t\t\tBerserk attack damage:\t\treplaced at offset 0x%lX to %06.2f\n", replace_offset, tempKAINconfig.weapons[j].berserk_damage );
                            } else
                            {
                                printf("\t\t\tBerserk attack damage:\t\tFailed to replace.\n");
                            }
                        }
                        else
                        {
                            float temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            if (program_mode == PATCH_MODE) printf("\t\t\tBerserk attack damage:\t\tkeeping value in file (no setting found) %06.2f\n", temp);
                            else printf("\t\t\tBerserk attack damage:\t\t%06.2f\n", temp);
                            if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(KainCsvFileptr, "%f;", temp);
                        }
                    } else if (dumpDataCSV && program_mode == DUMP_MODE)
                    {
                        fprintf(KainCsvFileptr, "0;");
                    }

                    //Final Berserk Damage
                    if (weapon_offsets[j].last_speed_attack != 0)
                    {
                       replace_offset = current->data.fileOffset+weapon_offsets[j].last_speed_attack;
                        if (tempKAINconfig.weapons[j].lastberserk_damage != -1 && program_mode == PATCH_MODE)
                        {
                            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempKAINconfig.weapons[j].lastberserk_damage , sizeof(tempKAINconfig.weapons[j].lastberserk_damage )) == 0)
                            {
                                printf("\t\t\tBerserk final attack damage:\treplaced at offset 0x%lX to %06.2f\n", replace_offset, tempKAINconfig.weapons[j].lastberserk_damage );
                            } else
                            {
                                printf("\t\t\tBerserk final attack damage:\tFailed to replace.\n");
                            }
                        }
                        else
                        {
                            float temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            if (program_mode == PATCH_MODE) printf("\t\t\tBerserk final attack damage:\tkeeping value in file (no setting found) %06.2f\n", temp);
                            else printf("\t\t\tBerserk final attack damage:\t%06.2f\n", temp);
                            if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(KainCsvFileptr, "%f;", temp);
                        }
                    } else if (dumpDataCSV && program_mode == DUMP_MODE)
                    {
                        fprintf(KainCsvFileptr, "0;");
                    }

                }//Weapon damage loop end

                //Since we found the kain*.tunedata file and modified it, we are done
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

//goto tag
skipKain:

    if (dumpDataCSV && program_mode == DUMP_MODE)
    {
        fprintf(npcCsvFileptr, "NPC;Level;HP;Crawl HP;Blood;Stealth Blood;Bloodsuck Rate;Lore;");
        fprintf(weaponCsvFileptr, "Weapon;Level;HP;Grab Attack #;");
        fprintf(chestCsvFileptr, "Chest;Level;Lore;\n");
    }

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
        /*
        //This is commented as now we rely on propper settings in the configuration file
        if (strcmp(get_filename(filename), "sl02.big")==0)
        {
            printf("\tSkipping file '%s' (no changes needed).\n", get_filename(filename));
            continue;
        }
        */

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
                //printf("\n\t%s.tunedata\n", current->data.fileName);
                //Check for Red lore chests
                if (strcmp(current->data.fileName, "coll_lore")==0)
                {

                    //We found a coll_lore.tunedata file. Load the config for this level
                    //note: missing values will be loaded from the "default" entry if found
                    if (levelFilenameNoExtension) free (levelFilenameNoExtension);
                    levelFilenameNoExtension = remove_extension(get_filename(filename));
                    get_config_CHEST(&config, &tempCHESTconfig, "coll_lore", levelFilenameNoExtension);

                    printf("\n\t\t%s.tunedata\n", current->data.fileName);

                    //Change collectible lore
                    replace_offset = current->data.fileOffset+collectable_lore_offset;
                    if ( strcmp(tempCHESTconfig.chestFile, "coll_lore") == 0 && tempCHESTconfig.lore != -1 && program_mode == PATCH_MODE)
                    {
                        if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempCHESTconfig.lore, sizeof(tempCHESTconfig.lore)) == 0)
                        {
                            printf("\t\t\tRed Chest lore:\treplaced at offset 0x%lX to %06.2f\n", replace_offset, tempCHESTconfig.lore);
                        } else
                        {
                            printf("\t\t\tRed Chest lore:\tFailed to replace.\n");
                        }
                    }
                    else
                    {
                        float temp = 0;
                        read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                        if (program_mode == PATCH_MODE) printf("\t\t\tRed Chest lore:\tkeeping value in file (no setting found) %06.2f\n", temp);
                        else printf("\t\t\tRed Chest lore:\t %06.2f\n", temp);
                        if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(chestCsvFileptr, "%s;%s;%f;\n", current->data.fileName, levelFilenameNoExtension, temp);
                    }

                    //Go to next file
                    current = current->next;
                    continue;
                }
                //Blue lore chests
                else if (strcmp(current->data.fileName, "coll_biglore")==0)
                {
                    //We found a coll_biglore.tunedata file. Load the config for this level
                    //note: missing values will be loaded from the "default" entry if found
                    if (levelFilenameNoExtension) free (levelFilenameNoExtension);
                    levelFilenameNoExtension = remove_extension(get_filename(filename));
                    get_config_CHEST(&config, &tempCHESTconfig, "coll_biglore", levelFilenameNoExtension);

                    printf("\n\t\t%s.tunedata\n", current->data.fileName);

                    //Change collectible lore
                    replace_offset = current->data.fileOffset+collectable_lore_offset;
                    if ( strcmp(tempCHESTconfig.chestFile, "coll_biglore") == 0 && tempCHESTconfig.lore != -1 && program_mode == PATCH_MODE)
                    {
                        if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempCHESTconfig.lore, sizeof(tempCHESTconfig.lore)) == 0)
                        {
                            printf("\t\t\tBlue Chest lore:\treplaced at offset 0x%lX to %06.2f\n", replace_offset, tempCHESTconfig.lore);
                        } else
                        {
                            printf("\t\t\tBlue Chest lore:\tFailed to replace.\n");
                        }
                    }
                    else
                    {
                        float temp = 0;
                        read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                        if (program_mode == PATCH_MODE) printf("\t\t\tBlue Chest lore:\tkeeping value in file (no setting found) %06.2f\n", temp);
                        else printf("\t\t\tBlue Chest lore:\t %06.2f\n", temp);
                        if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(chestCsvFileptr, "%s;%s;%f;\n", current->data.fileName, levelFilenameNoExtension, temp);
                    }

                    //Go to next file
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

                        //We found a NPC .tunedata file. Load the config for this npc and level if it exists
                        //note: missing values will be loaded from the "default" entry if found
                        if (levelFilenameNoExtension) free (levelFilenameNoExtension);
                        levelFilenameNoExtension = remove_extension(get_filename(filename));
                        get_config_NPC(&config, &tempNPCconfig, current->data.fileName, levelFilenameNoExtension);

                        printf("\n\t\t%s.tunedata\n", current->data.fileName);
                        if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(npcCsvFileptr, "\n%s;%s;", current->data.fileName, levelFilenameNoExtension);

                        //NPCs hitpoints
                        replace_offset = current->data.fileOffset+npc_HitPoints_offset;
                        if (tempNPCconfig.HitPoints != -1 && program_mode == PATCH_MODE)
                        {
                            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempNPCconfig.HitPoints, sizeof(tempNPCconfig.HitPoints)) == 0)
                            {
                                printf("\t\t\tHP:\t\t replaced at offset 0x%lX to %06.2f\n", replace_offset, tempNPCconfig.HitPoints);
                            } else
                            {
                                printf("\t\t\tHP:\t\t Failed to replace.\n");
                            }
                        }
                        else
                        {
                            float temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            if (program_mode == PATCH_MODE) printf("\t\t\tHP:\t\t keeping value in file (no setting found) %06.2f\n", temp);
                            else printf("\t\t\tHP:\t\t %06.2f\n", temp);
                            if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(npcCsvFileptr, "%f;", temp);
                        }

                    //Replace Crawl Hitpoints
                        replace_offset = current->data.fileOffset+npc_crawl_away_data_hitpoints_offset;
                        if (tempNPCconfig.CrawlHitPoints != -1 && program_mode == PATCH_MODE)
                        {
                            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempNPCconfig.CrawlHitPoints, sizeof(tempNPCconfig.CrawlHitPoints)) == 0)
                            {
                                printf("\t\t\tCrawling HP:\t replaced at offset 0x%lX to %06.2f\n", replace_offset, tempNPCconfig.CrawlHitPoints);
                            } else
                            {
                                printf("\t\t\tCrawling HP:\t Failed to replace.\n");
                            }
                        }
                        else if (tempNPCconfig.CrawlHitPoints == -1 || program_mode == DUMP_MODE)
                        {
                            float temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            if (program_mode == PATCH_MODE) printf("\t\t\tCrawling HP:\t keeping value in file (no setting found) %06.2f\n", temp);
                            else printf("\t\t\tCrawling HP:\t %06.2f\n", temp);
                            if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(npcCsvFileptr, "%f;", temp);
                        }

                    //Replace Max Blood
                        replace_offset = current->data.fileOffset+npc_BloodSuckTunedata_normalMaxBlood_offset;
                        if (tempNPCconfig.normalMaxBlood != -1 && program_mode == PATCH_MODE)
                        {
                            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempNPCconfig.normalMaxBlood, sizeof(tempNPCconfig.normalMaxBlood)) == 0)
                            {
                                printf("\t\t\tBlood:\t\t replaced at offset 0x%lX to %06.2f\n", replace_offset, tempNPCconfig.normalMaxBlood);
                            } else
                            {
                                printf("\t\t\tBlood:\t\t Failed to replace.\n");
                            }
                        }
                        else
                        {
                            float temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            if (program_mode == PATCH_MODE) printf("\t\t\tBlood:\t\t keeping value in file (no setting found) %06.2f\n", temp);
                            else printf("\t\t\tBlood:\t\t %06.2f\n", temp);
                            if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(npcCsvFileptr, "%f;", temp);
                        }

                    //Replace Max Stealth Blood
                        replace_offset = current->data.fileOffset+npc_BloodSuckTunedata_stealthKillMaxBlood_offset;
                        if (tempNPCconfig.stealtKillMaxBlood != -1 && program_mode == PATCH_MODE)
                        {
                            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempNPCconfig.stealtKillMaxBlood, sizeof(tempNPCconfig.stealtKillMaxBlood)) == 0)
                            {
                                printf("\t\t\tStealth Blood:\t replaced at offset 0x%lX to %06.2f\n", replace_offset, tempNPCconfig.stealtKillMaxBlood);
                            } else
                            {
                                printf("\t\t\tStealth Blood:\t Failed to replace.\n");
                            }
                        }
                        else
                        {
                            float temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            if (program_mode == PATCH_MODE) printf("\t\t\tStealth Blood:\t keeping value in file (no setting found) %06.2f\n", temp);
                            else printf("\t\t\tStealth Blood:\t %06.2f\n", temp);
                            if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(npcCsvFileptr, "%f;", temp);
                        }

                    //Replace Bloodsuck Rate
                        replace_offset = current->data.fileOffset+npc_BloodSuckTunedata_healthSuckSpeed_offset;

                        //Get this NPC's current max Blood, since it should have already been updated at this point
                        float MaxBlood = 0;
                        read_4_bytes_from_file(filename, current->data.fileOffset+npc_BloodSuckTunedata_normalMaxBlood_offset, (unsigned char *)&MaxBlood);
                        float new_bloodsuck_rate = 25; //Default is 25 HP per second
                        if (MaxBlood < 51) {
                            new_bloodsuck_rate = MaxBlood/EXTRA_cfg.npc_bloodsuckratio_0_50;
                        } else if (MaxBlood > 50 && MaxBlood < 76) {
                            new_bloodsuck_rate = MaxBlood/EXTRA_cfg.npc_bloodsuckratio_51_75;
                        } else if (MaxBlood > 75 && MaxBlood < 126) {
                            new_bloodsuck_rate = MaxBlood/EXTRA_cfg.npc_bloodsuckratio_76_125;
                        } else if (MaxBlood > 125 && MaxBlood < 151) {
                            new_bloodsuck_rate = MaxBlood/EXTRA_cfg.npc_bloodsuckratio_126_150;
                        } else { //MaxBlood > 151
                            new_bloodsuck_rate = MaxBlood/EXTRA_cfg.npc_bloodsuckratio_151plus;
                        }

                        //Keep default value if we aren't using proportional rate or there is no value for this NPC
                        if (tempNPCconfig.healthSuckSpeed == -1 && !EXTRA_cfg.proportionalBloodSuck && program_mode == PATCH_MODE)
                        {
                            float temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            if (program_mode == PATCH_MODE) printf("\t\t\tBlood Suck Rate: keeping value in file (no setting found) %06.2f\n", temp);
                            else printf("\t\t\tBlood Suck Rate: %06.2f\n", temp);
                            if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(npcCsvFileptr, "%f;", temp);
                        }
                        else if ( program_mode == PATCH_MODE) //We have to replace the blood suck rate
                        {
                            //Use BSR specified in config file if appropiate
                            if (!EXTRA_cfg.proportionalBloodSuck)
                            {
                                new_bloodsuck_rate = tempNPCconfig.healthSuckSpeed;
                            }

                            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&new_bloodsuck_rate, sizeof(new_bloodsuck_rate)) == 0)
                            {
                                printf("\t\t\tBlood Suck Rate: replaced at offset 0x%lX to %06.2f\n", replace_offset, new_bloodsuck_rate);
                            } else
                            {
                                printf("\t\t\tBlood Suck Rate: Failed to replace.\n");
                            }
                        } else
                        {
                            float temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            if (program_mode == PATCH_MODE) printf("\t\t\tBlood Suck Rate: keeping value in file (no setting found) %06.2f\n", temp);
                            else printf("\t\t\tBlood Suck Rate: %06.2f\n", temp);
                            if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(npcCsvFileptr, "%f;", temp);
                        }

                    //Replace Lore
                        replace_offset = current->data.fileOffset+npc_BloodSuckTunedata_maxLore_offset;
                        if (tempNPCconfig.maxLore != -1 && program_mode == PATCH_MODE)
                        {
                            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempNPCconfig.maxLore, sizeof(tempNPCconfig.maxLore)) == 0)
                            {
                                printf("\t\t\tLore:\t\t replaced at offset 0x%lX to %06.2f\n", replace_offset, tempNPCconfig.maxLore);
                            } else
                            {
                                printf("\t\t\tLore:\t\t Failed to replace.\n");
                            }
                        }
                        else
                        {
                            float temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            if (program_mode == PATCH_MODE) printf("\t\t\tLore:\t\t keeping value in file (no setting found) %06.2f\n", temp);
                            else printf("\t\t\tLore:\t\t %06.2f\n", temp);
                            if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(npcCsvFileptr, "%f;", temp);
                        }

                        break;//test next file
                    }
                }

                //Check for Weapon files inside the bigfile
                for (x=0;x<dropWeaponFilesToModNamesCount;x++)
                {
                    //If we find an weapon file, make the modifications and skip to next file
                    if (strcmp(current->data.fileName, dropWeaponFilesToModNames[x])==0)
                    {
                        //We found a weapon .tunedata file. Load the config for this weapon and level if it exists
                        //note: missing values will be loaded from the "default" entry if found
                        if (levelFilenameNoExtension) free (levelFilenameNoExtension);
                        levelFilenameNoExtension = remove_extension(get_filename(filename));
                        get_config_WEAPON(&config, &tempWEAPONconfig, current->data.fileName, levelFilenameNoExtension);

                        printf("\n\t\t%s.tunedata\n", current->data.fileName);
                        if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(weaponCsvFileptr, "\n%s;%s;", current->data.fileName, levelFilenameNoExtension);

                        //Update weapon parameters
                        //Change weapon's HP
                        replace_offset = current->data.fileOffset+weaponTune_weaponHP_offset;
                        if (tempWEAPONconfig.HP != -1 && program_mode == PATCH_MODE)
                        {
                            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempWEAPONconfig.HP, sizeof(tempWEAPONconfig.HP)) == 0)
                            {
                                printf("\t\t\tHP:\t\t replaced at offset 0x%lX to %06.2f\n", replace_offset, tempWEAPONconfig.HP);
                            } else
                            {
                                printf("\t\t\tHP:\t\t Failed to replace.\n");
                            }
                        }
                        else
                        {
                            float temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            if (program_mode == PATCH_MODE) printf("\t\t\tHP:\t\t keeping value in file (no setting found) %06.2f\n", temp);
                            else printf("\t\t\tHP:\t\t %06.2f\n", temp);
                            if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(weaponCsvFileptr, "%f;", temp);
                        }

                        //Change number of grab loops
                        replace_offset = current->data.fileOffset+weaponTune_GrabLoopTimes_offset;
                        if (tempWEAPONconfig.HP != -1 && program_mode == PATCH_MODE)
                        {
                            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempWEAPONconfig.grabLoops, sizeof(tempWEAPONconfig.grabLoops)) == 0)
                            {
                                printf("\t\t\tGrab attack #:\t replaced at offset 0x%lX to %02d\n", replace_offset, tempWEAPONconfig.grabLoops);
                            } else
                            {
                                printf("\t\t\tGrab attack #:\t Failed to replace.\n");
                            }
                        }
                        else
                        {
                            int32_t temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            if (program_mode == PATCH_MODE) printf("\t\t\tGrab attack #:\t keeping value in file (no setting found) %02d.\n", temp);
                            else printf("\t\t\tGrab attack #:\t %03d\n", temp);
                            if (dumpDataCSV && program_mode == DUMP_MODE) fprintf(weaponCsvFileptr, "%d;", temp);
                        }
                    }
                }

/*      //This code prints out all .tunedata files bigger than 3000 bytes (npc files are around 6000 bytes)
                if (current->data.fileLength > 3000)
                {
                    float tunedata_lore = 0;
                    read_4_bytes_from_file(filename, current->data.fileOffset+npc_lore_offset, (unsigned char*)&tunedata_lore);
                    printf("\t\t\tFile Name: %s | Type: %s | Size: %d bytes | Lore: %06.2f\n",
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
                    printf("\t\tFile Name: %s | Type: %s | Size: %d bytes | Lore: %06.2f\n",
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
    if (KainCsvFileptr){ fclose(KainCsvFileptr);}
    if (npcCsvFileptr){ fclose(npcCsvFileptr);}
    if (weaponCsvFileptr){ fclose(weaponCsvFileptr);}
    if (chestCsvFileptr){ fclose(chestCsvFileptr);}
    free_config(&config);
    printf("\nPress a key and then Enter to continue...\n");
    getchar();  // Waits for a character + Enter
    exit(0);
}
