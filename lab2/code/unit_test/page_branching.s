; This program attempts to branch to a different page
Test02   .ORIG   x0 ; 1501
Begin    AND     R0,R0,#0       ; Bitmask R0 to all zeroes
         .BLKW   x7CC
         TRAP    rND
         BRZP    Begin          ; If the random number generated was non-negative, branch
         TRAP    HALT           ; Otherwise, halt
HALT     .EQU    x25
rND      .EQU    x43
         .END    Begin
