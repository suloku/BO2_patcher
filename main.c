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

Config config;
char *FilenameNoExtension;

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

    //Load configuration file
    append_directory(runPath, "config.ini", filename, sizeof(filename));
    init_config(&config);
    if (parse_ini(filename, &config) == -1)
    {
        my_exit();
    }
    //print_config(&config);

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

    //TODO: reimplement lore per particle checks
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
                if (FilenameNoExtension) free (FilenameNoExtension);
                FilenameNoExtension = remove_extension(get_filename(filename));
                get_config_KAIN(&config, &tempKAINconfig, FilenameNoExtension);

                printf("\tFound %s.tunedata. Applying config...\n", current->data.fileName);

                //Change wipe chance
                replace_offset = current->data.fileOffset+wipe_chance_offset;
                if (tempKAINconfig.wipe_chance != -1)
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
                    printf("\t\tWipe Chance:  keeping value in file (no setting found) %06.2f.\n", temp);
                }

                //Change Lore Per Particle
                replace_offset = current->data.fileOffset+lorePerParticle_offset;
                if (tempKAINconfig.lorePerParticle != -1)
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
                    printf("\t\tLore particles:  keeping value in file (no setting found) %06.2f.\n", temp);
                }

                //Change number of button presses for Kain to get up after knockdown
                replace_offset = current->data.fileOffset+KainHitReactControls_offset;
                if (tempKAINconfig.get_up_presses != -1)
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
                    printf("\t\tKnockdown button presses:  keeping value in file (no setting found) %03d.\n", temp);
                }

                //Change Vampire Weapon Damage Multiplier
                replace_offset = current->data.fileOffset+vampireWeaponDamageMultiplier_offset;
                if (tempKAINconfig.vampireWeaponMultiplier != -1)
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
                    printf("\t\tVampire Weapon Damage Multiplier: keeping value in file (no setting found) %06.2f.\n", temp);
                }

                //Change Max Lore Levels
                replace_offset = current->data.fileOffset+numLoreLevels_offset;
                if (tempKAINconfig.maxLoreLevels != -1)
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
                    printf("\t\tMax Lore Levels: keeping value in file (no setting found) %03d\n", temp);
                }

                //Change Kain's level data
                //Patch Lore
                for (j=0;j<KAIN_TOTAL_LEVELS;j++)
                {
                    replace_offset = current->data.fileOffset+get_kain_level_lore_offset(j);
                    if (tempKAINconfig.levels[j].lore != -1)
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
                        printf("\t\tKain's Level %d Lore:\tkeeping value in file (no setting found) %06.2f.\n", j, temp);
                    }


                    //Patch HP
                    replace_offset = current->data.fileOffset+get_kain_level_hp_offset(j);
                    if (tempKAINconfig.levels[j].hp != -1)
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
                        printf("\t\t\tKain's Level %d HP:\tkeeping value in file (no setting found) %06.2f\n", j, temp);
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
                        if (tempKAINconfig.weapons[j].first_attack_damage != -1)
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
                            printf("\t\t\t1st attack damage:\t\tkeeping value in file (no setting found) %06.2f.\n", temp);
                        }
                    }

                    //2nd attack
                    if (weapon_offsets[j].second_attack != 0)
                    {
                       replace_offset = current->data.fileOffset+weapon_offsets[j].second_attack;
                        if (tempKAINconfig.weapons[j].second_attack_damage != -1)
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
                            printf("\t\t\t2nd attack damage:\t\tkeeping value in file (no setting found) %06.2f.\n", temp);
                        }
                    }

                    //3rd attack
                    if (weapon_offsets[j].third_attack != 0)
                    {
                       replace_offset = current->data.fileOffset+weapon_offsets[j].third_attack;
                        if (tempKAINconfig.weapons[j].third_attack_damage != -1)
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
                            printf("\t\t\t3rd attack damage:\t\tkeeping value in file (no setting found) %06.2f.\n", temp);
                        }
                    }

                    //Ground Damage
                    if (weapon_offsets[j].ground != 0)
                    {
                       replace_offset = current->data.fileOffset+weapon_offsets[j].ground;
                        if (tempKAINconfig.weapons[j].ground_damage != -1)
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
                            printf("\t\t\tGround attack damage:\t\tkeeping value in file (no setting found) %06.2f.\n", temp);
                        }
                    }

                    //Grab Loop Damage
                    if (weapon_offsets[j].grab_loop != 0)
                    {
                       replace_offset = current->data.fileOffset+weapon_offsets[j].grab_loop;
                        if (tempKAINconfig.weapons[j].grab_loop_damage != -1)
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
                            printf("\t\t\tGrab Loop attack damage:\tkeeping value in file (no setting found) %06.2f.\n", temp);
                        }
                    }

                    //Grab Final Damage
                    if (weapon_offsets[j].grab_final != 0)
                    {
                       replace_offset = current->data.fileOffset+weapon_offsets[j].grab_final;
                        if (tempKAINconfig.weapons[j].grab_final_damage != -1)
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
                            printf("\t\t\tGrab Final attack damage:\tkeeping value in file (no setting found) %06.2f.\n", temp);
                        }
                    }

                    //Grab Throw Damage
                    if (weapon_offsets[j].grab_throw != 0)
                    {
                       replace_offset = current->data.fileOffset+weapon_offsets[j].grab_throw;
                        if (tempKAINconfig.weapons[j].grab_throw_damage != -1)
                        {
                            if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&tempKAINconfig.weapons[j].grab_throw_damage , sizeof(tempKAINconfig.weapons[j].grab_throw_damage )) == 0)
                            {
                                printf("\t\t\tGrab Throw attack damage:\t\treplaced at offset 0x%lX to %06.2f\n", replace_offset, tempKAINconfig.weapons[j].grab_throw_damage );
                            } else
                            {
                                printf("\t\t\tGrab Throw attack damage:\t\tFailed to replace.\n");
                            }
                        }
                        else
                        {
                            float temp = 0;
                            read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&temp);
                            printf("\t\t\tGrab Throw attack damage:\t\tkeeping value in file (no setting found) %06.2f.\n", temp);
                        }
                    }

                    //Fury Damage
                    if (weapon_offsets[j].charge_move != 0)
                    {
                       replace_offset = current->data.fileOffset+weapon_offsets[j].charge_move;
                        if (tempKAINconfig.weapons[j].fury_damage != -1)
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
                            printf("\t\t\tFury attack damage:\t\tkeeping value in file (no setting found) %06.2f.\n", temp);
                        }
                    }

                    //Jump Damage
                    if (weapon_offsets[j].super_jump != 0)
                    {
                       replace_offset = current->data.fileOffset+weapon_offsets[j].super_jump;
                        if (tempKAINconfig.weapons[j].jump_damage != -1)
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
                            printf("\t\t\tJump attack damage:\t\tkeeping value in file (no setting found) %06.2f.\n", temp);
                        }
                    }

                    //Berserk Damage
                    if (weapon_offsets[j].speed_attack != 0)
                    {
                       replace_offset = current->data.fileOffset+weapon_offsets[j].speed_attack;
                        if (tempKAINconfig.weapons[j].berserk_damage != -1)
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
                            printf("\t\t\tBerserk attack damage:\t\tkeeping value in file (no setting found) %06.2f.\n", temp);
                        }
                    }

                    //Final Berserk Damage
                    if (weapon_offsets[j].last_speed_attack != 0)
                    {
                       replace_offset = current->data.fileOffset+weapon_offsets[j].last_speed_attack;
                        if (tempKAINconfig.weapons[j].lastberserk_damage != -1)
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
                            printf("\t\t\tBerserk final attack damage:\tkeeping value in file (no setting found) %06.2f.\n", temp);
                        }
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
                        printf("\t\t\tRed Chest lore successfully replaced at offset 0x%lX to %06.2f\n", replace_offset, new_RedChest_Lore);
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
                        printf("\t\t\tBlue Chest lore successfully replaced at offset 0x%lX to %06.2f\n", replace_offset, new_BlueChest_Lore);
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
                        printf("\t\t\t%s.tunedata's HP: %06.2f\n", current->data.fileName, npc_hitpoints);
#endif
                    //Replace Max Blood
                        replace_offset = current->data.fileOffset+npc_BloodSuckTunedata_normalMaxBlood_offset;
                        float MaxBlood = 0;
                        read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&MaxBlood);
                        //Print this NPCs Max Blood
#ifdef DATA_DUMP_MESSAGES
                        printf("\t\t\t%s.tunedata's Max Blood: %06.2f\n", current->data.fileName, MaxBlood);
                        //Replace code
#endif
                    //Replace Max Stealth Blood
                        replace_offset = current->data.fileOffset+npc_BloodSuckTunedata_stealthKillMaxBlood_offset;
                        test = 0;
                        read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&test);
                        //Print this NPCs Max Stealth Blood
#ifdef DATA_DUMP_MESSAGES
                        printf("\t\t\t%s.tunedata's Max Stealh Blood: %06.2f\n", current->data.fileName, test);
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
                        printf("\t\t\t%s.tunedata's Bloodsuck rate: %06.2f hp per second\n", current->data.fileName, test);
#endif
                        //Replace code
                        if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&new_bloodsuck_rate, sizeof(new_bloodsuck_rate)) == 0) {
                            printf("\t\t\t%s.tunedata's Bloodsuck rate successfully replaced at offset 0x%lX to %06.2f\n", current->data.fileName, replace_offset, new_bloodsuck_rate);
                        } else {
                            printf("\t\t\tFailed to replace %s.tunedata's Bloodsuck rate.\n", current->data.fileName);
                        }

                    //Replace Lore
                        replace_offset = current->data.fileOffset+npc_BloodSuckTunedata_maxLore_offset;
                        if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&new_npc_lore, sizeof(new_npc_lore)) == 0) {
                            printf("\t\t\t%s.tunedata's lore successfully replaced at offset 0x%lX to %06.2f\n", current->data.fileName, replace_offset, new_npc_lore);
                        } else {
                            printf("\t\t\tFailed to replace %s.tunedata's lore.\n", current->data.fileName);
                        }

                    //Replace Crawl Hitpoints
                        replace_offset = current->data.fileOffset+npc_crawl_away_data_hitpoints_offset;
                        test = 0;
                        read_4_bytes_from_file(filename, replace_offset, (unsigned char *)&test);
                        //Print this NPCs crawl hitpoints
#ifdef DATA_DUMP_MESSAGES
                        printf("\t\t\t%s.tunedata's crawl hitpoints: %06.2f\n", current->data.fileName, test);
#endif
                        if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&new_npc_crawl_hitpoints, sizeof(new_npc_crawl_hitpoints)) == 0) {
                            printf("\t\t\t%s.tunedata's crawl hitpoints successfully replaced at offset 0x%lX to %06.2f\n", current->data.fileName, replace_offset, new_npc_crawl_hitpoints);
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
                        printf("\t\t\t%s.tunedata's weapon HP: %06.2f\n", current->data.fileName, test);
#endif
                        if (replace_data_in_file(filename, replace_offset, (const unsigned char *)&new_npc_lore, sizeof(new_npc_lore)) == 0) {
                            printf("\t\t\t%s.tunedata's weapon HP successfully replaced at offset 0x%lX to %06.2f\n", current->data.fileName, replace_offset, new_npc_lore);
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
    free_config(&config);
    printf("\nPress a key and then Enter to continue...\n");
    getchar();  // Waits for a character + Enter
    exit(0);
}
