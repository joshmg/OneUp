; Out of bounds hex string
Poop 	.ORIG 	x10000 ; This is invalid.
Begin 	BRNZP	Begin
		.END	Begin
