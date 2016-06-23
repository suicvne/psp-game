# Binary Level Format 01

First two bytes: MS (my initials)
Short with the version: 0001
string with name: 'kek'
int with width in tiles: 32
int with height in tiles: 32
the tiles as their ids as shorts: 1010022 etc

to come: differentiating between tilesets

# Binary Level Format 02

First two bytes: MS (my initials)
Short with the version: 0003
string with name: 'kek'
string with tileset filename: 'textures.png' (loads from res/ dir)
int with width in tiles: 32
int with height in tiles: 32
the tiles as their ids as shorts: 1010022 etc

Lua Script Design

Each level will have an optional Lua script assosciated with them that is named "script.lua".
There will exist one single Lua state contained in the globals as there will always only be one level open
at a time.

WIP Lua Reference:

function onLoad() - called when the level file is initially loaded.
function onUpdate() - called whenever the level's `_update` function is called.

# Binary Level Format 03

First two bytes: MS (my initials)
Short with the version: 0003
string with name: 'kek'
string with tileset filename: 'textures.png' (loads from res/ dir)
int with width in tiles: 32
int with height in tiles: 32

tiles stored as
id, rotation (both shorts)
instead of ids one after the other
