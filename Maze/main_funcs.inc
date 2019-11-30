include funcs.inc

.code

INIT proc
	
	invoke adp_open_window, 800, 800, 0	;opens window with size 800 by 700
	invoke adp_load_image, offset back1, offset bg1	;loads pictures
	invoke adp_load_image, offset back2, offset bg2
	invoke adp_load_image, offset back3, offset bg3
	invoke adp_load_image, offset player.Pic, offset avatar	;player picture
	invoke adp_load_image, offset badGuy.Pic, offset badLoc ;bad guy
	invoke adp_load_image, offset dead, offset deadscreen	;death screen
	invoke adp_load_image, offset winImg, offset winscreen	;victory screen
	invoke adp_load_image, offset heart, offset life	;hearts
	invoke getStartTime ;finds the time of the beginning

	;mov miliTime, 0FFFFFFFFh			;reset hi score to maximum value
	;invoke writeScore


ret
INIT endp


DRAW proc
		
		invoke adp_clear_screen_to_color,0
		invoke drawBG	;draws the background

		invoke timer

		cmp win, 1
		je youWon
			invoke adp_draw_image, offset player.Pic, player.Pos.x, player.Pos.y ;draw the player if didn't win
			invoke adp_draw_image, offset badGuy.Pic, badGuy.Pos.x, badGuy.Pos.y
		youWon:

		invoke drawLives ;draw hearts

		cmp super, 1
		je superMode
			invoke drawdead ;if the player is in super mode the game doesn't check if he touches black
		superMode:

		invoke victory	;checks if won a level

		cmp win, 1
		jne notDone
			invoke whenWin	;if the player won the game activate the function
		notDone:

		
		invoke adp_main	;processes screen

		

				
ret
DRAW endp


UPDATE proc

	invoke masmkeys ;finds keystrokes
	invoke badGuyMove

ret
UPDATE endp