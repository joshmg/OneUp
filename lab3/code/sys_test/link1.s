link1	.ORIG
	.EXT foo,bar
	.ENT l1,l2,l3
l1	LD R0,l3
l2	ST R0,l4
l3	.FILL foo
l4	.FILL bar
	TRAP halt
	.END
