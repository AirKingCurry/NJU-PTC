;整体思路：
;首先，将0纸带上的内容拷贝到1纸带上
;然后找出输入字符串的中间位置
;所以我设置了两个状态，find_mid0,find_mid1来完成这件事
;先将两条纸带的读写头都置于开始位置
;当TM处在find_mid0状态时，只有0纸带向右移动一位
;当TM处在find_mid1状态时，0纸带和1纸带都向右移动一位
;这样，当0纸带走到blank位置时，1纸带的读写头就处在输入字符串的中间位置了
;当然，如果纸带的长度是奇数，那么就可以直接跳转到reject状态了
;如果纸带的长度是偶数，并且我们已经完成了中间位置的定位，那么再次把0纸带的读写头位置置于开始处
;如果0纸带当前符号和1纸带当前符号相同，就都向右移一位；如果不相同，直接跳转到reject状态
;当1纸带的读写头走到blank位置时，跳转到accept状态

#Q = {0,copy01,find_mid0,find_mid1,move_left,move_left_1,check,accept,reject,accept1,accept2,accept3,accept4,halt_accept,reject1,reject2,reject3,reject4,reject5,halt_reject}
;每个状态的含义在下面的转移函数部分会有附加说明

#S = {0,1}

; the complete set of tape symbols
#G = {0,1,_,T,r,u,e,F,a,l,s}   ;hahaah

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 2

; the transition functions

;0是初始状态，默认跳转到copy01状态
0 _* ** ** accept
0 ** ** ** copy01
;将0纸带的内容拷贝到1纸带上
copy01 0_ *0 rr copy01
copy01 1_ *1 rr copy01
copy01 __ ** ll move_left

;将0纸带和1纸带的读写头都移到开始处
move_left ** ** ll move_left
move_left __ ** rr find_mid0

;开始寻找中间位置（如果纸带长度是奇数直接reject）
find_mid0 ** ** r* find_mid1
find_mid1 ** ** rr find_mid0
find_mid1 _* ** ** reject
find_mid0 _* ** l* move_left_1

;再将0纸带的读写头移至开始处
move_left_1 ** ** l* move_left_1
move_left_1 _* ** r** check

;进行check工作
check 00 ** rr check
check 11 ** rr check
check *_ ** ** accept
check 01 ** ** reject
check 10 ** ** reject

;在0纸带上打印出True
accept ** ** r* accept
accept _* ** l* accept1
accept1 ** _* l* accept1
accept1 _* T* r* accept2
accept2 _* r* r* accept3
accept3 _* u* r* accept4
accept4 _* e* ** halt_accept

;在0纸带上打印出False
reject ** ** r* reject
reject _* ** l* reject1
reject1 ** _* l* reject1
reject1 _* F* r* reject2
reject2 _* a* r* reject3
reject3 _* l* r* reject4
reject4 _* s* r* reject5
reject5 _* e* ** halt_reject


