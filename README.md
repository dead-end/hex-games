# hex-games

## Hex Fields

Hex fields in ncurses have to be constructed with characters. A sinlge field 
consists of 12 characters. A character is not a square, so the resulting hex 
field will be stratched.

![Hex fields](res/hex-fields.png)

The hex fields can be arragnged in two ways, as shown in the following image. 
The first image has an up / down movement while the second one has a left / right
movement. In the following considerations I will prever the up / down movement.

![Hex fields](res/hex-field-arrangements.png)

Unlike the previous imapge, a character in ncurses has no border, so you cannot
tell adjacent hex fields with the same color. One solution is to use different
colors for adjacent hex fields.

![Hex fields with 3 colors](res/hex-fields-3-colors.png)