#include <stdint.h>
#include <stdio.h>

#define KAIN_TOTAL_WEAPONS 10

/*
Available Retail Game Weapons Damage

grab_throw seems to be the Claws grab_final since no other weapon can throw
charge_move is Fury's Dark Gift damage
super_jump is the damage done when attacking by jumping onto an enemy
lastspeedattack is Berserk's Dark Gift damage

Hands (Claws)
1stattack       10.0
2ndattack       10.0
3rdattack       30.0
ground          5.0
grab_loop       15.0
grab_final      30.0
charge_move     100.0
super_jump         50.0
speedattack        0.0
lastspeedattack  180.0
grab_throw        50.0

lswrod (Long Sword(Lsword)/Cutlass(Lsword4)/Scimitar(Lsword2) and unused lsword3)
1stattack         15.0
2ndattack         15.0
3rdattack         35.0
ground            10.0
grab_loop         25.0
grab_final        45.0
charge_move      100.0
speedattack        0.0
lastspeedattack  180.0

dagger (Dagger)
1stattack    15.0
2ndattack    15.0
3rdattack    30.0
ground        10.0
grab_loop    20.0
grab_final    40.0
charge_move    100.0
speedattack    0.0
lastspeedattack    180.0

handax (Short Axe(handax2))
1stattack    20.0
2ndattack    20.0
3rdattack    35.0
ground        10.0
grab_loop    50.0
grab_final    100.0
charge_move    100.0
speedattack    0.0
lastspeedattack    180.0

Crossbow
1stattack    10.0
grab_loop    5.0
grab_final    10.0
charge_move    100.0

Polearm (Double-Bladed Sword)
1stattack    15.0
2ndattack    20.0
3rdattack    35.0
ground        10.0
grab_loop    30.0
grab_final    50.0
charge_move    100.0
speedattack    0.0
lastspeedattack    180.0

hsword (Broadsword(Hsword)/Long Axe (Hax))
1stattack    20.0
2ndattack    20.0
3rdattack    40.0
ground        10.0
grab_loop    40.0
grab_final    0.0
charge_move    100.0
speedattack    0.0
lastspeedattack    180.0

club (Spiked club(LClub)/Sarafan Scepter (Scepter))
1stattack    10.0
2ndattack    15.0
3rdattack    30.0
ground        10.0
grab_loop    20.0
grab_final    25.0
Charge_move    100.0
speedattack    0.0
lastspeedattack    180.0

mace (cut weapon)
1stattack    10.0
2ndattack    15.0
3rdattack    30.0
ground        10.0
grab_loop    20.0
grab_final    25.0
Charge_move    100.0
speedattack    0.0
lastspeedattack    180.0

soulreaver
1stattack    100.0
2ndattack    100.0
3rdattack    100.0
ground        100.0
grab_loop    50.0
grab_final    755.0
Charge_move    500.0
speedattack    0.0
lastspeedattack    1000.0
*/

char* KainWeaponNames[] = {
    "Claws",
    "Long Swords",
    "Daggers",
    "Short Axes",
    "Crossbow",
    "Double-Bladed Sword",
    "Broadsword/Long Axe",
    "Spiked Club/Scepter",
    "Mace",
    "Soul Reaver"
};

//Offsets for each weapon's damage values
typedef struct {
    int first_attack;
    int second_attack;
    int third_attack;
    int ground;
    int grab_loop;
    int grab_final;
    int charge_move;
    int speed_attack;
    int last_speed_attack;
    int super_jump;
    int grab_throw;
} WeaponOffsets;

WeaponOffsets weapon_offsets[] = {
    // Hands (Claws)
    { 0x1064, 0x106C, 0x1074, 0x107C, 0x1084, 0x108C, 0x1094, 0x10A4, 0x10AC, 0x109C, 0x10B4 },
    // Lsword (Long Sword(Lsword)/Cutlass(Lsword4)/Scimitar(Lsword2) and unused lsword3)
    { 0x113C, 0x1144, 0x114C, 0x1154, 0x115C, 0x1164, 0x116C, 0x1174, 0x117C, 0x0, 0x0 },
    // Dagger (Dagger)
    { 0x11F0, 0x11F8, 0x1200, 0x1208, 0x1210, 0x1218, 0x1220, 0x1228, 0x1230, 0x0, 0x0 },
    // Handax (Short Axe(handax)/Unused handax2)
    { 0x12A4, 0x12AC, 0x12B4, 0x12BC, 0x12C4, 0x12CC, 0x12D4, 0x12DC, 0x12E4, 0x0, 0x0 },
    // Crossbow
    { 0x1358, 0x0, 0x0, 0x0, 0x1360, 0x1368, 0x1370, 0x0, 0x0, 0x0, 0x0 },
    // Polearm (Double-Bladed Sword)
    { 0x13AC, 0x13B4, 0x13BC, 0x13C4, 0x13CC, 0x13D4, 0x13DC, 0x13E4, 0x13EC, 0x0, 0x0 },
    // Hsword (Broadsword(Hsword)/Long Axe (Hax))
    { 0x1460, 0x1468, 0x1470, 0x1478, 0x1480, 0x1488, 0x1490, 0x1498, 0x14A0, 0x0, 0x0 },
    // Club (Spiked club(LClub)/Sarafan Scepter (Scepter))
    { 0x1510, 0x1518, 0x1520, 0x1528, 0x1530, 0x1538, 0x1540, 0x1548, 0x1550, 0x0, 0x0 },
    // Mace (cut weapon)
    { 0x15C0, 0x15C8, 0x15D0, 0x15D8, 0x15E0, 0x15E8, 0x15F0, 0x15F8, 0x1600, 0x0, 0x0 },
    // Soulreaver
    { 0x1678, 0x1680, 0x1688, 0x1690, 0x1698, 0x16A0, 0x16A8, 0x16B0, 0x16B8, 0x0, 0x0 }
};

/*
//Hands (Claws)
int hands_1stattack_offset        = 0x1064;
int hands_2ndattack_offset        = 0x106C;
int hands_3rdattack_offset        = 0x1074;
int hands_ground_offset           = 0x107C;
int hands_grab_loop_offset        = 0x1084;
int hands_grab_final_offset       = 0x108C;
int hands_charge_move_offset      = 0x1094;
int hands_super_jump_offset       = 0x109C;
int hands_speedattack_offset      = 0x10A4;
int hands_lastspeedattack_offset  = 0x10AC;
int hands_grab_throw_offset       = 0x10B4;

//lsword (Long Sword(Lsword)/Cutlass(Lsword4)/Scimitar(Lsword2) and unused lsword3)
int lsword_1stattack_offset        = 0x113C;
int lsword_2ndattack_offset        = 0x1144;
int lsword_3rdattack_offset        = 0x114C;
int lsword_ground_offset           = 0x1154;
int lsword_grab_loop_offset        = 0x115C;
int lsword_grab_final_offset       = 0x1164;
int lsword_charge_move_offset      = 0x116C;
int lsword_speedattack_offset      = 0x1174;
int lsword_lastspeedattack_offset  = 0x117C;

//dagger (Dagger)
int dagger_1stattack_offset        = 0x11F0;
int dagger_2ndattack_offset        = 0x11F8;
int dagger_3rdattack_offset        = 0x1200;
int dagger_ground_offset           = 0x1208;
int dagger_grab_loop_offset        = 0x1210;
int dagger_grab_final_offset       = 0x1218;
int dagger_charge_move_offset      = 0x1220;
int dagger_speedattack_offset      = 0x1228;
int dagger_lastspeedattack_offset  = 0x1230;

//handax (Short Axe(handax2))
int handax_1stattack_offset        = 0x12A4;
int handax_2ndattack_offset        = 0x12AC;
int handax_3rdattack_offset        = 0x12B4;
int handax_ground_offset           = 0x12BC;
int handax_grab_loop_offset        = 0x12C4;
int handax_grab_final_offset       = 0x12CC;
int handax_charge_move_offset      = 0x12D4;
int handax_speedattack_offset      = 0x12DC;
int handax_lastspeedattack_offset  = 0x12E4;

//Crossbow
int crossbow_1stattack_offset     = 0x1358;
int crossbow_grab_loop_offset     = 0x1360;
int crossbow_grab_final_offset    = 0x1368;
int crossbow_charge_move_offset   = 0x1370;

//Polearm (Double-Bladed Sword)
int polearm_1stattack_offset        = 0x13AC;
int polearm_2ndattack_offset        = 0x13B4;
int polearm_3rdattack_offset        = 0x13BC;
int polearm_ground_offset           = 0x13C4;
int polearm_grab_loop_offset        = 0x13CC;
int polearm_grab_final_offset       = 0x13D4;
int polearm_charge_move_offset      = 0x13DC;
int polearm_speedattack_offset      = 0x13E4;
int polearm_lastspeedattack_offset  = 0x13EC;

//hsword (Broadsword(Hsword)/Long Axe (Hax))
int hsword_1stattack_offset        = 0x1460;
int hsword_2ndattack_offset        = 0x1468;
int hsword_3rdattack_offset        = 0x1470;
int hsword_ground_offset           = 0x1478;
int hsword_grab_loop_offset        = 0x1480;
int hsword_grab_final_offset       = 0x1488;
int hsword_charge_move_offset      = 0x1490;
int hsword_speedattack_offset      = 0x1498;
int hsword_lastspeedattack_offset  = 0x14A0;

//club (Spiked club(LClub)/Sarafan Scepter (Scepter))
int club_1stattack_offset        = 0x1510;
int club_2ndattack_offset        = 0x1518;
int club_3rdattack_offset        = 0x1520;
int club_ground_offset           = 0x1528;
int club_grab_loop_offset        = 0x1530;
int club_grab_final_offset       = 0x1538;
int club_charge_move_offset      = 0x1540;
int club_speedattack_offset      = 0x1548;
int club_lastspeedattack_offset  = 0x1550;

//mace (cut weapon)
int mace_1stattack_offset        = 0x15c0;
int mace_2ndattack_offset        = 0x15c8;
int mace_3rdattack_offset        = 0x15d0;
int mace_ground_offset           = 0x15d8;
int mace_grab_loop_offset        = 0x15e0;
int mace_grab_final_offset       = 0x15e8;
int mace_charge_move_offset      = 0x15f0;
int mace_speedattack_offset      = 0x15f8;
int mace_lastspeedattack_offset  = 0x1600;

//soulreaver
int soulreaver_1stattack_offset        = 0x1678;
int soulreaver_2ndattack_offset        = 0x1680;
int soulreaver_3rdattack_offset        = 0x1688;
int soulreaver_ground_offset           = 0x1690;
int soulreaver_grab_loop_offset        = 0x1698;
int soulreaver_grab_final_offset       = 0x16a0;
int soulreaver_charge_move_offset      = 0x16a8;
int soulreaver_speedattack_offset      = 0x16b0;
int soulreaver_lastspeedattack_offset  = 0x16b8;
*/

//Offsets for each weapon tunedata values
int weaponTune_weaponClass_offset = 0x04;
int weaponTune_weaponHP_offset = 0x08;
int weaponTune_GrabLoopTimes_offset = 0x40;

const char *kainWeaponFilesToModNames[] = {
    "hands", //Claws
    "soulreaver" //Soul Reaver
};

size_t kainWeaponFilesToModCount = sizeof(kainWeaponFilesToModNames) / sizeof(kainWeaponFilesToModNames[0]);

const char *dropWeaponFilesToModNames[] = {
    "lsword", //Long Sword
    "lsword2", //Scimitar
    "lsword3", //unused
    "lsword4", //Cutlass
    "dagger",	//Dagger
    "handax", //Short Axe
    "handax2", //unused???
    "wxbow", //Crossbow (unused)
    "polearm", //Double-Bladed Sword
    "hsword", //Broadsword
    "hax", //Long Axe
    "l_club", //Spiked Club
    "scepter" //Sarafan Scepter
};
size_t dropWeaponFilesToModNamesCount = sizeof(dropWeaponFilesToModNames) / sizeof(dropWeaponFilesToModNames[0]);

typedef struct WeaponTuneData {
    const char* WeaponName;
    float HP;
    int32_t grabLoops;
}WeaponTuneData;

typedef enum {
    WEAPON_LSWORD,
    WEAPON_LSWORD2,
    WEAPON_LSWORD3,
    WEAPON_LSWORD4,
    WEAPON_DAGGER,
    WEAPON_HANDAX,
    WEAPON_HANDAX2,
    WEAPON_WXBOW,
    WEAPON_POLEARM,
    WEAPON_HSWORD,
    WEAPON_HAX,
    WEAPON_L_CLUB,
    WEAPON_SCEPTER,
    WEAPON_COUNT // always useful to know how many entries
} WeaponID;

WeaponTuneData DropWeaponData[] = {
    { .WeaponName = "lsword", .HP = 20, .grabLoops = 3 },
    { .WeaponName = "lsword2", .HP = 20, .grabLoops = 3 },
    { .WeaponName = "lsword3", .HP = 20, .grabLoops = 3 },
    { .WeaponName = "lsword4", .HP = 20, .grabLoops = 3 },
    { .WeaponName = "dagger", .HP = 20, .grabLoops = 3 },
    { .WeaponName = "handax", .HP = 20, .grabLoops = 3 },
    { .WeaponName = "handax2", .HP = 20, .grabLoops = 3 },
    { .WeaponName = "wxbow", .HP = 20, .grabLoops = 3 },
    { .WeaponName = "polearm", .HP = 20, .grabLoops = 3 },
    { .WeaponName = "hsword", .HP = 20, .grabLoops = 3 },
    { .WeaponName = "hax", .HP = 20, .grabLoops = 3 },
    { .WeaponName = "l_club", .HP = 20, .grabLoops = 3 },
    { .WeaponName = "scepter", .HP = 20, .grabLoops = 3 }
};
