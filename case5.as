        lw      0   1   input           
        lw      0   2   stack           
        lw      0   3   neg1            
        lw      0   4   pos1            
        lw      0   5   arAddr          
        jalr    5   6                   
        sw      0   7   none            
        noop
        halt                            
addR    add     2   3   2               
        add     2   3   2               
        sw      2   6   1               
        sw      2   1   0               
        beq     0   1   L1              
        add     1   3   1               
        lw      0   5   arAddr          
        jalr    5   6                   
        lw      2   6   1               
        lw      2   1   0               
        add     2   4   2               
        add     2   4   2               
        add     1   7   7               
        jalr    6   5                   
L1      add     2   4   2               
        add     2   4   2               
        add     0   0   7               
        jalr    6   5                   
input  .fill    3
neg1   .fill    -1
pos1   .fill    1
stack  .fill    100                     
none   .fill    0
arAddr .fill    addR
