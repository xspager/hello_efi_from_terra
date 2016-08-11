if [ ! -f uefi.img ]; then
	dd if=/dev/zero of=uefi.img bs=512 count=93750
	sgdisk -o uefi.img 
	sgdisk -n 0 uefi.img 
	sgdisk -t 0:ef00 uefi.img 
	sudo losetup --offset 1048576 --sizelimit 46934528 /dev/loop0 uefi.img
	sudo mkdosfs -F 32 /dev/loop0
else
	sudo losetup --offset 1048576 --sizelimit 46934528 /dev/loop0 uefi.img
fi

sudo mount /dev/loop0 /mnt
sudo mkdir -p /mnt/EFI/BOOT
sudo cp main.efi /mnt/EFI/BOOT/BOOTx64.EFI
#sudo cp main.efi /mnt/
sudo umount /mnt
sudo losetup -d /dev/loop0
#qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -drive file=uefi.img,index=0,media=disk,format=raw,if=ide -enable-kvm -vga cirrus -cpu kvm64
## Debug using the serial port with:
##  socat -,raw,echo=0 tcp4:localhost:6666
qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -drive file=uefi.img,index=0,media=disk,format=raw,if=ide -enable-kvm -vga cirrus -cpu kvm64 -monitor stdio -serial tcp::6666,server -s
