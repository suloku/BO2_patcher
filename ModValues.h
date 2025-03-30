/*
The different collectables in the game share the same structure.
The three of them can theorically reward lore and hp (even weapons, untested)
*/
//coll_lore.tunedata
float new_RedChest_Lore = 75;
//coll_biglore.tunedata
float new_BlueChest_Lore = 125;
//coll_weapon.tuneda
float new_weapon_Lore = 0;

//NPCs tunedata
float new_npc_lore = 0;
float new_npc_crawl_hitpoints = 5;

//kainX.tunedata
float wipe_chance = 5;
float lorePerParticle = 25; //Blue and Red chests Lore must be divisible by this number

//Kain Levels
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
    //950, // Level 16
    FLT_MAX, // Level 16
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
