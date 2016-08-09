#include <efi.h>
#include <efilib.h>
 
#include "modes.h"
#include "foo.h"

extern EFI_GUID GraphicsOutputProtocol;

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  int a = 10, b = 20;

  InitializeLib(ImageHandle, SystemTable);
  //Print(L"Hello, world!\n");
  terra_main(L"Hello, Terra from C!\n");
  Print(L"%d + %d = %d\n", a, b, terra_add(a,b));

  Print(L"%s\n", SystemTable->FirmwareVendor);

  EFI_STATUS status;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

  status = LibLocateProtocol(&GraphicsOutputProtocol, (void **)&gop);

  if(EFI_ERROR(status))
    return status;

  status = uefi_call_wrapper(SystemTable->BootServices->LocateProtocol, 3, &GraphicsOutputProtocol, NULL, &gop);

  Print(L"Framebuffer base is at %lx\n", gop->Mode->FrameBufferBase);
  
  print_modes(gop);

  paint_screen(gop, (UINT32)0x00FF0000);

  // FROM: https://github.com/vathpela/gnu-efi/blob/master/apps/t7.c
  EFI_INPUT_KEY efi_input_key;

  Print(L"\n\n\nHit any key to exit this image\n");

  WaitForSingleEvent(SystemTable->ConIn->WaitForKey, 0);
  
  uefi_call_wrapper(SystemTable->ConOut->OutputString, 2, SystemTable->ConOut, L"\n\n");
  
  status = uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, &efi_input_key);
  
  Print(L"ScanCode: %xh  UnicodeChar: %xh\n", efi_input_key.ScanCode, efi_input_key.UnicodeChar);

  return EFI_SUCCESS;
}
