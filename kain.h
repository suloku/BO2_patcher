/*Kain.h header

This file contains data used to locate all kainX.big files in BO2
installation folder.

It also contains data needed to locate kain.tunedata inside the big
files.

*/

#define KAIN_TOTAL_LEVELS 26

const char *kain_big_files[] = {
    "kain.big",
    "kainb.big", //Conceptual Edition
    "kainc.big",
    "kaind.big",
    "kaine.big",
    "kainf.big",
    "kaing.big", //Conceptual Edition
    "kainh.big", //Conceptual Edition
    "kaini.big" //Conceptual Edition
};
size_t kainbigfilecount = sizeof(kain_big_files) / sizeof(kain_big_files[0]);

struct kainlevel {
    float Lore;
    float HP;
};

int wipe_chance_offset = 0x500;
int KainHitReactControls_offset = 0x8e8;
int time_on_ground_offset = 0x99c;
int vampireWeaponDamageMultiplier_offset = 0x38;

float max_lore_level = FLT_MAX;
int kain_levels_offset = 0x22a8;
int numLoreLevels_offset = 0x08b8;


int lorePerParticle_offset = 0x514;
