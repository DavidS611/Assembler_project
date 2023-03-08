MAIN:   mov r3,LENGTH  ;123	
LOOP:   jmp L1(#-1,r6)

			;
        mcr m1 
                sub r1,r4 ;333231
                
                ;33
                bne END
        endmcr
        prn #-5		
        bne LOOP(r4,r3)
        m1
L1:     inc K
        bne LOOP(K,STR)
        mcr m2
                add r7,r1
                sub r6,r4
        endmcr
        m2
END:    stop
STR:    .string "abcdef"
LENGTH: .data 6,-9,15
K:      .data 22

