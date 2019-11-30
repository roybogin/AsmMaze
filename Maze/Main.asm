include \masm32\include\masm32rt.inc
include adp_gui.inc
includelib adp_gui.lib
include heinemannlib.inc
includelib heinemannlib.lib
include data.inc
include main_funcs.inc

.code

main proc

	invoke INIT

	main_loop:
		invoke DRAW
		invoke UPDATE
	jmp main_loop


ret
main endp

end main