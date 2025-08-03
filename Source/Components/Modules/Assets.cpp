#include "../Std_Include.h"
#include "Assets.h"

#include "../Utils/Hook.h"
#include "../Utils/String.h"
#include "../Utils/FileSystem.h"

namespace Assets
{
	namespace Loading
	{
		Utils::Hook::Detour Load_MapEntsPtr_Hook;
		void Load_MapEntsPtr(bool atStreamStart)
		{
			Structs::MapEnts** varMapEntsPtr = *(Structs::MapEnts***)0x82FC703C;

			auto Invoke = Load_MapEntsPtr_Hook.Invoke<void(*)(bool)>();
			Invoke(atStreamStart);

			// Validate pointer before dereferencing
			if (!varMapEntsPtr && !varMapEntsPtr[0])
			{
				Symbols::Com_PrintError(1, "Load_MapEntsPtr: varMapEntsPtr is NULL or invalid!\n");
				return;
			}

			Structs::MapEnts* varMapEnts = varMapEntsPtr[0];

			// Load map ents file
			std::string filePath = "game:\\Redlight\\raw\\";
			filePath += varMapEnts->name;
			filePath += ".mapents";

			// Replace forward slashes with backslashes
			std::replace(filePath.begin(), filePath.end(), '/', '\\');

			// Check if theres a .mapents file inside raw, replace entityString if yes
			if (Utils::FileSystem::FileExists(filePath.data()))
			{
				std::string newEntityString = Utils::FileSystem::ReadFileToString(filePath.c_str());
				if (newEntityString.empty())
				{
					return;
				}

				// Allocate new memory and copy the data
				size_t newSize = newEntityString.size() + 1; // Include null terminator
				char* newMemory = static_cast<char*>(malloc(newSize));
				if (!newMemory)
				{
					Symbols::Com_Printf(0, "Failed to allocate memory for entityString replacement!\n");
					return;
				}

				memcpy(newMemory, newEntityString.data(), newSize);

				// Update the entityString pointer to point to the new memory
				varMapEnts->entityString = newMemory;

				Symbols::Com_Printf(0, "Overriding mapents '%s' with '%s'\n", varMapEnts->name, filePath.data());
			}
		}

		Utils::Hook::Detour Load_RawFilePtr_Hook;
		void Load_RawFilePtr(bool atStreamStart)
		{
			Structs::RawFile** varRawFilePtr = *(Structs::RawFile***)0x82FC6928;

			auto Invoke = Load_RawFilePtr_Hook.Invoke<void(*)(bool)>();
			Invoke(atStreamStart);

			// Validate pointer before dereferencing
			if (!varRawFilePtr && !varRawFilePtr[0])
			{
				Symbols::Com_PrintError(1, "Load_RawFilePtr: varRawFilePtr is NULL or invalid!\n");
				return;
			}

			Structs::RawFile* varRawFile = varRawFilePtr[0];

			// Load rawfile
			std::string filePath = "game:\\Redlight\\raw\\";
			filePath += varRawFile->name;

			// Replace forward slashes with backslashes
			std::replace(filePath.begin(), filePath.end(), '/', '\\');

			// Check if theres a rawfile inside raw, replace buffer if yes
			if (Utils::FileSystem::FileExists(filePath.data()))
			{
				std::string newBuffer = Utils::FileSystem::ReadFileToString(filePath.c_str());
				if (newBuffer.empty())
				{
					return;
				}

				// Allocate new memory and copy the data
				size_t newSize = newBuffer.size() + 1; // Include null terminator
				char* newMemory = static_cast<char*>(malloc(newSize));
				if (!newMemory)
				{
					Symbols::Com_Printf(0, "Failed to allocate memory for entityString replacement!\n");
					return;
				}

				memcpy(newMemory, newBuffer.data(), newSize);

				// Update the buffer pointer to point to the new memory
				varRawFile->buffer = newMemory;
				varRawFile->len = static_cast<int>(newBuffer.size());

				Symbols::Com_Printf(0, "Overriding rawfile '%s' with '%s'\n", varRawFile->name, filePath.data());
			}
		}

		Utils::Hook::Detour Load_ScriptParseTreePtr_Hook;
		void Load_ScriptParseTreePtr(bool atStreamStart)
		{
			Structs::ScriptParseTree** varScriptParseTreePtr = *(Structs::ScriptParseTree***)0x82FC6D28;

			auto Invoke = Load_ScriptParseTreePtr_Hook.Invoke<void(*)(bool)>();
			Invoke(atStreamStart);

			// Validate pointer before dereferencing
			if (!varScriptParseTreePtr && !varScriptParseTreePtr[0])
			{
				Symbols::Com_PrintError(1, "Load_ScriptParseTreePtr: varScriptParseTreePtr is NULL or invalid!\n");
				return;
			}

			Structs::ScriptParseTree* varScriptParseTree = varScriptParseTreePtr[0];

			// Load script file
			std::string filePath = "game:\\Redlight\\raw\\";
			filePath += varScriptParseTree->name;

			// Replace forward slashes with backslashes
			std::replace(filePath.begin(), filePath.end(), '/', '\\');

			// Check if theres a script file inside raw, replace buffer if yes
			if (Utils::FileSystem::FileExists(filePath.data()))
			{
				std::string newBuffer = Utils::FileSystem::ReadFileToString(filePath.data());
				if (newBuffer.empty())
				{
					return;
				}

				// Allocate new memory and copy the data
				size_t newSize = newBuffer.size() + 1; // Include null terminator
				char* newMemory = static_cast<char*>(malloc(newSize));
				if (!newMemory)
				{
					Symbols::Com_Printf(0, "Failed to allocate memory for buffer replacement!\n");
					return;
				}

				memcpy(newMemory, newBuffer.data(), newSize);

				// Update the buffer pointer to point to the new memory
				varScriptParseTree->buffer = newMemory;
				varScriptParseTree->len = static_cast<int>(newBuffer.size());

				Symbols::Com_Printf(0, "Overriding scriptparsetree '%s' with '%s'\n", varScriptParseTree->name, filePath.data());
			}
		}

		typedef enum scriptInstance_t
		{
			SCRIPTINSTANCE_SERVER = 0x0,
			SCRIPTINSTANCE_CLIENT = 0x1,

			SCRIPT_INSTANCE_MAX = 0x2,
		} scriptInstance;

		Utils::Hook::Detour Scr_LoadScriptInternal_Hook;
		Structs::XAssetHeader Scr_LoadScriptInternal(scriptInstance inst, const char* filename)
		{
			auto Invoke = Scr_LoadScriptInternal_Hook.Invoke<Structs::XAssetHeader(*)(scriptInstance, const char*)>();
			Structs::XAssetHeader result = Invoke(inst, filename);

			if (result.scriptParseTree)
			{
				Structs::ScriptParseTree* scriptParseTree = result.scriptParseTree;

				// Load script file
				std::string filePath = "game:\\Redlight\\raw\\";
				filePath += scriptParseTree->name;

				// Replace forward slashes with backslashes
				std::replace(filePath.begin(), filePath.end(), '/', '\\');

				// Check if theres a script file inside raw, replace buffer if yes
				if (Utils::FileSystem::FileExists(filePath.data()))
				{
					std::string newBuffer = Utils::FileSystem::ReadFileToString(filePath.data());
					if (!newBuffer.empty())
					{
						size_t newSize = newBuffer.size() + 1;
						char* newMemory = static_cast<char*>(malloc(newSize));
						if (newMemory)
						{
							memcpy(newMemory, newBuffer.data(), newSize);
							scriptParseTree->buffer = newMemory;
							scriptParseTree->len = static_cast<int>(newBuffer.size());

							Symbols::Com_Printf(0, "Overriding scriptparsetree '%s' with '%s'\n", scriptParseTree->name, filePath.data());
						}
						else
						{
							Symbols::Com_PrintError(1, "Failed to allocate memory for buffer replacement!\n", filePath.data());
						}
					}
				}
			}
			return result;
		}

		void RegisterHooks()
		{
			Load_MapEntsPtr_Hook.Create(0x823685A0, Load_MapEntsPtr);
			Load_RawFilePtr_Hook.Create(0x8236BB90, Load_RawFilePtr);
			//Load_ScriptParseTreePtr_Hook.Create(0x8236C1B0, Load_ScriptParseTreePtr);
			//Scr_LoadScriptInternal_Hook.Create(0x826545F0, Scr_LoadScriptInternal);
		}
	}

	namespace Dumping
	{
		struct CardMemory
		{
		  int platform[1];
		};

		struct GfxImage
		{
			D3DBaseTexture basemap;
			char unknown2;
			char unknown3;
			char unknown4;
			char unknown5;
		  __declspec(align(4)) CardMemory cardMemory; //defines the size
		  unsigned __int16 width;
		  unsigned __int16 height;
		  unsigned __int16 depth;
		  char levelCount;
		  char streaming;
		  unsigned int baseSize;
		  char *pixels;
			char unknownData2[0x80];
		  const char *name;
		  unsigned int hash;

		  int dumpGfxImageAsset();
		};

		int GfxImage::dumpGfxImageAsset()
		{
			// Only print if in Debug env to prevent spam
#ifdef DEBUG
			Symbols::Com_Printf(0, "Dumping image '%s'\n", name);
#endif

			// TODO: Implement dumping from streamed textures, in .ipak files. 
			if (streaming)
			{
				Symbols::Com_PrintError(1, "Couldn't dump image '%s' because it's being streamed from an IPak\n", name);
				return ERROR_CALL_NOT_IMPLEMENTED;
			}

			if (basemap.Format.Type != GPUCONSTANTTYPE_TEXTURE)
			{
				Symbols::Com_PrintError(1, "Couldn't dump image '%s' because it's type is '%i'\n", name, basemap.Format.Type);
				return ERROR_CALL_NOT_IMPLEMENTED;
			}

			// TODO: Possibly support other formats, if needed..
			if (basemap.Format.DataFormat != GPUTEXTUREFORMAT_DXT1
				&& basemap.Format.DataFormat != GPUTEXTUREFORMAT_DXT2_3
				&& basemap.Format.DataFormat != GPUTEXTUREFORMAT_DXT4_5
				&& basemap.Format.DataFormat != GPUTEXTUREFORMAT_DXN
				&& basemap.Format.DataFormat != GPUTEXTUREFORMAT_8_8_8_8
				&& basemap.Format.DataFormat != GPUTEXTUREFORMAT_8_8
				&& basemap.Format.DataFormat != GPUTEXTUREFORMAT_8)
			{
				Symbols::Com_PrintError(1, "Couldn't dump image '%s' because it's format value is '%i'\n", name, basemap.Format.DataFormat);
				return ERROR_CALL_NOT_IMPLEMENTED;
			}

			// TODO: Support 1D and 3D,  if needed.. Note: 3D should be supported since some textures are in 3D
			if(basemap.Format.Dimension != GPUDIMENSION_2D && basemap.Format.Dimension != GPUDIMENSION_CUBEMAP)
			{
				Symbols::Com_PrintError(1, "Couldn't dump image '%s' because it's dimension value is '%i'\n", name, basemap.Format.Dimension);
				return ERROR_CALL_NOT_IMPLEMENTED;
			}

			int padAmount = 128;

			int vWidth = width;
			if (vWidth % padAmount != 0)
			{
				vWidth += (padAmount - vWidth % padAmount);
			}

			int vHeight = height;
			if (vHeight % padAmount != 0)
			{
				vHeight += (padAmount - vHeight % padAmount);
			}

			// Allocate memory
			size_t newSize = cardMemory.platform[0];
			char* memAlloc = static_cast<char*>(malloc(newSize));
			if (!memAlloc)
			{
				Symbols::Com_PrintError(1, "Couldn't dump image '%s' because there was not enough memory!\n", name);
				return ERROR_NOT_ENOUGH_MEMORY;
			}

			// Zero the memory
			ZeroMemory(memAlloc, cardMemory.platform[0]);

			static int counter = 0;
			const char* imageName = name;
			if (!imageName)
			{
				// Format it as '(null)_<number>'
				static char nameBuf[64];
				_snprintf(nameBuf, sizeof(nameBuf), "(null)_%d", ++counter);
				imageName = nameBuf;
			}

			// Check if the texture is already dumped
			const char* outputPath = Utils::String::Va("game:\\Redlight\\dump\\images\\%s.dds", imageName);
			if (Utils::FileSystem::FileExists(outputPath))
			{
				Symbols::Com_PrintWarning(0, "Silly you trying to dump '%s' twice...\n", name);
				return ERROR_DUP_NAME;
			}

			// Create just in case it doesn't exist
			CreateDirectory("game:\\Redlight\\dump\\images", 0);

			// Create the .DDS now
			FILE* file = fopen(outputPath, "wb");
			if (!file)
			{
				Symbols::Com_PrintError(1, "Couldn't dump image '%s' because it's name contains invalid characters!\n", name);
				free(memAlloc);
				return ERROR_BAD_NETPATH;
			}

			int dwMagic = DDS_MAGIC;
			fwrite(&dwMagic, 4, 1, file);

			int dwSize = _byteswap_ulong(124);
			fwrite(&dwSize, 4, 1, file);

			int dwFlags = (DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT);
			int dwPitchOrLinearSize;

			XGTEXTURE_DESC sourceDesc;
			XGGetTextureDesc(&basemap, 0, &sourceDesc);

			if (basemap.Format.DataFormat == GPUTEXTUREFORMAT_8_8_8_8
				|| basemap.Format.DataFormat == GPUTEXTUREFORMAT_8_8
				|| basemap.Format.DataFormat == GPUTEXTUREFORMAT_8)
			{
				dwFlags |= DDSD_PITCH;
				dwPitchOrLinearSize = _byteswap_ulong(width * sourceDesc.BitsPerPixel / 8);
			}
			else
			{
				dwFlags |= DDSD_LINEARSIZE;
				dwPitchOrLinearSize = _byteswap_ulong(baseSize);
			}

			int dwMipMapCount = 0;
			int levels = levelCount;
			if (levels > 1)
			{
				dwMipMapCount = levels;
				dwFlags |= DDSD_MIPMAPCOUNT;
			}

			dwFlags = _byteswap_ulong(dwFlags);
			fwrite(&dwFlags, 4, 1, file);

			int dwHeight = _byteswap_ulong(height);
			int dwWidth = _byteswap_ulong(width);
			fwrite(&dwHeight, 4, 1, file);
			fwrite(&dwWidth, 4, 1, file);

			fwrite(&dwPitchOrLinearSize, 4, 1, file);

			int nullOut = 0;
			fwrite(&nullOut, 4, 1, file);

			dwMipMapCount = _byteswap_ulong(dwMipMapCount);
			fwrite(&dwMipMapCount, 4, 1, file);

			for (int i = 0; i < 11; i++)
			{
				fwrite(&nullOut, 4, 1, file);
			}

			int dwSize2 = _byteswap_ulong(32);
			fwrite(&dwSize2, 4, 1, file);

			int dwFlags2 = 0;
			int dwFourCC = 0;

			switch (basemap.Format.DataFormat)
			{
			case GPUTEXTUREFORMAT_DXT1:
				dwFourCC	= 0x31545844; // 'DXT1'
				dwFlags2	|= 4;
				break;
			case GPUTEXTUREFORMAT_DXT2_3:
				dwFourCC	= 0x33545844; // 'DXT3'
				dwFlags2	|= 4;
				break;
			case GPUTEXTUREFORMAT_DXT4_5:
				dwFourCC	= 0x35545844; // 'DXT5'
				dwFlags2	|= 4;
				break;
			case GPUTEXTUREFORMAT_DXN:
				dwFourCC	= 0x32495441; // 'ATI2'
				dwFlags2	|= 4;
				break;
			case GPUTEXTUREFORMAT_8:
				dwFlags2	|= 0x2;
				break;
			case GPUTEXTUREFORMAT_8_8:
				dwFlags2	|= 0x20001;
				break;
			case GPUTEXTUREFORMAT_8_8_8_8:
				dwFlags2	|= 0x41;
				break;
			default:
				Symbols::Com_PrintWarning(0, "basemap.Format.DataFormat has encountered an supported format '0x%X' for '%s'\n", basemap.Format.DataFormat, name);
				break;
			}

			int dwCaps1 = 0;
			int dwCaps2 = 0;

			dwCaps1 |= 0x00001000;

			if (levels > 1)
			{
				dwCaps1 |= 0x00400008;
			}
			
			// Handle Cubemaps
			if (basemap.Format.Dimension == GPUDIMENSION_CUBEMAP)
			{
				dwCaps1 |= 2;
				dwCaps2 |= (DDSCAPS2_CUBEMAP
					| DDSCAPS2_CUBEMAP_POSITIVEX
					| DDSCAPS2_CUBEMAP_NEGATIVEX
					| DDSCAPS2_CUBEMAP_POSITIVEY
					| DDSCAPS2_CUBEMAP_NEGATIVEY
					| DDSCAPS2_CUBEMAP_POSITIVEZ
					| DDSCAPS2_CUBEMAP_NEGATIVEZ);
			}

			dwFlags2 = _byteswap_ulong(dwFlags2);
			fwrite(&dwFlags2, 4, 1, file);

			dwFourCC = _byteswap_ulong(dwFourCC);
			fwrite(&dwFourCC, 4, 1, file);

			int dwRGBBitCount;
			int dwRBitMask;
			int dwGBitMask;
			int dwBBitMask;
			int dwRGBAlphaBitMask;

			if (basemap.Format.DataFormat == GPUTEXTUREFORMAT_8_8_8_8)
			{
				dwRGBBitCount		= _byteswap_ulong(sourceDesc.BitsPerPixel);
				dwRBitMask			= _byteswap_ulong(0x00ff0000);
				dwGBitMask			= _byteswap_ulong(0x0000ff00);
				dwBBitMask			= _byteswap_ulong(0x000000ff);
				dwRGBAlphaBitMask	= _byteswap_ulong(0xff000000);
			}
			else if (basemap.Format.DataFormat == GPUTEXTUREFORMAT_8_8)
			{
				dwRGBBitCount		= _byteswap_ulong(sourceDesc.BitsPerPixel);
				dwRBitMask			= _byteswap_ulong(0x00ff);
				dwGBitMask			= 0x00000000;
				dwBBitMask			= 0x00000000;
				dwRGBAlphaBitMask	= _byteswap_ulong(0xff00);
			}
			else if (basemap.Format.DataFormat == GPUTEXTUREFORMAT_8)
			{
				dwRGBBitCount		= _byteswap_ulong(sourceDesc.BitsPerPixel);
				dwRBitMask			= 0x00000000;
				dwGBitMask			= 0x00000000;
				dwBBitMask			= 0x00000000;
				dwRGBAlphaBitMask	= _byteswap_ulong(0xff);
			}
			else
			{
				dwRGBBitCount		= 0x00000000;
				dwRBitMask			= 0x00000000;
				dwGBitMask			= 0x00000000;
				dwBBitMask			= 0x00000000;
				dwRGBAlphaBitMask	= 0x00000000;
			}

			fwrite(&dwRGBBitCount, 4, 1, file);
			fwrite(&dwRBitMask, 4, 1, file);
			fwrite(&dwGBitMask, 4, 1, file);
			fwrite(&dwBBitMask, 4, 1, file);
			fwrite(&dwRGBAlphaBitMask, 4, 1, file);

			dwCaps1 = _byteswap_ulong(dwCaps1);
			fwrite(&dwCaps1, 4, 1, file);

			dwCaps2 = _byteswap_ulong(dwCaps2);
			fwrite(&dwCaps2, 4, 1, file);

			for(int i = 0; i < 3; i++)
			{
				fwrite(&nullOut, 4, 1, file);
			}
		
			//Using memcpy, don't detile
			//memcpy(outputBuff, pixels, cardMemory.platform[0]);

			BOOL isBorderTexture = XGIsBorderTexture(&basemap);
			UINT mipMapTailLevel = XGGetMipTailBaseLevel(sourceDesc.Width, sourceDesc.Height, isBorderTexture);
			UINT slices = (basemap.Format.Dimension == GPUDIMENSION_CUBEMAP) ? 6 : 1;
			char* outBuff = memAlloc, *inBuff = pixels;

			UINT baseSize;
			XGGetTextureLayout(&basemap, 0, &baseSize, 0,0,0,0,0,0,0,0);

			if (basemap.Format.Dimension == GPUDIMENSION_CUBEMAP)
			{
				baseSize /= 6;
			}

			for (int slice = 0; slice < slices && inBuff < pixels + cardMemory.platform[0] && outBuff < memAlloc + cardMemory.platform[0]; slice++)
			{
				UINT mipMapOffset = XGGetMipLevelOffset(&basemap, slice, 0);
				outBuff = memAlloc + (slice * sourceDesc.SlicePitch);
				inBuff = pixels + mipMapOffset;

				for (int level = 0; level < levels && inBuff < pixels + cardMemory.platform[0] && outBuff < memAlloc + cardMemory.platform[0]; level++)
				{
					DWORD rowPitch = (sourceDesc.WidthInBlocks * sourceDesc.BytesPerBlock) / (1 << level);
					if (rowPitch < sourceDesc.BytesPerBlock)
					{
						rowPitch = sourceDesc.BytesPerBlock;
					}

					DWORD flags =
						((XGIsPackedTexture(&basemap)) ? 0 : XGTILE_NONPACKED) | ((isBorderTexture) ? XGTILE_BORDER : 0);
					
					// Convert tiled texture to linear layout using XGUntileTextureLevel
					XGUntileTextureLevel(
						sourceDesc.Width,						// Width
						sourceDesc.Height,						// Height
						level,									// Level
						XGGetGpuFormat(sourceDesc.Format),		// GpuFormat
						flags,									// Flags
						outBuff,								// pDestination
						rowPitch,								// RowPitch
						NULL,									// pPoint
						inBuff,									// pSource
						NULL									// pRect
					);

					UINT currentMipMapSize = baseSize / (1 << (2 * level));
					if (currentMipMapSize < sourceDesc.BytesPerBlock)
					{
						currentMipMapSize = sourceDesc.BytesPerBlock;
					}
			
					outBuff += currentMipMapSize;
					mipMapOffset = XGGetMipLevelOffset(&basemap, slice, level + 1);
					inBuff = pixels + baseSize + mipMapOffset;
				}
			}

			switch(basemap.Format.Endian)
			{
			case GPUENDIAN_8IN16:
				XGEndianSwapMemory(memAlloc, memAlloc, XGENDIAN_8IN16, 2, cardMemory.platform[0] / 2);
				break;
			case GPUENDIAN_8IN32:
				XGEndianSwapMemory(memAlloc, memAlloc, XGENDIAN_8IN32, 4, cardMemory.platform[0] / 4);
				break;
			case GPUENDIAN_16IN32:
				XGEndianSwapMemory(memAlloc, memAlloc, XGENDIAN_16IN32, 4, cardMemory.platform[0] / 4);
				break;
			default:
				break;
			}

			fwrite(memAlloc, cardMemory.platform[0], 1, file);
			fclose(file);
			free(memAlloc);
			return ERROR_SUCCESS;
		}

		struct ImageList
		{
		  unsigned int count;
		  GfxImage *image[4096];
		};

		typedef void (*R_GetImageList_t)(ImageList *a1);
		R_GetImageList_t R_GetImageList = R_GetImageList_t(0x82A1E4E8);

		Utils::Hook::Detour Load_GfxImageAsset_Hook;
		void Load_GfxImageAsset(GfxImage ** input)
		{
           // ImageList imageList;
           // R_GetImageList(&imageList);

            //for (unsigned int i = 0; i < imageList.count; i++)
            //{
            //    auto image = imageList.image[i];
//
            //    (*input)->dumpGfxImageAsset();
            //}

			(*input)->dumpGfxImageAsset();

			auto Invoke = Load_GfxImageAsset_Hook.Invoke<void(*)(GfxImage **)>();
			Invoke(input);
		}

		void Cmd_DumpMapEnts_f()
		{
			Structs::XAssetHeader files[2];
			int count = Symbols::DB_GetAllXAssetOfType_FastFile(Structs::ASSET_TYPE_MAP_ENTS, files, 2);

			for (int i = 0; i < count; i++)
			{
				Structs::MapEnts* varMapEnts = files[i].mapEnts;

				std::string assetName = varMapEnts->name;
				assetName += ".mapents";

				// Replace forward slashes with backslashes
				std::replace(assetName.begin(), assetName.end(), '/', '\\');

				int len = varMapEnts->numEntityChars;
				if (len > 0 && varMapEnts->entityString[len - 1] == '\0')
				{
					len--;
				}

				std::string outputPath = "game:\\Redlight\\dump\\" + assetName;
				Utils::FileSystem::WriteFileToDisk(outputPath.c_str(), varMapEnts->entityString, len);
			}
		}

		void Cmd_DumpGfxLight_f()
		{
			Structs::XAssetHeader files[1024];
			int count = Symbols::DB_GetAllXAssetOfType_FastFile(Structs::ASSET_TYPE_LIGHT_DEF, files, 1024);

			for (int i = 0; i < count; i++)
			{
				Structs::GfxLightDef* varGfxLight = files[i].lightDef;

				std::string assetName = varGfxLight->name;

				std::string outputPath = "game:\\Redlight\\dump\\lights\\" + assetName;
				Utils::FileSystem::WriteFileToDisk(outputPath.c_str(), varGfxLight->name, sizeof(Structs::GfxLightDef));
			}
		}

		const char* LocalizedStrings_GetFileNotes(const std::string& prefix)
		{
			if (prefix == "debug")
			{
				return "Strings used for debugging go in this file.";
			}
			if (prefix == "game")
			{
				return "Game messages made by the server game dll go in this file.";
			}
			else if (prefix == "menu")
			{
				return "Strings displayed in the menus go in this file.";
			}
			else if (prefix == "script")
			{
				return "All game messages made by script should go in this file.";
			}
			else if (prefix == "weapon")
			{
				return "Strings used in the weapon settings go in this file.";
			}
			return "No known Notes.";
		}

		void Cmd_DumpLocalizedStrings_f()
		{
			Structs::XAssetHeader files[20000];
			int count = Symbols::DB_GetAllXAssetOfType_FastFile(Structs::ASSET_TYPE_LOCALIZE_ENTRY, files, 20000);

			std::map<std::string, std::string> groupedStr;

			for (int i = 0; i < count; ++i)
			{
				Structs::LocalizeEntry* entry = files[i].localize;

				std::string name(entry->name);
				std::string value(entry->value);

				// Extract prefix and key
				std::string prefix = "default";
				std::string key = name;

				size_t underscore = name.find('_');
				if (underscore != std::string::npos)
				{
					prefix = name.substr(0, underscore);
					key = name.substr(underscore + 1); 
				}

				// Lowercase prefix for file naming
				std::transform(prefix.begin(), prefix.end(), prefix.begin(), ::tolower);

				size_t pos = 0;
				while ((pos = value.find('"', pos)) != std::string::npos)
				{
					value.insert(pos, "\"");
					pos += 2;
				}

				std::string& buffer = groupedStr[prefix];

				if (buffer.empty())
				{
					const char* fileNotes = LocalizedStrings_GetFileNotes(prefix);

					buffer +=
						"// Note to translators:\n"
						"// If a sentence is the same in your language then please change it to \"#same\"\n"
						"//\n"
						"// eg:\n"
						"// LANG_ENGLISH  \"HALT\"\n"
						"// LANG_GERMAN   \"#same\"\n"
						"//\n"
						"// (This is so we can tell which strings have been signed-off as ok to be the same words for QA\n"
						"//  and because we do not store duplicate strings, which will then get exported again next time\n"
						"//  as being untranslated.)\n"
						"//\n"
						"VERSION             \"1\"\n"
						"CONFIG              \"C:\\t6\\main\\game\\bin\\StringEd.cfg\"\n";
					buffer +=
						"FILENOTES           \"" + std::string(fileNotes) + "\"\n\n";
				}

				buffer += "REFERENCE           " + key + "\n";
				buffer += "LANG_ENGLISH        \"" + value + "\"\n\n";
			}

			for (std::map<std::string, std::string>::iterator it = groupedStr.begin(); it != groupedStr.end(); ++it)
			{
				it->second += "\nENDMARKER\n\n";

				std::string outPath = "game:\\Redlight\\dump\\english\\localizedstrings\\" + it->first + ".str";
				Utils::FileSystem::WriteFileToDisk(outPath.c_str(), it->second.c_str(), it->second.size());
			}
		}
	
		void Cmd_DumpRawFiles_f()
		{
			Structs::XAssetHeader files[1024];
			int count = Symbols::DB_GetAllXAssetOfType_FastFile(Structs::ASSET_TYPE_RAWFILE, files, 1024);

			for (int i = 0; i < count; i++)
			{
				Structs::RawFile* varRawFile = files[i].rawfile;
				std::string assetName = varRawFile->name;

				// Replace forward slashes with backslashes
				std::replace(assetName.begin(), assetName.end(), '/', '\\');

				std::string outputPath = "game:\\Redlight\\dump\\" + assetName;
				Utils::FileSystem::WriteFileToDisk(outputPath.c_str(), varRawFile->buffer, varRawFile->len);
			}
		}

		void Cmd_DumpStringTables_f()
		{
			Structs::XAssetHeader files[1024];
			int count = Symbols::DB_GetAllXAssetOfType_FastFile(Structs::ASSET_TYPE_STRINGTABLE, files, 128);

			for (int i = 0; i < count; ++i)
			{
				Structs::StringTable* varStringTable = files[i].stringTable;

				std::string assetName = varStringTable->name;

				// Replace forward slashes with backslashes
				std::replace(assetName.begin(), assetName.end(), '/', '\\');

				std::string csvTable;
				for (int j = 0; j < varStringTable->rowCount; j++)
				{
					for (int k = 0; k < varStringTable->columnCount; k++)
					{
						csvTable += varStringTable->values[j * varStringTable->columnCount + k].string;
						if (k < varStringTable->columnCount - 1)
						{
							csvTable += ",";
						}
					}
					csvTable += "\n";
				}

				std::string outPath = "game:\\Redlight\\dump\\" + assetName;
				Utils::FileSystem::WriteFileToDisk(outPath.c_str(), csvTable.c_str(), csvTable.size());
			}
		}

		void Cmd_DumpScriptParseTree_f()
		{
			Structs::XAssetHeader files[1024];
			int count = Symbols::DB_GetAllXAssetOfType_FastFile(Structs::ASSET_TYPE_SCRIPTPARSETREE, files, 1024);

			for (int i = 0; i < count; i++)
			{
				Structs::ScriptParseTree* varScriptParseTree = files[i].scriptParseTree;

				std::string assetName = varScriptParseTree->name;

				// Replace forward slashes with backslashes
				std::replace(assetName.begin(), assetName.end(), '/', '\\');

				std::string outputPath = "game:\\Redlight\\dump\\" + assetName;
				Utils::FileSystem::WriteFileToDisk(outputPath.c_str(), varScriptParseTree->buffer, varScriptParseTree->len);
			}
		}

		const char* GetKeyForHash(int hash, const char* nameSpace)
		{
			static char* keyList[] =
			{
				"GLOBAL",
				"ipak_read",
			};

			const char* key = Utils::String::Va("%%%i", hash);

			if (nameSpace && Symbols::Com_HashKey(nameSpace, 0x40) == hash)
			{
				key = nameSpace;
			}
			else
			{
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
				for (int i = 0; i < COUNT_OF(keyList); i++)
				{
					if (Symbols::Com_HashKey(keyList[i], 0x40) == hash)
					{
						key = keyList[i];
						break;
					}
				}
			}

			return key;
		}

		void Cmd_DumpKeyValuePair_f()
		{
			Structs::XAssetHeader files[1024];
			int count = Symbols::DB_GetAllXAssetOfType_FastFile(Structs::ASSET_TYPE_KEYVALUEPAIR, files, 1024);

			for (int i = 0; i < count; i++)
			{
				Structs::KeyValuePairs* keyValuePairs = files[i].keyValuePairs;
				std::string assetName = keyValuePairs->name;
				std::replace(assetName.begin(), assetName.end(), '/', '\\'); // Replace forward slashes with backslashes

				std::ostringstream output;
				for (int j = 0; j < keyValuePairs->numVariables; ++j)
				{
					Structs::KeyValuePair* kvp = &keyValuePairs->keyValuePairs[j];

					std::string nameSpace = GetKeyForHash(kvp->namespaceHash, keyValuePairs->name);
					std::string key = GetKeyForHash(kvp->keyHash, keyValuePairs->name);

					output << std::hex << std::showbase;
					output << key.data() << ",";
					output << nameSpace.data() << ",";
					output << kvp->value << "\n";
				}

				std::string outputStr = output.str();
				std::string filePath = "game:\\dump\\" + assetName;
				Utils::FileSystem::WriteFileToDisk(filePath.c_str(), outputStr.c_str(), outputStr.size());
			}
		}

		static Structs::cmd_function_s Cmd_DumpMapEnts_f_VAR;
		static Structs::cmd_function_s Cmd_DumpGfxLight_f_VAR;
		static Structs::cmd_function_s Cmd_DumpLocalizedStrings_f_VAR;
		static Structs::cmd_function_s Cmd_DumpRawFiles_f_VAR;
		static Structs::cmd_function_s Cmd_DumpStringTables_f_VAR;
		static Structs::cmd_function_s Cmd_DumpScriptParseTree_f_VAR;
		static Structs::cmd_function_s Cmd_DumpKeyValuePair_f_VAR;

		void RegisterCommands()
		{
			Load_GfxImageAsset_Hook.Create(0x82380168, Load_GfxImageAsset);

			// Okay so weirdly the game tries to re-register these every frame. So only run this function **once**.
			static bool alreadyRanFunction = FALSE;
			if (alreadyRanFunction)
			{
				alreadyRanFunction = TRUE;
				return;
			}
			alreadyRanFunction = TRUE;

			Symbols::Cmd_AddCommand("DumpMapEnts", Cmd_DumpMapEnts_f, &Cmd_DumpMapEnts_f_VAR);
			Symbols::Cmd_AddCommand("DumpGfxLight", Cmd_DumpGfxLight_f, &Cmd_DumpGfxLight_f_VAR);
			Symbols::Cmd_AddCommand("DumpLocalizedStrings", Cmd_DumpLocalizedStrings_f, &Cmd_DumpLocalizedStrings_f_VAR);
			Symbols::Cmd_AddCommand("DumpRawFiles", Cmd_DumpRawFiles_f, &Cmd_DumpRawFiles_f_VAR);
			Symbols::Cmd_AddCommand("DumpStringTables", Cmd_DumpStringTables_f, &Cmd_DumpStringTables_f_VAR);
			Symbols::Cmd_AddCommand("DumpScriptParseTree", Cmd_DumpScriptParseTree_f, &Cmd_DumpScriptParseTree_f_VAR);
			//Symbols::Cmd_AddCommand("DumpKeyValuePair", Cmd_DumpKeyValuePair_f, &Cmd_DumpKeyValuePair_f_VAR);
		}
	}

	void Load()
	{
		Loading::RegisterHooks();
		Dumping::RegisterCommands();
	}

	void Unload()
	{
		// Nothing yet
	}
}