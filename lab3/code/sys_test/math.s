;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;  File:       math.s        ;;;
;;;  Written by: Andrew Groot  ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Contains integer square root
; and unsigned multiplication
; subroutines. There are also
; some strings for use with
; printing the resultant root.
;
; Each funciton is documented more
; specifically before it's start 
MATH    .ORIG
        .ENT SQRT,MULT,PRESQRT,PSTSQRT
PRESQRT .STRZ "SQRT("  ; first thing to be printed
PSTSQRT .STRZ ") = "   ; to be printed after the input number
sqrtpc  .BLKW #1       ; store R7 while executing square root
; Take integer square root of R4 and store result in R0
; Uses R5 and R6, guesses one and goes up as necessary
SQRT    AND R5,R5,#0  ; Clear R5
        ADD R4,R4,#0  ; set CCR for R4
        ST R7,sqrtpc  ; Save R7, to allow for calling MULT
        BRZ sqfin     ; if R4 = 0, return 0
sqnext  ADD R5,R5,#1  ; R5++
        ADD R6,R5,#0  ; R6 = R5
        JSR MULT      ; R0 = R5*R6
; negate R0
        NOT R0,R0     ; ~R0
        ADD R0,R0,#1
; Add -R0 to R4 to see if it's bigger
        ADD R0,R0,R4  ; R0 = R0 + R4
        BRP sqnext
; R0 >= R4
        BRZ sqfin     ; don't decrement R5 if they are equal 
        ADD R5,R5,#-1 ; R5-- (because R0 is too large)
sqfin   ADD R0,R5,#0  ; R0 = R5
        LD R7,sqrtpc  ; Restore R7
        RET
; Multiply R5 by R6, put result into R0
; Repeated addition, recursive: consumes R5 value
MULT    AND R0,R0,#0  ; Clear R0
mnext   ADD R6,R6,#0  ; CCR corresponds to R6 value
        BRNZ done     ; while R6 > 0  -- shouldn't be negative, though
        ADD R0,R0,R5  ; R0 = R0 + R6
        ADD R6,R6,#-1 ; R6--
        BRNZP mnext   ; recurse
done    RET
        .END          ; Should theoretically never get here
