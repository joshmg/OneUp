        .MAIN
link1   .ORIG
        .EXT foo,bar
friend  .STRZ " friend."
        JSR foo
        LEA R0,friend
        JSR bar
	    TRAP halt
	    .END
