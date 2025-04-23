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

#define KAIN_MAX_LEVELS 26 // From level_00 to level_25
#define MAX_TUNEDATA_FILENAME 20 // For weapon names like "soulreaver"
#define KAIN_MAX_WEAPONS 10 // For weapon names like "soulreaver"
#define MAX_LEVEL_ARRAY 1024 //This is enough even if all level names are present

// Sub-structure for level data (LORE and HP pairs)
typedef struct {
    float lore;
    float hp;
} KainLevelData;

enum KainWeaponType
{
	KAINWEAPON_HANDS,
	KAINWEAPON_LSWORD,
	KAINWEAPON_DAGGER,
	KAINWEAPON_HANDAX,
	KAINWEAPON_CROSSBOW,
	KAINWEAPON_POLEARM,
	KAINWEAPON_HSWORD,
	KAINWEAPON_CLUB,
	KAINWEAPON_MACE,
	KAINWEAPON_SOULREAVER
};

// Sub-structure for weapon damage data
typedef struct {
    char name[MAX_TUNEDATA_FILENAME]; // e.g., "hands", "lsword"
    float first_attack_damage; // 1stattack_damage
    float second_attack_damage; // 2ndattack_damage
    float third_attack_damage; // 3rdattack_damage
    float ground_damage;
    float grab_loop_damage;
    float grab_final_damage;
    float fury_damage;
    float jump_damage; // Only used by hands
    float berserk_damage;
    float lastberserk_damage;
    float grab_throw_damage; // Only used by hands
} KainWeaponData;

// Main structure for all configuration data
typedef struct {
    char kainFile[MAX_TUNEDATA_FILENAME]; // e.g., "default", "Kain", "Kainb", "Kainc"...
    float wipe_chance;
    float lorePerParticle;
    int32_t get_up_presses; // Stored as float per requirement
    float vampireWeaponMultiplier;
    int32_t maxLoreLevels; // Stored as float per requirement
    //Thirst
    float health_Increase_Rate;
    float lore_Increase_Rate;
    float thirst_drain_rate;
    float thirst_min_percent;
    float thirst_drain_time;
    float thirst_drain_amount;
    //Dark gift
    float charm_regentime;
    float fury_cost;
    float immolate_cost;
    float immolate_damage;
    float berserk_duration; //changing this value doesn't seem to do anything...
    float berserk_cost;
    float stealth_regentime;
    float superjump_regentime;
    float telekinesis_regentime;
    float telekinesis_damage;
    //Rage Bar
    float rage_max_points;
    float rage_points_lost_per_second;
    float rage_normal_block_points;
    float rage_special_block_points;
    float rage_dodge_points;
    //Weapons
    float claws_holdtime;
    int32_t claws_grabLoops;
    int32_t soulreaver_grabLoops;
    KainLevelData levels[KAIN_MAX_LEVELS]; // level_00 to level_25
    KainWeaponData weapons[KAIN_MAX_WEAPONS]; // hands, lsword, dagger, handax, crossbow, polearm, hsword, club, mace, soulreaver
} KAIN_tunedata;

typedef struct {
    char npcFile[MAX_TUNEDATA_FILENAME]; //.tunedata files are quite short (8-16 chars), but better be safe
    float HitPoints;
    float CrawlHitPoints;
    float healthSuckSpeed;
    float normalMaxBlood;
    float stealtKillMaxBlood;
    float maxLore;
    char levels[MAX_LEVEL_ARRAY];  // Array of strings (level names). 1024 should be enough chars even if all level names are written.
} NPC_tunedata;

typedef struct {
    char weaponFile[MAX_TUNEDATA_FILENAME];
    float HP;
    int32_t grabLoops;
    char levels[MAX_LEVEL_ARRAY];  // Array of strings (level names). 1024 should be enough chars even if all level names are written.
}WEAPON_tunedata;

typedef struct {
    char chestFile[MAX_TUNEDATA_FILENAME];
    float lore;
    char levels[MAX_LEVEL_ARRAY];  // Array of strings (level names). 1024 should be enough chars even if all level names are written.
}CHEST_tunedata;

typedef struct {
    bool skipKainFiles;
    bool skipLevelFiles;
    bool proportionalBloodSuck;
    float npc_bloodsuckratio_0_50;
    float npc_bloodsuckratio_51_75;
    float npc_bloodsuckratio_76_125;
    float npc_bloodsuckratio_126_150;
    float npc_bloodsuckratio_151plus;
}EXTRA_config;

//Variables

//KAIN_tunedata defaultKAIN;
//NPC_tunedata defaultNPC;
//WEAPON_tunedata defaultWEAPON;
//CHEST_tunedata defaultCHEST;

//Prototypes
void init_config(Config* config);
void free_config(Config* config);
void print_config(const Config* config);
int parse_ini(const char* filename, Config* config);

void init_EXTRA_config(EXTRA_config* EXTRA_CFG);
void init_KAIN_config(KAIN_tunedata* KAIN);
void init_NPC_config(NPC_tunedata* NPC);
void init_WEAPON_config(WEAPON_tunedata* WEAPON);
void init_CHEST_config(CHEST_tunedata* CHEST);

bool get_config_NPC(Config* config, NPC_tunedata* NPC, const char* npcName, const char* levelName);
bool get_config_WEAPON(Config* config, WEAPON_tunedata* WEAPON, const char* weaponName, const char* levelName);
bool get_config_CHEST(Config* config, CHEST_tunedata* CHEST, const char* chestName, const char* levelName);
bool get_config_KAIN(Config* config, KAIN_tunedata* KAIN, const char* kainFile);
void get_config_EXTRA(Config* config, EXTRA_config* EXTRA_CFG);
