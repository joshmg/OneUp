; Invalid Branch Qualifiers
Prog .ORIG
HALT .EQU x25
Begin BRG Begin ; This shouldnt be valid
     .END Begin
