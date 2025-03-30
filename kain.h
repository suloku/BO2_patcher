/*Kain.h header

This file contains data used to locate all kainX.big files in BO2
installation folder.

It also contains data needed to locate kain.tunedata inside the big
files.

*/

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

float max_lore_level = FLT_MAX;
int kain_levels_offset = 0x22a8;
int lorelevel_19_lore_offset = 0x2340;


int lorePerParticle_offset = 0x514;
