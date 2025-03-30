//sl03.big -> first red lore box
//Sl08.big -> first blue lore box

const float default_redChest_Lore = 30;
const float default_blueChest_Lore = 50;

int collectable_type_offset = 0xb4;
int collectable_lore_offset = 0xc0;

enum CollectableType
{
	COLLECTABLE_NONE,				// this spot masks off the unused BLOOD SuckableType
	COLLECTABLE_LORE_BOX,
	COLLECTABLE_WEAPON_BOX
};

const char *CollectableType_Names[] = {
    "NONE",
    "LORE_BOX",
    "WEAPON_BOX"
};
