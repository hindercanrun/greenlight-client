#pragma once

namespace Structs
{
	typedef struct Material
	{
		// Empty for now
	} Material;

	union GfxTexture
	{
		IDirect3DBaseTexture9* basemap;
		IDirect3DTexture9* map;
		IDirect3DVolumeTexture9* volmap;
		IDirect3DCubeTexture9* cubemap;
	};

	struct Picmip
	{
		unsigned __int8 platform[2];
	};

	struct CardMemory
	{
		int platform[2];
	};

	struct GfxImage
	{
		GfxTexture texture;
		unsigned __int8 mapType;
		unsigned __int8 semantic;
		unsigned __int8 category;
		bool delayLoadPixels;
		Picmip picmip;
		bool noPicmip;
		unsigned __int8 track;
		CardMemory cardMemory;
		unsigned __int16 width;
		unsigned __int16 height;
		unsigned __int16 depth;
		unsigned __int8 levelCount;
		unsigned __int8 streaming;
		unsigned int baseSize;
		unsigned __int8* pixels;
		unsigned int loadedSize;
		unsigned __int8 skippedMipLevels;
		const char* name;
		unsigned int hash;
	};

	typedef struct Bounds
	{
		float midPoint[3];
		float halfSize[3];
	} Bounds;

	typedef struct TriggerModel
	{
		int contents;
		unsigned short hullCount;
		unsigned short firstHull;
	} TriggerModel;

	typedef struct TriggerHull
	{
		Bounds bounds;
		int contents;
		unsigned short slabCount;
		unsigned short firstSlab;
	} TriggerHull;

	typedef struct TriggerSlab
	{
		float dir[3];
		float midPoint;
		float halfSize;
	} TriggerSlab;

	typedef struct MapTriggers
	{
		unsigned int count;
		TriggerModel* models;
		unsigned int hullCount;
		TriggerHull* hulls;
		unsigned int slabCount;
		TriggerSlab* slabs;
	} MapTriggers;

	typedef struct MapEnts
	{
		const char* name;
		char* entityString;
		int numEntityChars;
		MapTriggers trigger;
	} MapEnts;

	struct GfxLightImage
	{
		struct GfxImage* image;
		unsigned __int8 samplerState;
	};

	typedef struct GfxLightDef
	{
		const char* name;
		GfxLightImage attenuation;
		int lmapLookupStart;
	} GfxLightDef;

	struct Glyph
	{
		unsigned __int16 letter;
		char x0;
		char y0;
		unsigned __int8 dx;
		unsigned __int8 pixelWidth;
		unsigned __int8 pixelHeight;
		float s0;
		float t0;
		float s1;
		float t1;
	};

	struct Font_s
	{
		const char* fontName;
		int pixelHeight;
		int glyphCount;
		Material* material;
		Material* glowMaterial;
		Glyph* glyphs;
	};

	typedef struct LocalizeEntry
	{
		const char* value;
		const char* name;
	} LocalizeEntry;

	typedef struct RawFile
	{
		const char* name;
		int len;
		const char* buffer;
	} RawFile;

	typedef struct StringTableCell
	{
		const char* string;
		int hash;
	} StringTableCell;

	typedef struct StringTable
	{
		const char* name;
		int columnCount;
		int rowCount;
		StringTableCell* values;
		short* cellIndex;
	} StringTable;

	typedef struct ScriptParseTree
	{
		const char* name;
		int len;
		const char* buffer;
	} ScriptParseTree;

	typedef struct KeyValuePair
	{
		int keyHash;
		int namespaceHash;
		const char* value;
	} KeyValuePair;

	typedef struct KeyValuePairs
	{
		const char* name;
		int numVariables;
		KeyValuePair* keyValuePairs;
	} KeyValuePairs;

	typedef union XAssetHeader
	{
		MapEnts* mapEnts;
		GfxLightDef* lightDef;
		Font_s* font;
		LocalizeEntry* localize;
		RawFile* rawfile;
		StringTable* stringTable;
		ScriptParseTree* scriptParseTree;
		KeyValuePairs* keyValuePairs;
		void* data;
	} XAssetHeader;

	typedef enum XAssetType
	{
		ASSET_TYPE_XMODELPIECES = 0x0,
		ASSET_TYPE_PHYSPRESET = 0x1,
		ASSET_TYPE_PHYSCONSTRAINTS = 0x2,
		ASSET_TYPE_DESTRUCTIBLEDEF = 0x3,
		ASSET_TYPE_XANIMPARTS = 0x4,
		ASSET_TYPE_XMODEL = 0x5,
		ASSET_TYPE_MATERIAL = 0x6,
		ASSET_TYPE_PIXEL_SHADER = 0x7,
		ASSET_TYPE_TECHNIQUE_SET = 0x8,
		ASSET_TYPE_IMAGE = 0x9,
		ASSET_TYPE_SOUND = 0xA,
		ASSET_TYPE_SOUND_PATCH = 0xB,
		ASSET_TYPE_CLIPMAP = 0xC,
		ASSET_TYPE_CLIPMAP_PVS = 0xD,
		ASSET_TYPE_COMWORLD = 0xE,
		ASSET_TYPE_GAMEWORLD_SP = 0xF,
		ASSET_TYPE_GAMEWORLD_MP = 0x10,
		ASSET_TYPE_MAP_ENTS = 0x11,
		ASSET_TYPE_GFXWORLD = 0x12,
		ASSET_TYPE_LIGHT_DEF = 0x13,
		ASSET_TYPE_UI_MAP = 0x14, // never used
		ASSET_TYPE_FONT = 0x15,
		ASSET_TYPE_FONT_ICON = 0x16,
		ASSET_TYPE_MENULIST = 0x17,
		ASSET_TYPE_MENU = 0x18,
		ASSET_TYPE_LOCALIZE_ENTRY = 0x19,
		ASSET_TYPE_WEAPON = 0x1A,
		ASSET_TYPE_WEAPONDEF = 0x1B,
		ASSET_TYPE_WEAPON_VARIANT = 0x1C,
		ASSET_TYPE_SNDDRIVER_GLOBALS = 0x1D,
		ASSET_TYPE_FX = 0x1E,
		ASSET_TYPE_IMPACT_FX = 0x1F,
		ASSET_TYPE_AITYPE = 0x20,
		ASSET_TYPE_MPTYPE = 0x21,
		ASSET_TYPE_MPBODY = 0x22,
		ASSET_TYPE_MPHEAD = 0x23,
		ASSET_TYPE_CHARACTER = 0x24,
		ASSET_TYPE_XMODELALIAS = 0x25,
		ASSET_TYPE_RAWFILE = 0x26,
		ASSET_TYPE_STRINGTABLE = 0x27,
		ASSET_TYPE_XGLOBALS = 0x28,
		ASSET_TYPE_DDL = 0x29,
		ASSET_TYPE_GLASSES = 0x2A,
		ASSET_TYPE_TEXTURE_SET = 0x2B,
		ASSET_TYPE_EMBLEMSET = 0x2C,
		ASSET_TYPE_SCRIPTPARSETREE = 0x2D,
		ASSET_TYPE_KEYVALUEPAIR = 0x2E,
		ASSET_TYPE_VEHICLEDEF = 0x2F,
		ASSET_TYPE_MEMORYBLOCK = 0x30,
		ASSET_TYPE_ADDON_MAP_ENTS = 0x31,
		ASSET_TYPE_TRACER = 0x32,
		ASSET_TYPE_SKINNEDVERTS = 0x33,

		ASSET_TYPE_COUNT = 0x34,
		ASSET_TYPE_STRING = 0x35,
		ASSET_TYPE_ASSETLIST = 0x36,
	} XAssetType;

	typedef struct XAsset
	{
		XAssetType type;
		XAssetHeader header;
	} XAsset;

	typedef struct XAssetEntry
	{
		XAsset asset;
		unsigned char zoneIndex;
		bool inuse;
		unsigned short nextHash;
		unsigned short nextOverride;
		unsigned short usageFrame;
	} XAssetEntry;
}