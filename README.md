# Printable-Encoder

<b> How to run </b>
<ol>
	<li>run <i>make</i> command in terminal. it will create all necessary binaries</li>
	<li> To encoded shellcode execute <i>encoded.sh</i> shell script. it will encode your shellcode and print it in output </li>
	<li> To test shellcode against printable filter execute <i>run.sh</i>  shell script. it will encode your shellcode and pass that encoded shellcode as STDIN to <i>filter</i> binary. which have pritable filter in it.
</ol>

<b> Folders </b>
<table>
	<tr>
		<td>encoder_c</td>
		<td>Contain encoder C file </td>
	</tr>
	<tr>
		<td>decoder_asm</td>
		<td>Contain all the decoder asm, obj and objdump files</td>
	</tr>
	<tr>
		<td>size_shellcode_c</td>
		<td>Contain some shell script comapring size of printable encoder and sub encoder</td>
	</tr>
	<tr>
		<td>test_encoder_c</td>
		<td>Contain file which test shellcode decoding during execution</td>
	</tr>
	<tr>
		<td>filter_c</td>
		<td>Continer C file that has printable charater filter </td>
	</tr>
</table>
