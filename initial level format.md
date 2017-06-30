levels are stored as binary files with little endian encoding.

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
id, rotation

Rotation is one of 3 unsigned char values representing an angle.

1: 0
2: 90
3: 270

Any angle in between will default to 0deg rotation.

# Binary Level Format 04

Everything included in the previous formats. However,

tiles are now stored as
`id, rotation, collision`

Collision is an unsigned char value being either..

0: passable
1: solid

# Binary Level Format 05

Version 05 introduces layer 2 support. Layer 2 is a basic layer that simply draws a secondary tile from a secondary sheet on top of another tile. Layer 2 inherits its collision from its parent tile.

After the tileset filename is another tileset filename which can also be `NONE` meaning no secondary tileset.

Tiles are now stored as:
`id, id_layer2, rotation, collision`

if `id_layer2` is -1, then that secondary layer is not rendered.

so now the format looks like:

First two bytes: MS (my initials)
Short with the version: 0500
string with level name: 'kek'
string with tileset filename: 'textures.png' (loads from res/ dir)
string with layer 2 tileset: 'NONE' or 'textures.png' or another tilesheet
int with width in tiles: 32
int with height in tiles: 32

followed by each of the tiles stored as shown above
