link2   .ORIG
        .ENT foo,bar
hello   .STRZ "hello"
foo     LEA R0,hello
bar     TRAP puts
        RET
        .END
