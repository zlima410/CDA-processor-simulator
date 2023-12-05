        lw      0       1       pos1    
        lw      0       2       neg1    
        lw      0       3       cntAdr  
        jalr    3       7
        noop
done    halt                            
cnt     lw      0       4       addrA   
        add     0       0       3
L1      lw      4       5       0       
        beq     0       5       start
        add     1       3       3
        add     1       4       4
        beq     0       0       L1      
start   add     0       3       1      
        jalr    7       3
pos1    .fill   1
neg1    .fill   -1
cntAdr  .fill   cnt
addrA   .fill   A0
sim1    .fill   11111111                
A0    .fill   4
A1    .fill   5
A2    .fill   3
A3    .fill   1
A4    .fill   2
A5    .fill   0                       
sim2    .fill   11111111                
