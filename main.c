#include <efi.h>
#include <efilib.h>
 
EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  int a = 10, b = 20;
  InitializeLib(ImageHandle, SystemTable);
  //Print(L"Hello, world!\n");
  terra_main(L"Hello, Terra from C!\n");
  Print(L"%d + %d = %d\n", a, b, terra_add(a,b));
  return EFI_SUCCESS;
}
