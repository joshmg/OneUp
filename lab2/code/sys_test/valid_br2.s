; Invalid Branch Qualifiers
Prog .ORIG
HALT .EQU x25
Begin BR Begin ; This should be valid
     .END Begin
