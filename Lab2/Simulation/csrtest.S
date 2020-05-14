#My csr test
.org 0x0
 	.global _start
_start:
	li t0,1
	li t1,0
	li t2,0xF0F
	li t3,0x0F0
	li t4,2
	li t5,0x012
	li t6,0x016 #01600f93
	csrrw t1,0x001,t0 #t1->0 csr->1 #00129373
	csrrs t1,0x001,t2 #t1->1 csr->0xF0F #0013a373
	csrrc t1,0x001,t2 #t1->0xF0F csr->0x000  #0013b373
	csrrwi t1,0x002,0x1 #t1->0 csr->1 #0020d373
	csrrsi t1,0x002,0x1F #t1->1 csr->0x1F #002fe373
	csrrci t1,0x002,0x1F #t1->0x1F csr->0 #002ff373
infinity_loop:
    jal   zero, infinity_loop       # 死循环

