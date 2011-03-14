; Invalid Branch Operation
Prog .ORIG
HALT .EQU x25
Begin BRNZPR Begin ; This shouldnt be valid
     .END Begin
