test  .ORIG x0
A     .BLKW #5
B     .FILL x0
C     .EQU A
      ADD R0,R0,x0
      ADD R0,R0,R0
      AND R0,R0,x0
      AND R0,R0,R0
      BR x0
      DBUG
      JSR x0
      JSRR R0,x0
      JMP x0
      JMPR R0,x0
      LD R0,x0
      LDI R0,x0
      LDR R0,R0,x0
      LEA R0,x0
      NOT R0,R0
      RET
      ST R0,x0
      STI R0,x0
      STR R0,R0,x0
      TRAP x0
      .END
