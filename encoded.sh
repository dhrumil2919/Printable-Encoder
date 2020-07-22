#!/bin/bash

if [ "$#" -ne 1 ]; then
	echo "Usage: ./run <Your_shellcode_in_quote"
	sleep 1
	echo "Using execv(/bin/sh) shellcode"
	sleep 1
else
	shellcode=$1
fi
encoder=$(./encoder $shellcode)
echo "Encoded shellcode: $encoder"
