rand 	.ORIG
		TRAP rnd
val     .BLKW #1
pop  	.FILL val
		ST R0,val
		LD R0,val
		TRAP outn

		LDI R0,poo
		TRAP outn

		TRAP halt
	 	.END
