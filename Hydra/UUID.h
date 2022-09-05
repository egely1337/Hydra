#pragma once
#include <random>
#include <sstream>
namespace uuid {
	static std::random_device              rd;
	static std::mt19937                    gen(rd());
	static std::uniform_int_distribution<> dis(0, 15);
	static std::uniform_int_distribution<> dis2(8, 11);

	std::string generate_uuid_v4() {
		std::stringstream ss;
		int i;
		ss << std::hex;
		for (i = 0; i < 8; i++) {
			ss << dis(gen);
		}
		ss << "-";
		for (i = 0; i < 4; i++) {
			ss << dis(gen);
		}
		ss << "-4";
		for (i = 0; i < 3; i++) {
			ss << dis(gen);
		}
		ss << "-";
		ss << dis2(gen);
		for (i = 0; i < 3; i++) {
			ss << dis(gen);
		}
		ss << "-";
		for (i = 0; i < 12; i++) {
			ss << dis(gen);
		};
		return ss.str();
	}
}

namespace BMP {
    bool HDCToFile(const char* FilePath, HDC Context, RECT Area, uint16_t BitsPerPixel = 24)
    {
        uint32_t Width = Area.right - Area.left;
        uint32_t Height = Area.bottom - Area.top;

        BITMAPINFO Info;
        BITMAPFILEHEADER Header;
        memset(&Info, 0, sizeof(Info));
        memset(&Header, 0, sizeof(Header));
        Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        Info.bmiHeader.biWidth = Width;
        Info.bmiHeader.biHeight = Height;
        Info.bmiHeader.biPlanes = 1;
        Info.bmiHeader.biBitCount = BitsPerPixel;
        Info.bmiHeader.biCompression = BI_RGB;
        Info.bmiHeader.biSizeImage = Width * Height * (BitsPerPixel > 24 ? 4 : 3);
        Header.bfType = 0x4D42;
        Header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


        char* Pixels = 0;
        HDC MemDC = CreateCompatibleDC(Context);
        HBITMAP Section = CreateDIBSection(Context, &Info, DIB_RGB_COLORS, (void**)&Pixels, 0, 0);
        DeleteObject(SelectObject(MemDC, Section));
        BitBlt(MemDC, 0, 0, Width, Height, Context, Area.left, Area.top, SRCCOPY);
        DeleteDC(MemDC);

        FILE* f = fopen(FilePath, "wb");
        if (f)
        {
            fwrite((char*)&Header, sizeof(Header), 1, f);
            fwrite((char*)&Info.bmiHeader, sizeof(Info.bmiHeader), 1, f);
            fwrite(Pixels, (((BitsPerPixel * Width + 31) & ~31) / 8) * Height, 1, f);
            fclose(f);
            DeleteObject(Section);
            return true;
        }

        DeleteObject(Section);
        return false;
    }
}
