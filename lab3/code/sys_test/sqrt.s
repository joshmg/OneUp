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
runsqt  .ORIG
        .EXT     SQRT,PRESQRT,PSTSQRT
; for imaginary roots
imgnry   .STRZ  "i"
; trap codes
puts    .EQU    x22
halt    .EQU    x25
outn    .EQU    x31
inn     .EQU    x33
; Get value from user
start   TRAP    inn          ; print prompt & get number
        ADD     R4,R0,#0     ; R4 = R0
        LEA     R0,PRESQRT   ; Get beginning output text
        TRAP    puts         ; Print "SQRT("
        AND     R0,R0,#0     ; Clear R0
        ADD     R0,R4,#0     ; R0 = R4 (input number)
        TRAP    outn         ; Print input number -- now have "SQRT(#"
        LEA     R0,PSTSQRT   ; Get last bit of text before printing answer
        TRAP    puts         ; Print ") = " -- now have "SQRT(#) = "
        ADD     R4,R4,#0     ; Set CCR for R4 -- input number
; If the value is negative, negate it and add an 'i' after the answer
; Otherwise, do the square root normally.
        BRN     neg
        BRZP    go
; Negate
neg     NOT     R4,R4       
        ADD     R4,R4,#1
        ADD     R2,R2,#1     ; R2 = 1 -- boolean for negative
; Get square root
go      JSR     SQRT
; Print answer
        TRAP    outn
        ADD     R2,R2,#0  ; Set CCR for R2
; if not zero, print an 'i'
        BRZ     end
        LEA     R0,imgnry
        TRAP    puts
; end of program
end     TRAP    halt
        .END    start
