# rundot-blinks-ggj20201

> Its like minesweeper, but the theres just one mine, and the mine moves.

## Overview

* Players: 1
* Blinks Required: 6
* Blinks Recommended: 12+

One evil blink out there is a mine hiding amongst the crowd. Hunt it down by tapping around to trap it and win. Just don't tap on the mine while your hunting and blow the whole thing up.

## Instructions

### Set Things Up

* Join all your blinks into a connected field and then **long press** one of them until it blinks white to reset the field.
* The field should now be pulsing white.

* **Double tap** any one of them to spawn a random mine. The blinks will spin white while they work and then pulse blue when done.
  * **Long press** at anytime to reset the game.

### Hunt Down The Evil Blink

* **Double Tap** any blink currently unchecked (blue).
* The checked blink will spin in anticipation.
* If the blink checked was evil, you loose and the whole board will spiral red in turmoil.
  * Take a deep breath, focus, and **long press** any blink to start over
* If the blink checked was a normal, law abiding blink, it will point in the direction of the evil blink.
  * The face nearest the evil blink will turn yellow (the rest green)
  * If two faces are equally distant, they will both turn yellow

### Evil Blink On The Move

* Every time you check a blink, if the evil blink can, it WILL ALWAYS move to a neighbor
  * It cannot moved to checked blinks
  * The last blink checked always points to this new location
* Every time you check a blink, the last blink checked (that one that was pointing to the evil blink) will stop pointing and become a boring, safe green checked blink.

### Right Where You Wanted Em'

If, after a blink is checked, the evil blink has no where to run to, its trapped! You Win! The board will now spin in green bliss. **Long press** any blink to end the celebration and start all over again.

## Debugging

This is a jam project after all. If you see something odd like a glitch or odd evil blink behavior you can switch the blink colors to debug mode to help troubleshoot (and or cheat).

In the [globals.h](main/globals.h) header. Uncomment the following line:

```C
// #define GLOBALS_DEBUG 
```

Now the evil blink lights up red and all the blinks point to it, all the time, always.
