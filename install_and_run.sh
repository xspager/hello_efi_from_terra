if [ ! -f uefi.img ]; then
	dd if=/dev/zero of=uefi.img bs=512 count=93750
fi

mformat -i uefi.img -f 1440 ::
mmd -i uefi.img ::/EFI
mmd -i uefi.img ::/EFI/BOOT
mcopy -i uefi.img main.efi ::/EFI/BOOT/BOOTx64.EFI

#qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -drive file=uefi.img,index=0,media=disk,format=raw,if=ide -enable-kvm -vga cirrus -cpu kvm64
## Debug using the serial port with:
##  socat -,raw,echo=0 tcp4:localhost:6666
#qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -drive file=uefi.img,index=0,media=disk,format=raw,if=ide -enable-kvm -vga cirrus -cpu kvm64 -monitor stdio -serial tcp::6666,server -s
