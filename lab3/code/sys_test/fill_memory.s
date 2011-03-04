mem  .ORIG x0
     .BLKW xFFFF ; full
     TRAP x25 ; halt, overflow of memory
     .END
