const char *levels_bigfile_names[] = {
    "cvarena.big",    // bossrooms/charmvampirearena - Boss battle with Marcus in the Cathedral
    "jvarena.big",    // bossrooms/jumpvampirearena - Boss battle with Faustus
    "jvarena01.big",  // bossrooms/jumpvampirearena - The Jump Dark Gift instruction area just after the battle with Faustus
    "mvarena.big",    // bossrooms/madvampirearena - Boss battle with Marcus in the Eternal Prison
    "slarena.big",    // bossrooms/sarafanlordarena - Boss battle with the Sarafan Lord in the Hylden Gate
    "svarena.big",    // bossrooms/speedvampirearena - Boss battle with Sebastian in the Industrial Quarter
    "cy01.big",       // canyons - Very beginning of the Canyons level
    "cy01a.big",      // canyons - Mist-kill-two swordswomen area
    "cy02.big",       // canyons - The rockfall/axe-chick destroys the bridge cave
    "cy02a.big",      // canyons - Cave that leads to the warehouse area
    "cy03.big",       // canyons - The wagon ambush area
    "cy04.big",       // canyons - First demon encounter area
    "cy04a.big",      // canyons - Midway demon-infested area just past the double-doorway in cy04
    "cy04a1.big",     // canyons - The double-doorway between cy04 and cy04a
    "cy04b.big",      // canyons - Minecart puzzle area
    "cy05.big",       // canyons - Two cranes puzzle area
    "cy06.big",       // canyons - The ghost town
    "cy07.big",       // canyons - The Seer's house
    "cydv.big",       // canyons - The outside of the Device + Kain meets The Beast
    "devicesanctuary.big", // device - Sanctuary after completing the Device level
    "dv01.big",       // device - Kain returns to The Beast
    "dv02.big",       // device - The first bridge over lava
    "dv03.big",       // device - The corridor down to the big mechanism rooms puzzle area
    "dv04.big",       // device - The first "Hylden deploys a demon" hallway
    "dv05.big",       // device - The "Hylden deploys a warrior Hylden" hallway
    "dv06.big",       // device - The entrance to the Hylden baby factory
    "dv07.big",       // device - The second "Hylden deploys a demon" hallway
    "dv08.big",       // device - The heart-box assembly line room
    "dv09.big",       // device - The elevator down to The Mass
    "dv10.big",       // device - The Janos-encaged room
    "ep01.big",       // eternal prison - The intro to the Eternal Prison level
    "ep02.big",       // eternal prison - The first chamber of the Eternal Prison
    "ep04.big",       // eternal prison - The Mines of Moria stairs chamber of the Eternal Prison
    "ep05.big",       // eternal prison - The free-floating-in-space mechanism portion of the Eternal Prison
    "ep06.big",       // eternal prison - The "lower boxes from the ceiling" room of the EP
    "ep07.big",       // eternal prison - The "timewarp the bridge so it doesn't get broken" room of the EP
    "ep08.big",       // eternal prison - The "torsos on hooks" room of the EP
    "ep09.big",       // eternal prison - The "charm someone down below to open the doorway" room
    "ep10.big",       // eternal prison - The outside battlements of the EP
    "ep11.big",       // eternal prison - The "timewarp machine in flames" room of the EP
    "ep12.big",       // eternal prison - The burning crosses maze
    "hubworld.big",   // hubworld - The Bonus Mode level-select room
    "hc_01.big",      // hyldencity - The intro cinematic for the Hylden City
    "hc_04.big",      // hyldencity - The elevator down to the main part of the City
    "hc_04b.big",     // hyldencity - The "and now… we take it back" room
    "hc_04c.big",     // hyldencity - The "fry the Invid demon" room
    "hc_04d.big",     // hyldencity - Just past the first water elevator puzzle
    "hc_04e.big",     // hyldencity - Deleted area? No terrain in default location…
    "hc_04f.big",     // hyldencity - Deleted area? Black screen, no response to controls
    "hc_05.big",      // hyldencity - The floating boxes puzzles area
    "hc_05b.big",     // hyldencity - The Hylden murals room
    "hc_05c.big",     // hyldencity - The jump down into mist hallway
    "hc_05d.big",     // hyldencity - Just past the poison-gas pillars jumping puzzle
    "hc_05e.big",     // hyldencity - The forcefield-and-examination-table puzzle room
    "hc_05f.big",     // hyldencity - The "release humans to distract the Invid demon" hallway
    "hc_05g.big",     // hyldencity - The cargo shuttlecraft bay
    "hc_06.big",      // hyldencity - The well-light pool room
    "hc_07.big",      // hyldencity - The bridge to the Hylden Gate
    "hc_10.big",      // hyldencity - The other cargo shuttlecraft bay
    "iq_01.big",      // industrial quarter - The first building of the Industrial Quarter
    "iq_01a.big",     // industrial quarter - The "sounds like fighting below, and then a thump!" room
    "iq_02.big",      // industrial quarter - Just before the "These humans never look twice at me" cutscene
    "iq_03.big",      // industrial quarter - The mist-kill/old-school radio program area
    "iq_04.big",      // industrial quarter - The tram station, near side
    "iq_05.big",      // industrial quarter - The tram station, far side
    "iq_06.big",      // industrial quarter - The ladder up to the grate walkway w/waterfalls
    "iq_07.big",      // industrial quarter - High walkway with view of industrial metropolis
    "iq_08.big",      // industrial quarter - The first possession-through-windows sequence
    "iq_09.big",      // industrial quarter - The mine tunnel with falling rocks
    "iq_10.big",      // industrial quarter - The second possession-through-windows sequence
    "iq_11.big",      // industrial quarter - The large room w/minecar tracks
    "iq_12.big",      // industrial quarter - The blow-up-the-minecar room
    "iq_intro.big",   // industrial quarter - Introduction cutscene for the Industrial Quarter
    "after_iq.big",   // industrial quarter/cinematics - Unknown, presumably a cutscene after Kain defeats Sebastian
    "sanctuary_keep.big", // keep - Unknown, presumably Sanctuary with Vorador directing Kain to the Keep
    "sk01.big",       // keep - Intro to the Sarafan Keep level
    "sk02.big",       // keep - Just inside the Sarafan Keep
    "sk03a.big",      // keep - The Sarafan Symbol shadows hallway
    "sk03b.big",      // keep - More of sk03a
    "sk03c.big",      // keep - The empty pool w/bones
    "sk04.big",       // keep - Storage room
    "sk05.big",       // keep - The main interior of the Keep
    "sk06.big",       // keep - The balcony above the room with mass knights
    "sk07.big",       // keep - Spiral staircase room
    "kaintakesumahtosanctuary.big", // keep/cinematics - cutscene - Kain returns to Sanctuary with Umah
    "lc01.big",       // lowercity - Intro to the Lower City
    "lc02a.big",      // lowercity - Alleyway to the "town square" area
    "lc02airlock.big", // lowercity - Replace-the-broken-glyph-battery puzzle
    "lc02b.big",      // lowercity - "You need the password to enter!" alley
    "lc03.big",       // lowercity - Warehouse area
    "lc04.big",       // lowercity - In the sewers
    "lc05.big",       // lowercity - In the alley outside the guillotine and playhouse
    "lc05a.big",      // lowercity - Outside the City Reformatory
    "lc06.big",       // lowercity - The sewers part 2
    "lc07.big",       // lowercity - Just before the mini-gang-war
    "lc10.big",       // lowercity - The water/bridge/box puzzle room
    "kainmeetsvorador.big", // lowercity/cinematics - Sanctuary - empty but explorable even on PC
    "sanctuary.big",  // sanctuary - Sanctuary - empty, stuck outside invisible barriers
    "sl01.big",       // slums - First area of the game
    "sl02.big",       // slums - Just before the first spiked gate that closes behind you
    "sl03.big",       // slums - Just before the cutscene about water
    "sl04.big",       // slums - Umah teaches you how to fight
    "sl05.big",       // slums - Learn how to shift to mist
    "sl06.big",       // slums - Moored-boats area of the Slums
    "sl07.big",       // slums - Turn-off-steam puzzle
    "sl08.big",       // slums - Learn to use ladders
    "sd01.big",       // smugglersden/regionfiles - Intro to the Smuggler's Den
    "sd02.big",       // smugglersden/regionfiles - Inner portion of the Smuggler's Den
    "sd03.big",       // smugglersden/regionfiles - Just above the wine cellar
    "sd04.big",       // smugglersden/regionfiles - Just outside the tavern
    "sd05.big",       // smugglersden/regionfiles - The cathedral area
    "sd06.big",       // smugglersden/regionfiles - Elevator to the industrial part of the level
    "sd07.big",       // smugglersden/regionfiles - Industrial part of the level, part 2
    "uc01a.big",      // uppercitynew - Intro to the Upper City
    "uc01b.big",      // uppercitynew - Rotating walkway puzzle area
    "uc02a.big",      // uppercitynew - "conduct me to the Bishop's manner on the instant!" cutscene
    "uc02a2.big",     // uppercitynew - Chasing Marcus to the Cathedral
    "uc02b.big",      // uppercitynew - Move box to reach ladder room
    "uc02b2.big",     // uppercitynew - Some kind of crazy game editing mode, only the sky and some weapons are visible…
    "uc03a.big",      // uppercitynew - First? Glyph Knight fight
    "uc03a2.big",     // uppercitynew - First Armoured Knight battle
    "uc03b.big",      // uppercitynew - Black waterfall puzzle
    "uc04a.big",      // uppercitynew - Statues in pools puzzle
    "uc04b.big",      // uppercitynew - Next to the glyph energy conversation people
    "uc05.big",       // uppercitynew - Above the searchlight area
    "uc06a.big",      // uppercitynew - "We were told to be on the lookout for a vampire!" cutscene
    "uc06b.big",      // uppercitynew - Inside the Bishop's manor
    "uc07.big",       // uppercitynew - The entrance to the cemetary
    "uc07b.big",      // uppercitynew - Meet the Bishop
    "wh01.big",       // wharves/regionfiles - Intro to the Wharves
    "wh02.big",       // wharves/regionfiles - Outside on the docks, near the lighthouse puzzle
    "wh03.big",       // wharves/regionfiles - Outside on the docks, across from wh02
    "wh04.big",       // wharves/regionfiles - Break the floor with a box puzzle
    "wh05.big",       // wharves/regionfiles - Outside on the docks, Armoured Knight round the way
    "wh08.big",       // wharves/regionfiles - Just inside the door guarded by the two caged crossbow girls
    "wh09.big",       // wharves/regionfiles - Box drops on thugs
    "wh10.big",       // wharves/regionfiles - Double Glyph Knight battle room
    "wh11.big",       // wharves/regionfiles - Out on the docks with the gunboat
    "wh12.big",       // wharves/regionfiles - Inside the tavern
    "wh13.big"        // wharves/regionfiles - Outside on the docks again
};

size_t levelBigFileCount = sizeof(levels_bigfile_names) / sizeof(levels_bigfile_names[0]);
