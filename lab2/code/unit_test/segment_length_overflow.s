; Object File Length too large...
Lab2EG 	.ORIG 	x30B0
count 	.BLKW 	xFFFF
Begin 	LD		R1,count			;R1 <- 4
		LEA		R0,msg
loop	TRAP	x22						;print "hi! "
		ADD		R1,R1,#-1		;R1--
		BRP		loop
		JMP		Next
msg		.STRZ	"hi! "
Next	AND		R0,R0,x0		;R0 <- 0
		NOT 	R0,R0				;R0 <- xFFFF
		ST		R0,Array			;M[Array] <- xFFFF
		LEA		R5,Array
		LD		R6,=#100			;R6 <= #100
		STR		R0,R5,#1		;M[Array+1] <= xFFFF
		TRAP	x25
ACC		.EQU	#1
; ----- Scratch Space -----
Array	.BLKW 	#3
		.FILL	x10
		.END	Begin
