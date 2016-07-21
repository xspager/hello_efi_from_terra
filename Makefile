FLAGS = -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -I /usr/include/efi -I /usr/include/efi/x86_64 -DEFI_FUNCTION_WRAPPER

.PHONY: hello all

all:	main.efi

main.efi:	main.so foo.so
	objcopy -j .text                \
          -j .sdata               \
          -j .data                \
          -j .dynamic             \
          -j .dynsym              \
          -j .rel                 \
          -j .rela                \
          -j .reloc --target=efi-app-x86_64 main.so main.efi

main.so:	main.o foo.o
	ld main.o foo.o /usr/lib/crt0-efi-x86_64.o -nostdlib -znocombreloc -T /usr/lib/elf_x86_64_efi.lds -shared -Bsymbolic -L /usr/lib -l:libgnuefi.a -l:libefi.a -o main.so

foo.o:	foo.t
	../terra-Linux-x86_64-332a506/bin/terra foo.t	

main.o:	main.c
	gcc -c main.c foo.o $(FLAGS) -o main.o
