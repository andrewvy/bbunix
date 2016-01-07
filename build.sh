#clear screen
clear

#check if cdrtools is installed
if hash mkisofs 2>/dev/null; then
	echo "cdrtools found"
else
	echo "cdrtools (required) was not found"
	echo "Would you like to install it (y/n)"
	read -r yn
	if [ "$yn" == "y" ]; then
		echo "Installing cdrtools"
		if hash brew 2>/dev/null; then
			echo "Found brew"
		else
			#download and install brew
			echo "Brew (required to download cdrtools) was not found"
			echo "Would you like to install it (y/n)"
			read -r yn
			if [ "$yn" == "y" ]; then
				ruby -e "$(curl -fsSL https://raw.github.com/Homebrew/homebrew/go/install)"  
			else
				echo 'Setup failed'
				exit 0
			fi 
		fi
		#install cdrtools
		brew install cdrtools
	else
		#fail due to lack of dependancy
		echo 'Setup failed'
		exit 0
	fi
fi

#check if gcc i586-elf cross compiler is installed
if hash i686-elf-gcc 2>/dev/null; then
	echo "GCC-i686-elf found"
else
	echo "GCC-i686-elf (required) was not found"
	exit 0
fi

#check if nasm cross compiler is installed
if hash nasm 2>/dev/null; then
	echo "NASM found"
else
	echo "NASM (required) was not found"
	echo "Would you like to install it (y/n)"
	read -r yn
	if [ "$yn" == "y" ]; then
		echo "Installing NASM"
		if hash brew 2>/dev/null; then
			echo "Found brew"
		else
			#download and install brew
			echo "Brew (required to download NASM) was not found"
			echo "Would you like to install it (y/n)"
			read -r yn
			if [ "$yn" == "y" ]; then
				ruby -e "$(curl -fsSL https://raw.github.com/Homebrew/homebrew/go/install)"  
			else
				echo 'Setup failed'
				exit 0
			fi    
		fi
		#install nasm
		brew install nasm
	else
		#fail due to lack of dependancy
		echo 'Setup failed'
		exit 0
	fi
fi

#set default values
vmame="os"
autovm="true"
include="include"
commitmessage="Auto-commit"
branch="master"
osname="os"

#init user configs
if [ ! -f ./build.cfg ]; then
	echo "vmname=\"os\"" >> ./build.cfg
	echo "autovm=\"true\"" >> ./build.cfg
	echo "include=\"include\"" >> ./build.cfg
	echo "commitmessage=\"Auto-commit\"" >> ./build.cfg
	echo "branch=\"master\"" >> ./build.cfg
	echo "osname=\"os\"" >> ./build.cfg
fi
#init makefile
if [ ! -f ./linker.ld ]; then
	echo "ENTRY(_start)" >> ./linker.ld
	echo "SECTIONS" >> ./linker.ld
	echo "{" >> ./linker.ld
	echo ". = 1M;" >> ./linker.ld
	echo ".text BLOCK(4K) : ALIGN(4K)" >> ./linker.ld
	echo "{" >> ./linker.ld
	echo "*(.multiboot)" >> ./linker.ld
	echo "*(.text)" >> ./linker.ld
	echo "}" >> ./linker.ld
	echo ".rodata BLOCK(4K) : ALIGN(4K)" >> ./linker.ld
	echo "{" >> ./linker.ld
	echo "*(.rodata)" >> ./linker.ld
	echo "}" >> ./linker.ld
	echo ".data BLOCK(4K) : ALIGN(4K)" >> ./linker.ld
	echo "{" >> ./linker.ld
	echo "*(.data)" >> ./linker.ld
	echo "}" >> ./linker.ld
	echo ".bss BLOCK(4K) : ALIGN(4K)" >> ./linker.ld
	echo "{" >> ./linker.ld
	echo "*(COMMON)" >> ./linker.ld
	echo "*(.bss)" >> ./linker.ld
	echo "*(.bootstrap_stack)" >> ./linker.ld
	echo "}" >> ./linker.ld
	echo "}" >> ./linker.ld
fi
#create include folder for gcc
if [ ! -d ./$include/ ]; then
    echo "Creating include folder"
	mkdir ./$include
fi

#download, extract and copy nessesary grub file
if [ ! -f ./include/stage2_eltorito.bin ]; then
    echo "Downloading grub el torito"
	curl -get http://ijmul.com/dnlds/grub_files.tar.gz -o grub.tar.gz
	mkdir ./grub
	tar -xf grub.tar.gz -C ./grub
	cp ./grub/stage2_eltorito ./include/stage2_eltorito.bin
	rm -rf ./grub
	rm ./grub.tar.gz
fi

#read user config
source ./build.cfg

echo ""

#set gcc include paths
C_INCLUDE_PATH=./$include
export C_INCLUDE_PATH

CPLUS_INCLUDE_PATH=./$include
export CPLUS_INCLUDE_PATH

#set counter for assembled files
A=0

#find and assemble all .asm files in current directory and all subdirectories
find ./ -type f -name "*.asm" |\
{ while read asm; do
	((A++))
	nasm -felf $asm -o ${asm%.*}".ao"    
done
echo "Assembled $A ASM files with nasm"
}

Z=0

#find and compile all .c files in current directory and all subdirectories
find ./ -type f -name "*.s" |\
{
while read s; do
	((Z++))
	i586-elf-as $s -o ${s%.*}".ao"
done
echo "Assembled $Z S files with gas"
}

#set counter for compiled c files
C=0

#find and compile all .c files in current directory and all subdirectories
find ./ -type f -name "*.c" |\
{
while read c; do
	((C++))
	i686-elf-gcc -c $c -o ${c%.*}".o" -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Isrc/include -Isrc/include/libc
done
echo "Compiled $C C files"
}

#set counter for compiled cpp files
CPP=0

#find and compile all .cpp files in current directory and all subdirectories
find ./ -type f -name "*.cpp" |\
{
while read c; do
	((CPP++))
	i586-elf-g++ -c $c -o ${c%.*}".o" -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Isrc/include
done
echo "Compiled $CPP C++ files"
}

echo ""

#setting up directory structure
mkdir ./os
mkdir ./os/boot
mkdir ./os/boot/grub

#link object files
i686-elf-gcc -T linker.ld -o $osname".bin" -ffreestanding -O2 -nostdlib $(find ./ -type f \( -name "*.o" -or -name "*.ao" \)) -lgcc

#move os binary to appropriate directory
mv $osname".bin" ./os/boot

#copy grub
cp ./include/stage2_eltorito.bin ./os/boot/grub/stage2_eltorito.bin

#remove all object files (.o for c and c++ files, .ao for assembled files)
find ./ -type f \( -name "*.o" -or -name "*.ao" \) | while read o; do
	rm $o
done

#create bootable iso
mkisofs -R -b boot/grub/stage2_eltorito.bin -no-emul-boot -boot-load-size 4 -boot-info-table -o os/$osname".iso" ./os

echo ""


#run qemu
if [ "$autovm" == "true" ]; then
	if hash qemu-system-i386 2>/dev/null; then
		echo "Executing VM $vmname"
		qemu-system-i386 -kernel os/boot/$osname".bin"
	else
		echo "qemu-system-i386 not found"
	fi
fi

echo "done!"
