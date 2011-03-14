jkdlsf	.ORIG
str1	.STRZ "Hello, world" ; comment after strz
str2	.STRZ "Semicolon in strz ;"
str3	.STRZ "Escaped \"" ;)

start	LEA R0,str1
		TRAP puts
		LEA R0,str2
		TRAP puts
		LEA R0,str3
		TRAP puts
	
		;TRAP halt

	.END start
