rand 	.ORIG
		TRAP rnd
		BRNZP start
val     .BLKW #1
poo  	.FILL val
start	ST R0,val
		LD R0,val
		TRAP outn

		LDI R0,poo
		TRAP outn

		TRAP halt
	 	.END
