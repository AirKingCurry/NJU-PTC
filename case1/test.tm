;整体思路：
;考虑到斐波那契数的表示形式是F(n)=F(n-1)+F(n-2)，所以要表示出F(n-1),F(n-2)，这样才能够判断
;采用4个纸带，tape0是用来记录input，tape1是用来记录F(n-2),tape2用来记录F(n-1)，tape3用来暂存F(n-1)
;TM的运作流程（是一个递归规程）：
;tape3始终跟随tape0移动，即tape0遍历了几个0，tape3上就有几个0.
;tape0读写头前面所有的0构成了F(n-1),tape1中的0构成了F(n-2),
;所以，每次流程开始之前tape1的读写头置于开始处，然后tape0和tape1同时移动。当tape1移动到blank时，即tape0遍历了F(n-1)+F(n-2)个0
;此时tape0已经遍历了F(n)个0，tape3上也有F(n)个0，而tape2上有F(n-1)个0
;将tape2上的内容拷贝到tape1上，将tape3上的内容拷贝到tape2上。
;所以此时的状态是：tape1上有F(n-1)个0,tape2上有F(n)个0,tape3上有F(n)个0
;即tape1记录了F(n-1),tape2记录了F(n),tape3记录了F(n)，形成了递归过程

; the finite set of states
#Q = {0,1,2,q,ml1,ml2,ml1',ml2',ml1'',ml2'',ml1''',ml2''',ml3,ml3',copy21,copy32,accept,accept1,accept2,accept3,accept4,halt_accept,reject,reject1,reject2,reject3,reject4,reject5,halt_reject}

; the finite set of input symbols
#S = {0}

; the complete set of tape symbols
#G = {0,_,T,r,u,e,F,a,l,s}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 4

; the transition functions

;初始状态，处理只有1个0的情况
0 0___ **00 r*rr 1
0 ____ **** **** reject

;处理只有2个0的情况
1 ____ **** **** accept
1 0___ *000 r*rr 2
2 _0__ **** **** accept
2 **** **** **** q

;处理3个0及以上的情况
q ____ **** **** accept
q 00__ ***0 rr*r q
q 0___ **** **** ml2
;如果tape0已经到了blank，而tape1没有到blank，说明k!=F(n-1)+F(n-2)，即k不是斐波那契数，拒绝接收
q _0__ **** **** reject

;将tape2的读写头移动到开始处
ml2 0___ **** **l* ml2'
ml2' 0_0_ **** **l* ml2'
ml2' 0___ **** **r* ml1

;将tape1的读写头移动到开始处
ml1 0_0_ **** *l** ml1'
ml1' 000_ **** *l** ml1'
ml1' 0_0_ **** *r** copy21

;将tape2中的内容拷贝到tape1中
copy21 **0* *0** *rr* copy21
copy21 **_* **** **** ml2''

;将tape2的读写头再次移动到开始处（准备tape3向tape2的拷贝）
ml2'' 0___ **** **l* ml2'''
ml2''' 0_0_ **** **l* ml2'''
ml2''' 0___ **** **r* ml1''

;将tape1的读写头再次移动到开始处（为下一次递归过程做准备）
ml1'' 0_0_ **** *l** ml1'''
ml1''' 000_ **** *l** ml1'''
ml1''' 0_0_ **** *r** ml3

;将tape3的读写头移动到开始处
ml3 000_ **** ***l ml3'
ml3' 0000 **** ***l ml3'
ml3' 000_ **** ***r copy32

;将tape3中的内容拷贝到tape2中，之后再次进行递归过程
copy32 ***0 **0* **rr copy32
copy32 ***_ **** **** q

;将tape0上的内容改写为accept
accept _*** **** l*** accept1
accept1 **** _*** l*** accept1
accept1 _*** T*** r*** accept2
accept2 _*** r*** r*** accept3
accept3 _*** u*** r*** accept4
accept4 _*** e*** **** halt_accept

;将tape0上的内容改写为reject
reject 0*** **** r*** reject
reject _*** **** l*** reject1
reject1 **** _*** l*** reject1
reject1 _*** F*** r*** reject2
reject2 _*** a*** r*** reject3
reject3 _*** l*** r*** reject4
reject4 _*** s*** r*** reject5
reject5 _*** e*** **** halt_reject
