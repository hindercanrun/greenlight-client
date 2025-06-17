#pragma once

namespace game
{
	struct cmd_function_s
	{
		cmd_function_s* next;

		const char* name;
		const char* autoCompleteDir;
		const char* autoCompleteExt;
		void(__cdecl* function)();
	};

	enum errorParm_t : __int32
	{
		ERR_FATAL = 0x0,
		ERR_DROP = 0x1,
		ERR_SERVERDISCONNECT = 0x2,
		ERR_DISCONNECT = 0x3,
		ERR_SCRIPT = 0x4,
		ERR_SCRIPT_DROP = 0x5,
		ERR_LOCALIZATION = 0x6,
		ERR_MAPLOADERRORSUMMARY = 0x7,
	};

	enum DvarFlags : std::uint16_t
	{
		DVAR_NONE = 0, // No flags
		DVAR_ARCHIVE = 1 << 0, // Save to config.cfg
		DVAR_LATCH = 1 << 1, // Unchange-able unless if game does a Dvar_Get()
		DVAR_CHEAT = 1 << 2, // Can not be changed if cheats are disabled
		DVAR_CODINFO = 1 << 3, // On change, this is sent to all clients (if you are host)
		DVAR_SCRIPTINFO = 1 << 4, // Unknown
		DVAR_TEMP = 1 << 5, // You can probably guess what this does
		DVAR_SAVED = 1 << 6, // Unknown
		DVAR_INTERNAL = 1 << 7, // You can probably guess what this does
		DVAR_EXTERNAL = 1 << 8, // Created by set command
		DVAR_USERINFO = 1 << 9, // Sent to server on connect or change
		DVAR_SERVERINFO = 1 << 10, // Sent in response to front end requests
		DVAR_INIT = 1 << 11, // Don't allow change from console at all
		DVAR_SYSTEMINFO = 1 << 12, // Duplicated on all clients
		DVAR_ROM = 1 << 13, // Unchange-able, cannot be set by user at all
		DVAR_CHANGEABLE_RESET = 1 << 14, // Unknown
		DVAR_AUTOEXEC = 1 << 15, // Never be set by game since isLoadingAutoExecGlobalFlag is always false
	};

	enum dvarType_t : __int32
	{
		DVAR_TYPE_BOOL = 0x0,
		DVAR_TYPE_FLOAT = 0x1,
		DVAR_TYPE_FLOAT_2 = 0x2,
		DVAR_TYPE_FLOAT_3 = 0x3,
		DVAR_TYPE_FLOAT_4 = 0x4,
		DVAR_TYPE_INT = 0x5,
		DVAR_TYPE_ENUM = 0x6,
		DVAR_TYPE_STRING = 0x7,
		DVAR_TYPE_COLOR = 0x8,
		DVAR_TYPE_INT64 = 0x9,
		DVAR_TYPE_LINEAR_COLOR_RGB = 0xA,
		DVAR_TYPE_COLOR_XYZ = 0xB,
		DVAR_TYPE_COUNT = 0xC,
	};

	union DvarValue
	{
		bool enabled;
		int integer;
		unsigned int unsignedInt;
		__int64 integer64;
		unsigned __int64 unsignedInt64;
		float value;
		float vector[4];
		const char* string;
		unsigned __int8 color[4];
	};

	struct dvar_s
	{
		const char* name;
		const char* description;
		int hash;
		unsigned int flags;
		dvarType_t type;

		bool modified;
		bool loadedFromSaveGame;

		DvarValue current;
		DvarValue latched;
		DvarValue reset;
		DvarValue saved;

		union domain
		{
			struct enumeration
			{
				int stringCount;
				const char** strings;
			};

			struct integer
			{
				int min;
				int max;
			};

			struct integer64
			{
				__int64 min;
				__int64 max;
			};

			struct value
			{
				float min;
				float max;
			};

			struct vector
			{
				float min;
				float max;
			};
		};

		dvar_s* hashNext;
	};

	struct Material; // forced

	struct ComPrimaryLight
	{
		unsigned __int8 type;
		unsigned __int8 canUseShadowMap;
		unsigned __int8 exponent;
		char priority;
		__int16 cullDist;
		unsigned __int8 _pad[2];
		float color[3];
		float dir[3];
		float origin[3];
		float radius;
		float cosHalfFovOuter;
		float cosHalfFovInner;
		float cosHalfFovExpanded;
		float rotationLimit;
		float translationLimit;
		float mipDistance;
		float diffuseColor[4];
		float specularColor[4];
		float attenuation[4];
		float falloff[4];
		float angle[4];
		float aAbB[4];
		float cookieControl0[4];
		float cookieControl1[4];
		float cookieControl2[4];
		const char* defName;
	};

	struct ComWaterHeader
	{
		int minx;
		int miny;
		int maxx;
		int maxy;
	};

	struct ComWaterCell
	{
		__int16 waterheight;
		char flooroffset;
		unsigned __int8 shoredist;
		unsigned __int8 color[4];
	};

	struct ComBurnableHeader
	{
		int minx;
		int miny;
		int maxx;
		int maxy;
	};

	struct ComBurnableSample
	{
		unsigned __int8 state;
	};

	struct ComBurnableCell
	{
		int x;
		int y;
		ComBurnableSample* data;
	};

	struct ComWorld
	{
		const char* name;
		int isInUse;
		unsigned int primaryLightCount;
		ComPrimaryLight* primaryLights;
		ComWaterHeader waterHeader;
		unsigned int numWaterCells;
		ComWaterCell* waterCells;
		ComBurnableHeader burnableHeader;
		unsigned int numBurnableCells;
		ComBurnableCell* burnableCells;
	};

	struct MapEnts
	{
		const char* name;
		char* entityString;
		int numEntityChars;
	};

	struct Font_s; // forced again
	struct menuDef_t; // forced again again

	struct menuClientNum_t
	{
		menuDef_t* menu;
		int localClientNum;
	};

	struct UiContext
	{
		menuClientNum_t menuStack[16];
	};

	struct loadAssets_t
	{
		float fadeClamp;
		int fadeCycle;
		float fadeAmount;
		float fadeInAmount;
	};

	enum expDataType : __int32
	{
		VAL_INT = 0x0,
		VAL_FLOAT = 0x1,
		VAL_STRING = 0x2,
	};

	union operandInternalDataUnion
	{
		int intVal;
		float floatVal;
		const char* string;
	};

	struct Operand
	{
		expDataType dataType;
		operandInternalDataUnion internals;
	};

	union expressionRpnDataUnion
	{
		Operand constant;
		void* cmd;
		int cmdIdx;
	};

	struct expressionRpn
	{
		int type;
		expressionRpnDataUnion data;
	};

	struct ExpressionStatement
	{
		char* filename;
		int line;
		int numRpn;
		expressionRpn* rpn;
	};

	struct itemDef_s; // forced again again again
	struct menuDef_t; // forced again again again again

	struct MenuList
	{
		const char* name;
		int menuCount;
		menuDef_t** menus;
	};

	struct RawFile
	{
		const char* name;
		int len;
		const char* buffer;
	};

	struct StringTableCell
	{
		const char* string;
		int hash;
	};

	struct StringTable
	{
		const char *name;
		int columnCount;
		int rowCount;
		StringTableCell* values;
		__int16 *cellIndex;
	};

	struct ScriptParseTree
	{
		const char* name;
		int len;
		const char* buffer;
	};

	union XAssetHeader
	{
		ComWorld* comWorld;
		MapEnts* mapEnts;
		MenuList* menuList;
		RawFile* rawfile;
		ScriptParseTree* scriptParseTree;
		void* data;
	};

	enum XAssetType : __int32
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
	};

	struct XAsset
	{
		XAssetType type;
		XAssetHeader header;
	};

	struct XAssetEntry
	{
		XAsset asset;
		unsigned __int8 zoneIndex;
		bool inuse;
		unsigned __int16 nextHash;
		unsigned __int16 nextOverride;
		unsigned __int16 usageFrame;
	};

	struct XZoneInfo
	{
		const char *name;
		int allocFlags;
		int freeFlags;
	};

	enum bdLogMessageType : __int32
	{
		BD_LOG_INFO = 0x0,
		BD_LOG_WARNING = 0x1,
		BD_LOG_ERROR = 0x2,
	};

	enum scriptInstance_t : __int32
	{
		SCRIPTINSTANCE_SERVER = 0x0,
		SCRIPTINSTANCE_CLIENT = 0x1,
		SCRIPT_INSTANCE_MAX = 0x2,
	};

	struct fileRetryInfo
	{
		int lastAttemptTime;
		int lastAttemptInterval;
		int retryCount;
	};

	struct ScreenPlacement
	{
		float scaleVirtualToReal[2];
		float scaleVirtualToFull[2];
		float scaleRealToVirtual[2];
		float virtualViewableMin[2];
		float virtualViewableMax[2];
		float virtualTweakableMin[2];
		float virtualTweakableMax[2];
		float realViewportBase[2];
		float realViewportSize[2];
		float realViewportMid[2];
		float realViewableMin[2];
		float realViewableMax[2];
		float realTweakableMin[2];
		float realTweakableMax[2];
		float subScreen[2];
	};

	struct CG_PerfData
	{
		int history[32];
		int count;
		int index;
		int instant;
		int total;
		float average;
		float variance;
		int min;
		int max;
	};

	struct CG_PerfInfo
	{
		bool initialized;
		CG_PerfData frame;
		CG_PerfData script;
		CG_PerfData cscript;
		CG_PerfData server;
		CG_PerfData renderExec;
		CG_PerfData renderSwap;
	};

	enum fsMode_t : __int32
	{
		FS_READ = 0x0,
		FS_WRITE = 0x1,
		FS_APPEND = 0x2,
		FS_APPEND_SYNC = 0x3,
	};

	enum FsThread : __int32
	{
		FS_THREAD_MAIN = 0x0,
		FS_THREAD_STREAM = 0x1,
		FS_THREAD_DATABASE = 0x2,
		FS_THREAD_BACKEND = 0x3,
		FS_THREAD_SERVER = 0x4,
		FS_THREAD_COUNT = 0x5,
		FS_THREAD_INVALID = 0x6,
	};

	struct XNADDR_
	{
		unsigned __int8 addrBuff[25];
	};
}
