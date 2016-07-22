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

void
print_modes(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop)
{
  int i, imax;
  EFI_STATUS status;

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

    switch(info->PixelFormat) {
      case PixelRedGreenBlueReserved8BitPerColor:
      	Print(L"RGBR");
      	break;
      case PixelBlueGreenRedReserved8BitPerColor:
      	Print(L"BGRR");
      	break;
      case PixelBitMask:
      	Print(L"R:%08x G:%08x B:%08x X:%08x",
      		info->PixelInformation.RedMask,
      		info->PixelInformation.GreenMask,
      		info->PixelInformation.BlueMask,
      		info->PixelInformation.ReservedMask);
      	break;
      case PixelBltOnly:
      	Print(L"(blt only)");
      	break;
      default:
      	Print(L"(Invalid pixel format)");
      	break;
    }
    Print(L" pitch %d\n", info->PixelsPerScanLine);
  }
}
