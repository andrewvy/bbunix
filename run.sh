clear

# load config
source ./build.cfg

if hash qemu-system-i386 2>/dev/null; then
	echo "Executing VM $vmname"
	qemu-system-i386 -kernel os/boot/$osname".bin"
else
	echo "qemu-system-i386 not found"
	echo "Would you like to install it? (y/n)"
	if [ "$yn" == "y" ]; then
		echo "Installing qemu"
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
		#install qemu
		brew install qemu
	else
		#fail due to lack of dependancy
		echo 'Starting VM failed'
		exit 0
	fi
fi
