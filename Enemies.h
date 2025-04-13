#include <stdint.h>

//Bosses and Greater demons don't reward lore

int32_t npc_DeathTunedata_offset = 0x0428; //4*4 bytes long
int32_t npc_crawl_away_data_offset = 0x300; // 4*3 bytes long (3 floats)

int32_t npc_crawl_away_data_hitpoints_offset = 0x308;

int32_t npc_HitPoints_offset = 0x04;
int32_t npc_BloodSuckTunedata_offset = 0x438;
int32_t npc_BloodSuckTunedata_healthSuckSpeed_offset = 0x43c;
int32_t npc_BloodSuckTunedata_normalMaxBlood_offset = 0x440;
int32_t npc_BloodSuckTunedata_stealthKillMaxBlood_offset = 0x444;
int32_t npc_BloodSuckTunedata_maxLore_offset = 0x448;

const char *npcFilesToModNames[] = {
    "arisanm",  //Aristocrat
    "arisbnm",
    "ariscnf",
    "arisdnf",
    "arisfnf",

    "bshpnm",   //Bishop

    "bmadanf",     //Barmaid

    "spider",   //Demon Spirder

    "eylanm",   //Eyeless wretches
    "eylbnf",

    "sggrdwm",  //Glyph Guard
    "sggrdwm_polearm",

    "sgkntgm",  //Sarafan Glyph knight -> sgknt

    "glpwnm",   //Glyphwright

    "ubdem",    //Greater demon

    "hylworhm", //Hylden scientist

    "hylwarhm", //Hylden warrior

    "wrkanm",   //Industrial worker
    "wrkrcnf",

    "floater",  //Lesser demon

    "ksrvnm",   //Manservant

    "mercawm",   //Mercenary
    "mercdwf",

    "mrchntnm", //Merchant
    "mrchnbnf",
    "chained_mrchntnm",

    "psntanm", //Peasant
    "psntbnm",
    "psntcnf",
    "psntdnf",
    "chained_psntanm",
    "chained_psntbnm",
    "chained_psntcnf",
    "chained_psntdnf",

    "poltergm", //Poltergeist

    "wardengm", //Prison guardian

    "archwf",   //Sarafan archer

    "sgrdwm",   //Sarafan Guard

    "skntgm",   //Sarafan knight
    "shgkntgm",

    "prstwm",   //Sarafan Priest
    "prstwm_no_xbow",

    "slavnm",   //Slave
    "chained_slavnm",

    "smugawm",  //Smuggler
    "smugcwf",

    "thifcwf",  //Thieve
    "thifcwf_no_xbow",

    "thugawm",  //Thugs

    //"umahvf",   //Umah Can't be bloodsucked or interacted with.
    //"umahvf_train_attack",
    //"umahvf_train_block",
    //"umahvf_train_grab",

    //Animals
    "cat",
    "rat"

//Bosses
    //Magnus
	//"madvm_ep",
	//"mad_vampire",
	//"madvmcactor",
	//"madvm", // this file is in the boss battle level

    //Marcus
	//"charmvm",

    //Sebastian
	//"spdvm",

    //Faustus
	//"jumpvm",

	//Sarafan Lord
	//"slordgm1",
	//"slordgm2",
	//"slordgm3"

};

size_t npcFilesToModCount = sizeof(npcFilesToModNames) / sizeof(npcFilesToModNames[0]);


//The full list of NPC filenames
const char *npcFileNames[] = {
    "arisanm",  //Aristocrat
    "arisbnm",
    "ariscnf",
    "arisdnf",
    "arisfnf",

    "bshpnm",   //Bishop

    "bmadanf",     //Barmaid

    "spider",   //Demon Spirder

    "eylanm",   //Eyeless wretches
    "eylbnf",

    "sggrdwm",  //Glyph Guard
    "sggrdwm_polearm",

    "sgkntgm",  //Sarafan Glyph knight -> sgknt

    "glpwnm",   //Glyphwright

    "ubdem",    //Greater demon

    "hylworhm", //Hylden scientist

    "hylwarhm", //Hylden warrior

    "wrkanm",   //Industrial worker
    "wrkrcnf",

    "floater",  //Lesser demon

    "ksrvnm",   //Manservant

    "mercawm",   //Mercenary
    "mercdwf",

    "mrchntnm", //Merchant
    "mrchnbnf",
    "chained_mrchntnm",

    "psntanm", //Peasant
    "psntbnm",
    "psntcnf",
    "psntdnf",
    "chained_psntanm",
    "chained_psntbnm",
    "chained_psntcnf",
    "chained_psntdnf",

    "poltergm", //Poltergeist

    "wardengm", //Prison guardian

    "archwf",   //Sarafan archer

    "sgrdwm",   //Sarafan Guard

    "skntgm",   //Sarafan knight
    "shgkntgm",

    "prstwm",   //Sarafan Priest
    "prstwm_no_xbow",

    "slavnm",   //Slave
    "chained_slavnm",

    "smugawm",  //Smuggler
    "smugcwf",

    "thifcwf",  //Thieve
    "thifcwf_no_xbow",

    "thugawm",  //Thugs

    "umahvf",   //Umah

    //The following umah files, if they have a lore value at all, it is at a different offset
    "umahvf_train_attack",
    "umahvf_train_block",
    "umahvf_train_grab",

    //Animals
    "cat",
    "rat",

//Bosses
//If they have a lore value at all, it is at a different offset
    //Magnus
	"madvm_ep",
	"mad_vampire",
	"madvmcactor",
	"madvm", // this file is in the boss battle level

    //Marcus
	"charmvm",

    //Sebastian
	"spdvm",

    //Faustus
	"jumpvm",

	//Sarafan Lord
	"slordgm1",
	"slordgm2",
	"slordgm3"

};

size_t npcFileCount = sizeof(npcFileNames) / sizeof(npcFileNames[0]);
