#include "config.h"

// Function to trim whitespace from both ends of a string
void trim_whitespace(char* str) {
    char* end;

    // Trim leading spaces
    while (isspace((unsigned char)*str)) str++;

    // Trim trailing spaces
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end + 1) = '\0';
}

// Function to initialize the Config structure
void init_config(Config* config) {
    config->section_count = 0;
    config->section_capacity = INITIAL_SECTION_CAPACITY;
    config->sections = malloc(config->section_capacity * sizeof(Section));
    if (!config->sections) {
        perror("Error allocating memory for sections");
        exit(EXIT_FAILURE);
    }
}

// Function to add a new section
Section* add_section(Config* config, const char* section_name) {
    // Resize the sections array if necessary
    if (config->section_count >= config->section_capacity) {
        config->section_capacity *= 2;
        config->sections = realloc(config->sections, config->section_capacity * sizeof(Section));
        if (!config->sections) {
            perror("Error reallocating memory for sections");
            exit(EXIT_FAILURE);
        }
    }

    // Add the new section
    Section* section = &config->sections[config->section_count++];
    strncpy(section->name, section_name, sizeof(section->name) - 1);
    section->name[sizeof(section->name) - 1] = '\0'; // Ensure null termination

    section->key_count = 0;
    section->key_capacity = INITIAL_KEY_CAPACITY;
    section->keys = malloc(section->key_capacity * sizeof(KeyValue));
    if (!section->keys) {
        perror("Error allocating memory for keys");
        exit(EXIT_FAILURE);
    }

    return section;
}

// Function to add a key-value pair to a section
void add_key_value(Section* section, const char* key, const char* value) {
    // Resize the keys array if necessary
    if (section->key_count >= section->key_capacity) {
        section->key_capacity *= 2;
        section->keys = realloc(section->keys, section->key_capacity * sizeof(KeyValue));
        if (!section->keys) {
            perror("Error reallocating memory for keys");
            exit(EXIT_FAILURE);
        }
    }

    // Add the key-value pair
    KeyValue* kv = &section->keys[section->key_count++];
    strncpy(kv->key, key, sizeof(kv->key) - 1);
    strncpy(kv->value, value, sizeof(kv->value) - 1);

    kv->key[sizeof(kv->key) - 1] = '\0'; // Ensure null termination
    kv->value[sizeof(kv->value) - 1] = '\0'; // Ensure null termination
}

// Function to parse the INI file
int parse_ini(const char* filename, Config* config) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening config file (%s)", filename);
        return -1;
    }

    char line[256];
    Section* current_section = NULL;

    while (fgets(line, sizeof(line), file)) {
        trim_whitespace(line);

        // Skip empty lines and comments
        if (line[0] == '\0' || line[0] == '#' || line[0] == ';') {
            continue;
        }

        // Check for section header
        if (line[0] == '[') {
            char* end = strchr(line, ']');
            if (end) {
                *end = '\0'; // Remove the closing bracket
                current_section = add_section(config, line + 1);
            }
        } else if (current_section) {
            // Parse key-value pair
            char* equals = strchr(line, '=');
            if (equals) {
                *equals = '\0'; // Split key and value
                char key[50];
                char value[100];
                strncpy(key, line, sizeof(key) - 1);
                strncpy(value, equals + 1, sizeof(value) - 1);

                key[sizeof(key) - 1] = '\0'; // Ensure null termination
                value[sizeof(value) - 1] = '\0'; // Ensure null termination

                trim_whitespace(key);
                trim_whitespace(value);

                add_key_value(current_section, key, value);
            }
        }
    }

    fclose(file);

    return 1;
}

// Function to print the parsed configuration
void print_config(const Config* config) {
    for (int i = 0; i < config->section_count; i++) {
        const Section* section = &config->sections[i];
        printf("Section: [%s]\n", section->name);
        for (int j = 0; j < section->key_count; j++) {
            const KeyValue* kv = &section->keys[j];
            printf("  %s = %s\n", kv->key, kv->value);
        }
    }
}

// Function to free the dynamically allocated memory
void free_config(Config* config) {
    for (int i = 0; i < config->section_count; i++) {
        free(config->sections[i].keys);
    }
    free(config->sections);
}

bool contains_word_exact(const char *input, const char *target) {

    if (!input || !target) {
        return false;
    }

    // Create a copy of input to avoid modifying the original
    char *input_copy = strdup(input);
    if (!input_copy) {
        return false; // Memory allocation failed
    }

    bool found = false;
    char *token = strtok(input_copy, ",");

    while (token != NULL) {
        // Remove leading/trailing spaces
        while (*token == ' ') token++;
        char *end = token + strlen(token) - 1;
        while (end > token && *end == ' ') end--;
        *(end + 1) = '\0';

        if (strcmp(token, target) == 0) {
            found = true;
            break;
        }
        token = strtok(NULL, ",");
    }

    free(input_copy);
    return found;
}

void trim_leading_spaces(char *str) {
    /*
     * Remove leading spaces from a string in-place.
     *
     * Args:
     *     str: The input string to modify.
     */
    if (!str || !*str) {
        return; // Handle NULL or empty string
    }

    // Find the first non-space character
    char *start = str;
    while (isspace((unsigned char)*start)) {
        start++;
    }

    // Shift string to remove leading spaces
    if (start != str) {
        memmove(str, start, strlen(start) + 1); // +1 for null terminator
    }
}

// Function to initialize NPC data.
// -1/NULL is used to identify a missing config, thus use the default config
// If the default config is being used, -1/NULL is used to not make any changes to the files
void init_NPC_config(NPC_tunedata* NPC)
{
    NPC->npcFile[0] = '\0';
    NPC->HitPoints = -1;
    NPC->CrawlHitPoints = -1;
    NPC->healthSuckSpeed = -1;;
    NPC->normalMaxBlood = -1;;
    NPC->stealtKillMaxBlood = -1;;
    NPC->maxLore = -1;
    NPC->levels[0] = '\0';
}

bool get_config_NPC(Config* config, NPC_tunedata* NPC, const char* npcName, const char* levelName) {
    bool found = false;
    bool found_default = false;

    //Initialize with values we can detect.
    NPC_tunedata defaultNPC;
    init_NPC_config(&defaultNPC);

    for (int i = 0; i < config->section_count; i++)
    {
        const Section* section = &config->sections[i];
        //printf("Section: [%s]\n", section->name);
        if (strcmp(section->name, "NPC") == 0)
        {
            init_NPC_config(NPC); //Reset before reading in new values
            for (int j = 0; j < section->key_count; j++)
            {
                const KeyValue* kv = &section->keys[j];
                trim_leading_spaces((char*)kv->value);

                if ( j == 0 && strcmp(kv->value, npcName)!= 0)
                {
                    break;//Skip section since name doesn't match
                }

                if (strcmp(kv->key, "npcFile") == 0)
                {
                    strcpy(NPC->npcFile, kv->value);
                }
                else if (strcmp(kv->key, "HitPoints") == 0)
                {
                    NPC->HitPoints = atof(kv->value);
                }
                else if (strcmp(kv->key, "CrawlHitPoints") == 0)
                {
                    NPC->CrawlHitPoints = atof(kv->value);
                }
                else if (strcmp(kv->key, "healthSuckSpeed") == 0)
                {
                    NPC->healthSuckSpeed = atof(kv->value);
                }
                else if (strcmp(kv->key, "normalMaxBlood") == 0)
                {
                    NPC->normalMaxBlood = atof(kv->value);
                }
                else if (strcmp(kv->key, "stealtKillMaxBlood") == 0)
                {
                    NPC->stealtKillMaxBlood = atof(kv->value);
                }
                else if (strcmp(kv->key, "maxLore") == 0)
                {
                    NPC->maxLore = atof(kv->value);
                }
                else if (strcmp(kv->key, "levels") == 0)
                {
                    strcpy(NPC->levels, kv->value);
                }
                else //Unrecognized key
                {
                    printf("\n\tUnrecognized key in config file:\n\t\t[%s] %s = %s\n\n", section->name, kv->key, kv->value);
                    printf("Can't proceed. Correct the configuration file and try again.");
                    exit(0);
                }
            }//For loop end

            //Check if this file is set as default, and copy it to the default structure
            if (contains_word_exact(NPC->levels, "default"))
            {
                //printf("\nFound default");
                defaultNPC = *NPC;
                found_default = true;
            }
            else if (contains_word_exact(NPC->levels, levelName))
            {
                //We found the NPC, break
                //printf("\nFound");
                found = true;

                //Fill any missing data with default data
                if (found_default)
                {
                    if (NPC->HitPoints == -1) NPC->HitPoints = defaultNPC.HitPoints;
                    if (NPC->CrawlHitPoints == -1) NPC->CrawlHitPoints = defaultNPC.CrawlHitPoints;
                    if (NPC->healthSuckSpeed == -1) NPC->healthSuckSpeed = defaultNPC.healthSuckSpeed;
                    if (NPC->normalMaxBlood == -1) NPC->normalMaxBlood = defaultNPC.normalMaxBlood;
                    if (NPC->stealtKillMaxBlood == -1) NPC->stealtKillMaxBlood = defaultNPC.stealtKillMaxBlood;
                    if (NPC->maxLore == -1) NPC->maxLore = defaultNPC.maxLore;
                }

                break;
            }


        }//Section check end

        if (found) break;

    }//Section loop end

    //If we didn't find the specified NPC for this level, but there was a default setting
    if (!found && found_default)
    {
        *NPC = defaultNPC;
        found = true;
    }
    return true;
}

// Function to initialize WEAPON data.
// -1/NULL is used to identify a missing config, thus use the default config
// If the default config is being used, -1/NULL is used to not make any changes to the files
void init_WEAPON_config(WEAPON_tunedata* WEAPON)
{
    WEAPON->weaponFile[0] = '\0';
    WEAPON->HP = -1;
    WEAPON->grabLoops = -1;
    WEAPON->levels[0] = '\0';
}

bool get_config_WEAPON(Config* config, WEAPON_tunedata* WEAPON, const char* weaponName, const char* levelName) {
    bool found = false;
    bool found_default = false;

    //Initialize with values we can detect
    WEAPON_tunedata defaultWEAPON;
    init_WEAPON_config(&defaultWEAPON);

    for (int i = 0; i < config->section_count; i++)
    {
        const Section* section = &config->sections[i];
        //printf("Section: [%s]\n", section->name);
        if (strcmp(section->name, "WEAPON") == 0)
        {
            init_WEAPON_config(WEAPON); //Reset before reading in new values
            for (int j = 0; j < section->key_count; j++)
            {
                const KeyValue* kv = &section->keys[j];
                trim_leading_spaces((char*)kv->value);

                if ( j == 0 && strcmp(kv->value, weaponName)!= 0)
                {
                    break;//Skip section since name doesn't match
                }

                if (strcmp(kv->key, "weaponFile") == 0)
                {
                    strcpy(WEAPON->weaponFile, kv->value);
                }
                else if (strcmp(kv->key, "HP") == 0)
                {
                    WEAPON->HP = atof(kv->value);
                }
                else if (strcmp(kv->key, "grabLoops") == 0)
                {
                    WEAPON->grabLoops = atoi(kv->value);
                }
                else if (strcmp(kv->key, "levels") == 0)
                {
                    strcpy(WEAPON->levels, kv->value);
                }
                else //Unrecognized key
                {
                    printf("\n\tUnrecognized key in config file:\n\t\t[%s] %s = %s\n\n", section->name, kv->key, kv->value);
                    printf("Can't proceed. Correct the configuration file and try again.");
                    exit(0);
                }
            }//For loop end

            //Check if this file is set as default, and copy it
            if (contains_word_exact(WEAPON->levels, "default"))
            {
                //printf("\nFound default\n");
                defaultWEAPON = *WEAPON;
                found_default = true;
            }
            else if (contains_word_exact(WEAPON->levels, levelName))
            {
                //We found the WEAPON for this level, break
                //printf("\nFound level\n");
                found = true;

                //Fill any missing data with default data
                if (found_default)
                {
                    if (WEAPON->HP == -1) WEAPON->HP = defaultWEAPON.HP;
                    if (WEAPON->grabLoops == -1) WEAPON->grabLoops = defaultWEAPON.grabLoops;
                }

                break;
            }


        }//Section check end

        if (found) break;

    }//Section loop end

    //If we didn't find the specified WEAPON for this level, but there was a default setting
    if (!found && found_default)
    {
        *WEAPON = defaultWEAPON;
        found = true;
    }
    return found;
}

// Function to initialize additional configuration file data.
// In this case there are some default values
void init_EXTRA_config(EXTRA_config* EXTRA_CFG)
{
    EXTRA_CFG->proportionalBloodSuck = false;
    EXTRA_CFG->npc_bloodsuckratio_0_50 = 1.0;
    EXTRA_CFG->npc_bloodsuckratio_51_75 = 1.25;
    EXTRA_CFG->npc_bloodsuckratio_76_125 = 1.5;
    EXTRA_CFG->npc_bloodsuckratio_126_150 = 1.75;
    EXTRA_CFG->npc_bloodsuckratio_151plus = 2.0;
}

//If there is no config data found, this will become the default configuration, so no need to return anything
void get_config_EXTRA(Config* config, EXTRA_config* EXTRA_CFG) {

    //Initialize with values we can detect
    init_EXTRA_config(EXTRA_CFG);

    for (int i = 0; i < config->section_count; i++)
    {
        const Section* section = &config->sections[i];
        //printf("Section: [%s]\n", section->name);
        if (strcmp(section->name, "EXTRA") == 0)
        {

            for (int j = 0; j < section->key_count; j++)
            {
                const KeyValue* kv = &section->keys[j];
                trim_leading_spaces((char*)kv->value);

                if (strcmp(kv->key, "proportionalBSR") == 0)
                {
                    if (strcmp(kv->value, "true") == 0)
                    {
                        EXTRA_CFG->proportionalBloodSuck = true;
                    }
                }
                else if (strcmp(kv->key, "pBSR_0_50") == 0)
                {
                    EXTRA_CFG->npc_bloodsuckratio_0_50 = atof(kv->value);
                }
                else if (strcmp(kv->key, "pBSR_51_75") == 0)
                {
                    EXTRA_CFG->npc_bloodsuckratio_51_75 = atof(kv->value);
                }
                else if (strcmp(kv->key, "pBSR_76_125") == 0)
                {
                    EXTRA_CFG->npc_bloodsuckratio_76_125 = atof(kv->value);
                }
                else if (strcmp(kv->key, "pBSR_126_150") == 0)
                {
                    EXTRA_CFG->npc_bloodsuckratio_126_150 = atof(kv->value);
                }
                else if (strcmp(kv->key, "pBSR_151_plus") == 0)
                {
                    EXTRA_CFG->npc_bloodsuckratio_151plus = atof(kv->value);
                }
                else //Unrecognized key
                {
                    printf("\n\tUnrecognized key in config file:\n\t\t[%s] %s = %s\n\n", section->name, kv->key, kv->value);
                    printf("Can't proceed. Correct the configuration file and try again.");
                    exit(0);
                }
            }//For loop end

            //Found EXTRA section and processed it
            break;
        }//Section check end

    }//Section loop end

    return;
}

// Function to initialize CHEST data.
// -1/NULL is used to identify a missing config, thus use the default config
// If the default config is being used, -1/NULL is used to not make any changes to the files
void init_CHEST_config(CHEST_tunedata* CHEST)
{
    CHEST->lore = -1;
    CHEST->levels[0] = '\0';
}

bool get_config_CHEST(Config* config, CHEST_tunedata* CHEST, const char* chestName, const char* levelName) {
    bool found = false;
    bool found_default = false;

    //Initialize with values we can detect
    CHEST_tunedata defaultCHEST;
    init_CHEST_config(&defaultCHEST);

    for (int i = 0; i < config->section_count; i++)
    {
        const Section* section = &config->sections[i];
        //printf("Section: [%s]\n", section->name);
        if (strcmp(section->name, "CHEST") == 0)
        {
            init_CHEST_config(CHEST); //Reset before reading in new values
            for (int j = 0; j < section->key_count; j++)
            {
                const KeyValue* kv = &section->keys[j];
                trim_leading_spaces((char*)kv->value);

                if ( j == 0 && strcmp(kv->value, chestName)!= 0)
                {
                    break;//Skip section since name doesn't match
                }

                if (strcmp(kv->key, "chestFile") == 0)
                {
                    strcpy(CHEST->chestFile, kv->value);
                }
                else if (strcmp(kv->key, "lore") == 0)
                {
                    CHEST->lore = atof(kv->value);
                }
                else if (strcmp(kv->key, "levels") == 0)
                {
                    strcpy(CHEST->levels, kv->value);
                }
                else //Unrecognized key
                {
                    printf("\n\tUnrecognized key in config file:\n\t\t[%s] %s = %s\n\n", section->name, kv->key, kv->value);
                    printf("Can't proceed. Correct the configuration file and try again.");
                    exit(0);
                }
            }//For loop end

            //Check if this file is set as default, and copy it
            if (contains_word_exact(CHEST->levels, "default"))
            {
                //printf("\nFound default\n");
                defaultCHEST = *CHEST;
                found_default = true;
            }
            else if (contains_word_exact(CHEST->levels, levelName))
            {
                //We found the CHEST for this level, break
                //printf("\nFound level\n");
                found = true;

                //Fill any missing data with default data
                if (found_default)
                {
                    if (CHEST->lore == -1) CHEST->lore = defaultCHEST.lore;
                }

                break;
            }


        }//Section check end

        if (found) break;

    }//Section loop end

    //If we didn't find the specified CHEST for this level, but there was a default setting
    if (!found && found_default)
    {
        *CHEST = defaultCHEST;
        found = true;
    }
    return found;
}

// Function to initialize KAIN data.
// -1/NULL is used to identify a missing config, thus use the default config
// If the default config is being used, -1/NULL is used to not make any changes to the files
void init_KAIN_config(KAIN_tunedata* KAIN)
{
    KAIN->kainFile[0] = '\0';
    KAIN->wipe_chance = -1;
    KAIN->lorePerParticle = -1;
    KAIN->get_up_presses = -1; // Stored as float per requirement
    KAIN->vampireWeaponMultiplier = -1;
    KAIN->maxLoreLevels = -1; // Stored as float per requirement
    for (int i = 0; i < KAIN_MAX_LEVELS; i++)
    {
        KAIN->levels[i].hp = -1;
        KAIN->levels[i].lore = -1;
    }
    for (int i = 0; i < KAIN_MAX_WEAPONS; i++)
    {
        KAIN->weapons[i].name[0] = '\0';
        KAIN->weapons[i].first_attack_damage = -1;
        KAIN->weapons[i].second_attack_damage = -1;
        KAIN->weapons[i].third_attack_damage = -1;
        KAIN->weapons[i].ground_damage = -1;
        KAIN->weapons[i].grab_loop_damage = -1;
        KAIN->weapons[i].grab_final_damage = -1;
        KAIN->weapons[i].fury_damage = -1;
        KAIN->weapons[i].jump_damage = -1; // Only used by hands
        KAIN->weapons[i].berserk_damage = -1;
        KAIN->weapons[i].lastberserk_damage = -1;
        KAIN->weapons[i].grab_throw_damage = -1; // Only used by hands
    }
}

bool get_config_KAIN(Config* config, KAIN_tunedata* KAIN, const char* kainFile) {
    bool found = false;
    bool found_default = false;

    //Initialize with values we can detect
    KAIN_tunedata defaultKAIN;
    init_KAIN_config(&defaultKAIN);

    for (int i = 0; i < config->section_count; i++)
    {
        const Section* section = &config->sections[i];
        //printf("Section: [%s]\n", section->name);
        if (strcmp(section->name, "KAIN") == 0)
        {
            init_KAIN_config(KAIN); //Reset before reading in new values
            for (int j = 0; j < section->key_count; j++)
            {
                const KeyValue* kv = &section->keys[j];
                trim_leading_spaces((char*)kv->value);

                if (strcmp(kv->key, "kainFile") == 0)
                {
                    strcpy(KAIN->kainFile, kv->value);
                }
                else if (strcmp(kv->key, "wipe_chance") == 0)
                {
                    KAIN->wipe_chance = atof(kv->value);
                }
                else if (strcmp(kv->key, "lorePerParticle") == 0)
                {
                    KAIN->lorePerParticle = atof(kv->value);
                }
                else if (strcmp(kv->key, "get_up_presses") == 0)
                {
                    KAIN->get_up_presses = atoi(kv->value);
                }
                else if (strcmp(kv->key, "VampireWeaponMultiplier") == 0)
                {
                    KAIN->vampireWeaponMultiplier = atof(kv->value);
                }
                else if (strcmp(kv->key, "MaxLorelevels") == 0)
                {
                    KAIN->maxLoreLevels = atoi(kv->value);
                }
                else if (strncmp(kv->key, "level_", 6) == 0)
                {
                    int level;
                    char type[5];
                    if (sscanf(kv->key + 6, "%02d_%4s", &level, type) == 2 && level >= 0 && level < KAIN_MAX_LEVELS)
                    {
                        if (strcmp(type, "LORE") == 0) {
                            KAIN->levels[level].lore = atof(kv->value);
                        } else if (strcmp(type, "HP") == 0) {
                            KAIN->levels[level].hp = atof(kv->value);
                        }
                    }
                }
                else if (strncmp(kv->key, "hands_", 6) == 0)
                {
                    char attack[20];
                    if (sscanf(kv->key + 6, "%s", attack) == 1 )
                    {
                        int cur_weapon = KAINWEAPON_HANDS;
                        if (strcmp(attack, "1stattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].first_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "2ndattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].second_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "3rdattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].third_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "ground_damage") == 0) {
                            KAIN->weapons[cur_weapon].ground_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_loop_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_loop_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_final_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_final_damage = atof(kv->value);
                        } else if (strcmp(attack, "fury_damage") == 0) {
                            KAIN->weapons[cur_weapon].fury_damage = atof(kv->value);
                        } else if (strcmp(attack, "berserk_damage") == 0) {
                            KAIN->weapons[cur_weapon].berserk_damage = atof(kv->value);
                        } else if (strcmp(attack, "lastberserk_damage") == 0) {
                            KAIN->weapons[cur_weapon].lastberserk_damage = atof(kv->value);
                        } else if (strcmp(attack, "jump_damage") == 0) {
                            KAIN->weapons[cur_weapon].jump_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_throw_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_throw_damage = atof(kv->value);
                        }
                    }
                }
                else if (strncmp(kv->key, "lsword_", 7) == 0)
                {
                    char attack[20];
                    if (sscanf(kv->key + 7, "%s", attack) == 1 )
                    {
                        int cur_weapon = KAINWEAPON_LSWORD;
                        if (strcmp(attack, "1stattack_damage") == 0) {
                            printf("\nLword %f\n", atof(kv->value));
                            KAIN->weapons[cur_weapon].first_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "2ndattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].second_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "3rdattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].third_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "ground_damage") == 0) {
                            KAIN->weapons[cur_weapon].ground_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_loop_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_loop_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_final_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_final_damage = atof(kv->value);
                        } else if (strcmp(attack, "fury_damage") == 0) {
                            KAIN->weapons[cur_weapon].fury_damage = atof(kv->value);
                        } else if (strcmp(attack, "berserk_damage") == 0) {
                            KAIN->weapons[cur_weapon].berserk_damage = atof(kv->value);
                        } else if (strcmp(attack, "lastberserk_damage") == 0) {
                            KAIN->weapons[cur_weapon].lastberserk_damage = atof(kv->value);
                        } else if (strcmp(attack, "jump_damage") == 0) {
                            KAIN->weapons[cur_weapon].jump_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_throw_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_throw_damage = atof(kv->value);
                        }
                    }
                } else if (strncmp(kv->key, "dagger_", 7) == 0)
                {
                    char attack[20];
                    if (sscanf(kv->key + 7, "%s", attack) == 1 )
                    {
                        int cur_weapon = KAINWEAPON_DAGGER;
                        if (strcmp(attack, "1stattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].first_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "2ndattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].second_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "3rdattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].third_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "ground_damage") == 0) {
                            KAIN->weapons[cur_weapon].ground_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_loop_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_loop_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_final_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_final_damage = atof(kv->value);
                        } else if (strcmp(attack, "fury_damage") == 0) {
                            KAIN->weapons[cur_weapon].fury_damage = atof(kv->value);
                        } else if (strcmp(attack, "berserk_damage") == 0) {
                            KAIN->weapons[cur_weapon].berserk_damage = atof(kv->value);
                        } else if (strcmp(attack, "lastberserk_damage") == 0) {
                            KAIN->weapons[cur_weapon].lastberserk_damage = atof(kv->value);
                        } else if (strcmp(attack, "jump_damage") == 0) {
                            KAIN->weapons[cur_weapon].jump_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_throw_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_throw_damage = atof(kv->value);
                        }
                    }
                } else if (strncmp(kv->key, "handax_", 7) == 0)
                {
                    char attack[20];
                    if (sscanf(kv->key + 7, "%s", attack) == 1 )
                    {
                        int cur_weapon = KAINWEAPON_HANDAX;
                        if (strcmp(attack, "1stattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].first_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "2ndattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].second_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "3rdattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].third_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "ground_damage") == 0) {
                            KAIN->weapons[cur_weapon].ground_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_loop_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_loop_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_final_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_final_damage = atof(kv->value);
                        } else if (strcmp(attack, "fury_damage") == 0) {
                            KAIN->weapons[cur_weapon].fury_damage = atof(kv->value);
                        } else if (strcmp(attack, "berserk_damage") == 0) {
                            KAIN->weapons[cur_weapon].berserk_damage = atof(kv->value);
                        } else if (strcmp(attack, "lastberserk_damage") == 0) {
                            KAIN->weapons[cur_weapon].lastberserk_damage = atof(kv->value);
                        } else if (strcmp(attack, "jump_damage") == 0) {
                            KAIN->weapons[cur_weapon].jump_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_throw_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_throw_damage = atof(kv->value);
                        }
                    }
                } else if (strncmp(kv->key, "crossbow_", 9) == 0)
                {
                    char attack[20];
                    if (sscanf(kv->key + 9, "%s", attack) == 1 )
                    {
                        int cur_weapon = KAINWEAPON_CROSSBOW;
                        if (strcmp(attack, "1stattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].first_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "2ndattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].second_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "3rdattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].third_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "ground_damage") == 0) {
                            KAIN->weapons[cur_weapon].ground_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_loop_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_loop_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_final_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_final_damage = atof(kv->value);
                        } else if (strcmp(attack, "fury_damage") == 0) {
                            KAIN->weapons[cur_weapon].fury_damage = atof(kv->value);
                        } else if (strcmp(attack, "berserk_damage") == 0) {
                            KAIN->weapons[cur_weapon].berserk_damage = atof(kv->value);
                        } else if (strcmp(attack, "lastberserk_damage") == 0) {
                            KAIN->weapons[cur_weapon].lastberserk_damage = atof(kv->value);
                        } else if (strcmp(attack, "jump_damage") == 0) {
                            KAIN->weapons[cur_weapon].jump_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_throw_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_throw_damage = atof(kv->value);
                        }
                    }
                } else if (strncmp(kv->key, "polearm_", 8) == 0)
                {
                    char attack[20];
                    if (sscanf(kv->key + 8, "%s", attack) == 1 )
                    {
                        int cur_weapon = KAINWEAPON_POLEARM;
                        if (strcmp(attack, "1stattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].first_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "2ndattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].second_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "3rdattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].third_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "ground_damage") == 0) {
                            KAIN->weapons[cur_weapon].ground_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_loop_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_loop_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_final_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_final_damage = atof(kv->value);
                        } else if (strcmp(attack, "fury_damage") == 0) {
                            KAIN->weapons[cur_weapon].fury_damage = atof(kv->value);
                        } else if (strcmp(attack, "berserk_damage") == 0) {
                            KAIN->weapons[cur_weapon].berserk_damage = atof(kv->value);
                        } else if (strcmp(attack, "lastberserk_damage") == 0) {
                            KAIN->weapons[cur_weapon].lastberserk_damage = atof(kv->value);
                        } else if (strcmp(attack, "jump_damage") == 0) {
                            KAIN->weapons[cur_weapon].jump_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_throw_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_throw_damage = atof(kv->value);
                        }
                    }
                } else if (strncmp(kv->key, "hsword_", 7) == 0)
                {
                    char attack[20];
                    if (sscanf(kv->key + 7, "%s", attack) == 1 )
                    {
                        int cur_weapon = KAINWEAPON_HSWORD;
                        if (strcmp(attack, "1stattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].first_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "2ndattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].second_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "3rdattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].third_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "ground_damage") == 0) {
                            KAIN->weapons[cur_weapon].ground_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_loop_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_loop_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_final_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_final_damage = atof(kv->value);
                        } else if (strcmp(attack, "fury_damage") == 0) {
                            KAIN->weapons[cur_weapon].fury_damage = atof(kv->value);
                        } else if (strcmp(attack, "berserk_damage") == 0) {
                            KAIN->weapons[cur_weapon].berserk_damage = atof(kv->value);
                        } else if (strcmp(attack, "lastberserk_damage") == 0) {
                            KAIN->weapons[cur_weapon].lastberserk_damage = atof(kv->value);
                        } else if (strcmp(attack, "jump_damage") == 0) {
                            KAIN->weapons[cur_weapon].jump_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_throw_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_throw_damage = atof(kv->value);
                        }
                    }
                } else if (strncmp(kv->key, "club_", 5) == 0)
                {
                    char attack[20];
                    if (sscanf(kv->key + 5, "%s", attack) == 1 )
                    {
                        int cur_weapon = KAINWEAPON_CLUB;
                        if (strcmp(attack, "1stattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].first_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "2ndattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].second_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "3rdattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].third_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "ground_damage") == 0) {
                            KAIN->weapons[cur_weapon].ground_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_loop_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_loop_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_final_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_final_damage = atof(kv->value);
                        } else if (strcmp(attack, "fury_damage") == 0) {
                            KAIN->weapons[cur_weapon].fury_damage = atof(kv->value);
                        } else if (strcmp(attack, "berserk_damage") == 0) {
                            KAIN->weapons[cur_weapon].berserk_damage = atof(kv->value);
                        } else if (strcmp(attack, "lastberserk_damage") == 0) {
                            KAIN->weapons[cur_weapon].lastberserk_damage = atof(kv->value);
                        } else if (strcmp(attack, "jump_damage") == 0) {
                            KAIN->weapons[cur_weapon].jump_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_throw_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_throw_damage = atof(kv->value);
                        }
                    }
                } else if (strncmp(kv->key, "mace_", 5) == 0)
                {
                    char attack[20];
                    if (sscanf(kv->key + 5, "%s", attack) == 1 )
                    {
                        int cur_weapon = KAINWEAPON_MACE;
                        if (strcmp(attack, "1stattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].first_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "2ndattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].second_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "3rdattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].third_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "ground_damage") == 0) {
                            KAIN->weapons[cur_weapon].ground_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_loop_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_loop_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_final_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_final_damage = atof(kv->value);
                        } else if (strcmp(attack, "fury_damage") == 0) {
                            KAIN->weapons[cur_weapon].fury_damage = atof(kv->value);
                        } else if (strcmp(attack, "berserk_damage") == 0) {
                            KAIN->weapons[cur_weapon].berserk_damage = atof(kv->value);
                        } else if (strcmp(attack, "lastberserk_damage") == 0) {
                            KAIN->weapons[cur_weapon].lastberserk_damage = atof(kv->value);
                        } else if (strcmp(attack, "jump_damage") == 0) {
                            KAIN->weapons[cur_weapon].jump_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_throw_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_throw_damage = atof(kv->value);
                        }
                    }
                } else if (strncmp(kv->key, "soulreaver_", 11) == 0)
                {
                    //printf("\nSoulReaver\n");
                    char attack[20];
                    if (sscanf(kv->key + 11, "%s", attack) == 1 )
                    {
                        //printf("\t%s\n", attack);
                        int cur_weapon = KAINWEAPON_SOULREAVER;
                        if (strcmp(attack, "1stattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].first_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "2ndattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].second_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "3rdattack_damage") == 0) {
                            KAIN->weapons[cur_weapon].third_attack_damage = atof(kv->value);
                        } else if (strcmp(attack, "ground_damage") == 0) {
                            KAIN->weapons[cur_weapon].ground_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_loop_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_loop_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_final_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_final_damage = atof(kv->value);
                        } else if (strcmp(attack, "fury_damage") == 0) {
                            KAIN->weapons[cur_weapon].fury_damage = atof(kv->value);
                        } else if (strcmp(attack, "berserk_damage") == 0) {
                            KAIN->weapons[cur_weapon].berserk_damage = atof(kv->value);
                        } else if (strcmp(attack, "lastberserk_damage") == 0) {
                            KAIN->weapons[cur_weapon].lastberserk_damage = atof(kv->value);
                        } else if (strcmp(attack, "jump_damage") == 0) {
                            KAIN->weapons[cur_weapon].jump_damage = atof(kv->value);
                        } else if (strcmp(attack, "grab_throw_damage") == 0) {
                            KAIN->weapons[cur_weapon].grab_throw_damage = atof(kv->value);
                        }
                    }
                }
                else //Unrecognized key
                {
                    printf("\n\tUnrecognized key in config file:\n\t\t[%s] %s = %s\n\n", section->name, kv->key, kv->value);
                    printf("Can't proceed. Correct the configuration file and try again.");
                    exit(0);
                }
            }//For loop end

            //Check if this file is set as default, and copy it
            if (contains_word_exact(KAIN->kainFile, "default"))
            {
                //printf("\nFound default\n");
                defaultKAIN = *KAIN;
                found_default = true;
            }
            else if (contains_word_exact(KAIN->kainFile, kainFile))
            {
                //We found the kain file config for this kain file, break
                //printf("\nFound kain file config\n");
                found = true;

                //Fill any missing data with default data
                if (found_default)
                {
                    //KAIN->kainFile[0] = '\0';
                    if ( KAIN->wipe_chance == -1) KAIN->wipe_chance = defaultKAIN.wipe_chance;
                    if ( KAIN->lorePerParticle == -1) KAIN->lorePerParticle = defaultKAIN.lorePerParticle;
                    if ( KAIN->get_up_presses == -1) KAIN->get_up_presses = defaultKAIN.get_up_presses;
                    if ( KAIN->vampireWeaponMultiplier == -1) KAIN->vampireWeaponMultiplier = defaultKAIN.vampireWeaponMultiplier;
                    if ( KAIN->maxLoreLevels == -1) KAIN->maxLoreLevels = defaultKAIN.maxLoreLevels;
                    for (int i = 0; i < KAIN_MAX_LEVELS; i++)
                    {
                        if (KAIN->levels[i].hp == -1) KAIN->levels[i].hp = defaultKAIN.levels[i].hp;
                        if (KAIN->levels[i].lore == -1) KAIN->levels[i].lore = defaultKAIN.levels[i].lore;
                    }
                    for (int i = 0; i < KAIN_MAX_WEAPONS; i++)
                    {
                        //KAIN->weapons[i].name[0] = '\0';
                        if (KAIN->weapons[i].first_attack_damage == -1) KAIN->weapons[i].first_attack_damage = defaultKAIN.weapons[i].first_attack_damage;
                        if (KAIN->weapons[i].second_attack_damage == -1) KAIN->weapons[i].second_attack_damage = defaultKAIN.weapons[i].second_attack_damage;
                        if (KAIN->weapons[i].third_attack_damage == -1) KAIN->weapons[i].third_attack_damage = defaultKAIN.weapons[i].third_attack_damage;
                        if (KAIN->weapons[i].ground_damage == -1) KAIN->weapons[i].ground_damage = defaultKAIN.weapons[i].ground_damage;
                        if (KAIN->weapons[i].grab_loop_damage == -1) KAIN->weapons[i].grab_loop_damage = defaultKAIN.weapons[i].grab_loop_damage;
                        if (KAIN->weapons[i].grab_final_damage == -1) KAIN->weapons[i].grab_final_damage = defaultKAIN.weapons[i].grab_final_damage;
                        if (KAIN->weapons[i].fury_damage == -1) KAIN->weapons[i].fury_damage = defaultKAIN.weapons[i].fury_damage;
                        if (KAIN->weapons[i].jump_damage == -1) KAIN->weapons[i].jump_damage = defaultKAIN.weapons[i].jump_damage;
                        if (KAIN->weapons[i].berserk_damage == -1) KAIN->weapons[i].berserk_damage = defaultKAIN.weapons[i].berserk_damage;
                        if (KAIN->weapons[i].lastberserk_damage == -1) KAIN->weapons[i].lastberserk_damage = defaultKAIN.weapons[i].lastberserk_damage;
                        if (KAIN->weapons[i].grab_throw_damage == -1) KAIN->weapons[i].grab_throw_damage = defaultKAIN.weapons[i].grab_throw_damage;
                    }
                    KainWeaponData weapons[KAIN_MAX_WEAPONS];
                }

                break;
            }


        }//Section check end

        if (found) break;

    }//Section loop end

    //If we didn't find the specified KAIN, but there was a default setting
    if (!found && found_default)
    {
        *KAIN = defaultKAIN;
        found = true;
    }
    return found;
}

/*
int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <ini_file>\n", argv[0]);
        return 1;
    }

    Config config;
    init_config(&config);

    parse_ini(argv[1], &config);
    print_config(&config);

    free_config(&config);

    return 0;
}
*/
