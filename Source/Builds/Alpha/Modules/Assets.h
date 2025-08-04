#pragma once

namespace Assets
{
#define DDSCAPS2_CUBEMAP				0x200
#define DDSCAPS2_CUBEMAP_POSITIVEX		0x400
#define DDSCAPS2_CUBEMAP_NEGATIVEX		0x800
#define DDSCAPS2_CUBEMAP_POSITIVEY		0x1000
#define DDSCAPS2_CUBEMAP_NEGATIVEY		0x2000
#define DDSCAPS2_CUBEMAP_POSITIVEZ		0x4000
#define DDSCAPS2_CUBEMAP_NEGATIVEZ		0x8000
#define DDSCAPS2_VOLUME					0x200000

#define DDSD_CAPS						0x00000001
#define DDSD_HEIGHT						0x00000002
#define DDSD_WIDTH						0x00000004
#define DDSD_PITCH						0x00000008
#define DDSD_PIXELFORMAT				0x00001000
#define DDSD_MIPMAPCOUNT				0x00020000
#define DDSD_LINEARSIZE					0x00080000

#define DDSCAPS_TEXTURE					0x00001000
#define DDSCAPS_MIPMAP					0x00400000
#define DDSCAPS_COMPLEX					0x00000008

#define DDS_MAGIC						0x44445320 // 'DDS '

#pragma pack(push, 1)
	typedef struct
	{
		uint32_t dwSize;
		uint32_t dwFlags;
		uint32_t dwFourCC;
		uint32_t dwRGBBitCount;
		uint32_t dwRBitMask;
		uint32_t dwGBitMask;
		uint32_t dwBBitMask;
		uint32_t dwABitMask;
	} DDS_PIXELFORMAT;

	typedef struct
	{
		uint32_t dwSize;
		uint32_t dwFlags;
		uint32_t dwHeight;
		uint32_t dwWidth;
		uint32_t dwPitchOrLinearSize;
		uint32_t dwDepth;
		uint32_t dwMipMapCount;
		uint32_t dwReserved1[11];
		DDS_PIXELFORMAT ddspf;
		uint32_t dwCaps;
		uint32_t dwCaps2;
		uint32_t dwCaps3;
		uint32_t dwCaps4;
		uint32_t dwReserved2;
	} DDS_HEADER;
#pragma pack(pop)


	void Load();
	void Unload();
}