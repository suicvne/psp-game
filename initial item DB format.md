the item database in the game is going to be ridiculously simple.

# Item Database Format 01

* First two bytes: MS
* Short with the version: 0001
* String: "ITEMS"
  * this is to differentiate between the level format and the item db format.
  
Items themselves are relatively simple. Their effects are implemented lua side (so all that C knows is their basic information and loading/saving).

Item Struct
* short: item ID (corresponds to the items.png sheet)
* string: Item Name
* string: Item Description

that's it, i dont know what more you expected
