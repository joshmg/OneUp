;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;  File:       run_sqrt.s    ;;;
;;;  Written by: Andrew Groot  ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; A program to prompt the user for
; a number and display the square root.
; Requires linking with math.s.
; .MAIN may need to be removed.
        .MAIN
RUNSQT  .ORIG
        .EXT SQRT,PRESQRT,PSTSQRT
; Get value from user
start   
; Get square root

; Print answer

        .END
