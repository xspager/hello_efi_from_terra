/* Graphic mode list code from: https://github.com/vathpela/gnu-efi/blob/master/apps/modelist.c*/

#include <efi.h>
#include <efilib.h>
 
extern EFI_GUID GraphicsOutputProtocol;

static int memcmp(const void *s1, const void *s2, UINTN n)
{
	const unsigned char *c1 = s1, *c2 = s2;
	int d = 0;

	if (!s1 && !s2)
		return 0;
	if (s1 && !s2)
		return 1;
	if (!s1 && s2)
		return -1;

	while (n--) {
		d = (int)*c1++ - (int)*c2++;
		if (d)
			break;
	}
	return d;
}

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
  EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

  InitializeLib(ImageHandle, SystemTable);

  status = LibLocateProtocol(&GraphicsOutputProtocol, (void **)&gop);

  if(EFI_ERROR(status))
    return status;

  status = uefi_call_wrapper(SystemTable->BootServices->LocateProtocol, 3, &gop_guid, NULL, &gop);
  //status = EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE(SystemTable->BootServices->LocateProtocol, 3, &gop_guid, NULL, &gop);

  Print(L"Framebuffer base is at %lx\n", gop->Mode->FrameBufferBase);

  int i, imax;

  imax = gop->Mode->MaxMode;

  Print(L"Max mode %d\n", imax);

  for(i = 0; i < imax; i++){
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    UINTN SizeOfInfo;
    status = uefi_call_wrapper(gop->QueryMode, 4, gop, i, &SizeOfInfo, &info);
    if(EFI_ERROR(status) && status == EFI_NOT_STARTED){
      status = uefi_call_wrapper(gop->SetMode, 2, gop, gop->Mode->Mode);
      status = uefi_call_wrapper(gop->QueryMode, 4, gop, i, &SizeOfInfo, &info);
    }
    if(EFI_ERROR(status)){
      CHAR16 Buffer[64];
      StatusToString(Buffer, status);
      Print(L"%d: Bad response from QueryMode: %s (%d)\n", i, Buffer, status);
      continue;
    }
    Print(L"%c%d: %dx%d ", memcmp(info, gop->Mode->Info, sizeof(*info)) == 0 ? '*' : ' ', i, info->HorizontalResolution, info->VerticalResolution);
  }

  // FROM: https://github.com/vathpela/gnu-efi/blob/master/apps/t7.c

  EFI_INPUT_KEY efi_input_key;

  Print(L"\n\n\nHit any key to exit this image\n");

  WaitForSingleEvent(SystemTable->ConIn->WaitForKey, 0);
  
  uefi_call_wrapper(SystemTable->ConOut->OutputString, 2, SystemTable->ConOut, L"\n\n");
  
  status = uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, &efi_input_key);
  
  Print(L"ScanCode: %xh  UnicodeChar: %xh\n", efi_input_key.ScanCode, efi_input_key.UnicodeChar);

  return EFI_SUCCESS;
}
