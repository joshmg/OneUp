; Invalid Branch Qualifiers
Prog .ORIG
HALT .EQU x25
Begin BR Begin ; This shouldnt be valid
     .END Begin
