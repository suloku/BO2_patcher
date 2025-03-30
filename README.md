# Blood Omen 2 Patcher

##Version 0.1 by suloku '25

This is a simple patcher for the game Blood Omen 2.

If you are interested in rebalancing/fine tunning the game by changing Kain/NPC behavior, enemies HP, weapon damage... please contact me, I'll gladly help to the best of my ability, there are a lot of configurable parameters.

The retail version of the game feels slow and repetitive. This is mainly due to Lore (experience points) being tied to the blood sucking mechanich. This makes it mandatory to kill and suck every single enemy and NPC if you want to level up. This was probably done to enhance the vampiric experience and wreak havoc in all the game areas, but turns it into a chore. The fact that every time after blood sucking a ~2 second animation of Kain wiping his mouth plays doesn't help.

This patcher aims to mod the game to make the gameplay experience more straightforward. This is mainly acomplished by removing Lore being tied to NPCs and moving all possible Lore to the games collectibles: Red Coffers and Blue Coffers.


##Patcher features
Here's a list of all the changes the patcher will currentyle make

- NPCs give no lore (except the two tutorial chained prisoners, as they are mandatory as the game requires kain to level up to proceed).
- Red Coffers have 75 Lore.
- Blue Coffers have 125 Lore.
- You get the same amount of particles as retail when "sucking" Lore coffers (3 and 5)
- You can easily get to level 15 (480 hp) even when missing some coffers. Level 15 seems to be the intended level you would have end-game for casual retail gameplay.
- Max level is now level 16. You need ALL Lore coffers to reach level 16. HP for this level has been changed from 485 to 500 (though to be consistent with the progresion the game keeps trough all level ups it should be 510, but the devs made 500 the maximum at level 19)
- Note: Once you reach level 16 the Lore bar will "glitch", but will disappear on reload/respawn. This is due the Lore to the next level being set to MAX, which is virtually impossible to do. This prevents the Lore overflow bug from happening (a bug involving exploiting checkpoints to get more Lore). Though currently if you reach level 16 there's no more lore to collect, not having the lore bar displayed is a good indicator of Kain having reached max level.
- Changes the chance of Kain wiping his mouth after sucking blood from 100% to 5% (so it only happens rarely instead of completely removing the animation).
- All NPCs that crawl away from you can be killed with one kick instead of two.

##Usage
- Place "bo2_patcher.exe" in the game's installation folder (where "bo2.exe" and "data" folders are).
- Optional: run "backup_files.bat". A folder named "data_backup" will be created with a copy of all the files the patcher will modify. This folder will take about 1.2 gigabytes for standard 1.0.2 and about 1.5 gigabytes for Conceptual Edition 1.0.3.
- Run "bo2_patcher.exe".

The patching process takes between 12-15 minutes, so be patient.

##To do
- Add configuration file support (priority).
- Add possibility to modify all of Kain's Lore and HP values for each level.

Maybe:
- Add more Kain's configurable values (i.e. thirst drain parameters).
- Add more NPC configurations

##How was this done and how does it work

The retail game does include source files inside its .big files. Using the program Soul Spiral, this files can be accessed. Inside of it there are files named .strmfl, which have plain text description of game's data structures, including some developer comments about default values and what each variable does.

After some investigation it appeared that each game element has its own .tunedata file, which holds these structures inside with the configuration of how that game element should behave. This includes Kain (kain.tunedata), collectables (coll_lore.tunedata, coll_biglore.tunedata, coll_weapon.tunedata), NPCs (i.e. psntcnf.tunedata)... By analizing this structures, the default values (thanks to William Faure, who already made an spreadsheet with all the data) and some debugging with Cheat Engine, I was able to find the values inside the corresponding .tunedata files, thus allowing me to change them.

This patcher was coded with the goal to automate the process rather than relying on individual patches (i.e. xdelta), which would make them compatible with only one version of the game, and since my goal was to make it compatible with the Conceptual Edition and Spanish versions of the games, as well as with any future mods that may be done. Also, the work was done as groundwork in the case somone willing to finetune/rebalance the game even further doesn't need to start from scratch.

##FAQ
- What versions is this compatible with?
 - It should work with any PC version of the game, be it original DVD release (in any language), GOG or Steam.
- Is it compatible with the Conceptual Edition mod?
 - It is, but you need to install the Conceptual Edition mod BEFORE running the patcher.
- Does this work with the XXX version of the game?
 - The patcher scans trough the .big files searching for the game's .tunedata files. It is most likely that all versions of the game share the same data structures, but the patcher has only been tested with PC versions. It has been untested with PS2 and XBOX versions of the game (XBOX version uses .bgx files, but I don't know if it's just a different extension or a different file format). It will NOT be compatible with the GameCube version, since those files are compressed. If in the future the files can be decompressed and recompressed, then this patcher might be able to also patch the GameCube version.

##Credits
- PebbleInThePond, for making the
- Soul Spiral by Ben Lincoln and Andrew Fradley. Thanks to it this project was possible to be started, and without its source code the patcher wouldn't be able to scan .big files in search of individual files. https://www.thelostworlds.net/Software/Soul_Spiral.html
- William Faure, for his spreadsheet with all NPC and weapon data: https://docs.google.com/spreadsheets/d/13WLfEsONmWsezHlSm_G-9luBS0NHv3Y2/
- Everyone who helped at the Legacy of Kain discord server
