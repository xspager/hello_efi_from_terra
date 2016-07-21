sudo losetup --offset 1048576 --sizelimit 46934528 /dev/loop0 uefi.img
sudo mount /dev/loop0 /mnt
#sudo mkdir -p /mnt/EFI/BOOT
#sudo cp main.efi /mnt/EFI/BOOT
sudo cp main.efi /mnt/
sudo umount /mnt
sudo losetup -d /dev/loop0
sudo qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -drive file=uefi.img,if=ide -L .
