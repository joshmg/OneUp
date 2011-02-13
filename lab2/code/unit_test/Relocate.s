Prog   .ORIG
X      .FILL	Y
Halt   .EQU	x25
Start  LD 	R1,X
       ST	R1,Y
       TRAP	Halt
Y      .BLKW	#1
       .END     START
