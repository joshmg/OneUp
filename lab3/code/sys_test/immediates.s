imm .ORIG x0
    ADD R0,R0,x10  ; R0 += 16
    ADD R0,R0,xF   ; R0 += 15
    ADD R0,R0,x1F  ; R0 -= 1
    ADD R1,R1,#-16 ; R1 -= 16
    ADD R1,R1,#15  ; R1 += 15
    ADD R1,R1,#-1  ; R1 -= 1
    ADD R2,R0,R1   ; R2 = R0 + R1
    .END
