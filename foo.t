terralib.includepath = terralib.includepath..";/usr/include/efi;/usr/include/efi/x86_64" 

local C = terralib.includecstring [[
	#include <efi.h>
	#include <efilib.h>
]]

terra terra_main(str: &uint16)
	C.Print(str)
end

terra add(a: int, b: int)
	return a + b
end

terralib.saveobj('foo.o', {
	terra_main=terra_main,
	terra_add=add
}, {"-c", "-fno-stack-protector", "-fpic", "-fshort-wchar", "-mno-red-zone"})
