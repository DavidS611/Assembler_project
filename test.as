: mov r1,r2         ; missing label name error
1: mov r1,r2        ; first char label name error
a1 : mov r1,r2      ; label name space error
mov: mov r1,r2      ; label name reserve name error
a:mov r1,r2         ; missing space error
a: mov r1,r2        ; label name is already exists error
a: .entry a         ; warning for first label
    .entry          ; missing argument
.entry r1           ; reserve
    .entry a1  r    ; extra chars
    .entry fsaf     ; doesn't exists
b: .extern b        ; warning
.extern             ;missing
.extern a           ; entry & extern error
.extern mov         ; reserve
.extern c a         ; extra chars
s2: .data           ; missing numbers
a1: .data 333333    ; out of range
.data -333333       ; out of range
.data -#3           ; not number
.data 1, 1,         ; comma error
.data 3,0 3         ;
s1: .string         ; missing string
.string ""      k   ; extra chars
    .string "       ; missing quatation mark
.string """"        ; no error
.string abc"        ; missing quation mark
    .string abc     ; quation marks
    .string "mov"   ; no error
mov r0,#3           ; dest op err
mov r1 r2           ;
mov afasfa, r1      ; label error
mov r1,r2   r       ; extra err
mov                 ; missing
mov r1              ; missing
cmp #3,#-222222     ; no error
lea r1, r2          ; src err
lea #3 , r2         ; src err
lea s1, #3          ; dest err
inc #3              ; number err
inc r1 instanceof   ; extra
inc                 ;missing
inc mov             ; reserve
prn                 ; missing
prn #3 r1           ;extra
prn mov             ;reserve
jmp #3              ; number err
jmp r1 instanceof   ; extra
jmp                 ;missing
jmp .string         ; dest invalid + reserve
bne s1(r1 ,r2)      ; space err
bne s1(r1 r2)       ; comma err
jsr s1(r1,,r2)      ; err
jmp asdd(r1,#3)     ; dest err
jmp   s1(,r1)       ; missing
jmp s1(s1,)         ;missing
jsr s1(s1,s1)  3    ; extra
jsr s1 s1,s1)       ;
jsr r1(r1,r1)       ; dest err
jsr 41(#-4,#2)      ;dest err
rts instanceof      ; extra
stop f              ; extra

            ;;;** Valid inputs **;;;
mov #3, s1
mov #3, r1
mov s1,s1
mov s1, r1
mov r1,s1
mov r1,r2
cmp #-40,#200
cmp #3, s1
cmp #3, r2
cmp s1, #3
cmp s1, s1
cmp s1, r1
cmp r1,#3
cmp r1,s1
cmp r1,r1
add #3,s1
add #3, r1
add s1,s1
add s1, r1
add r1,s1
add r1, r1
sub #3,s1
sub #3, r1
sub s1,s1
sub s1, r1
sub r1,s1
sub r1, r1
not s1
not r1
clr s1
clr r1
inc s1
inc r1
dec s1
dec r1
jmp s1
jmp r1
bne s1
bne r1
red s1
red r1
prn #-5
prn s1
prn r1
jsr s1
jsr r1
jsr s1(r1,r2)
jmp s1(s1,s1)
bne s1(#3,#3)
jsr s1(r1,s1)
jsr s1(r1,#3)
jmp s1(s1,r1)
jmp s1(s1,#3)
bne s1(#3,s1)
bne s1(#3,r2)
rts
stop