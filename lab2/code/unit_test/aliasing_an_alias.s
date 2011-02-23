;Should be valid
Lab2EG   .ORIG x0
count    .FILL   #4
Begin    LD      ACC,count
ACC2     .EQU    #1
ACC      .EQU    ACC2              ; This should be okay.
         .END    Begin
