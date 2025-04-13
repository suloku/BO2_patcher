#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

#define INITIAL_SECTION_CAPACITY 10
#define INITIAL_KEY_CAPACITY 10

// Structure to hold a key-value pair
typedef struct {
    char key[50];
    char value[100];
} KeyValue;

// Structure to hold a section with multiple keys
typedef struct {
    char name[50];
    KeyValue* keys;
    int key_count;
    int key_capacity;
} Section;

// Structure to hold all sections
typedef struct {
    Section* sections;
    int section_count;
    int section_capacity;
} Config;

//Config structures
typedef struct {
    char npcFile[20]; //.tunedata files are quite short (8-16 chars), but better be safe
    float HitPoints;
    float CrawlHitPoints;
    float healthSuckSpeed;
    float normalMaxBlood;
    float stealtKillMaxBlood;
    float maxLore;
    char levels[1024];  // Array of strings (level names). 1024 should be enough chars even if all level names are written.
} NPC_tunedata;

typedef struct {
    char weaponFile[20];
    float HP;
    int32_t grabLoops;
    char levels[1024];  // Array of strings (level names). 1024 should be enough chars even if all level names are written.
}WEAPON_tunedata;

typedef struct {
    char chestFile[20];
    float lore;
    char levels[1024];  // Array of strings (level names). 1024 should be enough chars even if all level names are written.
}CHEST_tunedata;

//Variables

NPC_tunedata defaultNPC;
WEAPON_tunedata defaultWEAPON;
CHEST_tunedata defaultCHEST;

//Prototypes
void print_config(const Config* config);
void parse_ini(const char* filename, Config* config);
bool get_config_NPC(Config* config, NPC_tunedata* NPC, const char* npcName, const char* levelName);
bool get_config_WEAPON(Config* config, WEAPON_tunedata* WEAPON, const char* weaponName, const char* levelName);
bool get_config_CHEST(Config* config, CHEST_tunedata* CHEST, const char* chestName, const char* levelName);
