# Blood Omen 2 Retuner

### Version 0.2 by suloku '25

This is a patcher for the game Blood Omen 2.

If you are interested in rebalancing/fine tunning the game by changing Kain/NPC behavior, enemies HP, weapon damage... please contact me, I'll gladly help to the best of my ability, there are a lot of configurable parameters.

The retail version of the game feels slow and repetitive. This is mainly due to Lore (experience points) being tied to the blood sucking mechanich. This makes it mandatory to kill and suck every single enemy and NPC if you want to level up. This was probably done to enhance the vampiric experience and wreak havoc in all the game areas, but turns it into a chore. The fact that every time after blood sucking a ~2 second animation of Kain wiping his mouth plays doesn't help, but for later game enemies, sucking all the blood takes about 7 seconds per enemy, which coupled with the wiping mouth animation, it's 9 seconds PER ENEMY!

This patcher aims to provide a way to mod the game with the goal of making gameplay experience more straightforward. 

Several configuration files are provided:
- 01 Retail Configuration.ini: this file is a template to build any other configuration file. It has all the values present in the retail version of the game.
- 02 Lore Overflow Bugfix.ini: this file's aim is just to prevent the "Lore Overflow bug". More info inside the file.
- 03 Faster Sucking.ini: this file prevents the "Lore Overflow Bug" plus reduces all bloodsuck animations to 1-2 seconds and Kain's wipe mouth animation only happens 5% of the time. The retail experience, but faster to play.
- 04 No Lore - Faster Sucking.ini: same as "Faster Sucking", but additionally NPCs give no Lore. All lore is found in the Lore Chests, so sucking blood from NPCs is only needed to recover health. Max level has been adjusted for this change.
- 05 BO2 Retuned Mod.ini: Includes the changes in "No Lore - Faster Sucking" and has several aditional rebalance changes. Refer to the Retuned Mod section for more information. 

Please open and read the comments inside each configuration file for more information.


## Changelog
- The patcher now works with configuration files instead of hard coded values.
- The configuration file supports per sub-level configurations. This means you can, for example, have the Red chests in later leves have different amounts of lore. Same for NPCs.
- A lot of new configuration options have been added: Dark Gifts settings, Kain's Thirst settings, Weapon Damage, Weapon duration...
- User-made configuration files are directly supported, just add them to the bo2tuner_cfg folder.
- Added a simple logger, creating a file named bo2tuner_log.txt that contains the patching process messages.
- Added "Dump mode", wich outputs and creates csv files with the game's configuration.

## Patcher features
Here's a list of all the changes the patcher supports:

- Kain settings: wipe mouth animation chance, number of button presses to get up quickly, amount of lore in each lore "orb", damage multiplier for the weapon chests, all levels HP and needed Lore, thirst parameters (frequency, how much health it drains...), Dark Gift parameters (damage, wait time, Rage Bar cost...), Weapon Damage.
	- Note: the game uses different kain*.big files for different levels (each file is a different kain's attire). Different configurations for each file are supported.
- Lore Chests settings: amount of lore inside red/blue chests
- NPC settings: HP, HP when crawling, sucking speed, health to be drained, health to be drained when stealth killed, lore to be drained.
	- Note: different settings for each npc file inside each level is supported. I.e. it is possible to have later NPCs give more lore.
- NPCs give no lore (except the two tutorial chained prisoners, as they are mandatory as the game requires kain to level up to proceed).
	- Note: different settings for each waepon file inside each level is supported. I.e. it is possible to have later level weapons be more durable.
	
Please refer to the "docs" folder for some helper files about what files are in each level.

## Usage
- Place "bo2_patcher.exe" in the game's installation folder (where "bo2.exe" and "data" folders are).
- Optional: run "backup_files.bat". A folder named "data_backup" will be created with a copy of all the files the patcher will modify. This folder will take about 1.2 gigabytes for standard 1.0.2 and about 1.5 gigabytes for Conceptual Edition 1.0.3.
- Run "bo2_patcher.exe".

The patching process takes between 12-15 minutes, so be patient.

## Retuned Mod

This is my personal take on rebalancing the game. Here's a list of changes:

- Bloodsuck animation time reduced: sucking takes 1-2 seconds depending on the NPC, wipe mouth animation only happens 5% of the time.
- All lore moved to Lore Chests.
- Max Kain level is 16 with 500 HP (this is consistent with HP increases until level 15) You can miss a couple lore chests and still reach max level at chapter 11.
- Lore Overflow Bug can't happen.
- Weapon Chest bonus is 2.0 damage instead of 1.25.
- Fury Dark Gift cost reduced from 270 (9 blocks) to 150 (5 blocks).
- Fury and Berserk Dark Gift damage changed for each weapon (previously all weapons had the same damage, including Claws).
- Time to use Charm Dark Gift again reduced from 5 seconds to 1 second.
- Time to use Telekinesis Dark Gift again reduced from 6 seconds to 5 seconds.
- Telekinesis Dark Gift damage increased from 50 to 100.
- Super Jump Dark Gift damage increased from 50 to 100.
- Grab attacks when unnarmed increased from 3 to 5.
- Weapon damage rebalance. Please refer to "docs/Retail-Retuned_damagetable.png" for a comparision table.
- Weapon HP and grab attack number rebalance. Please refer to "docs/Retail-Retuned_damagetable.png" for a comparision table.
- Weapon damage, HP and grab attack number rebalance. The goal was to make fights less tedious when armed and make different weapons feel a bit more different, as in retail version many weapons are more like skins rather than different weapons.
- Polearm can now one-hit kill when using the grabbing final attack.
- NPCs are unchanged, except that they reward no lore.
- All NPCs that crawl away can be killed with a single kick instead of two.
- The Sarafan Knight in the Upper City is stronger, acting like a mini boss of sorts.

How does level progression compare with normal gameplay?

I'm not really sure how to anser this question, but here's a table with the level at the start of each chapter when getting all the Lore Chests compared with some savegames I found at speedrun.com. You are a bit stronger (have more HP) at the beggining, but in my test playtrough I didn't really notice much difference:

Chapter	 1: Speedrun Save  0 / Retuned Mod  0
Chapter	 2: Speedrun Save  3 / Retuned Mod  4
Chapter	 3: Speedrun Save  4 / Retuned Mod  5
Chapter	 4: Speedrun Save  6 / Retuned Mod  7
Chapter	 5: Speedrun Save  7 / Retuned Mod  8
Chapter	 6: Speedrun Save  8 / Retuned Mod  8
Chapter  7: Speedrun Save  9 / Retuned Mod 10
Chapter	 8: Speedrun Save 11 / Retuned Mod 11
Chapter	 9: Speedrun Save 12 / Retuned Mod 12
Chapter	10: Speedrun Save 13 / Retuned Mod 14
Chapter	11: Speedrun Save 14 / Retuned Mod 15

Note: for the Retuned Mod, in the case of chapters 2 you are 2 chests away from level up, and for 3, 6 and 8, 1 chest away.

## To do
- Somehow check all loreperparticle and lore in chests are multiple of one another, to prevent user error. 

## How was this done and how does it work

The retail game does include source files inside its .big files. Using the program Soul Spiral, this files can be accessed. Inside of it there are files named .strmfl, which have plain text description of game's data structures, including some developer comments about default values and what each variable does.

After some investigation it appeared that each game element has its own .tunedata file, which holds these structures inside with the configuration of how that game element should behave. This includes Kain (kain.tunedata), collectables (coll_lore.tunedata, coll_biglore.tunedata, coll_weapon.tunedata), NPCs (i.e. psntcnf.tunedata)... By analizing this structures, the default values (thanks to William Faure, who already made an spreadsheet with all the data) and some debugging with Cheat Engine, I was able to find the values inside the corresponding .tunedata files, thus allowing me to map the data and change them.

This patcher was coded with the goal to automate the process rather than relying on individual patches (i.e. xdelta), which would make them compatible with only one version of the game, and since my goal was to make it compatible with the Conceptual Edition and Spanish versions of the games, as well as with any future mods that may be done. Also, the work was done as groundwork in the case somone willing to finetune/rebalance the game even further doesn't need to start from scratch.

## FAQ
- What versions is this compatible with?
  - It should work with any PC version of the game, be it original DVD release (in any language), GOG or Steam.
- Is it compatible with the Conceptual Edition mod?
  - It is, but you need to install the Conceptual Edition mod BEFORE running the patcher.
- Does this work with the XXX version of the game?
  - The patcher scans trough the .big files searching for the game's .tunedata files. It is most likely that all versions of the game share the same data structures, but the patcher has only been tested with PC versions. It has been untested with PS2 and XBOX versions of the game (XBOX version uses .bgx files, but I don't know if it's just a different extension or a different file format). It will NOT be compatible with the GameCube version, since those files are compressed. If in the future the files can be decompressed and recompressed, then this patcher might be able to also patch the GameCube version.

## Credits
- PebbleInThePond, for making the
- Soul Spiral by Ben Lincoln and Andrew Fradley. Thanks to it this project was possible to be started, and without its source code the patcher wouldn't be able to scan .big files in search of individual files. https://www.thelostworlds.net/Software/Soul_Spiral.html
- William Faure, for his spreadsheet with all NPC and weapon data: https://docs.google.com/spreadsheets/d/13WLfEsONmWsezHlSm_G-9luBS0NHv3Y2/
- Everyone who helped at the Legacy of Kain discord server
