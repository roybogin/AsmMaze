
.data

;;;;;;;;;;;;;Structures;;;;;;;;;;;;;;;;;

Chara STRUCT	;the player that you play as
	Pic Img <> ;player image
	Pos POINT <> ;player position
	side DWORD 30 ;player size
Chara ENDS


;;;;;;;;;;;;;;Player;;;;;;;;;;

player Chara <<>, <30,30>> ;player

badGuy Chara <<>, <400,300>> ;bad guy


;;;;;;;;;;;;;;;;;Picture Locations;;;;;;;;;;;;;;;

avatar byte "C:\Users\roybo\source\repos\Maze\Maze\player.bmp", 0	;the player location

bg1 byte "C:\Users\roybo\source\repos\Maze\Maze\bg\maze1.bmp", 0	;first background location

bg2 byte "C:\Users\roybo\source\repos\Maze\Maze\bg\maze2.bmp", 0 ;second background location

bg3 byte "C:\Users\roybo\source\repos\Maze\Maze\bg\maze3.bmp", 0 ;third background location

life byte "C:\Users\roybo\source\repos\Maze\Maze\heart.bmp", 0 ;heart

winscreen byte "C:\Users\roybo\source\repos\Maze\Maze\bg\win.bmp", 0 ;victory screen

deadscreen byte "C:\Users\roybo\source\repos\Maze\Maze\bg\dead.bmp", 0 ;death screen

badLoc byte "C:\Users\roybo\source\repos\Maze\Maze\bad.bmp", 0 ;bad guy


;;;;;;;;;;;;;;Image Instances;;;;;;;;;;;;;;

winImg Img<> ;victory

dead Img <> ;death

back1 Img <> ;first background

back2 Img <> ;second background

back3 Img <> ;third background

heart Img<>;lives


;;;;;;;;;;;;Collision Detector;;;;;;;;;;;;;;;;

color DWORD 15 ;15 is inital value (doesn't activate anything)

color2 DWORD 15 ;two colors for each corner

GREEN equ 5026082 ;color of wining

BLUE equ 14847	;color of player

Bcolor DWORD 15	;colors that the bad guy detects

Bcolor2 DWORD 15


;;;;;;;;;;;;;;Booleans;;;;;;;;;;;;

super byte 0 ;is the player on super mode

win byte 0 ;did the player win the game

winTime byte 0 ;do you need to calculate time of end (you need to calculate only once)

isDead byte 0 ;is the player dead

brokeHighScore byte 0 ;did the player break the high score


;;;;;;;;;;;;;;;Time;;;;;;;;;;;

time SYSTEMTIME<?> ;get the time in year, month, day, hour, minute, second, millisecond

miliStart WORD 0 ;milliseconds of start
secStart WORD 0 ;seconds of start
minStart WORD 0 ;minutes of start

miliEnd WORD 0 ;miliseconds of end (becomes difference in milliseconds)
secEnd WORD 0 ;seconds of end (becomes difference in seconds)
minEnd WORD 0 ;minutes of end (becomes difference in minutes)

miliTime DWOrD 0	;time difference in milliseconds (for comparing time)


;;;;;;;;;Time String;;;;;;;;;;;

timeString DWORD ?	;min:sec.mil -> time it took the player to win

hiScoreString DWORD ? ;min:sec.mil -> time of the high score


;;;;;;;;;;;;Other;;;;;;;;;;;;

level byte 0 ;current level

levelNum equ 3 ;number of levels

lives byte 3 ;number of lives

Speed equ 1 ;player speed

hiScore DWORD 0 ;highest score achieved (read from file)

nameScore TextField<> ;text field to enter your name into if you have won

submit Button<> ;button to submit name

playerName DWORD ? ;name of players

