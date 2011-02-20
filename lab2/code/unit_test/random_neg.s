; This short program counts how many random numbers are generated before a
; negative is produced.
Test01	 .ORIG   x30B0
inc	 .FILL   #1
Begin	 AND	 R1,R1,#0	; Bitmask R1 to all zeroes
Loop  TRAP	 rND
         BRN	 End		; Branch to end of program if the N CCR is set
	 ADD	 R1,R1,inc	; Increment R1
	 BRNZP	 Loop		; Unconditionally branch back to Loop
End	 AND	 R0,R0,x1F	; Bitmask R0 to all ones
	 AND	 R0,R0,R1	; Copy the value of R1 into R0
	 TRAP	 OUTN		; Print R0 to the console as a decimal integer
	 TRAP	 HALT
HALT	 .EQU	 x25
rND	 .EQU	 x43
	 .END	 Begin
