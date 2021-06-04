#!/bin/bash 
echo "*****************************************************"
echo "this is post build!! current configure is :$1"
if [[ $2 = tc32 ]]; then
	tc32-elf-objcopy -v -O binary $1.elf  $1.bin
	../tools/tl_check_fw2.exe  $1.bin
	if [[ $1 = assoc_dev_8258 ]]; then
	../tools/tl_ota_tool.exe 1 $1.bin
	elif [[ $1 = assoc_dev_826x ]]; then
	../tools/tl_ota_tool.exe 1 $1.bin
	elif [[ $1 = assoc_dev_8278 ]]; then
	../tools/tl_ota_tool.exe 1 $1.bin	
	fi
elif [[ $2 = riscv ]]; then
	riscv32-elf-objcopy -S -O binary $1.elf  output/$1.bin
	../tools/tl_check_fw2.exe  output/$1.bin
fi
echo "**************** end of post build ******************"
