; Out of bounds .EQU hex string
Poop 	.ORIG 			; This is invalid.
gAH		.EQU	x10000
Begin 	BRNZP	Begin
		.END	Begin
