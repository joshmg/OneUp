Prog .ORIG x3020
HALT .EQU x25
Begin LD R0,N 		;R0 <- #13
	  LD R1,=#16 	;R1 <- #16
	  ST R0,Ans 	;M[Ans]<-R0
      Trap HALT
N 	  .FILL #13
Ans   .BLKW #1
      .END Begin
