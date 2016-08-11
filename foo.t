terralib.includepath = terralib.includepath..";/usr/include/efi;/usr/include/efi/x86_64" 

local target = terralib.newtarget {
	Triple = "x86_64-pc-none";
	--CPU = ;
	Features = "-avx"; -- don't use AVX instructions
}

local C = terralib.includecstring([[
	#include <efi.h>
	#include <efilib.h>
	#define EFI_FUNCTION_WRAPPER
]], {}, target)

terra terra_main(str: &uint16)
	C.Print(str)
end

terra add(a: int, b: int)
	return a + b
end

terra paint_screen(gop: &C.EFI_GRAPHICS_OUTPUT_PROTOCOL, color: uint32)
	var fb_address = [&uint32](gop.Mode.FrameBufferBase)

	var width = gop.Mode.Info.HorizontalResolution
	var height = gop.Mode.Info.VerticalResolution

	C.APrint([&uint8]("%d x %d\n"), width, height)
	C.APrint([&uint8]("color: 0x%x\n"), color)
	C.APrint([&uint8]("fb_address: 0x%x\n"), fb_address)

	for i = 0, width * height do
		fb_address[i] = color
	end
end

terralib.saveobj('foo.o', {
	terra_main=terra_main,
	terra_add=add,
	paint_screen=paint_screen,
},{
	"-fno-stack-protector",
	"-fpic",
	"-fshort-wchar",
	"-mno-red-zone",
}, target)
