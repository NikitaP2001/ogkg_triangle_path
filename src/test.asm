

.code
asm_proc proc

	
	mov      rbx,3
	mov      r8,rax
	mov      rcx,rax
	dec      rcx

	and      rax,rcx
	xor      edx,edx
	div      rbx
	mov      rsi,rdx

	mov      rax,r8
	or       rax,rcx
	xor      edx,edx
	div      rbx
	inc      rdx
	cmp      rdx,rbx
	sbb      rdi,rdi
	and      rdi,rdx

	bsf      rax,r8

	ret

asm_proc endp





END