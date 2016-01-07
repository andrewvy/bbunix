# BBUNIX

A UNIX-like OS in C for educational purposes.

Following tutorials from [wiki.osdev.org](http://wiki.osdev.org/)

![image](https://cloud.githubusercontent.com/assets/2051361/12124935/400370b4-b39d-11e5-93ca-908150a3e527.png)

----

Run `build.sh`. `i686-elf-gcc` is required for cross compilation.

OS iso is a ["el torito no-emulation" bootable CD](http://wiki.osdev.org/Bootable_El-Torito_CD_with_GRUB_Legacy)

---

You can assemble a bootable image:

`nasm -felf boot.asm -o boot.o`

`i686-elf-gcc -c src/kernel.c -o build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra`

`i686-elf-gcc -T linker.ld -o boot/os.bin -ffreestanding -O2 -nostdlib boot.o build/kernel.o -lgcc`

`mkisofs -R -b boot/grub/stage2_eltorito.bin -no-emul-boot -boot-load-size 4 -boot-info-table -o os.iso ./os`
