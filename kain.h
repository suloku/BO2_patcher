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

//Thirst
int kain_healthIncreaseRate_offset = 0x89c;		    // desc=Health_and_max_health_bar_grow_rates_after_suck_in_or_lore_level_upgrade default=50
int kain_loreIncreaseRate_offset = 0x8A0;			// desc=How_fast_lore_accumulated_by_particles_actually_gets_added_per_second default=1
int kain_thirstDrainRate_offset = 0x8A4;			// desc=How_many_hit_points_we_lose_per_second_due_to_thirst default=0.1
int kain_thirstMinPercent_offset = 0x8A8;			// desc=What_is_the_minimum_percentage_of_health_the_thirst_can_drain_us_to default=10.0
int	kain_thirstDrainTime_offset = 0x8ac;			// desc=Number_of_seconds_between_thirst_drains default=30.0
int	kain_thirstDrainAmount_offset = 0x8b0;			// desc=Number_of_hit_points_that_get_drained_per_thirst default=10.0

//Dark Gifts
int kain_charm_regen_time_offset = 0x594;
int kain_fury_cost_offset = 0x5A8;
int kain_immolate_cost_offset = 0x5E8;
int kain_immolate_damage_offset = 0x60c;
int kain_berserk_duration_offset = 0x640;	        // desc=Number_of_seconds_speed_lasts_for default=10.
//int kain_berserk_slowSpeed_offset = 0x644;			// desc=What_speed_we_set_the_rest_of_the_world_to_when_attacking default=0.50
//int kain_berserk_frozenSpeed_offset = 0x648;		// desc=What_speed_we_set_the_rest_of_the_world_to_when_we_freeze_the_world default=0.15
//int kain_berserk_speedChangeRate_offset = 0x64c;	// desc=How_fast_the_time_class_rates_can_change default=1.5
int kain_berserk_cost_offset = 0x650;
int kain_stealth_regen_time_offset = 0x6b8;
int kain_superjump_regen_time_offset = 0x6d0;
int kain_telekinesis_regen_time_offset = 0x76c;
int kain_telekinesis_damage_offset = 0x7a0;

float max_lore_level = FLT_MAX;
int kain_levels_offset = 0x22a8;
int numLoreLevels_offset = 0x08b8;


int lorePerParticle_offset = 0x514;
