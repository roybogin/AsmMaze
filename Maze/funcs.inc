
.code

										;;;;;;;;;;;Collision Functions;;;;;;;;;;;;;

GetColor PROC colorAddr:DWORD, xxx:DWORD , yyy:DWORD				;;;;;;;;gets the color on xxx, yyy into colorAddr
pusha

 
    invoke adp_get_main_hdc           ;Get the window device context
 
    invoke GetPixel, eax, xxx, yyy   ; Get the color as COLORREF: decimal value to the color's hex into eax
  

    mov ebx, colorAddr	;move the value into colorAddr
    mov [ebx], eax

 
popa
ret
GetColor ENDP


drawdead proc															;;;;;;what happens when you touch black
pusha 

	mov ecx, color2	;checks if the character is touching black
	and color, ecx ;returns 0 if at least one is black
	
	cmp color,0	;if it is not zero, the characterr doesn't touch black
	jne notouch
		
		cmp lives, 1 ;it's 1 because it checks and only the decreases the lives
		jg notdead

			invoke adp_clear_screen_to_color, 0	;if lives are over the character is dead
			mov isDead, 1	;the character is dead
			invoke adp_draw_image, offset dead, 0, 0	;draw the death screen
			jmp notouch

		notdead:
		dec lives	;if lives are not over decrement the number of lives
		mov player.Pos.x, 30	;return to the original position
		mov player.Pos.y, 30
		mov color, 15	;colors will not detect black or green
		mov color2, 15
		invoke Sleep, 500	;wait, the player won't accidentally run into the wall again
	
	notouch:

popa
ret
drawdead endp



victory proc														;;;;what happens when you touch green
pusha

cmp win, 1	;don't need to check if already won
je noWin

	cmp color, GREEN	;checks if the player is touching green
	je youWin

		cmp color2, GREEN	;if both aren't green you didn't win
		jne noWin
				
			youWin:
			inc level	;go to next level
			mov player.Pos.x, 30	;original position
			mov player.Pos.y, 30
			mov badGuy.Pos.x, 400
			mov badGuy.Pos.y, 300
			mov color, 15	;don't detect green
			mov color2, 15
			invoke Sleep, 500	;wait
			jmp noWin
	noWin:

	cmp level, levelNum	;level equals to levelNum if the player won the game
	jne changeWin
		mov win, 1	;if won move 1 into the winning boolean
	changeWin:



popa
ret
victory endp




									;;;;;;;;;;;;;;;;;;;;Time Functions;;;;;;;;;;;;;;;;;




getStartTime proc								;gets the time of the beginning
	pusha
	
	invoke GetSystemTime, offset time	;find the time and puts it into time
	
	mov bx, time.wMilliseconds	;gets the milliseconds
	mov miliStart, bx
	
	mov bx, time.wSecond	;gets the seconds
	mov secStart, bx
	
	mov bx, time.wMinute	;gets the minutes
	mov minStart, bx

	popa
ret
getStartTime endp



getEndTime proc									;gets the time of the ending
	pusha

	invoke GetSystemTime, addr time			;find the time and puts it into time
	
	mov bx, time.wMilliseconds	;gets the milliseconds
	mov miliEnd, bx
	
	mov bx, time.wSecond	;gets the seconds
	mov secEnd, bx
	
	mov bx, time.wMinute	;gets the minutes
	mov minEnd, bx

	popa
ret
getEndTime endp



calcTime proc							;calculate the time difference between the beginning and ending
	pusha
	mov cx, miliStart	;move to miliEnd the difference in milliseconds
	sub miliEnd, cx
	mov cx, secStart	;move to secEnd the difference in seconds
	sub secEnd, cx
	mov cx, minStart	;move to minEnd the difference in minutes
	sub minEnd, cx

	cmp secEnd, 0	;if the second difference is negative take a minute down and add 60 seconds
	jge secOkay
		
		add secEnd, 60
		dec minEnd

	secOkay:

	cmp miliEnd, 0	;if the millisecond difference is negative take a second down and add 1000 milliseconds
	jge miliOkay

		add miliEnd, 1000
		dec secEnd

	miliOkay:

	popa
ret
calcTime endp




timeToString proc					;;;;;;makes a string of the time difference. Format minsec.mili
pusha

	invoke int_to_string, minEnd	;min
	mov ebx, eax

	invoke concat, ebx, reparg(":")	;min
	mov timeString, eax

	pusha
	invoke Free, ebx
	popa

	invoke int_to_string, secEnd
	mov ebx, eax


	invoke concat,timeString , ebx	;minsec
	mov timeString, eax

	pusha
	invoke Free, ebx
	popa

	invoke concat, timeString, reparg(".")		;minsec.
	mov timeString, eax


	invoke int_to_string, miliEnd
	mov ebx, eax


	invoke concat,timeString , ebx	;minsec.mili
	mov timeString, eax

	pusha
	invoke Free, ebx
	popa

popa
ret
timeToString endp



timeToMilliseconds proc					;converts your time to milliseconds
pusha
	
	xor eax, eax
	mov ax, miliEnd
	mov miliTime, eax		;miliseconds

	xor eax, eax
	mov ax, secEnd
	mov ebx, 1000	
	mul ebx
	add miliTime, eax	;1 second = 1000 milliseconds

	xor eax, eax
	mov ax, minEnd
	mov ebx, 60000
	mul ebx
	add miliTime, eax	;1 minute = 60000 milliseconds

popa
ret
timeToMilliseconds endp



milliToString proc		;converts the high score's time in milliseconds to a string with the format min:sec.mil
pusha
	
	xor edx, edx	;edx = 0
	mov eax, hiScore
	mov ebx, 1000
	div ebx	;eax -> total seconds, edx -> needed milliseconds
	mov ebx, eax	;the next functions change eax
	invoke int_to_string, edx
	invoke concat, reparg("."), eax ; .mil
	mov hiScoreString, eax 
	
	xor edx, edx	;edx = 0
	mov eax, ebx ;eax -> total seconds
	mov ebx, 60
	div ebx ;eax ->needed minutes, edx ->needed seconds

	mov ebx, eax	;the next functions change eax
	invoke int_to_string, edx
	invoke concat, eax, hiScoreString	;sec.mil
	invoke concat, reparg(":"), eax	;:sec.mil
	mov hiScoreString, eax
	invoke int_to_string, ebx
	invoke concat, eax, hiScoreString	;min:sec.mil

	mov hiScoreString, eax

popa
ret
milliToString endp


timer proc
pusha

	invoke getEndTime	;calculates your time now
	
	invoke calcTime	;calculates the difference in time

	invoke int_to_string, minEnd
	invoke adp_draw_text, 100,650, eax, 35, 0ffffffh	;writes the minutes
	
	pusha 
	invoke Free, eax
	popa
	
	invoke adp_draw_text, 117,648, reparg(":"), 35, 0ffffffh	;wtites the colon

	cmp secEnd, 10	;checks if the seconds is a single digit or a double digit
	jge twoDig
		invoke adp_draw_text, 125,650, reparg("0"), 35, 0ffffffh	;writes a 0
		invoke int_to_string, secEnd
		invoke adp_draw_text, 141,650, eax, 35, 0ffffffh	;writes the seconds
	
		pusha 
		invoke Free, eax
		popa
	
		invoke adp_draw_text, 157,648, reparg("."), 35, 0ffffffh	;writes the period
	
		invoke int_to_string, miliEnd
		invoke adp_draw_text, 165,650, eax, 35, 0ffffffh	;writes the milliseconds
	
		pusha 
		invoke Free, eax
		popa
	popa
	ret
	twoDig:
	
	invoke int_to_string, secEnd
	invoke adp_draw_text, 123,650, eax, 35, 0ffffffh	;if the number is double digit writes the seconds
	
	pusha 
	invoke Free, eax
	popa
	
	invoke adp_draw_text, 157,648, reparg("."), 35, 0ffffffh	;colon
	
	invoke int_to_string, miliEnd
	invoke adp_draw_text, 165,650, eax, 35, 0ffffffh	;milliseconds
	
	pusha 
	invoke Free, eax
	popa


popa
ret
timer endp





											;;;;;;;;;;;;;;;;Score Functions;;;;;;;;;;;;;;;

readScore proc
local file:HANDLE	;handle to the score file
pusha

	mov file, fopen("score.dat")	;opens the file with the score
	mov eax, fread(file, offset hiScore, 4)	;writes into hiScore the first DWORD in the file (high score)
	fclose file	;closes file

popa
ret
readScore endp



writeScore proc
local file:HANDLE	;handle to the score file
pusha
	

	invoke DeleteFileA, reparg("score.dat")	;deletes the file
	invoke CreateFileA, reparg("score.dat"), GENERIC_READ or GENERIC_WRITE, 0, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0	;opens an empty file with the same name
	mov file, eax	;file is the new file handle
	mov eax, fwrite(file, offset miliTime, 4)	;writes a DWORD into the file (score)
	fclose file	;closes file
	

popa
ret
writeScore endp



cmpScore proc							;sees if hi score is broken

invoke readScore	;returns current hi score into hiScore
mov eax, hiScore	
cmp miliTime, eax	;checks if score is smaller than hiScore
jae noBreak

mov eax, 1	;eax = 1 if the high score was broken
ret

noBreak:

xor eax, eax ;eax = 0 if the high score wasn't broken

ret
cmpScore endp



submitName proc				;writes the name into winner file
local file:HANDLE,lngth:DWORD	;handle to the score file
pusha
	
	invoke DeleteFileA, reparg("winner.dat")	;deletes the file
	invoke CreateFileA, reparg("winner.dat"), GENERIC_READ or GENERIC_WRITE, 0, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0 ;creates a new file with the same name
	mov file, eax	;handle to new file
	invoke adp_textfield_get_text,offset nameScore	;gets text from textfield
	mov playerName,eax	;puts text in playerName
	invoke str_length,eax	;finds the name length
	mov lngth, eax	;lngth is the name's length
	mov eax,fwrite(file,Addr lngth,4)	;writes length of name
	mov eax, fwrite(file, playerName, lngth)	;writes name
	fclose file	;closes file
	invoke adp_textfield_delete, offset nameScore ;deletes button and textfield, writes name instead
	invoke adp_button_delete, offset submit
	invoke writeScore
	

popa
ret
submitName endp


getName proc			;gets the winner's name and score
local file:HANDLE, lngth:DWORD	;handle to the score file and length of name
pusha
	invoke milliToString ;gets the string of the current high score into hiScoreString
	invoke Alloc,512 ; eax = empty 512 bytes in ram
	mov playerName, eax ;playerName is a pointer to the empty space
	mov file, fopen("winner.dat")	;opens the winner's name file
	mov eax, fread(file, Addr lngth, 4)	;puts length of name into lngth
	mov eax, fread(file, playerName, lngth) ;puts name into playerName
	fclose file ;closes the file

	invoke concat, reparg("The high score is "), hiScoreString
	invoke concat, eax, reparg(" and was achieved by ")
	invoke concat, eax, playerName
	mov playerName, eax ;playerName is "The high score is " + high score + " and was achieved by " + player's nam

popa
ret
getName endp






												;;;;;;;;;;;Other Functions;;;;;;;;;;;;;;



drawBG proc								;find the level number and draw the correct background
pusha
	cmp level, 0	
	jne not0
		invoke adp_clear_screen_to_color, 0
		invoke adp_draw_image, offset back1, 0, 0
		jmp funcEnd
	not0:

	cmp level, 1
	jne not1
		invoke adp_clear_screen_to_color, 0
		invoke adp_draw_image, offset back2, 0, 0
		jmp funcEnd
	not1:

	cmp level, 2
	jne not2
		invoke adp_clear_screen_to_color, 0
		invoke adp_draw_image, offset back3, 0, 0
		jmp funcEnd
	not2:
	
	funcEnd:
popa
ret
drawBG endp



drawLives proc								;find how many lives are left and draw them
	
	cmp win, 0	;don't need to draw lives if the player won
	jne dontneed
		cmp lives, 3
		jne no3
			invoke adp_draw_image, offset heart, 100, 595
			invoke adp_draw_image, offset heart, 150, 595
			invoke adp_draw_image, offset heart, 200, 595
		no3:
		cmp lives, 2
		jne no2
			invoke adp_draw_image, offset heart, 100, 595
			invoke adp_draw_image, offset heart, 150, 595
		no2:
		cmp lives, 1
		jne no1
			invoke adp_draw_image, offset heart, 100, 595
		no1:
	dontneed:

ret
drawLives endp



whenWin proc												;what happens when player wins
pusha
	
	invoke adp_clear_screen_to_color, 0	;clear the screen if won
	invoke adp_draw_image, offset winImg, 0, 0 ;if won draw winning screen
		cmp winTime, 0	;checks if found the end time already
		jne alreadyDid
			mov winTime, 1	;don't calculate end time again
			invoke getEndTime	;get the end time
			invoke calcTime	;calculate how much time it took
			invoke timeToString ;puts your time in the format min:sec.mil into timeString
		alreadyDid:
	invoke concat, reparg("your time is:" ), timeString	;prints the player's time
	invoke adp_draw_text, 250,600, eax, 35, 0ffffffh
	invoke Free, eax
	invoke timeToMilliseconds	;gets player's time in milliseconds
	cmp brokeHighScore, 0	;checks if brokeHighScore was found
	jne prntScore
	invoke cmpScore	;checks if high score is broken
	mov brokeHighScore,al	;brokeHighScore is 1 if score was broken and 0 if not
	cmp brokeHighScore, 0
	je noBreak
		invoke adp_create_textfield, offset nameScore, 430, 710, 100, 20;if high score was broken show the textbox and button to write your name in
		invoke adp_create_button, offset submit, 540, 710, 100, 20, reparg("Submit"), submitName
		prntScore:
		invoke adp_draw_text, 200, 650, reparg("YOU BROKE THE HIGH SCORE"), 35, 0ffffffh	;write the broke high score text
		invoke adp_draw_text, 200, 700, reparg("Enter your name: "), 35, 0ffffffh	;tell the player to write his name
		cmp playerName, 0
		je endFunc
		invoke adp_draw_text, 430, 700, playerName, 35, 0ffffffh	;if the player submitted his name, write it on the screen
		jmp endFunc

	noBreak:
	cmp playerName, 0
	jne drawName
		invoke getName
	drawName:
	invoke adp_draw_text, 50, 650, playerName, 35, 0ffffffh	;if the player broke the high score write the current high score and the player that broke it

endFunc:
popa
ret
whenWin endp



masmkeys proc						;moves the character and finds colors for colission detector
pusha
cmp isDead, 1	;check if the player moves only if he is not dead
je noNeed

invoke GetAsyncKeyState, 40; down
cmp eax,0
je notDown
	
	add player.Pos.y, Speed	;move the player


	mov eax, player.Pos.x	;check colors that his corners touch
	mov ebx, player.Pos.y

	add ebx, player.side
	dec ebx

	invoke GetColor, offset color, eax, ebx	
	
	add eax, player.side
	dec eax
		
	invoke GetColor, offset color2, eax, ebx


notDown:

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

invoke GetAsyncKeyState, 38 ;up
cmp eax,0
je notUp

	sub player.Pos.y, Speed	;move the player


	mov eax, player.Pos.x	;check colors that his corners touch
	mov ebx, player.Pos.y

	invoke GetColor, offset color, eax, ebx
		
	add eax, player.side
	dec eax
		
	invoke GetColor, offset color2, eax, ebx


notUp:

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

invoke GetAsyncKeyState, 37; left
cmp eax,0
je notLeft
	sub player.Pos.x, Speed	;move the player


	mov eax, player.Pos.x	;check colors that his corners touch
	mov ebx, player.Pos.y

	add ebx, 2

	invoke GetColor, offset color, eax, ebx
	
	add ebx, player.side
	sub ebx, 2
		
	invoke GetColor, offset color2, eax, ebx


notLeft:

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


invoke GetAsyncKeyState, 39 ;right
cmp eax,0
je notRight

	add player.Pos.x, Speed	;move the player

													
	mov eax, player.Pos.x	;check colors that his corners touch
	mov ebx, player.Pos.y

	inc ebx
	add eax, player.side
	dec eax



	invoke GetColor, offset color, eax, ebx
	
	add ebx, player.side
	sub ebx, 2
		
	invoke GetColor, offset color2, eax, ebx

notRight:

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
noNeed:

invoke GetAsyncKeyState, 82 ;R - restart button
cmp eax,0
je notR
	
	mov isDead, 0	;not dead anymore
	mov lives, 3	;reset lives
	mov level, 0	;go to first level
	mov super, 0	;deactivate super mode
	mov color, 15	;colors won't detect anything
	mov color2, 15
	mov player.Pos.x, 30	;go to original position
	mov player.Pos.y, 30
	mov badGuy.Pos.x, 400
	mov badGuy.Pos.y, 300
	invoke getStartTime		;reset the time

notR:

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

invoke GetAsyncKeyState, 67 ;C - super mode
cmp eax,0
je notC

	xor super, 1	;super mode is what super mode wasn't before
	invoke Sleep, 500	;wait

notC:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

popa
ret
masmkeys endp



badGuyMove proc	;moves the bad guy
local counter:DWORD
pusha


	mov counter, 0	;resets counter

	start:
	invoke getEndTime	;gets the time now (random)

	xor dx, dx	;dx = 0
	mov ax, miliEnd
	add ax, secEnd
	add ax, minEnd	;ax = milliseconds + seconds + minutes
	mov bx, 7
	div bx	;devides by 7 (only care about the remainder)



	cmp dx, 0	;if the remainder is 0
	jne not0
		
		mov ecx, badGuy.Pos.x
		add ecx, badGuy.side
		cmp ecx, 780	;if the bad guy isn't next to right wall
		jge start


		add badGuy.Pos.x, Speed	;move the bad guy to the right


		pusha	;checks what color the bad guy touches (if it's blue)
		mov eax, player.Pos.x
		mov ebx, player.Pos.y

		inc ebx
		add eax, player.side
		dec eax

		invoke GetColor, offset Bcolor, eax, ebx
	
		add ebx, player.side
		sub ebx, 2
		
		invoke GetColor, offset Bcolor2, eax, ebx
		popa



		inc counter	;add to the counter
		cmp counter, 10	;run 10 times
		jne start

	not0:

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	cmp dx, 1	;is remainder 1
	jne not1

		mov ecx, badGuy.Pos.x
		cmp ecx, 20	;the bad guy is not next to left wall
		jle start

		sub badGuy.Pos.x, Speed

		pusha	;collision
		mov eax, player.Pos.x
		mov ebx, player.Pos.y

		add ebx, 2

		invoke GetColor, offset Bcolor, eax, ebx
	
		add ebx, player.side
		sub ebx, 2
		
		invoke GetColor, offset Bcolor2, eax, ebx
		popa

		inc counter
		cmp counter, 10
		jne start
	not1:

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	cmp dx, 2	;remainder 2
	jne not2

		mov ecx, badGuy.Pos.y
		add ecx, badGuy.side
		cmp ecx, 580
		jge start

		add badGuy.Pos.y, Speed
		pusha

		mov eax, player.Pos.x	;colission
		mov ebx, player.Pos.y

		add ebx, player.side
		dec ebx

		invoke GetColor, offset Bcolor, eax, ebx	
	
		add eax, player.side
		dec eax
		
		invoke GetColor, offset Bcolor2, eax, ebx
		popa


		inc counter
		cmp counter, 10
		jne start
	not2:

	;;;;;;;;;;;;;;;;;;;;;;;;;;

	cmp dx, 3	;check remiander
	jne not3

		mov ecx, badGuy.Pos.y
		cmp ecx, 20
		jle start

		sub badGuy.Pos.y, Speed

		
		pusha
		mov eax, player.Pos.x	;check colors that his corners touch
		mov ebx, player.Pos.y

		invoke GetColor, offset color, eax, ebx
		
		add eax, player.side
		dec eax
		
		invoke GetColor, offset color2, eax, ebx
		popa

		inc counter
		cmp counter, 10
		jne start
	not3:


popa
ret
badGuyMove endp


isBlue proc

	cmp Bcolor, BLUE	;checks if bad guy touches blue (player)
	je touch

		cmp Bcolor2, BLUE	;if both aren't blue you didn't touch him
		jne notouch
		
		touch:
		cmp lives, 1 ;it's 1 because it checks and only then decreases the lives
		jg notdead

			invoke adp_clear_screen_to_color, 0	;if lives are over the character is dead
			mov isDead, 1	;the character is dead
			invoke adp_draw_image, offset dead, 0, 0	;draw the death screen
			jmp notouch

		notdead:
		dec lives	;if lives are not over decrement the number of lives
		mov player.Pos.x, 30	;return to the original position
		mov player.Pos.y, 30
		mov color, 15	;colors will not detect black or green
		mov color2, 15
		invoke Sleep, 500	;wait, the player won't accidentally run into the wall again

	notouch:

ret
isBlue endp