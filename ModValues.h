/*
The different collectables in the game share the same structure.
The three of them can theorically reward lore and hp (even weapons, untested)
*/
//coll_lore.tunedata
float new_RedChest_Lore = 60;
//coll_biglore.tunedata
float new_BlueChest_Lore = 100;
//coll_weapon.tuneda
float new_weapon_Lore = 0;

//NPCs tunedata
float new_npc_lore = 0;
float new_npc_crawl_hitpoints = 5;

float npc_bloodsukratio_0_50 = 1.0;
float npc_bloodsukratio_51_75 = 1.25;
float npc_bloodsukratio_76_125 = 1.5;
float npc_bloodsukratio_126plus = 1.75;

//kainX.tunedata
float wipe_chance = 5;
float lorePerParticle = 20; //Blue and Red chests Lore must be divisible by this number
int32_t kain_get_up_presses = 1;
float VampireWeaponMultiplier = 2;

//Kain Levels
int32_t kain_MaxLoreLevels = 16;
float kainlevel_hp_values[] = {
    30,  // Level 0
    60,  // Level 1
    90,  // Level 2
    120, // Level 3
    150, // Level 4
    180, // Level 5
    210, // Level 6
    240, // Level 7
    270, // Level 8
    300, // Level 9
    330, // Level 10
    360, // Level 11
    390, // Level 12
    420, // Level 13
    450, // Level 14
    480, // Level 15
    //485, // Level 16
    500, // Level 16
    490, // Level 17
    495, // Level 18
    500, // Level 19
    500, // Level 20
    500, // Level 21
    500, // Level 22
    500, // Level 23
    500, // Level 24
    500  // Level 25
};

/*
These values are the lore needed to be collected for Kain to level up.
There's a bug where if you collect 950 lore at level 25, your HP goes back to
the level 0 value (30 hp). There seems to be code preventing thirst from killing
Kain at low levels, but after this overflow it doesn't trigger, thus preveting finishing
the game. Setting level's 25 lore to FLT_MAX prevents this from happening, as the player
will virtually never reach the needed lore to level up and overflow.
*/
float kainlevel_lore_values[] = {
    6,   // Level 0
    30,  // Level 1
    120, // Level 2
    240, // Level 3
    320, // Level 4
    400, // Level 5
    470, // Level 6
    540, // Level 7
    610, // Level 8
    670, // Level 9
    730, // Level 10
    780, // Level 11
    830, // Level 12
    880, // Level 13
    930, // Level 14
    950, // Level 15
    950, // Level 16
    950, // Level 17
    950, // Level 18
    950, // Level 19
    950, // Level 20
    950, // Level 21
    950, // Level 22
    950, // Level 23
    950, // Level 24
    950  // Level 25
};

/*
Release Game Kain Level table for reference

Lvl    HP    Lore   Accum. Lore
 0     30      6        6
 1     60     30       36
 2     90    120      156
 3    120    240      396
 4    150    320      716
 5    180    400     1116
 6    210    470     1586
 7    240    540     2126
 8    270    610     2736
 9    300    670     3406
10    330    730     4136
11    360    780     4916
12    390    830     5746
13    420    880     6626
14    450    930     7556
15    480    950     8506
16    485    950     9456
17    490    950    10406
18    495    950    11356
19    500    950    12306
20    500    950    13256
21    500    950    14206
22    500    950    15156
23    500    950    16106
24    500    950    17056
25    500    950    18006
*/

//Weapons

typedef enum {
    HANDS = 0,
    LSWORD,
    DAGGER,
    HANDAX,
    CROSSBOW,
    POLEARM,
    HSWORD,
    CLUB,
    MACE,
    SOULREAVER
} WeaponType;

typedef struct {
    float first_attack;
    float second_attack;
    float third_attack;
    float ground;
    float grab_loop;
    float grab_final;
    float charge_move;
    float speed_attack;
    float last_speed_attack;
    float super_jump;
    float grab_throw;
} WeaponDamage;

WeaponDamage weaponDamageValues[] = {
    // Hands (Claws)
    { 10.0f, 10.0f, 30.0f, 5.0f, 15.0f, 3.0f, 100.0f, 50.0f, 0.0f, 180.0f, 50.0f },
    // Lsword (Long Sword(Lsword)/Cutlass(Lsword4)/Scimitar(Lsword2) and unused lsword3)
    { 15.0f, 15.0f, 35.0f, 10.0f, 25.0f, 45.0f, 100.0f, 0.0f, 180.0f, 0.0f, 0.0f },
    // Dagger (Dagger)
    { 15.0f, 15.0f, 30.0f, 10.0f, 20.0f, 40.0f, 100.0f, 0.0f, 180.0f, 0.0f, 0.0f },
    // Handax (Short Axe(handax2))
    { 20.0f, 20.0f, 35.0f, 10.0f, 50.0f, 100.0f, 100.0f, 0.0f, 180.0f, 0.0f, 0.0f },
    // Crossbow
    { 10.0f, 0.0f, 0.0f, 0.0f, 5.0f, 10.0f, 100.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    // Polearm (Double-Bladed Sword)
    { 15.0f, 20.0f, 35.0f, 10.0f, 30.0f, 50.0f, 100.0f, 0.0f, 180.0f, 0.0f, 0.0f },
    // Hsword (Broadsword(Hsword)/Long Axe (Hax))
    { 20.0f, 20.0f, 40.0f, 10.0f, 40.0f, 0.0f, 100.0f, 0.0f, 180.0f, 0.0f, 0.0f },
    // Club (Spiked club(LClub)/Sarafan Scepter (Scepter))
    { 10.0f, 15.0f, 30.0f, 10.0f, 20.0f, 25.0f, 100.0f, 0.0f, 180.0f, 0.0f, 0.0f },
    // Mace (cut weapon)
    { 10.0f, 15.0f, 30.0f, 10.0f, 20.0f, 25.0f, 100.0f, 0.0f, 180.0f, 0.0f, 0.0f },
    // Soulreaver
    { 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 755.0f, 500.0f, 0.0f, 1000.0f, 0.0f, 0.0f }
};

/*
//Hands (Claws)
float hands_1stattack_damage        = 10;
float hands_2ndattack_damage        = 10;
float hands_3rdattack_damage        = 30;
float hands_ground_damage           = 5;
float hands_grab_loop_damage        = 15;
float hands_grab_final_damage       = 3;
float hands_charge_move_damage      = 100;
float hands_super_jump_damage       = 50;
float hands_speedattack_damage      = 0;
float hands_lastspeedattack_damage  = 180;
float hands_grab_throw_damage       = 50;

//lsword (Long Sword(Lsword)/Cutlass(Lsword4)/Scimitar(Lsword2) and unused lsword3)
float lsword_1stattack_damage        = 15;
float lsword_2ndattack_damage        = 15;
float lsword_3rdattack_damage        = 35;
float lsword_ground_damage           = 10;
float lsword_grab_loop_damage        = 25;
float lsword_grab_final_damage       = 45;
float lsword_charge_move_damage      = 100;
float lsword_speedattack_damage      = 0;
float lsword_lastspeedattack_damage  = 180;

//dagger (Dagger)
float dagger_1stattack_damage        = 15;
float dagger_2ndattack_damage        = 15;
float dagger_3rdattack_damage        = 30;
float dagger_ground_damage           = 10;
float dagger_grab_loop_damage        = 20;
float dagger_grab_final_damage       = 40;
float dagger_charge_move_damage      = 100;
float dagger_speedattack_damage      = 0;
float dagger_lastspeedattack_damage  = 180;

//handax (Short Axe(handax2))
float handax_1stattack_damage        = 20;
float handax_2ndattack_damage        = 20;
float handax_3rdattack_damage        = 35;
float handax_ground_damage           = 10;
float handax_grab_loop_damage        = 50;
float handax_grab_final_damage       = 100;
float handax_charge_move_damage      = 100;
float handax_speedattack_damage      = 0;
float handax_lastspeedattack_damage  = 180;

//Crossbow
float crossbow_1stattack_damage     = 10;
float crossbow_grab_loop_damage     = 5;
float crossbow_grab_final_damage    = 10;
float crossbow_charge_move_damage   = 100;

//Polearm (Double-Bladed Sword)
float polearm_1stattack_damage        = 15;
float polearm_2ndattack_damage        = 20;
float polearm_3rdattack_damage        = 35;
float polearm_ground_damage           = 10;
float polearm_grab_loop_damage        = 30;
float polearm_grab_final_damage       = 50;
float polearm_charge_move_damage      = 100;
float polearm_speedattack_damage      = 0;
float polearm_lastspeedattack_damage  = 180;

//hsword (Broadsword(Hsword)/Long Axe (Hax))
float hsword_1stattack_damage        = 20;
float hsword_2ndattack_damage        = 20;
float hsword_3rdattack_damage        = 40;
float hsword_ground_damage           = 10;
float hsword_grab_loop_damage        = 40;
float hsword_grab_final_damage       = 0; //Decapitation
float hsword_charge_move_damage      = 100;
float hsword_speedattack_damage      = 0;
float hsword_lastspeedattack_damage  = 180;

//club (Spiked club(LClub)/Sarafan Scepter (Scepter))
float club_1stattack_damage        = 10;
float club_2ndattack_damage        = 15;
float club_3rdattack_damage        = 30;
float club_ground_damage           = 10;
float club_grab_loop_damage        = 20;
float club_grab_final_damage       = 25;
float club_charge_move_damage      = 100;
float club_speedattack_damage      = 0;
float club_lastspeedattack_damage  = 180;

//mace (cut weapon)
float mace_1stattack_damage        = 10;
float mace_2ndattack_damage        = 15;
float mace_3rdattack_damage        = 30;
float mace_ground_damage           = 10;
float mace_grab_loop_damage        = 20;
float mace_grab_final_damage       = 25;
float mace_charge_move_damage      = 100;
float mace_speedattack_damage      = 0;
float mace_lastspeedattack_damage  = 180;

//soulreaver
float soulreaver_1stattack_damage        = 100;
float soulreaver_2ndattack_damage        = 100;
float soulreaver_3rdattack_damage        = 100;
float soulreaver_ground_damage           = 100;
float soulreaver_grab_loop_damage        = 100;
float soulreaver_grab_final_damage       = 755;
float soulreaver_charge_move_damage      = 500;
float soulreaver_speedattack_damage      = 0;
float soulreaver_lastspeedattack_damage  = 1000;
*/
