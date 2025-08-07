#include "../Std_Include.h"
#include "Assets.h"

#include "../../../Utils/Hook.h"
#include "../../../Utils/String.h"
#include "../../../Utils/FileSystem.h"

#include "../Dependencies/zlib.h"

namespace Assets
{
	enum XAssetType
	{
	  ASSET_TYPE_XMODELPIECES = 0x0,
	  ASSET_TYPE_PHYSPRESET = 0x1,
	  ASSET_TYPE_PHYSCONSTRAINTS = 0x2,
	  ASSET_TYPE_DESTRUCTIBLEDEF = 0x3,
	  ASSET_TYPE_XANIMPARTS = 0x4,
	  ASSET_TYPE_XMODEL = 0x5,
	  ASSET_TYPE_MATERIAL = 0x6,
	  ASSET_TYPE_PIXELSHADER = 0x7,
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
	  ASSET_TYPE_UI_MAP = 0x14,
	  ASSET_TYPE_FONT = 0x15,
	  ASSET_TYPE_FONTICON = 0x16,
	  ASSET_TYPE_MENULIST = 0x17,
	  ASSET_TYPE_MENU = 0x18,
	  ASSET_TYPE_LOCALIZE_ENTRY = 0x19,
	  ASSET_TYPE_WEAPON = 0x1A,
	  ASSET_TYPE_WEAPONDEF = 0x1B,
	  ASSET_TYPE_WEAPON_VARIANT = 0x1C,
	  ASSET_TYPE_WEAPON_FULL = 0x1D,
	  ASSET_TYPE_ATTACHMENT = 0x1E,
	  ASSET_TYPE_ATTACHMENT_UNIQUE = 0x1F,
	  ASSET_TYPE_WEAPON_CAMO = 0x20,
	  ASSET_TYPE_SNDDRIVER_GLOBALS = 0x21,
	  ASSET_TYPE_FX = 0x22,
	  ASSET_TYPE_IMPACT_FX = 0x23,
	  ASSET_TYPE_AITYPE = 0x24,
	  ASSET_TYPE_MPTYPE = 0x25,
	  ASSET_TYPE_MPBODY = 0x26,
	  ASSET_TYPE_MPHEAD = 0x27,
	  ASSET_TYPE_CHARACTER = 0x28,
	  ASSET_TYPE_XMODELALIAS = 0x29,
	  ASSET_TYPE_RAWFILE = 0x2A,
	  ASSET_TYPE_STRINGTABLE = 0x2B,
	  ASSET_TYPE_LEADERBOARD = 0x2C,
	  ASSET_TYPE_XGLOBALS = 0x2D,
	  ASSET_TYPE_DDL = 0x2E,
	  ASSET_TYPE_GLASSES = 0x2F,
	  ASSET_TYPE_TEXTURELIST = 0x30,
	  ASSET_TYPE_EMBLEMSET = 0x31,
	  ASSET_TYPE_SCRIPTPARSETREE = 0x32,
	  ASSET_TYPE_KEYVALUEPAIRS = 0x33,
	  ASSET_TYPE_VEHICLEDEF = 0x34,
	  ASSET_TYPE_MEMORYBLOCK = 0x35,
	  ASSET_TYPE_ADDON_MAP_ENTS = 0x36,
	  ASSET_TYPE_TRACER = 0x37,
	  ASSET_TYPE_SKINNEDVERTS = 0x38,
	  ASSET_TYPE_QDB = 0x39,
	  ASSET_TYPE_SLUG = 0x3A,
	  ASSET_TYPE_FOOTSTEP_TABLE = 0x3B,
	  ASSET_TYPE_FOOTSTEPFX_TABLE = 0x3C,
	  ASSET_TYPE_ZBARRIER = 0x3D,
	  ASSET_TYPE_COUNT = 0x3E,
	  ASSET_TYPE_STRING = 0x3E,
	  ASSET_TYPE_ASSETLIST = 0x3F,
	  ASSET_TYPE_REPORT = 0x40,
	  ASSET_TYPE_DEPEND = 0x41,
	  ASSET_TYPE_FULL_COUNT = 0x42,
	};

	enum AssetEntryType
	{
		ASSET_ENTRY_BYTE,
		ASSET_ENTRY_BOOL,
		ASSET_ENTRY_BOOL_AS_INT,
		ASSET_ENTRY_SHORT,
		ASSET_ENTRY_UNSIGNED_SHORT,
		ASSET_ENTRY_TIME_AS_INT,
		ASSET_ENTRY_INT,
		ASSET_ENTRY_UNSIGNED_INT,
		ASSET_ENTRY_FLOAT,
		ASSET_ENTRY_STRING,
		ASSET_ENTRY_SCRIPT_STRING,
		ASSET_ENTRY_ASSET_PTR,
		ASSET_ENTRY_CONTAINED,
		ASSET_ENTRY_CONTAINED_REVERSED,
		ASSET_ENTRY_EMBEDDED,
		ASSET_ENTRY_EMBEDDED_REVERSED,
		ASSET_ENTRY_ENUM,
		ASSET_ENTRY_ENUM_SHORT
	};

	struct AssetEntry
	{
		char* name;
		int offset;
		AssetEntryType type;
		AssetEntry* next;
		unsigned int enumCount;
	};

	bool DumpAssetEntry(
		FILE* f,
		AssetEntryType currentEntryType,
		const char* currentEntryName,
		void* assetData,
		AssetEntry* containedItems,
		int enumCount,
		const char* parentName)
	{
		if (!f || !currentEntryName || !assetData)
			return FALSE;

		if (currentEntryType != ASSET_ENTRY_CONTAINED &&
			currentEntryType != ASSET_ENTRY_EMBEDDED &&
			currentEntryType != ASSET_ENTRY_CONTAINED_REVERSED &&
			currentEntryType != ASSET_ENTRY_EMBEDDED_REVERSED)
		{
			fprintf(f, "\\%s%s", parentName, currentEntryName);
		}

		switch (currentEntryType)
		{
		case ASSET_ENTRY_BYTE:
		case ASSET_ENTRY_BOOL:
			fprintf(f, "\\%i", *(byte*)assetData);
			break;
		case ASSET_ENTRY_SHORT:
			fprintf(f, "\\%hd", *(short*)assetData);
			break;
		case ASSET_ENTRY_UNSIGNED_SHORT:
			fprintf(f, "\\%hu", *(unsigned short*)assetData);
			break;
		case ASSET_ENTRY_BOOL_AS_INT:
			fprintf(f, "\\%i", *(int*)assetData != 0);
			break;
		case ASSET_ENTRY_INT:
			fprintf(f, "\\%i", *(int*)assetData);
			break;
		case ASSET_ENTRY_UNSIGNED_INT:
			fprintf(f, "\\%u", *(unsigned int*)assetData);
			break;
		case ASSET_ENTRY_TIME_AS_INT:
			fprintf(f, "\\%g", ((float)*(unsigned int*)assetData) / 1000);
			break;
		case ASSET_ENTRY_FLOAT:
			fprintf(f, "\\%g", *(float*)assetData);
			break;
		case ASSET_ENTRY_STRING:
			fprintf(f, "\\%s", *(char**)assetData ? *(char**)assetData : "");
			break;
		case ASSET_ENTRY_SCRIPT_STRING:
		{
			const char* stringVal = Symbols::SL_ConvertToString(*(unsigned short*)assetData);
			fprintf(f, "\\%s", (stringVal) ? stringVal : "");
		}
		break;
		case ASSET_ENTRY_ASSET_PTR:
			fprintf(f, "\\%s", *(char***)assetData ? (**(char***)assetData) : "");
			break;
		case ASSET_ENTRY_ENUM:
			fprintf(f, "\\%s", *(int*)assetData < enumCount ? *(char**)((int)(containedItems) + (4*(*(int*)assetData))) : "");
			break;
		case ASSET_ENTRY_ENUM_SHORT:
			fprintf(f, "\\%s", *(short*)assetData < enumCount ? *(char**)((int)(containedItems) + (4*(*(short*)assetData))) : "");
			break;
		case ASSET_ENTRY_EMBEDDED:
			if (containedItems)
			{
				for (int i = 0; containedItems[i].name; i++)
				{
					DumpAssetEntry(
						f,
						containedItems[i].type,
						containedItems[i].name,
						(void*)((int)assetData + containedItems[i].offset),
						containedItems[i].next,
						containedItems[i].enumCount,
						currentEntryName);
				}
			}
			break;
		case ASSET_ENTRY_EMBEDDED_REVERSED:
			if (containedItems)
			{
				for (int i = 0; containedItems[i].name; i++)
				{
					DumpAssetEntry(
						f,
						containedItems[i].type,
						currentEntryName,
						(void*)((int)assetData + containedItems[i].offset),
						containedItems[i].next,
						containedItems[i].enumCount,
						containedItems[i].name);
				}
			}
			break;
		case ASSET_ENTRY_CONTAINED_REVERSED:
			if (containedItems && *(int*)assetData)
			{
				for (int i = 0; containedItems[i].name; i++)
				{
					DumpAssetEntry(
						f,
						containedItems[i].type,
						currentEntryName,
						(void*)(*(int*)assetData + containedItems[i].offset),
						containedItems[i].next,
						containedItems[i].enumCount,
						containedItems[i].name);
				}
			}
			break;
		case ASSET_ENTRY_CONTAINED:
			if (containedItems && *(int*)assetData)
			{
				for (int i = 0; containedItems[i].name; i++)
				{
					DumpAssetEntry(
						f,
						containedItems[i].type,
						containedItems[i].name,
						(void*)(*(int*)assetData + containedItems[i].offset),
						containedItems[i].next,
						containedItems[i].enumCount,
						currentEntryName);
				}
			}
		default: 
			break;
		}
		return TRUE;
	}

	bool DumpAssetEntry(
		FILE* f,
		AssetEntryType currentEntryType,
		const char* currentEntryName,
		void* assetData,
		AssetEntry* containedItems = 0,
		int enumCount = 0,
		const char* parentName = "");

	AssetEntry OffsetEntry[] =
	{
		{ "X",		0,		ASSET_ENTRY_FLOAT,		0,		0 },
		{ "Y",		4,		ASSET_ENTRY_FLOAT,		0,		0 },
		{ "Z",		8,		ASSET_ENTRY_FLOAT,		0,		0 },
		{ NULL,		0,		(AssetEntryType)0,		0,		0 }
	};

	AssetEntry MinMax_FloatEntry[] =
	{
		{ "Min",	0,		ASSET_ENTRY_FLOAT,		0,		0 },
		{ "Max",	4,		ASSET_ENTRY_FLOAT,		0,		0 },
		{ NULL,		0,		(AssetEntryType)0,		0,		0 }
	};

	struct PhysPreset
	{
		const char* name;
		int flags;
		float mass;
		float bounce;
		float friction;
		float bulletForceScale;
		float explosiveForceScale;
		const char* sndAliasPrefix;
		float piecesSpreadFraction;
		float piecesUpwardVelocity;
		int canFloat;
		float gravityScale;
		float centerOfMassOffset[3];
		float buoyancyBoxMin[3];
		float buoyancyBoxMax[3];

		int DumpPhysPresetAsset();
	};

	AssetEntry PhysPresetEntries[] =
	{
		{ "flags",						offsetof(PhysPreset, flags),						ASSET_ENTRY_INT,				0,					0 },
		{ "mass",						offsetof(PhysPreset, mass),							ASSET_ENTRY_FLOAT,				0,					0 },
		{ "bounce",						offsetof(PhysPreset, bounce),						ASSET_ENTRY_FLOAT,				0,					0 },
		{ "friction",					offsetof(PhysPreset, friction),						ASSET_ENTRY_FLOAT,				0,					0 },
		{ "bulletForceScale",			offsetof(PhysPreset, bulletForceScale),				ASSET_ENTRY_FLOAT,				0,					0 },
		{ "explosiveForceScale",		offsetof(PhysPreset, explosiveForceScale),			ASSET_ENTRY_FLOAT,				0,					0 },
		{ "sndAliasPrefix",				offsetof(PhysPreset, sndAliasPrefix),				ASSET_ENTRY_STRING,				0,					0 },
		{ "piecesSpreadFraction",		offsetof(PhysPreset, piecesSpreadFraction),			ASSET_ENTRY_FLOAT,				0,					0 },
		{ "piecesUpwardVelocity",		offsetof(PhysPreset, piecesUpwardVelocity),			ASSET_ENTRY_FLOAT,				0,					0 },
		{ "canFloat",					offsetof(PhysPreset, canFloat),						ASSET_ENTRY_BOOL_AS_INT,		0,					0 },
		{ "gravityScale",				offsetof(PhysPreset, gravityScale),					ASSET_ENTRY_FLOAT,				0,					0 },
		{ "centerOfMassOffset",			offsetof(PhysPreset, centerOfMassOffset),			ASSET_ENTRY_EMBEDDED,			OffsetEntry,		0 },
		{ "buoyancyBoxMin",				offsetof(PhysPreset, buoyancyBoxMin),				ASSET_ENTRY_EMBEDDED,			OffsetEntry,		0 },
		{ "buoyancyBoxMax",				offsetof(PhysPreset, buoyancyBoxMax),				ASSET_ENTRY_EMBEDDED,			OffsetEntry,		0 },
		{ NULL,							NULL,												(AssetEntryType)NULL,			NULL,				NULL }
	};

	int PhysPreset::DumpPhysPresetAsset()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\physic", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\physic\\%s", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* file = fopen(outputPath, "wb");
		if (!file)
		{
			return ERROR_BAD_NETPATH;
		}

		fprintf(file, "PHYSIC\\isFrictionInfinity\\%i", friction >= 340282346638528860000000000000000000000.f);

		for (int i = 0; PhysPresetEntries[i].name; i++)
		{
			if (PhysPresetEntries[i].offset == offsetof(PhysPreset, friction) && friction >= 340282346638528860000000000000000000000.f)
			{
				fprintf(file,"\\friction\\0.5");
			}
			else
			{
				DumpAssetEntry(
					file,
					PhysPresetEntries[i].type,
					PhysPresetEntries[i].name,
					(void*)((int)(this) + PhysPresetEntries[i].offset),
					PhysPresetEntries[i].next);
			}
		}

		fclose(file);
		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_PhysPresetAsset_Hook;
	void Load_PhysPresetAsset(PhysPreset** input)
	{
		PhysPreset* currentPhysPreset = *input;
		(*input)->DumpPhysPresetAsset();

		// TODO: Add loading from disk

		auto Invoke = Load_PhysPresetAsset_Hook.Invoke<void(*)(PhysPreset**)>();
		Invoke(input);
	}

	typedef enum
	{
		CONSTRAINT_NONE			= 0,
		CONSTRAINT_POINT		= 1,
		CONSTRAINT_DISTANCE		= 2,
		CONSTRAINT_HINGE		= 3,
		CONSTRAINT_JOINT		= 4,
		CONSTRAINT_ACTUATOR		= 5,
		CONSTRAINT_FAKE_SHAKE	= 6,
		CONSTRAINT_LAUNCH		= 7,
		CONSTRAINT_ROPE			= 8,
		CONSTRAINT_LIGHT		= 9,
		NUM_CONSTRAINT_TYPES
	} ConstraintType;

	typedef enum
	{
		ATTACH_POINT_WORLD = 0x0,
		ATTACH_POINT_DYNENT = 0x1,
		ATTACH_POINT_ENT = 0x2,
		ATTACH_POINT_BONE = 0x3
	} AttachPointType;

	struct PhysConstraint
	{
		unsigned __int16 targetname;
		ConstraintType type;
		AttachPointType attach_point_type1;
		int target_index1;
		unsigned __int16 target_ent1;
		const char* target_bone1;
		AttachPointType attach_point_type2;
		int target_index2;
		unsigned __int16 target_ent2;
		const char* target_bone2;
		float offset[3];
		float pos[3];
		float pos2[3];
		float dir[3];
		int flags;
		int timeout;
		int min_health;
		int max_health;
		float distance;
		float damp;
		float power;
		float scale[3];
		float spin_scale;
		float minAngle;
		float maxAngle;
		struct Material* material;
		int constraintHandle;
		int rope_index;
		int centity_num[4];
	};

	struct PhysConstraints
	{
		const char* name;
		unsigned int count;
		PhysConstraint data[16];

		int DumpPhysConstraints();
	};

	const char* s_constraintTypeNames[10] =
	{
		"none",
		"point",
		"distance",
		"hinge",
		"joint",
		"actuator",
		"fake_shake",
		"launch",
		"rope",
		"light"
	};

	char* s_attachPointTypeNames[] =
	{
		"world",
		"dynent",
		"ent",
		"bone"
	};

	AssetEntry PhysConstraintEntries[] =
	{
		{ "targetname",					offsetof(PhysConstraint, targetname),				ASSET_ENTRY_SCRIPT_STRING,		0,											0 },
		{ "type",						offsetof(PhysConstraint, type),						ASSET_ENTRY_ENUM,				(AssetEntry*)s_constraintTypeNames,			NUM_CONSTRAINT_TYPES },
		{ "attach_point_type1",			offsetof(PhysConstraint, attach_point_type1),		ASSET_ENTRY_ENUM,				(AssetEntry*)s_attachPointTypeNames,		ATTACH_POINT_BONE + 1 },
		{ "target_index1",				offsetof(PhysConstraint, target_index1),			ASSET_ENTRY_INT,				0,											0 },
		{ "target_ent1",				offsetof(PhysConstraint, target_ent1),				ASSET_ENTRY_SCRIPT_STRING,		0,											0 },
		{ "target_bone1",				offsetof(PhysConstraint, target_bone1),				ASSET_ENTRY_STRING,				0,											0 },
		{ "attach_point_type2",			offsetof(PhysConstraint, attach_point_type2),		ASSET_ENTRY_ENUM,				(AssetEntry*)s_attachPointTypeNames,		ATTACH_POINT_BONE + 1 },
		{ "target_index2",				offsetof(PhysConstraint, target_index2),			ASSET_ENTRY_INT,				0,											0 },
		{ "target_ent2",				offsetof(PhysConstraint, target_ent2),				ASSET_ENTRY_SCRIPT_STRING,		0,											0 },
		{ "target_bone2",				offsetof(PhysConstraint, target_bone2),				ASSET_ENTRY_STRING,				0,											0 },
		{ "offset_x",					offsetof(PhysConstraint, offset),					ASSET_ENTRY_FLOAT,				0,											0 },
		{ "offset_y",					offsetof(PhysConstraint, offset) + 4,				ASSET_ENTRY_FLOAT,				0,											0 },
		{ "offset_z",					offsetof(PhysConstraint, offset) + 8,				ASSET_ENTRY_FLOAT,				0,											0 },
		{ "pos_x",						offsetof(PhysConstraint, pos),						ASSET_ENTRY_FLOAT,				0,											0 },
		{ "pos_y",						offsetof(PhysConstraint, pos) + 4,					ASSET_ENTRY_FLOAT,				0,											0 },
		{ "pos_z",						offsetof(PhysConstraint, pos) + 8,					ASSET_ENTRY_FLOAT,				0,											0 },
		{ "pos2_x",						offsetof(PhysConstraint, pos2),						ASSET_ENTRY_FLOAT,				0,											0 },
		{ "pos2_y",						offsetof(PhysConstraint, pos2) + 4,					ASSET_ENTRY_FLOAT,				0,											0 },
		{ "pos2_z",						offsetof(PhysConstraint, pos2) + 8,					ASSET_ENTRY_FLOAT,				0,											0 },
		{ "dir_x",						offsetof(PhysConstraint, dir),						ASSET_ENTRY_FLOAT,				0,											0 },
		{ "dir_y",						offsetof(PhysConstraint, dir) + 4,					ASSET_ENTRY_FLOAT,				0,											0 },
		{ "dir_z",						offsetof(PhysConstraint, dir) + 8,					ASSET_ENTRY_FLOAT,				0,											0 },
		{ "flags",						offsetof(PhysConstraint, flags),					ASSET_ENTRY_INT,				0,											0 },
		{ "timeout",					offsetof(PhysConstraint, timeout),					ASSET_ENTRY_INT,				0,											0 },
		{ "min_health",					offsetof(PhysConstraint, min_health),				ASSET_ENTRY_INT,				0,											0 },
		{ "max_health",					offsetof(PhysConstraint, max_health),				ASSET_ENTRY_INT,				0,											0 },
		{ "distance",					offsetof(PhysConstraint, distance),					ASSET_ENTRY_FLOAT,				0,											0 },
		{ "damp",						offsetof(PhysConstraint, damp),						ASSET_ENTRY_FLOAT,				0,											0 },
		{ "power",						offsetof(PhysConstraint, power),					ASSET_ENTRY_FLOAT,				0,											0 },
		{ "scale_x",					offsetof(PhysConstraint, scale),					ASSET_ENTRY_FLOAT,				0,											0 },
		{ "scale_y",					offsetof(PhysConstraint, scale) + 4,				ASSET_ENTRY_FLOAT,				0,											0 },
		{ "scale_z",					offsetof(PhysConstraint, scale) + 8,				ASSET_ENTRY_FLOAT,				0,											0 },
		{ "spin_scale",					offsetof(PhysConstraint, spin_scale),				ASSET_ENTRY_FLOAT,				0,											0 },
		{ "minAngle",					offsetof(PhysConstraint, minAngle),					ASSET_ENTRY_FLOAT,				0,											0 },
		{ "maxAngle",					offsetof(PhysConstraint, maxAngle),					ASSET_ENTRY_FLOAT,				0,											0 },
		{ "material",					offsetof(PhysConstraint, material),					ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_MATERIAL,			0 },
		{ "constraintHandle",			offsetof(PhysConstraint, constraintHandle),			ASSET_ENTRY_INT,				0,											0 },
		{ "rope_index",					offsetof(PhysConstraint, rope_index),				ASSET_ENTRY_INT,				0,											0 },
		{ "centity_num1",				offsetof(PhysConstraint, centity_num[0]),			ASSET_ENTRY_INT,				0,											0 },
		{ "centity_num2",				offsetof(PhysConstraint, centity_num[1]),			ASSET_ENTRY_INT,				0,											0 },
		{ "centity_num3",				offsetof(PhysConstraint, centity_num[2]),			ASSET_ENTRY_INT,				0,											0 },
		{ "centity_num4",				offsetof(PhysConstraint, centity_num[3]),			ASSET_ENTRY_INT,				0,											0 },
		{ 0,							0,													(AssetEntryType)0,				0,											0 }
	};

	AssetEntry PhysConstraintsEntries[] =
	{
		{ "count",		offsetof(PhysConstraints, count),			ASSET_ENTRY_UNSIGNED_INT,		0,							0 },
		{ "c1_",		offsetof(PhysConstraints, data[0]),			ASSET_ENTRY_EMBEDDED,			PhysConstraintEntries,		0 },
		{ "c2_",		offsetof(PhysConstraints, data[1]),			ASSET_ENTRY_EMBEDDED,			PhysConstraintEntries,		0 },
		{ "c3_",		offsetof(PhysConstraints, data[2]),			ASSET_ENTRY_EMBEDDED,			PhysConstraintEntries,		0 },
		{ "c4_",		offsetof(PhysConstraints, data[3]),			ASSET_ENTRY_EMBEDDED,			PhysConstraintEntries,		0 },
		{ "c5_",		offsetof(PhysConstraints, data[4]),			ASSET_ENTRY_EMBEDDED,			PhysConstraintEntries,		0 },
		{ "c6_",		offsetof(PhysConstraints, data[5]),			ASSET_ENTRY_EMBEDDED,			PhysConstraintEntries,		0 },
		{ "c7_",		offsetof(PhysConstraints, data[6]),			ASSET_ENTRY_EMBEDDED,			PhysConstraintEntries,		0 },
		{ "c8_",		offsetof(PhysConstraints, data[7]),			ASSET_ENTRY_EMBEDDED,			PhysConstraintEntries,		0 },
		{ "c9_",		offsetof(PhysConstraints, data[8]),			ASSET_ENTRY_EMBEDDED,			PhysConstraintEntries,		0 },
		{ "c10_",		offsetof(PhysConstraints, data[9]),			ASSET_ENTRY_EMBEDDED,			PhysConstraintEntries,		0 },
		{ "c11_",		offsetof(PhysConstraints, data[10]),		ASSET_ENTRY_EMBEDDED,			PhysConstraintEntries,		0 },
		{ "c12_",		offsetof(PhysConstraints, data[11]),		ASSET_ENTRY_EMBEDDED,			PhysConstraintEntries,		0 },
		{ "c13_",		offsetof(PhysConstraints, data[12]),		ASSET_ENTRY_EMBEDDED,			PhysConstraintEntries,		0 },
		{ "c14_",		offsetof(PhysConstraints, data[13]),		ASSET_ENTRY_EMBEDDED,			PhysConstraintEntries,		0 },
		{ "c15_",		offsetof(PhysConstraints, data[14]),		ASSET_ENTRY_EMBEDDED,			PhysConstraintEntries,		0 },
		{ "c16_",		offsetof(PhysConstraints, data[15]),		ASSET_ENTRY_EMBEDDED,			PhysConstraintEntries,		0 },
		{ 0,			0,											(AssetEntryType)0,				0,							0 }
	};

	int PhysConstraints::DumpPhysConstraints()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\physconstraints\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\physconstraints\\%s", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* file = fopen(outputPath, "wb");
		if (!file)
		{
			return ERROR_BAD_NETPATH;
		}

		fprintf(file, "PHYSCONSTRAINTS\\count\\%u", count);
	
		for(int i = 0; i < count; i++)
		{
			char tempBuf[10];
			sprintf(tempBuf, "c%i_", i + 1);

			for (int j = 0; PhysConstraintEntries[j].name; j++)
			{
				DumpAssetEntry(
					file,
					PhysConstraintEntries[j].type,
					PhysConstraintEntries[j].name,
					(void*)((int)&data[i] + PhysConstraintEntries[j].offset), PhysConstraintEntries[j].next, PhysConstraintEntries[j].enumCount, tempBuf);
			}
		}

		fclose(file);
		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_PhysConstraintsAsset_Hook;
	void Load_PhysConstraintsAsset(PhysConstraints** input)
	{
		PhysConstraints* currentPhysConstraints = *input;
		(*input)->DumpPhysConstraints();

		// TODO: Add loading from disk

		auto Invoke = Load_PhysConstraintsAsset_Hook.Invoke<void(*)(PhysConstraints**)>();
		Invoke(input);
	}

	struct DestructibleStage
	{
		unsigned __int16 showBone;
		float breakHealth;
		float maxTime;
		unsigned int flags;
		struct FxEffectDef* breakEffect;
		const char* breakSound;
		const char* breakNotify;
		const char* loopSound;
		struct XModel* spawnModel[3];
		PhysPreset* physPreset;
	};

	struct DestructiblePiece
	{
		DestructibleStage stages[5];
		char parentPiece;
		float parentDamagePercent;
		float bulletDamageScale;
		float explosiveDamageScale;
		float meleeDamageScale;
		float impactDamageScale;
		float entityDamageTransfer;
		PhysConstraints *physConstraints;
		int health;
		const char* damageSound;
		struct FxEffectDef* burnEffect;
		const char* burnSound;
		unsigned __int16 enableLabel;
		int hideBones[5];

		int DumpDestructiblePiece(const char* parentName, int pieceIndex);
	};

	AssetEntry DestructibleStageEntries[] =
	{
		{ "showBone",			offsetof(DestructibleStage, showBone),			ASSET_ENTRY_SCRIPT_STRING,		0,										0 },
		{ "breakHealth",		offsetof(DestructibleStage, breakHealth),		ASSET_ENTRY_FLOAT,				0,										0 },
		{ "maxTime",			offsetof(DestructibleStage, maxTime),			ASSET_ENTRY_FLOAT,				0,										0 },
		{ "flags",				offsetof(DestructibleStage, flags),				ASSET_ENTRY_UNSIGNED_INT,		0,										0 },
		{ "breakEffect",		offsetof(DestructibleStage, breakEffect),		ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_FX,				0 },
		{ "breakSound",			offsetof(DestructibleStage, breakSound),		ASSET_ENTRY_STRING,				0,										0 },
		{ "breakNotify",		offsetof(DestructibleStage, breakNotify),		ASSET_ENTRY_STRING,				0,										0 },
		{ "loopSound",			offsetof(DestructibleStage, loopSound),			ASSET_ENTRY_STRING,				0,										0 },
		{ "spawnModel1",		offsetof(DestructibleStage, spawnModel[0]),		ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_XMODEL,			0 },
		{ "spawnModel2",		offsetof(DestructibleStage, spawnModel[1]),		ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_XMODEL,			0 },
		{ "spawnModel3",		offsetof(DestructibleStage, spawnModel[2]),		ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_XMODEL,			0 },
		{ "physPreset",			offsetof(DestructibleStage, physPreset),		ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_PHYSPRESET,		0 },
		{ 0,					0,												(AssetEntryType)0,				0,										0 }
	};

	AssetEntry DestructiblePieceEntries[] =
	{
		{ "",				offsetof(DestructiblePiece, stages[0]),				ASSET_ENTRY_EMBEDDED_REVERSED,		DestructibleStageEntries,0},
		{ "D1",				offsetof(DestructiblePiece, stages[1]),				ASSET_ENTRY_EMBEDDED_REVERSED,		DestructibleStageEntries,0},
		{ "D2",				offsetof(DestructiblePiece, stages[2]),				ASSET_ENTRY_EMBEDDED_REVERSED,		DestructibleStageEntries,0},
		{ "D3",				offsetof(DestructiblePiece, stages[3]),				ASSET_ENTRY_EMBEDDED_REVERSED,		DestructibleStageEntries,0},
		{ "D4",				offsetof(DestructiblePiece, stages[4]),				ASSET_ENTRY_EMBEDDED_REVERSED,		DestructibleStageEntries,0},
		{ "parentPiece",		offsetof(DestructiblePiece, parentPiece),			ASSET_ENTRY_BYTE,0,0},
		{ "parentDamagePercent",			offsetof(DestructiblePiece, parentDamagePercent),			ASSET_ENTRY_FLOAT,0,0},
		{ "bulletDamageScale",			offsetof(DestructiblePiece, bulletDamageScale),			ASSET_ENTRY_FLOAT,0,0},
		{ "explosiveDamageScale",			offsetof(DestructiblePiece, explosiveDamageScale),			ASSET_ENTRY_FLOAT,0,0},
		{ "meleeDamageScale",			offsetof(DestructiblePiece, meleeDamageScale),			ASSET_ENTRY_FLOAT,0,0},
		{ "impactDamageScale",			offsetof(DestructiblePiece, impactDamageScale),			ASSET_ENTRY_FLOAT,0,0},
		{ "entityDamageTransfer",			offsetof(DestructiblePiece, entityDamageTransfer),			ASSET_ENTRY_FLOAT,0,0},
		{ "physConstraints",		offsetof(DestructiblePiece, physConstraints),		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_PHYSCONSTRAINTS,0},
		{ "health",			offsetof(DestructiblePiece, health),			ASSET_ENTRY_INT,0,0},
		{ "damageSound",			offsetof(DestructiblePiece, damageSound),				ASSET_ENTRY_STRING,0,0},
		{ "burnEffect",		offsetof(DestructiblePiece, burnEffect),		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,0},
		{ "burnSound",			offsetof(DestructiblePiece, burnSound),				ASSET_ENTRY_STRING,0,0},
		{ "enableLabel",			offsetof(DestructiblePiece, enableLabel),				ASSET_ENTRY_SCRIPT_STRING,0,0},
		{ "hideBone1",			offsetof(DestructiblePiece, hideBones[0]),			ASSET_ENTRY_INT,0,0},
		{ "hideBone2",			offsetof(DestructiblePiece, hideBones[1]),			ASSET_ENTRY_INT,0,0},
		{ "hideBone3",			offsetof(DestructiblePiece, hideBones[2]),			ASSET_ENTRY_INT,0,0},
		{ "hideBone4",			offsetof(DestructiblePiece, hideBones[3]),			ASSET_ENTRY_INT,0,0},
		{ "hideBone5",			offsetof(DestructiblePiece, hideBones[4]),			ASSET_ENTRY_INT,0,0},
		{ 0, 0, (AssetEntryType)0,0,0}
	};

	int DestructiblePiece::DumpDestructiblePiece(const char* parentName, int pieceIndex)
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\destructiblepiece\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", parentName);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\destructiblepiece\\%s_%i", (char*)parentName, pieceIndex);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", parentName, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* file = fopen(outputPath, "w");
		if (!file)
		{
			return ERROR_BAD_NETPATH;
		}

		fprintf(file, "DESTRUCTIBLEPIECE");

		for (int i = 0; DestructiblePieceEntries[i].name; i++)
		{
			DumpAssetEntry(
				file,
				DestructiblePieceEntries[i].type,
				DestructiblePieceEntries[i].name,
				(void*)((int)(this) + DestructiblePieceEntries[i].offset),
				DestructiblePieceEntries[i].next,
				DestructiblePieceEntries[i].enumCount);
		}

		fclose(file);
		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", parentName);
		return ERROR_SUCCESS;
	}

	struct DestructibleDef
	{
		const char* name;
		struct XModel* model;
		struct XModel* pristineModel;
		int numPieces;
		DestructiblePiece* pieces;
		int clientOnly;

		int DumpDestructibleDef();
	};

	AssetEntry DestructibleDefEntries[] =
	{
		{ "model",				offsetof(DestructibleDef, model),				ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_XMODEL,		0 },
		{ "pristineModel",		offsetof(DestructibleDef, pristineModel),		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_XMODEL,		0 },
		{ "numPieces",			offsetof(DestructibleDef, numPieces),			ASSET_ENTRY_INT,			0,									0 },
		{ "clientOnly",			offsetof(DestructibleDef, clientOnly),			ASSET_ENTRY_INT,			0,									0 },
		{ 0,					0,												(AssetEntryType)0,			0,									0 }
	};

	int DestructibleDef::DumpDestructibleDef()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\destructibledef\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\destructibledef\\%s", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* file = fopen(outputPath, "w");
		if (!file)
		{
			return ERROR_BAD_NETPATH;
		}

		fprintf(file, "DESTRUCTIBLEDEF");

		for (int i = 0; DestructibleDefEntries[i].name; i++)
		{
			DumpAssetEntry(
				file,
				DestructibleDefEntries[i].type,
				DestructibleDefEntries[i].name,
				(void*)((int)(this) + DestructibleDefEntries[i].offset),
				DestructibleDefEntries[i].next,
				DestructibleDefEntries[i].enumCount);
		}
	
		for (int i = 0; i < numPieces; i++)
		{
			fprintf(file, "\\piece%i\\%s_%i", i, name, i);
			pieces[i].DumpDestructiblePiece(name, i);
		}

		fclose(file);
		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_DestructibleDefAsset_Hook;
	void Load_DestructibleDefAsset(DestructibleDef** input)
	{
		DestructibleDef* currentDestructibleDef = *input;
		(*input)->DumpDestructibleDef();

		// TODO: Add loading from disk

		auto Invoke = Load_DestructibleDefAsset_Hook.Invoke<void(*)(DestructibleDef**)>();
		Invoke(input);
	}

	struct XModel
	{
		const char* name;
		char numBones;
		char numRootBones;
		char numsurfs;
		char lodRampType;
		unsigned __int16 *boneNames;
		char *parentList;
		__int16 *quats;
		float *trans;
		char *partClassification;
		//DObjAnimMat *baseMat;
		//XSurface* surfs;
		struct Material **materialHandles;
		//XModelLodInfo lodInfo[4];
		//XModelCollSurf_s *collSurfs;
		int numCollSurfs;
		int contents;
		//XBoneInfo *boneInfo;
		float radius;
		//vec3_t mins;
		//vec3_t maxs;
		__int16 numLods;
		__int16 collLod;
		float *himipInvSqRadii;
		int memUsage;
		int flags;
		bool bad;
		PhysPreset *physPreset;
		char numCollmaps;
		//Collmap *collmaps;
		PhysConstraints *physConstraints;
		//vec3_t lightingOriginOffset;
		float lightingOriginRange;

		int DumpXModelAsset();
	};

	int XModel::DumpXModelAsset()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\xmodel\\", 0);
		CreateDirectory(Utils::String::Va("game:\\Alpha\\dump\\xmodel\\%s", name), 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\xmodel\\%s", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* file = fopen(outputPath, "w");
		if (!file)
		{
			return ERROR_BAD_NETPATH;
		}

		// TODO: properly dump

		fclose(file);
		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_XModelAsset_Hook;
	void Load_XModelAsset(XModel** input)
	{
		XModel* currentXModel = *input;
		(*input)->DumpXModelAsset();

		// TODO: Add loading from disk

		auto Invoke = Load_XModelAsset_Hook.Invoke<void(*)(XModel**)>();
		Invoke(input);
	}

	struct GfxDrawSurfFields
	{
		unsigned __int64 objectId				: 16;
		unsigned __int64 customIndex			: 9;
		unsigned __int64 reflectionProbeIndex	: 5;
		unsigned __int64 dlightMask				: 2;
		unsigned __int64 materialSortedIndex	: 12;
		unsigned __int64 primaryLightIndex		: 8;
		unsigned __int64 surfType				: 4;
		unsigned __int64 prepass				: 2;
		unsigned __int64 primarySortKey			: 6;
	};

	union GfxDrawSurf
	{
		GfxDrawSurfFields fields;
		unsigned __int64 packed;
	};

	struct MaterialInfo
	{
		const char* name;
		unsigned int gameFlags;
		unsigned __int8 pad;
		unsigned __int8 sortKey;
		unsigned __int8 textureAtlasRowCount;
		unsigned __int8 textureAtlasColumnCount;
		GfxDrawSurf drawSurf;
		unsigned int surfaceTypeBits;
		unsigned int layeredSurfaceTypes;
		unsigned __int16 hashIndex;
		int surfaceFlags;
		int contents;
	};

	struct MaterialTextureDef
	{
		unsigned int nameHash;
		char nameStart;
		char nameEnd;
		unsigned __int8 samplerState;
		unsigned __int8 semantic;
		unsigned __int8 isMatureContent;
		unsigned __int8 pad[3];
		struct GfxImage* image;
	};

	struct MaterialConstantDef
	{
		unsigned int nameHash;
		char name[12];
		float literal[4];
	};

	struct Material
	{
		MaterialInfo info;
		unsigned __int8 stateBitsEntry[36];
		unsigned __int8 textureCount;
		unsigned __int8 constantCount;
		unsigned __int8 stateBitsCount;
		unsigned __int8 stateFlags;
		unsigned __int8 cameraRegion;
		unsigned __int8 probeMipBits;
		struct MaterialTechniqueSet* localTechniqueSet;
		struct MaterialTechniqueSet* techniqueSet;
		MaterialTextureDef* textureTable;
		MaterialConstantDef* localConstantTable;
		MaterialConstantDef* constantTable;

		//GfxStateBits* stateBitsTable;
		Material* thermalMaterial;

		int DumpMaterialAsset();
	};

	int Material::DumpMaterialAsset()
	{
	CreateDirectory("game:\\Alpha\\dump\\materials\\", 0);

	const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\materials\\%s.raw", info.name);
	FILE* file = fopen(outputPath, "wb");
	if (!file) return ERROR_BAD_NETPATH;

	// -------------------------
	// Write MaterialInfoRaw
	// -------------------------

	fwrite(&info.gameFlags, sizeof(info.gameFlags), 1, file);
	fwrite(&info.surfaceTypeBits, sizeof(info.surfaceTypeBits), 1, file);
	fwrite(&info.pad, sizeof(info.pad), 1, file);
	fwrite(&info.sortKey, sizeof(info.sortKey), 1, file);
	fwrite(&info.textureAtlasRowCount, sizeof(info.textureAtlasRowCount), 1, file);
	fwrite(&info.textureAtlasColumnCount, sizeof(info.textureAtlasColumnCount), 1, file);

	fwrite(&info.drawSurf, sizeof(info.drawSurf), 1, file);
	fwrite(&info.layeredSurfaceTypes, sizeof(info.layeredSurfaceTypes), 1, file);
	fwrite(&info.hashIndex, sizeof(info.hashIndex), 1, file);
	fwrite(&info.surfaceFlags, sizeof(info.surfaceFlags), 1, file);
	fwrite(&info.contents, sizeof(info.contents), 1, file);

	// nameOffset: 0 (we’ll write name at end)
	uint32_t nameOffset = 0x58; // After header+tables
	fwrite(&nameOffset, sizeof(nameOffset), 1, file);

	// -------------------------
	// Texture count + table
	// -------------------------
	fwrite(&textureCount, sizeof(textureCount), 1, file);

	if (textureTable && textureCount > 0)
	{
		uint32_t texNameOffset = 0x58 + 0x0A; // start of string blob (example offset)
		fwrite(&texNameOffset, sizeof(texNameOffset), 1, file);
		fwrite(&textureTable[0].samplerState, sizeof(textureTable[0].samplerState), 1, file);
		fwrite(&textureTable[0].semantic, sizeof(textureTable[0].semantic), 1, file);
		uint16_t pad = 0;
		fwrite(&pad, sizeof(pad), 1, file);

		uint32_t imageNameOffset = texNameOffset + 8;
		fwrite(&imageNameOffset, sizeof(imageNameOffset), 1, file);
	}

	// -------------------------
	// TechniqueSet, offset tables
	// -------------------------
	uint32_t techniqueSetOffset = 0x40;
	uint32_t textureTableOffset = 0x4C;
	uint32_t constantTableOffset = 0x60;
	fwrite(&techniqueSetOffset, sizeof(techniqueSetOffset), 1, file);
	fwrite(&textureTableOffset, sizeof(textureTableOffset), 1, file);
	fwrite(&constantTableOffset, sizeof(constantTableOffset), 1, file);

	fwrite(&constantCount, sizeof(constantCount), 1, file);
	fwrite(&stateBitsCount, sizeof(stateBitsCount), 1, file);
	fwrite(&stateFlags, sizeof(stateFlags), 1, file);
	fwrite(&cameraRegion, sizeof(cameraRegion), 1, file);

	// -------------------------
	// String blob (e.g. "default", "colorMap")
	// -------------------------
	fwrite("default\0", 8, 1, file);
	fwrite("default\0", 8, 1, file);
	fwrite("colorMap\0", 9, 1, file);

	fclose(file);
	Symbols::Com_Printf(0, "Wrote partial material '%s' to %s\n", info.name, outputPath);
	return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_MaterialAsset_Hook;
	void Load_MaterialAsset(Material** input)
	{
		Material* currentMaterial = *input;
		(*input)->DumpMaterialAsset();

		// TODO: Add loading from disk

		auto Invoke = Load_MaterialAsset_Hook.Invoke<void(*)(Material**)>();
		Invoke(input);
	}

	struct MaterialStreamRouting
	{
		unsigned __int8 source;
		unsigned __int8 dest;
	};

	struct MaterialVertexStreamRouting
	{
		MaterialStreamRouting data[16];
		IDirect3DVertexDeclaration9* decl[18];
	};

	struct MaterialVertexDeclaration
	{
		unsigned __int8 streamCount;
		bool hasOptionalSource;
		bool isLoaded;
		MaterialVertexStreamRouting routing;
	};

	struct MaterialVertexShaderProgram
	{
		IDirect3DVertexShader9 *vs;
	};

	struct MaterialVertexShader
	{
		const char* name;
		MaterialVertexShaderProgram prog;
	};

	struct MaterialPixelShaderProgram
	{
		IDirect3DPixelShader9* ps;
	};

	struct MaterialPixelShader
	{
		const char* name;
		MaterialPixelShaderProgram prog;
	};

	union MaterialArgumentLocation
	{
		unsigned __int16 offset;
		unsigned __int8 textureIndex;
		unsigned __int8 samplerIndex;
	};

	struct MaterialArgumentCodeConst
	{
		unsigned __int16 index;
		unsigned __int8 firstRow;
		unsigned __int8 rowCount;
	};

	union MaterialArgumentDef
	{
		const float* literalConst;
		MaterialArgumentCodeConst codeConst;
		unsigned int codeSampler;
		unsigned int nameHash;
	};

	struct MaterialShaderArgument
	{
		unsigned __int16 type;
		MaterialArgumentLocation location;
		unsigned __int16 size;
		unsigned __int16 buffer;
		MaterialArgumentDef u;
	};

	struct MaterialPass
	{
		MaterialVertexDeclaration* vertexDecl;
		MaterialVertexShader* vertexShader;
		MaterialPixelShader* pixelShader;
		MaterialPixelShader* localPixelShader;
		unsigned __int8 perPrimArgCount;
		unsigned __int8 perObjArgCount;
		unsigned __int8 stableArgCount;
		unsigned __int8 customSamplerFlags;
		unsigned __int8 precompiledIndex;
		unsigned __int8 materialType;
		MaterialShaderArgument* localArgs;
		MaterialShaderArgument* args;
	};

	struct MaterialTechnique
	{
		const char* name;
		unsigned __int16 flags;
		unsigned __int16 passCount;
		MaterialPass passArray[1];
	};

	struct MaterialTechniqueSet
	{
		const char* name;
		unsigned __int8 worldVertFormat;
		MaterialTechnique* techniques[36];

		int DumpMaterialTechniqueSetAsset();
	};

	int DumpMaterialTechnique(MaterialTechnique* tech, const char* filename)
	{
		FILE* f = fopen(filename, "wb");
		if (!f)
		{
			Symbols::Com_PrintError(1, "Failed to open '%s' for writing!\n", filename);
			return ERROR_CANNOT_MAKE;
		}

		fprintf(f, "{\n");

		// This matches the stateMap line
		fprintf(f, "\tstateMap \"%s\";\n\n", tech->name ? tech->name : "unknown");

		for (int i = 0; i < tech->passCount; ++i)
		{
			MaterialPass* pass = &tech->passArray[i];

			// Vertex Shader
			if (pass->vertexShader && pass->vertexShader->name)
			{
				fprintf(f, "\tvertexShader 2.0 \"%s\"\n", pass->vertexShader->name);
				fprintf(f, "\t{\n\t}\n\n");
			}

			// Pixel Shader
			if (pass->pixelShader && pass->pixelShader->name)
			{
				fprintf(f, "\tpixelShader 2.0 \"%s\"\n", pass->pixelShader->name);
				fprintf(f, "\t{\n\t}\n\n");
			}

			// Vertex attrib remaps
			if (pass->vertexDecl)
			{
				const MaterialVertexStreamRouting& routing = pass->vertexDecl->routing;
				for (int k = 0; k < 16; ++k)
				{
					const MaterialStreamRouting& route = routing.data[k];
					if (route.source != 0 || route.dest != 0)
					{
						fprintf(f, "\tvertex.attrib%d = stream%d;\n", route.dest, route.source);
					}
				}
			}
		}

		fprintf(f, "}\n");
		fclose(f);
		return ERROR_SUCCESS;
	}

	int MaterialTechniqueSet::DumpMaterialTechniqueSetAsset()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\techniques\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\techniques\\%s.tech", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		MaterialTechnique* lastTech = nullptr;

		for (int i = 0; i < 36; ++i)
		{
			MaterialTechnique* tech = techniques[i];
			if (!tech || tech == lastTech)
				continue;

			lastTech = tech;

			const char* filename = Utils::String::Va("game:\\Alpha\\dump\\techniques\\%s.tech", tech->name);
			int result = DumpMaterialTechnique(tech, filename);
			if (result != ERROR_SUCCESS)
			{
				Symbols::Com_PrintWarning(0, "Failed to dump technique '%s'\n", tech->name);
			}
		}
		return ERROR_SUCCESS;
	}

	// TODO: More research needed
	Utils::Hook::Detour Load_MaterialTechniqueSetAsset_Hook;
	void Load_MaterialTechniqueSetAsset(MaterialTechniqueSet** input)
	{
		MaterialTechniqueSet* currentTechSet = *input;
		(*input)->DumpMaterialTechniqueSetAsset();

		auto invoke = Load_MaterialTechniqueSetAsset_Hook.Invoke<void(*)(MaterialTechniqueSet**)>();
		invoke(input);
	}

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
			if (basemap.Format.Dimension != GPUDIMENSION_2D &&
				basemap.Format.Dimension != GPUDIMENSION_CUBEMAP)
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
			const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\images\\%s.dds", imageName);
			if (Utils::FileSystem::FileExists(outputPath))
			{
				Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
				return ERROR_DUP_NAME;
			}

			// Create just in case it doesn't exist
			CreateDirectory("game:\\Alpha\\dump\\images", 0);

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

		switch (basemap.Format.Endian)
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

	Utils::Hook::Detour Load_GfxImageAsset_Hook;
	void Load_GfxImageAsset(GfxImage** input)
	{
		(*input)->dumpGfxImageAsset();

		auto Invoke = Load_GfxImageAsset_Hook.Invoke<void(*)(GfxImage**)>();
		Invoke(input);
	}

	typedef struct
	{
		int contents;
		unsigned __int16 hullCount;
		unsigned __int16 firstHull;
	} TriggerModel;

	typedef struct
	{
		float midPoint[3];
		float halfSize[3];
	} Bounds;
 
	typedef struct
	{
		Bounds bounds;
		int contents;
		unsigned __int16 slabCount;
		unsigned __int16 firstSlab;
	} TriggerHull;
 
	typedef struct
	{
		float dir[3];
		float midPoint;
		float halfSize;
	} TriggerSlab;
 
	typedef struct
	{
		unsigned int count;
		TriggerModel* models;
		unsigned int hullCount;
		TriggerHull* hulls;
		unsigned int slabCount;
		TriggerSlab* slabs;
	} MapTriggers;
 
	typedef struct
	{
		const char* name;
		char* entityString;
		int numEntityChars;
		MapTriggers trigger;

		int DumpMapEntities();
	} MapEnts;

	int MapEnts::DumpMapEntities()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\%s", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* file = fopen(outputPath, "wb");
		if (!file)
		{
			return ERROR_BAD_NETPATH;
		}
	
		fwrite(entityString, strlen(entityString), 1, file);
		fclose(file);

		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_MapEntsAsset_Hook;
	void Load_MapEntsAsset(MapEnts** input)
	{
		(*input)->DumpMapEntities();

		// TODO: Add loading from disk

		auto Invoke = Load_MapEntsAsset_Hook.Invoke<void(*)(MapEnts**)>();
		Invoke(input);
	}

	#pragma pack(push, 4)
	typedef struct
	{
		GfxImage* image;
		char samplerState;
	} GfxLightImage;
	#pragma pack(pop)
 
	typedef struct
	{
		const char* name;
		GfxLightImage attenuation;
		int lmapLookupStart;

		int DumpGfxLightDef();
	} GfxLightDef;

	int GfxLightDef::DumpGfxLightDef()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\lights\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\lights\\%s", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* file = fopen(outputPath, "wb");
		if (!file)
		{
			return ERROR_BAD_NETPATH;
		}
	
		fprintf(file, "%c", attenuation.samplerState);
		fwrite(attenuation.image->name, 1, strlen(attenuation.image->name) + 1, file);
		fclose(file);

		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_LightDefAsset_Hook;
	void Load_LightDefAsset(GfxLightDef** input)
	{
		(*input)->DumpGfxLightDef();

		// TODO: Add loading from disk

		auto Invoke = Load_LightDefAsset_Hook.Invoke<void(*)(GfxLightDef**)>();
		Invoke(input);
	}

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

	/* 1817 */
	struct KerningPairs
	{
	  unsigned __int16 wFirst;
	  unsigned __int16 wSecond;
	  int iKernAmount;
	};

	/* 1818 */
	typedef struct Font_s
	{
		const char* fontName;
		int pixelHeight;
		int isScalingAllowed;
		int glyphCount;
		int kerningPairsCount;
		Material *material;
		Material *glowMaterial;
		Glyph *glyphs;
		KerningPairs *kerningPairs;

	  int DumpFontAsset();
	} Font;

	int Font::DumpFontAsset()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", fontName);

		const uint8_t* blobStart = reinterpret_cast<const uint8_t*>(glyphs);
		const char* fontNames = fontName;
		const char* materialName = material->info.name;

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\%s", fontName);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", fontName, outputPath);
			return ERROR_DUP_NAME;
		}

	// Calculate relative offsets (from +0x10 base)
	const uint32_t fontNameOffset = (uint32_t)((const uint8_t*)fontNames - blobStart) + 16;
	const uint32_t materialNameOffset = (uint32_t)((const uint8_t*)materialName - blobStart) + 16;

	// Calculate blob size (glyphs + fontName + materialName)
	const uint8_t* blobEnd = std::max(
		(const uint8_t*)(fontNames + strlen(fontNames) + 1),
		(const uint8_t*)(materialName + strlen(materialName) + 1)
	);
	const uint32_t blobSize = (uint32_t)(blobEnd - blobStart);

		FILE* file = fopen(outputPath, "wb");
		if (!file)
		{
			return ERROR_BAD_NETPATH;
		}
	
	// Write header
	fwrite(&fontNameOffset, sizeof(uint32_t), 1, file);
	fwrite(&pixelHeight, sizeof(uint32_t), 1, file);
	fwrite(&glyphCount, sizeof(uint32_t), 1, file);
	fwrite(&materialNameOffset, sizeof(uint32_t), 1, file);

	// Write glyphs + fontName + materialName blob
	fwrite(blobStart, 1, blobSize, file);
	fclose(file);

		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", fontName);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_FontAsset_Hook;
	void Load_FontAsset(Font** input)
	{
		(*input)->DumpFontAsset();

		// TODO: Add loading from disk

		auto Invoke = Load_FontAsset_Hook.Invoke<void(*)(Font**)>();
		Invoke(input);
	}

	typedef struct
	{
		const char* szInternalName;

		int DumpWeaponVariantDefAsset();
	} WeaponVariantDef;

	int WeaponVariantDef::DumpWeaponVariantDefAsset()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\weapons\\", 0);
		CreateDirectory("game:\\Alpha\\dump\\weapons\\mp\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", szInternalName);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\weapons\\mp\\%s", szInternalName);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", szInternalName, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* f = fopen(outputPath, "w");
		if (!f)
		{
			Symbols::Com_PrintError(1, "Failed to open '%s' for writing!\n", szInternalName);
			return ERROR_CANNOT_MAKE;
		}

		fclose(f);
		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", szInternalName);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_WeaponVariantDefAsset_Hook;
	void Load_WeaponVariantDefAsset(WeaponVariantDef** input)
	{
		(*input)->DumpWeaponVariantDefAsset();

		// TODO: Add loading from disk

		auto Invoke = Load_WeaponVariantDefAsset_Hook.Invoke<void(*)(WeaponVariantDef**)>();
		Invoke(input);
	}

	// TODO: add menu def and menu list dumping

	// TODO: Add loc string dumping

	// TODO: Add weapon def dumping

	struct FxSpawnDefLooping
	{
		int intervalMsec;
		int count;
	};

	struct FxIntRange
	{
		int base;
		int amplitude;
	};

	struct FxSpawnDefOneShot
	{
		FxIntRange count;
	};

	union FxSpawnDef
	{
		FxSpawnDefLooping looping;
		FxSpawnDefOneShot oneShot;
	};

	struct FxFloatRange
	{
		float base;
		float amplitude;
	};

	struct FxElemAtlas
	{
		unsigned __int8 behavior;
		unsigned __int8 index;
		unsigned __int8 fps;
		unsigned __int8 loopCount;
		unsigned __int8 colIndexBits;
		unsigned __int8 rowIndexBits;
		unsigned __int16 entryCountAndIndexRange;
	};

	struct FxElemVec3Range
	{
		float base[3];
		float amplitude[3];
	};

	struct FxElemVelStateInFrame
	{
		FxElemVec3Range velocity;
		FxElemVec3Range totalDelta;
	};

	struct FxElemVelStateSample
	{
		FxElemVelStateInFrame local;
		FxElemVelStateInFrame world;
	};

	struct FxElemVisualState
	{
		unsigned __int8 color[4];
		float rotationDelta;
		float rotationTotal;
		float size[2];
		float scale;
	};

	struct FxElemVisStateSample
	{
		FxElemVisualState base;
		FxElemVisualState amplitude;
	};

	struct FxElemMarkVisuals
	{
		Material* materials[2];
	};

	union FxEffectDefRef
	{
		FxEffectDef* handle;
		const char* name;
	};

	union FxElemVisuals
	{
		const void* anonymous;
		Material* material;
		XModel* model;
		FxEffectDefRef effectDef;
		const char* soundName;
		GfxLightDef* lightDef;
	};

	union FxElemDefVisuals
	{
		FxElemMarkVisuals* markArray;
		FxElemVisuals* array;
		FxElemVisuals instance;
	};

	struct FxTrailVertex
	{
		float pos[2];
		float normal[2];
		float texCoord;
	};

	struct FxTrailDef
	{
		int scrollTimeMsec;
		int repeatDist;
		int splitDist;
		int vertCount;

		union
		{
			FxTrailVertex* vertsEA;
			FxTrailVertex* verts;
		};

		int indCount;

		union
		{
			unsigned __int16* indsEA;
			unsigned __int16* inds;
		};
	};

	struct FxSpotLightDef
	{
		float fovInnerFraction;
		float startRadius;
		float endRadius;
	};

	union FxElemExtendedDefPtr
	{
		FxTrailDef* localTrailDef;
		FxTrailDef* trailDef;
		FxSpotLightDef* localSpotLightDef;
		FxSpotLightDef* spotLightDef;
		void* unknownDef;
	};

	struct FxBillboardTrim
	{
		float topWidth;
		float bottomWidth;
	};

	union FxElemDefUnion
	{
		FxBillboardTrim billboard;
		FxIntRange cloudDensityRange;
	};

	struct FxElemSpawnSound
	{
		const char* spawnSound;
	};

	const struct FxElemDef
	{
		int flags;
		FxSpawnDef spawn;
		FxFloatRange spawnRange;
		FxFloatRange fadeInRange;
		FxFloatRange fadeOutRange;
		float spawnFrustumCullRadius;
		FxIntRange spawnDelayMsec;
		FxIntRange lifeSpanMsec;
		FxFloatRange spawnOrigin[3];
		FxFloatRange spawnOffsetRadius;
		FxFloatRange spawnOffsetHeight;
		FxFloatRange spawnAngles[3];
		FxFloatRange angularVelocity[3];
		FxFloatRange initialRotation;
		unsigned int rotationAxis;
		FxFloatRange gravity;
		FxFloatRange reflectionFactor;
		FxElemAtlas atlas;
		float windInfluence;
		unsigned __int8 elemType;
		unsigned __int8 visualCount;
		unsigned __int8 velIntervalCount;
		unsigned __int8 visStateIntervalCount;
		FxElemVelStateSample* velSamples;
		FxElemVisStateSample* visSamples;
		FxElemDefVisuals visuals;
		float collMins[3];
		float collMaxs[3];
		FxEffectDefRef effectOnImpact;
		FxEffectDefRef effectOnDeath;
		FxEffectDefRef effectEmitted;
		FxFloatRange emitDist;
		FxFloatRange emitDistVariance;
		FxEffectDefRef effectAttached;
		FxElemExtendedDefPtr extended;
		unsigned __int8 sortOrder;
		unsigned __int8 lightingFrac;
		unsigned __int8 unused[2];
		unsigned __int16 alphaFadeTimeMsec;
		unsigned __int16 maxWindStrength;
		unsigned __int16 spawnIntervalAtMaxWind;
		unsigned __int16 lifespanAtMaxWind;
		FxElemDefUnion u;
		FxElemSpawnSound spawnSound;
		float billboardPivot[2];
	};

	struct FxEffectDef
	{
		const char* name;
		unsigned __int16 flags;
		unsigned __int8 efPriority;
		__int16 elemDefCountLooping;
		__int16 elemDefCountOneShot;
		__int16 elemDefCountEmission;
		int totalSize;
		int msecLoopingLife;
		int msecNonLoopingLife;

		union
		{
			FxElemDef* elemDefs;
			FxElemDef* localElemDefs;
		};

		float efBoundingBoxDim[3];
		float efBoundingBoxCentre[3];

		float occlusionQueryDepthBias;
		int occlusionQueryFadeIn;
		int occlusionQueryFadeOut;
		FxFloatRange occlusionQueryScaleRange;

		int DumpFxEffectDefAsset();
	};

	void DumpFxEffectHeader(FILE* f, FxEffectDef def)
	{
		// Dump the header for the .efx

		// Version number.
		fprintf(
			f,
			"%s %i\n\n",
			"iwfx",
			3 // Version is hardcoded to '3' in T5 and T6.
		);
		// Priority for the FX.
		fprintf(
			f,
			"\t%s %i;\n",
			"efPriority",
			def.efPriority
		);
		// Bounding box dim.
		fprintf(
			f,
			"\t%s %g %g %g;\n",
			"efBoundingBoxDim",
			def.efBoundingBoxDim[0], // X
			def.efBoundingBoxDim[1], // Y
			def.efBoundingBoxDim[2]	 // Z
		);
		// Bounding box centre.
		fprintf(
			f,
			"\t%s %g %g %g;\n",
			"efBoundingBoxCentre",
			def.efBoundingBoxCentre[0], // X
			def.efBoundingBoxCentre[1], // Y
			def.efBoundingBoxCentre[2]	// Z
		);
	}

	void DumpFxEffectContents(FILE* f, FxEffectDef def, const char* name)
	{
		fprintf(f, "{\n");

		fprintf(f, "\t%s \"%s\";\n", "name", name);
		fprintf(f, "\t%s \"%s\";\n", "editorFlags", "def.flags");
		fprintf(f, "\t%s \"%s\";\n", "flags", "def.flags");
		fprintf(f, "\t%s %g %g;\n", "spawnRange", def.elemDefs->spawnRange.base, def.elemDefs->spawnRange.amplitude);

		fprintf(f, "}\n");
	}

	int FxEffectDef::DumpFxEffectDefAsset()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\fx\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\fx\\%s.efx", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* f = fopen(outputPath, "wb");
		if (!f)
		{
			Symbols::Com_PrintError(1, "Failed to open '%s' for writing!\n", name);
			return ERROR_CANNOT_MAKE;
		}

		// Write the header.
		DumpFxEffectHeader(f, *this);

		// Write the .efx contents.
		DumpFxEffectContents(f, *this, name);

		fclose(f);
		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_FxEffectDefAsset_Hook;
	void Load_FxEffectDefAsset(FxEffectDef** input)
	{
		(*input)->DumpFxEffectDefAsset();

		// TODO: Add loading from disk

		auto Invoke = Load_FxEffectDefAsset_Hook.Invoke<void(*)(FxEffectDef**)>();
		Invoke(input);
	}

	typedef struct
	{
		const char* name;
		int len;
		const char* buffer;

		int DumpRawFileAsset();
	} RawFile;

	int DecompressAnimtree(const void* buffer, size_t len, const char* outputPath, const char* assetName)
	{
		if (len <= 4)
		{
			Symbols::Com_PrintError(1, "Invalid size of animtree file '%s'!\n", assetName);
			return ERROR_INVALID_DATA;
		}

		uint32_t expectedDecompressedSize = *(const uint32_t*)buffer;

		z_stream stream;
		memset(&stream, 0, sizeof(stream));
		stream.next_in = (Bytef*)((const char*)buffer + 4);
		stream.avail_in = (uInt)(len - 4);

		int ret = inflateInit2(&stream, -15); // raw DEFLATE
		if (ret != Z_OK)
		{
			Symbols::Com_PrintError(1, "inflateInit2 failed for '%s' with error %d\n", assetName, ret);
			return ERROR_INVALID_DATA;
		}

		FILE* f = fopen(outputPath, "wb");
		if (!f)
		{
			Symbols::Com_PrintError(1, "Failed to open '%s' for writing\n", outputPath);
			inflateEnd(&stream);
			return ERROR_CANNOT_MAKE;
		}

		Bytef outBuf[4096];
		do
		{
			stream.next_out = outBuf;
			stream.avail_out = sizeof(outBuf);

			ret = inflate(&stream, Z_SYNC_FLUSH);
			if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR)
			{
				Symbols::Com_PrintError(1, "inflate failed for '%s' with error %d\n", assetName, ret);
				fclose(f);
				inflateEnd(&stream);
				return ERROR_INVALID_DATA;
			}

			size_t have = sizeof(outBuf) - stream.avail_out;
			fwrite(outBuf, 1, have, f);
		} while (stream.avail_out == 0);

		fclose(f);
		inflateEnd(&stream);
		return ERROR_SUCCESS;
	}

	int RawFile::DumpRawFileAsset()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\%s", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		const char* extension = strrchr(name, '.');
		// Check if it's an animtree, if yes then decompress it
		if (extension && strcmp(extension, ".atr") == 0)
		{
			DecompressAnimtree(buffer, len, outputPath, name);

			Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
			return ERROR_SUCCESS;
		}
		else
		{
			FILE* f = fopen(outputPath, "wb");
			if (!f)
			{
				Symbols::Com_PrintError(1, "Failed to open '%s' for writing!\n", name);
				return ERROR_CANNOT_MAKE;
			}

			fwrite(buffer, 1, len, f);
			fclose(f);

			Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
			return ERROR_SUCCESS;
		}
	}

	Utils::Hook::Detour Load_RawFileAsset_Hook;
	void Load_RawFileAsset(RawFile** input)
	{
		(*input)->DumpRawFileAsset();

		// TODO: Add loading from disk

		auto Invoke = Load_RawFileAsset_Hook.Invoke<void(*)(RawFile**)>();
		Invoke(input);
	}

	typedef struct
	{
		const char* string;
		int hash;
	} StringTableCell;

	typedef struct
	{
		const char* name;
		int columnCount;
		int rowCount;
		StringTableCell* values;
		short* cellIndex;

		int DumpStringTableAsset();
	} StringTable;

	int StringTable::DumpStringTableAsset()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\%s", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* f = fopen(outputPath, "w");
		if (!f)
		{
			Symbols::Com_PrintError(1, "Failed to open '%s' for writing!\n", name);
			return ERROR_CANNOT_MAKE;
		}

		for (int i = 0; i < rowCount; i++)
		{
			for (int j = 0; j < columnCount; j++)
			{
				const char* value = (j == columnCount - 1)
					? "%s\n"
					: "%s,";
				fprintf(f, value, values[columnCount * i + j].string);
			}
		}

		fclose(f);
		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_StringTableAsset_Hook;
	void Load_StringTableAsset(StringTable** input)
	{
		(*input)->DumpStringTableAsset();

		// TODO: Add loading from disk

		auto Invoke = Load_StringTableAsset_Hook.Invoke<void(*)(StringTable**)>();
		Invoke(input);
	}

	typedef enum
	{
		LBCOL_TYPE_NUMBER		= 0,
		LBCOL_TYPE_TIME			= 1,
		LBCOL_TYPE_LEVELXP		= 2,
		LBCOL_TYPE_PRESTIGE		= 3,
		LBCOL_TYPE_BIGNUMBER	= 4,
		LBCOL_TYPE_PERCENT		= 5,
		LBCOL_TYPE_TIME_FULL	= 6,

		LBCOL_TYPE_COUNT		= 7,
	} LbColType;

	typedef enum
	{
		LBAGG_TYPE_MIN			= 0,
		LBAGG_TYPE_MAX			= 1,
		LBAGG_TYPE_ADD			= 2,
		LBAGG_TYPE_REPLACE		= 3,

		LBAGG_TYPE_COUNT		= 4,
	} LbAggType;

	typedef struct
	{
		const char* name;
		int colId;
		int dwColIndex;
		bool hidden;
		const char* statName;
		LbColType type;
		int precision;
		LbAggType agg;
		const char* localization;
		int uiCalColX;
		int uiCalColY;
	} LbColumnDef;

	typedef enum
	{
		LBUPDATE_TYPE_NORMAL	= 0,
		LBUPDATE_TYPE_RANK		= 1,
		LBUPDATE_TYPE_COMBINE	= 2,

		LBUPDATE_TYPE_COUNT		= 3,
	} LbUpdateType;

	typedef struct
	{
		const char* name;
		unsigned int id;
		int columnCount;
		int dwColumnCount;
		int xpColId;
		int prestigeColId;
		LbColumnDef* columns;
		LbUpdateType updateType;
		int trackTypes;

		int DumpLeaderboardDefAsset();
	} LeaderboardDef;

	int LeaderboardDef::DumpLeaderboardDefAsset()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\leaderboard\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\leaderboard\\%s", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* f = fopen(outputPath, "wb");
		if (!f)
		{
			Symbols::Com_PrintError(1, "Failed to open '%s' for writing!\n", name);
			return ERROR_CANNOT_MAKE;
		}

		fwrite(&id, sizeof(unsigned int), 1, f);
		fwrite(&columnCount, sizeof(int), 1, f);
		fwrite(&dwColumnCount, sizeof(int), 1, f);
		fwrite(&xpColId, sizeof(int), 1, f);
		fwrite(&prestigeColId, sizeof(int), 1, f);
		fwrite(&updateType, sizeof(LbUpdateType), 1, f);
		fwrite(&trackTypes, sizeof(int), 1, f);

		unsigned int nameLen = (name)
			? (unsigned int)strlen(name)
			: 0;
		fwrite(&nameLen, sizeof(unsigned int), 1, f);

		if (nameLen > 0)
		{
			fwrite(name, nameLen, 1, f);
		}

		fwrite(&columnCount, sizeof(int), 1, f);

		for (int i = 0; i < columnCount; ++i)
		{
			LbColumnDef* col = &columns[i];

			fwrite(&col->colId, sizeof(int), 1, f);
			fwrite(&col->dwColIndex, sizeof(int), 1, f);
			fwrite(&col->hidden, sizeof(bool), 1, f);
			fwrite(&col->type, sizeof(LbColType), 1, f);
			fwrite(&col->precision, sizeof(int), 1, f);
			fwrite(&col->agg, sizeof(LbAggType), 1, f);
			fwrite(&col->uiCalColX, sizeof(int), 1, f);
			fwrite(&col->uiCalColY, sizeof(int), 1, f);

			const char* strings[3] =
			{
				col->name,
				col->statName,
				col->localization
			};

			for (int j = 0; j < 3; ++j)
			{
				unsigned int len = (strings[j])
					? (unsigned int)strlen(strings[j])
					: 0;
				fwrite(&len, sizeof(unsigned int), 1, f);

				if (len > 0)
				{
					fwrite(strings[j], len, 1, f);
				}
			}
		}

		fclose(f);
		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_LeaderboardDefAsset_Hook;
	void Load_LeaderboardDefAsset(LeaderboardDef** input)
	{
		(*input)->DumpLeaderboardDefAsset();

		// TODO: Add loading from disk

		auto Invoke = Load_LeaderboardDefAsset_Hook.Invoke<void(*)(LeaderboardDef**)>();
		Invoke(input);
	}

	typedef enum ddlPrimitiveTypes_e
	{
		DDL_INVALID_TYPE		= -1,

		DDL_BYTE_TYPE			= 0,
		DDL_SHORT_TYPE			= 1,
		DDL_UINT_TYPE			= 2,
		DDL_INT_TYPE			= 3,
		DDL_INT64_TYPE			= 4,
		DDL_FLOAT_TYPE			= 5,
		DDL_FIXEDPOINT_TYPE		= 6,
		DDL_STRING_TYPE			= 7,
		DDL_STRUCT_TYPE			= 8,
		DDL_ENUM_TYPE			= 9
	} ddlPrimitiveTypes;
 
	typedef struct ddlHash_t
	{
		int hash;
		int index;
	} ddlHash;
 
	typedef struct ddlEnumDef_t
	{
		const char* name;
		int memberCount;
		const char** members;
		ddlHash* hashTable;
	} ddlEnumDef;
 
	typedef struct ddlMemberDef_t
	{
		const char* name;
		int size;
		int offset;
		ddlPrimitiveTypes type;
		int externalIndex;
		unsigned int rangeLimit;
		unsigned int serverDelta;
		unsigned int clientDelta;
		int arraySize;
		int enumIndex;
		int permission;
	} ddlMemberDef;
 
	typedef struct ddlStructDef_t
	{
		const char* name;
		int size;
		int memberCount;
		ddlMemberDef* members;
		ddlHash* hashTable;
	} ddlStructDef;
 
	typedef struct ddlDef_t
	{
		int version;
		int size;
		ddlStructDef* structList;
		int structCount;
		ddlEnumDef* enumList;
		int enumCount;
		ddlDef_t* next;
	} ddlDef;
 
	typedef struct
	{
		const char* name;
		ddlDef_t* ddlDef;

		int DumpDDLAsset();
	} ddlRoot_t;

	int ddlRoot_t::DumpDDLAsset()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\%s", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* f = fopen(outputPath, "wb");
		if (!f)
		{
			Symbols::Com_PrintError(1, "Failed to open '%s' for writing!\n", name);
			return ERROR_CANNOT_MAKE;
		}

		for (ddlDef_t* currentEntry = ddlDef; currentEntry; currentEntry = currentEntry->next)
		{
			fprintf(f, "//////////////////\n// VERSION %i //\n//////////////////\n\n", currentEntry->version);

			for (int i = 0; i < currentEntry->enumCount; i++)
			{
				fprintf(f, "enum %s\r\n{\r\n", currentEntry->enumList[i].name);

				for (int j = 0; j < currentEntry->enumList[i].memberCount; j++)
				{
					fprintf(f, "\t%s = %i,\r\n", currentEntry->enumList[i].members[j], j);
				}

				fprintf(f, "};\r\n\r\n");
			}

			for (int i = 0; i < currentEntry->structCount; i++)
			{
				fprintf(f, "//Bit Size: 0x%X\r\n", currentEntry->structList[i].size);
				fprintf(f, "struct %s\r\n{\r\n", currentEntry->structList[i].name);

				for (int j = 0; j < currentEntry->structList[i].memberCount; j++)
				{  
					fprintf(f, "\t//Bit Offset: 0x%X\r\n", currentEntry->structList[i].members[j].offset);
					fprintf(f, "\t//Bit Size: 0x%X\r\n", currentEntry->structList[i].members[j].size);
					fprintf(f, "\t//Permissions: 0x%X\r\n", currentEntry->structList[i].members[j].permission);
					fprintf(f, "\t//rangeLimit: %u\r\n", currentEntry->structList[i].members[j].rangeLimit);
					fprintf(f, "\t//serverDelta: %u\r\n", currentEntry->structList[i].members[j].serverDelta);
					fprintf(f, "\t//clientDelta: %u\r\n", currentEntry->structList[i].members[j].clientDelta);
					fprintf(f, "\t");

					switch (currentEntry->structList[i].members[j].type)
					{
					case DDL_BYTE_TYPE:
						fprintf(f, "byte ");
						break;
					case DDL_SHORT_TYPE:
						fprintf(f, "short ");
						break;
					case DDL_UINT_TYPE:
						fprintf(f, "unsigned ");
					case DDL_INT_TYPE:
						fprintf(f, "int ");
						break;
					case DDL_INT64_TYPE:
						fprintf(f, "__int64 ");
						break;
					case DDL_FLOAT_TYPE:
						fprintf(f, "float ");
						break;
					case DDL_FIXEDPOINT_TYPE:
						fprintf(f, "FixedPointTime ");
						break;
					case DDL_STRING_TYPE:
						fprintf(f, "char ");
						break;
					case DDL_STRUCT_TYPE:
						fprintf(f, "%s ", currentEntry->structList[currentEntry->structList[i].members[j].externalIndex].name);
						break;
					case DDL_ENUM_TYPE:
						fprintf(f, "enum ");
						break;
					}

					fprintf(f, currentEntry->structList[i].members[j].name);

					if (currentEntry->structList[i].members[j].arraySize > 1)
					{
						if (currentEntry->structList[i].members[j].enumIndex != -1 &&
							currentEntry->enumList[currentEntry->structList[i].members[j].enumIndex].memberCount == currentEntry->structList[i].members[j].arraySize)
						{
							fprintf(f, "[%s]", currentEntry->enumList[currentEntry->structList[i].members[j].enumIndex].name);
						}
						else
						{
							fprintf(f, "[%i]", currentEntry->structList[i].members[j].arraySize);
						}
					}

					fprintf(f, ";\r\n\r\n");
				}

				fprintf(f, "};\r\n\r\n");
			}
		}

		fclose(f);
		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_DDLAsset_Hook;
	void Load_DDLAsset(ddlRoot_t** input)
	{
		(*input)->DumpDDLAsset();

		// TODO: Add loading from disk

		auto Invoke = Load_DDLAsset_Hook.Invoke<void(*)(ddlRoot_t**)>();
		Invoke(input);
	}

	typedef struct
	{
		char magic[8];
		unsigned int source_crc;
		unsigned int include_offset;
		unsigned int animtree_offset;
		unsigned int cseg_offset;
		unsigned int stringtablefixup_offset;
		unsigned int exports_offset;
		unsigned int imports_offset;
		unsigned int fixup_offset;
		unsigned int profile_offset;
		unsigned int cseg_size;
		unsigned __int16 name;
		unsigned __int16 stringtablefixup_count;
		unsigned __int16 exports_count;
		unsigned __int16 imports_count;
		unsigned __int16 fixup_count;
		unsigned __int16 profile_count;
		char include_count;
		char animtree_count;
		char flags;
	} GSC_OBJ;

	typedef struct
	{
		unsigned __int16 name;
		unsigned __int16 name_space;
		unsigned __int16 num_address;
		char param_count;
		char flags;
	} GSC_IMPORT_ITEM;

	typedef struct
	{
		const char* name;
		int len;
		char* buffer;

		int DumpScriptParseTree();
	} ScriptParseTree;

	int ScriptParseTree::DumpScriptParseTree()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\%s", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* file = fopen(outputPath, "wb");
		if (!file)
		{
			return ERROR_BAD_NETPATH;
		}

		if (buffer)
		{ 
			if (((GSC_OBJ*)buffer)->fixup_offset != len || ((GSC_OBJ*)buffer)->profile_offset != len)
			{
				Symbols::Com_PrintWarning(0, "\\\\\\\\\\\\\\ An unknown section has been found in %s \\\\\\\\\\\\\\\n\\\\\\\\\\\\\\ Report this to CraigChrist8239 right away!! \\\\\\\\\\\\\\\n");
			}
			fwrite(buffer, 1, len, file);
		}

		fclose(file);

		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_ScriptParseTreeAsset_Hook;
	void Load_ScriptParseTreeAsset(ScriptParseTree** input)
	{
		ScriptParseTree* myTree = *input;
		myTree->DumpScriptParseTree();

		// TODO: Add loading from disk

		auto Invoke = Load_ScriptParseTreeAsset_Hook.Invoke<void(*)(ScriptParseTree**)>();
		Invoke(input);
	}

	struct KeyValuePair
	{
		int keyHash;
		int namespaceHash;
		const char* value;
	};

	typedef struct
	{
		const char* name;
		int numVariables;
		KeyValuePair* keyValuePairs;

		int DumpKeyValuePairsAsset();
	} KeyValuePairs;

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

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

	int KeyValuePairs::DumpKeyValuePairsAsset()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\kvp\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\kvp\\%s.kvp", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* f = fopen(outputPath, "w");
		if (!f)
		{
			Symbols::Com_PrintError(1, "Failed to open '%s' for writing!\n", name);
			return ERROR_CANNOT_MAKE;
		}

		std::string buffer;

		for (int i = 0; i < numVariables; i++)
		{
			KeyValuePair* varKeyValuePair = &keyValuePairs[i];

			std::string nameSpace = GetKeyForHash(varKeyValuePair->namespaceHash, name);
			std::string key = GetKeyForHash(varKeyValuePair->keyHash, name);

			buffer.append(Utils::String::Va("\"%s\",\"%s\",\"%s\"\n", key.data(), nameSpace.data(), varKeyValuePair->value));
		}

		fprintf(f, buffer.c_str());

		fclose(f);
		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_KeyValuePairsAsset_Hook;
	void Load_KeyValuePairsAsset(KeyValuePairs** input)
	{
		KeyValuePairs* currentKVP = *input;
		currentKVP->DumpKeyValuePairsAsset();

		// TODO: Add loading from disk

		auto Invoke = Load_KeyValuePairsAsset_Hook.Invoke<void(*)(KeyValuePairs**)>();
		Invoke(input);
	}

	typedef enum
	{
		VEH_WHEELS_4		= 0,
		VEH_MOTORCYCLE		= 1,
		VEH_TANK			= 2,
		VEH_PLANE			= 3,
		VEH_BOAT			= 4,
		VEH_ARTILLERY		= 5,
		VEH_HELICOPTER		= 6,

		NUM_VEHICLE_TYPES	= 7,
	} VehicleTypes;
 
	typedef enum
	{
		VEH_MODE_FIRSTPERSON	= 0,
		VEH_MODE_CHASECAM		= 1,
		VEH_MODE_HLO			= 2,
		VEH_MODE_STRAFE			= 3,
		VEH_MODE_HORSE			= 4,
		VEH_MODE_OLDTANK		= 5,
		VEH_MODE_HOVER			= 6,
		VEH_MODE_VTOL			= 7,

		VEH_MODE_MAX			= 8
	} EVehicleCameraMode;
 
	typedef enum
	{
		TRACTION_TYPE_FRONT	= 0,
		TRACTION_TYPE_BACK	= 1,
		TRACTION_TYPE_ALL_WD	= 2,

		NUM_TRACTION_TYPES
	} TractionType;

	typedef enum
	{
		EVENT_ACTIVATE		= 0,
		EVENT_DEACTIVATE	= 1,
		EVENT_ACCEPT		= 2,
		EVENT_UPDATE		= 3,
		EVENT_DRAW			= 4,
		EVENT_SAVE			= 5
	} DevEventType;
 
	struct view_limits_t
	{
		float horizSpanLeft;
		float horizSpanRight;
		float vertSpanUp;
		float vertSpanDown;
		float horizResistLeft;
		float horizResistRight;
		float vertResistUp;
		float vertResistDown;
	};
 
	struct DevGraph
	{
		float* knots[2];
		int* knotCount;
		int knotCountMax;
		int selectedKnot;
		void (__cdecl* eventCallback)(DevGraph*, DevEventType, Structs::LocalClientNum_t);
		void (__cdecl* textCallback)(DevGraph*, const float, const float, char*, const int);
		void* data;
		bool disableEditingEndPoints;
	};
 
	struct GraphFloat
	{
		char name[64];
		float knots[32][2];
		int knotCount;
		float scale;
		DevGraph devguiGraph;
	};
 
	struct VehicleParameter
	{
		float m_speed_max;
		float m_accel_max;
		float m_reverse_scale;
		float m_steer_angle_max;
		float m_steer_angle_speed_scale;
		float m_steer_speed;
		float m_wheel_radius;
		float m_susp_spring_k;
		float m_susp_damp_k;
		float m_susp_adj;
		float m_susp_hard_limit;
		float m_susp_min_height;
		float m_tire_fric_fwd;
		float m_tire_fric_side;
		float m_tire_fric_brake;
		float m_tire_fric_hand_brake;
		float m_body_mass;
		float m_roll_stability;
		float m_pitch_stability;
		float m_pitch_roll_resistance;
		float m_yaw_resistance;
		float m_upright_strength;
		float m_tilt_fakey;
		float m_peel_out_max_speed;
		float m_tire_damp_coast;
		float m_tire_damp_brake;
		float m_tire_damp_hand;
		float m_auto_hand_brake_min_speed;
		TractionType m_traction_type;
		const char* m_name;
		float m_bbox_min[3];
		float m_bbox_max[3];
		float m_mass_center_offset[3];
		float m_buoyancybox_min[3];
		float m_buoyancybox_max[3];
		float m_water_speed_max;
		float m_water_accel_max;
		float m_water_turn_accel;
		float m_water_turn_speed_max;
		float m_water_ebrake_power;
		float m_boat_motor_offset[3];
		float m_boat_speed_rise;
		float m_boat_speed_tilt;
		float m_boat_side_fric_scale;
		float m_boat_forward_fric_scale;
		float m_boat_vertical_fric_scale;
		float m_jump_force;
		float m_tire_fric_side_max;
		bool m_drive_on_walls;
		float m_linear_drag_scale;
		float m_angular_drag_scale;
	};
 
	struct VehicleDriveBySound
	{
		int apex;
		const char* name;
		unsigned int alias;
	};
 
	struct VehicleEngineSound
	{
		const char* name;
		unsigned int alias;
		float params[5];
	};
 
	struct VehicleGearData
	{
		float minRPM;
		float maxRPM;
		float ratio;
	};
 
	struct VehicleEngine
	{
		float idleRpms;
		float maxRpms;
		float maxTorque;
		float brakingCoeff;
		float onLoadFadeInStart;
		float onLoadFadeInEnd;
		float offLoadFadeOutStart;
		float offLoadFadeOutEnd;
		float loadScale;
		float loadSmoothing;
		float throttleLag;
		float pitchScale;
		VehicleEngineSound onload[5];
		 VehicleEngineSound offload[5];
		int numGears;
		int loopLastGear;
		VehicleGearData gears[10];
	};
 
	struct VehicleAntenna
	{
		float springK;
		float damp;
		float length;
		float gravity;
	};
 
	struct VehicleDef
	{
	  const char * name;
	  VehicleTypes type;
	  int remoteControl;
	  int bulletDamage;
	  int armorPiercingDamage;
	  int grenadeDamage;
	  int projectileDamage;
	  int projectileSplashDamage;
	  int heavyExplosiveDamage;
	  EVehicleCameraMode cameraMode;
	  int autoRecenterOnAccel;
	  int thirdPersonDriver;
	  int thirdPersonUseVehicleRoll;
	  int thirdPersonCameraPitchVehicleRelative;
	  int thirdPersonCameraHeightWorldRelative;
	  float thirdPersonCameraRange;
	  float thirdPersonCameraMinPitchClamp;
	  float thirdPersonCameraMaxPitchClamp;
	  float thirdPersonCameraHeight[2];
	  float thirdPersonCameraPitch[2];
	  int cameraAlwaysAutoCenter;
	  float cameraAutoCenterLerpRate;
	  float cameraAutoCenterMaxLerpRate;
	  float thirdPersonCameraSpringDistance;
	  float thirdPersonCameraSpringTime;
	  float thirdPersonCameraHandbrakeTurnRateInc;
	  float cameraFOV;
	  float cameraRollFraction;
	  float tagPlayerOffset[3];
	  int killcamCollision;
	  float killcamDist;
	  float killcamZDist;
	  float killcamMinDist;
	  float killcamZTargetOffset;
	  float killcamFOV;
	  float killcamNearBlur;
	  float killcamNearBlurStart;
	  float killcamNearBlurEnd;
	  float killcamFarBlur;
	  float killcamFarBlurStart;
	  float killcamFarBlurEnd;
	  int isDrivable;
	  int numberOfSeats;
	  int numberOfGunners;
	  int seatSwitchOrder[11];
	  int driverControlledGunPos;
	  float enterRadiusDriver;
	  float enterRadiusGunner[4];
	  float texureScrollScale;
	  float wheelRotRate;
	  float extraWheelRotScale;
	  int wheelChildTakesSteerYaw;
	  float maxSpeed;
	  float maxSpeedVertical;
	  float accel;
	  float accelVertical;
	  float rotRate;
	  float rotAccel;
	  float maxBodyPitch;
	  float maxBodyRoll;
	  float collisionDamage;
	  float collisionSpeed;
	  float suspensionTravel;
	  float heliCollisionScalar;
	  float viewPitchOffset;
	  float viewInfluence;
	  float tiltFromAccelerationPitch;
	  float tiltFromAccelerationRoll;
	  float tiltFromDecelerationPitch;
	  float tiltFromDecelerationRoll;
	  float tiltFromVelocityPitch;
	  float tiltFromVelocityRoll;
	  float tiltSpeedPitch;
	  float tiltSpeedRoll;
	  const char * turretWeapon;
	  view_limits_t turretViewLimits;
	  float turretRotRate;
	  int turretClampPlayerView;
	  int turretLockTurretToPlayerView;
	  const char * gunnerWeapon[4];
	  unsigned __int16 gunnerWeaponIndex[4];
	  float gunnerRotRate;
	  float gunnerRestAngles[4][2];
	  view_limits_t passengerViewLimits[6];
	  const char * turretSpinSndName;
	  const char * turretStopSndName;
	  unsigned int turretSpinSndIndex;
	  unsigned int turretStopSndIndex;
	  const char * wheelRoadNoiseSnd;
	  const char * wheelSlidingSnd;
	  const char * wheelPeelingOutSnd;
	  float skidSpeedMin;
	  float skidSpeedMax;
	  const char *futzName;
	  float futzBlend;
	  int animType;
	  const char *animSet;
	  int scriptedAnimationEntry;
	  float mantleAngleFront;
	  float mantleAngleBack;
	  float mantleAngleLeft;
	  float mantleAngleRight;
	  unsigned __int16 extraWheelLeft1;
	  unsigned __int16 extraWheelRight1;
	  unsigned __int16 extraWheelLeft2;
	  unsigned __int16 extraWheelRight2;
	  unsigned __int16 driverHideTag;
	  XModel * attachmentModels[4];
	  unsigned __int16 attachmentTags[4];
	  XModel * deathAttachmentModels[4];
	  unsigned __int16 deathAttachmentTags[4];
	  float tracerOffsetForward;
	  float tracerOffsetUp;
	  XModel * model;
	  XModel * viewModel;
	  XModel * deathModel;
	  XModel * enemyModel;
	  float modelSwapDelay;
	  FxEffectDef * exhaustFx;
	  int oneExhaust;
	  FxEffectDef *treadFx[32]; //See surfaceNames_t for the names of these.
	  FxEffectDef *deathFx;
	  unsigned __int16 deathFxTag;
	  const char * deathFxSound;
	  FxEffectDef * lightFxName[4];
	  unsigned __int16 lightFxTag[4];
	  FxEffectDef * friendlyLightFx;
	  unsigned __int16 friendlyLightFxTag;
	  FxEffectDef * enemyLightFx;
	  unsigned __int16 enemyLightFxTag;
	  float radiusDamageMin;
	  float radiusDamageMax;
	  float radiusDamageRadius;
	  const char *shootShock;
	  const char *shootRumble;
	  float deathQuakeScale;
	  float deathQuakeDuration;
	  float deathQuakeRadius;
	  const char *rumbleType;
	  float rumbleScale;
	  float rumbleDuration;
	  float rumbleRadius;
	  float rumbleBaseTime;
	  float rumbleAdditionalTime;
	  int healthDefault;
	  int healthMin;
	  int healthMax;
	  int eTeam;
	  int boostAccelMultiplier;
	  float boostDuration;
	  float boostSpeedIncrease;
	  int addToCompass;
	  int addToCompassEnemy;
	  const char *compassIcon;
	  Material *compassIconMaterial;
	  const char *gasButtonName;
	  int gasButton;
	  const char *reverseBrakeButtonName;
	  int reverseBrakeButton;
	  const char *handBrakeButtonName;
	  int handBrakeButton;
	  const char *attackButtonName;
	  int attackButton;
	  const char *attackSecondaryButtonName;
	  int attackSecondaryButton;
	  const char *boostButtonName;
	  int boostButton;
	  const char *moveUpButtonName;
	  int moveUpButton;
	  const char *moveDownButtonName;
	  int moveDownButton;
	  const char *switchSeatButtonName;
	  int switchSeatButton;
	  const char *steerGraphName;
	  GraphFloat *steerGraph;
	  const char *accelGraphName;
	  GraphFloat *accelGraph;
	  int isNitrous;
	  int isFourWheelSteering;
	  int useCollmap;
	  float radius;
	  float minHeight;
	  float maxHeight;
	  float max_fric_tilt_angle;
	  float max_fric_tilt;
	  int noDirectionalDamage;
	  int fakeBodyStabilizer;
	  float vehHelicopterBoundsRadius;
	  float vehHelicopterDecelerationFwd;
	  float vehHelicopterDecelerationSide;
	  float vehHelicopterDecelerationUp;
	  float vehHelicopterTiltFromControllerAxes;
	  float vehHelicopterTiltFromFwdAndYaw;
	  float vehHelicopterTiltFromFwdAndYaw_VelAtMaxTilt;
	  float vehHelicopterTiltMomentum;
	  int vehHelicopterQuadRotor;
	  int vehHelicopterAccelTwardsView;
	  float maxRotorArmMovementAngle;
	  float maxRotorArmRotationAngle;
	  int vehHelicopterMaintainHeight;
	  int vehHelicopterMaintainMaxHeight;
	  float vehHelicopterMaintainHeightLimit;
	  float vehHelicopterMaintainHeightAccel;
	  float vehHelicopterMaintainHeightMinimum;
	  float vehHelicopterMaintainHeightMaximum;
	  float vehHelicopterMaintainCeilingMinimum;
	  int joltVehicle;
	  int joltVehicleDriver;
	  float joltMaxTime;
	  float joltTime;
	  float joltWaves;
	  float joltIntensity;
	  VehicleParameter nitrousVehParams;
	  float p_driveby_sound_radius;
	  float s_driveby_sound_radius;
	  VehicleDriveBySound p[20];
	  VehicleDriveBySound s[20];
	  int doFootSteps;
	  int isSentient;
	  VehicleEngine engine;
	  VehicleAntenna antenna[2];
	  char *csvInclude;
	  float customFloat[3];
	  int customBool[3];

	  int DumpVehicleDef();
	};

	const char* s_vehicleClassNames[] =
	{
		"4 wheel",
		"motorcycle",
		"tank",
		"plane",
		"boat",
		"artillery",
		"helicopter"
	};

	const char* s_tractionTypeNames[] =
	{
		"TRACTION_TYPE_FRONT",
		"TRACTION_TYPE_BACK",
		"TRACTION_TYPE_ALL_WD"
	};

	const char* s_vehicleCameraModes[] =
	{
		"first",
		"chase",
		"view",
		"strafe"
		"horse",
		"oldtank"
	};

	AssetEntry BlurEntry[] =
	{
		{ "",			0,	ASSET_ENTRY_FLOAT,		0,		0 },
		{ "Start",		4,	ASSET_ENTRY_FLOAT,		0,		0 },
		{ "End",		8,	ASSET_ENTRY_FLOAT,		0,		0 },
		{ 0,			0, (AssetEntryType)0,		0,		0 }
	};

	AssetEntry Vec2OffsetEntry[] =
	{
		{ "X",		0,		ASSET_ENTRY_FLOAT,		0,		0 },
		{ "Y",		4,		ASSET_ENTRY_FLOAT,		0,		0 },
		{ 0,		0,		(AssetEntryType)0,		0,		0 }
	};

	AssetEntry VehScriptStringArray[] =
	{
		{ "1",		0,			ASSET_ENTRY_SCRIPT_STRING,		0,		0 },
		{ "2",		4,			ASSET_ENTRY_SCRIPT_STRING,		0,		0 },
		{ "3",		8,			ASSET_ENTRY_SCRIPT_STRING,		0,		0 },
		{ "4",		12,			ASSET_ENTRY_SCRIPT_STRING,		0,		0 },
		{ 0,		0,			(AssetEntryType)0,				0,		0 }
	};

	AssetEntry SeatSwitchOrderEntry[] =
	{
		{ "1",		0,			ASSET_ENTRY_INT,		0,		0 },
		{ "2",		4,			ASSET_ENTRY_INT,		0,		0 },
		{ "3",		8,			ASSET_ENTRY_INT,		0,		0 },
		{ "4",		0xC,		ASSET_ENTRY_INT,		0,		0 },
		{ "5",		0x10,		ASSET_ENTRY_INT,		0,		0 },
		{ "6",		0x14,		ASSET_ENTRY_INT,		0,		0 },
		{ "7",		0x18,		ASSET_ENTRY_INT,		0,		0 },
		{ "8",		0x1C,		ASSET_ENTRY_INT,		0,		0 },
		{ "9",		0x20,		ASSET_ENTRY_INT,		0,		0 },
		{ "10",		0x24,		ASSET_ENTRY_INT,		0,		0 },
		{ "11",		0x28,		ASSET_ENTRY_INT,		0,		0 },
		{ 0,		0,			(AssetEntryType)0,		0,		0 }
	};

	AssetEntry EnterRadiusGunnerEntry[] =
	{
		{ "1",		0,			ASSET_ENTRY_FLOAT,		0,		0 },
		{ "2",		4,			ASSET_ENTRY_FLOAT,		0,		0 },
		{ "3",		8,			ASSET_ENTRY_FLOAT,		0,		0 },
		{ "4",		0xC,		ASSET_ENTRY_FLOAT,		0,		0 },
		{ 0,		0,			(AssetEntryType)0,		0,		0 }
	};

	AssetEntry GunnerWeaponEntry[] =
	{
		{ "1",		0,			ASSET_ENTRY_STRING,		0,		0 },
		{ "2",		4,			ASSET_ENTRY_STRING,		0,		0 },
		{ "3",		8,			ASSET_ENTRY_STRING,		0,		0 },
		{ "4",		0xC,		ASSET_ENTRY_STRING,		0,		0 },
		{ 0,		0,			(AssetEntryType)0,		0,		0 }
	};

	AssetEntry VehModelArrayEntry[] =
	{
		{ "1",		0,			ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_XMODEL,		0 },
		{ "2",		4,			ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_XMODEL,		0 },
		{ "3",		8,			ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_XMODEL,		0 },
		{ "4",		0xC,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_XMODEL,		0 },
		{ 0,		0,			(AssetEntryType)0,			0,									0 }
	};

	AssetEntry SpanEntry[] =
	{
		{ "HorizSpanLeft",			0,			ASSET_ENTRY_FLOAT,		0,		0 },
		{ "HorizSpanRight",			4,			ASSET_ENTRY_FLOAT,		0,		0 },
		{ "VertSpanUp",				8,			ASSET_ENTRY_FLOAT,		0,		0 },
		{ "VertSpanDown",			0xC,		ASSET_ENTRY_FLOAT,		0,		0 },
		{ "HorizResistLeft",		0x10,		ASSET_ENTRY_FLOAT,		0,		0 },
		{ "HorizResistRight",		0x14,		ASSET_ENTRY_FLOAT,		0,		0 },
		{ "VertResistUp",			0x18,		ASSET_ENTRY_FLOAT,		0,		0 },
		{ "VertResistDown",			0x1C,		ASSET_ENTRY_FLOAT,		0,		0 },
		{ 0,						0,			(AssetEntryType)0,		0,		0 }
	};

	AssetEntry SurfaceFxEntries[] =
	{
		{ "Default",			0,			ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Bark",				4,			ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Brick",				8,			ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Carpet",				0xC,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Cloth",				0x10,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Concrete",			0x14,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Dirt",				0x18,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Flesh",				0x1C,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Foliage",			0x20,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Glass",				0x24,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Grass",				0x28,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Gravel",				0x2C,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Ice",				0x30,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Metal",				0x34,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Mud",				0x38,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Paper",				0x3C,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Plaster",			0x40,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Rock",				0x44,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Sand",				0x48,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Snow",				0x4C,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Water",				0x50,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Wood",				0x54,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Asphalt",			0x58,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Ceramic",			0x5C,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Plastic",			0x60,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Rubber",				0x64,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Cushion",			0x68,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Fruit",				0x6C,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "PaintedMetal",		0x70,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Player",				0x74,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Tallgrass",			0x78,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "Riotshield",			0x7C,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ "OpaqueGlass",		0x80,		ASSET_ENTRY_ASSET_PTR,		(AssetEntry*)ASSET_TYPE_FX,		0 },
		{ 0,					0,			(AssetEntryType)0,			0,								0 }
	};

	AssetEntry VehFXArrayEntry[] = {
		{"1",	0,	ASSET_ENTRY_ASSET_PTR,	(AssetEntry*)ASSET_TYPE_FX,0},
		{"2",	4,	ASSET_ENTRY_ASSET_PTR,	(AssetEntry*)ASSET_TYPE_FX,0},
		{"3",	8,	ASSET_ENTRY_ASSET_PTR,	(AssetEntry*)ASSET_TYPE_FX,0},
		{"4",	0xC,ASSET_ENTRY_ASSET_PTR,	(AssetEntry*)ASSET_TYPE_FX,0},
		{0, 0, (AssetEntryType)0,0,0}
	};

	AssetEntry VehButtonEntry[] = {
		{"Name",	0,	ASSET_ENTRY_STRING,0,0},
		{"Index",	4,	ASSET_ENTRY_INT,0,0},
		{0, 0, (AssetEntryType)0,0,0}
	};

	AssetEntry VehHeliMaintainEntries[] = {
		{"Height",			0,		ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"MaxHeight",		4,		ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"HeightLimit",		8,		ASSET_ENTRY_FLOAT,0,0},
		{"HeightAccel",		0xC,	ASSET_ENTRY_FLOAT,0,0},
		{"HeightMinimum",	0x10,	ASSET_ENTRY_FLOAT,0,0},
		{"HeightMaximum",	0x14,	ASSET_ENTRY_FLOAT,0,0},
		{"CeilingMinimum",	0x18,	ASSET_ENTRY_FLOAT,0,0},
		{0, 0, (AssetEntryType)0,0,0}
	};

	AssetEntry VehNosEntries[] = {
		{"maxSpeed",				offsetof(VehicleParameter, m_speed_max),					ASSET_ENTRY_FLOAT,0,0},
		{"accel",					offsetof(VehicleParameter, m_accel_max),					ASSET_ENTRY_FLOAT,0,0},
		{"reverse_scale",			offsetof(VehicleParameter, m_reverse_scale),				ASSET_ENTRY_FLOAT,0,0},
		{"steer_angle_max",			offsetof(VehicleParameter, m_steer_angle_max),				ASSET_ENTRY_FLOAT,0,0},
		{"steer_angle_speed_scale",	offsetof(VehicleParameter, m_steer_angle_speed_scale),		ASSET_ENTRY_FLOAT,0,0},
		{"steer_speed",				offsetof(VehicleParameter, m_steer_speed),					ASSET_ENTRY_FLOAT,0,0},
		{"wheel_radius",			offsetof(VehicleParameter, m_wheel_radius),					ASSET_ENTRY_FLOAT,0,0},
		{"susp_spring_k",			offsetof(VehicleParameter, m_susp_spring_k),				ASSET_ENTRY_FLOAT,0,0},
		{"susp_damp_k",				offsetof(VehicleParameter, m_susp_damp_k),					ASSET_ENTRY_FLOAT,0,0},
		{"susp_adj",				offsetof(VehicleParameter, m_susp_adj),						ASSET_ENTRY_FLOAT,0,0},
		{"susp_hard_limit",			offsetof(VehicleParameter, m_susp_hard_limit),				ASSET_ENTRY_FLOAT,0,0},
		{"susp_min_height",			offsetof(VehicleParameter, m_susp_min_height),				ASSET_ENTRY_FLOAT,0,0},
		{"tire_fric_fwd",			offsetof(VehicleParameter, m_tire_fric_fwd),				ASSET_ENTRY_FLOAT,0,0},
		{"tire_fric_side",			offsetof(VehicleParameter, m_tire_fric_side),				ASSET_ENTRY_FLOAT,0,0},
		{"tire_fric_brake",			offsetof(VehicleParameter, m_tire_fric_brake),				ASSET_ENTRY_FLOAT,0,0},
		{"tire_fric_hand_brake",	offsetof(VehicleParameter, m_tire_fric_hand_brake),			ASSET_ENTRY_FLOAT,0,0},
		{"body_mass",				offsetof(VehicleParameter, m_body_mass),					ASSET_ENTRY_FLOAT,0,0},
		{"roll_stability",			offsetof(VehicleParameter, m_roll_stability),				ASSET_ENTRY_FLOAT,0,0},
		{"pitch_stability",			offsetof(VehicleParameter, m_pitch_stability),				ASSET_ENTRY_FLOAT,0,0},
		{"pitch_roll_resistance",	offsetof(VehicleParameter, m_pitch_roll_resistance),		ASSET_ENTRY_FLOAT,0,0},
		{"yaw_resistance",			offsetof(VehicleParameter, m_yaw_resistance),				ASSET_ENTRY_FLOAT,0,0},
		{"upright_strength",		offsetof(VehicleParameter, m_upright_strength),				ASSET_ENTRY_FLOAT,0,0},
		{"tilt_fakey",				offsetof(VehicleParameter, m_tilt_fakey),					ASSET_ENTRY_FLOAT,0,0},
		{"peel_out_max_speed",		offsetof(VehicleParameter, m_peel_out_max_speed),			ASSET_ENTRY_FLOAT,0,0},
		{"tire_damp_coast",			offsetof(VehicleParameter, m_tire_damp_coast),				ASSET_ENTRY_FLOAT,0,0},
		{"tire_damp_brake",			offsetof(VehicleParameter, m_tire_damp_brake),				ASSET_ENTRY_FLOAT,0,0},
		{"tire_damp_hand",			offsetof(VehicleParameter, m_tire_damp_hand),				ASSET_ENTRY_FLOAT,0,0},
		{"auto_hand_brake_min_speed",offsetof(VehicleParameter, m_auto_hand_brake_min_speed),	ASSET_ENTRY_FLOAT,0,0},
		{"traction_type",			offsetof(VehicleParameter, m_traction_type),				ASSET_ENTRY_ENUM,		(AssetEntry*)s_tractionTypeNames,	NUM_TRACTION_TYPES},
		{"name",					offsetof(VehicleParameter, m_name),							ASSET_ENTRY_STRING,0,0},
		{"boundingbox_min_x",		offsetof(VehicleParameter, m_bbox_min),						ASSET_ENTRY_FLOAT,0,0},
		{"boundingbox_min_y",		offsetof(VehicleParameter, m_bbox_min) + 4,					ASSET_ENTRY_FLOAT,0,0},
		{"boundingbox_min_z",		offsetof(VehicleParameter, m_bbox_min) + 8,					ASSET_ENTRY_FLOAT,0,0},
		{"boundingbox_max_x",		offsetof(VehicleParameter, m_bbox_max),						ASSET_ENTRY_FLOAT,0,0},
		{"boundingbox_max_y",		offsetof(VehicleParameter, m_bbox_max) + 4,					ASSET_ENTRY_FLOAT,0,0},
		{"boundingbox_max_z",		offsetof(VehicleParameter, m_bbox_max) + 8,					ASSET_ENTRY_FLOAT,0,0},
		{"mass_center_x",			offsetof(VehicleParameter, m_mass_center_offset),			ASSET_ENTRY_FLOAT,0,0},
		{"mass_center_y",			offsetof(VehicleParameter, m_mass_center_offset) + 4,		ASSET_ENTRY_FLOAT,0,0},
		{"mass_center_z",			offsetof(VehicleParameter, m_mass_center_offset) + 8,		ASSET_ENTRY_FLOAT,0,0},
		{"buoyancybox_min_x",		offsetof(VehicleParameter, m_buoyancybox_min),				ASSET_ENTRY_FLOAT,0,0},
		{"buoyancybox_min_y",		offsetof(VehicleParameter, m_buoyancybox_min) + 4,			ASSET_ENTRY_FLOAT,0,0},
		{"buoyancybox_min_z",		offsetof(VehicleParameter, m_buoyancybox_min) + 8,			ASSET_ENTRY_FLOAT,0,0},
		{"buoyancybox_max_x",		offsetof(VehicleParameter, m_buoyancybox_max),				ASSET_ENTRY_FLOAT,0,0},
		{"buoyancybox_max_y",		offsetof(VehicleParameter, m_buoyancybox_max) + 4,			ASSET_ENTRY_FLOAT,0,0},
		{"buoyancybox_max_z",		offsetof(VehicleParameter, m_buoyancybox_max) + 8,			ASSET_ENTRY_FLOAT,0,0},
		{"water_speed_max",			offsetof(VehicleParameter, m_water_speed_max),				ASSET_ENTRY_FLOAT,0,0},
		{"water_accel_max",			offsetof(VehicleParameter, m_water_accel_max),				ASSET_ENTRY_FLOAT,0,0},
		{"water_turn_accel",		offsetof(VehicleParameter, m_water_turn_accel),				ASSET_ENTRY_FLOAT,0,0},
		{"water_turn_speed_max",	offsetof(VehicleParameter, m_water_turn_speed_max),			ASSET_ENTRY_FLOAT,0,0},
		{"water_ebrake_power",		offsetof(VehicleParameter, m_water_ebrake_power),			ASSET_ENTRY_FLOAT,0,0},
		{"boat_motor_x",			offsetof(VehicleParameter, m_boat_motor_offset),			ASSET_ENTRY_FLOAT,0,0},
		{"boat_motor_y",			offsetof(VehicleParameter, m_boat_motor_offset) + 4,		ASSET_ENTRY_FLOAT,0,0},
		{"boat_motor_z",			offsetof(VehicleParameter, m_boat_motor_offset) + 8,		ASSET_ENTRY_FLOAT,0,0},
		{"boat_speed_rise",			offsetof(VehicleParameter, m_boat_speed_rise),				ASSET_ENTRY_FLOAT,0,0},
		{"boat_speed_tilt",			offsetof(VehicleParameter, m_boat_speed_tilt),				ASSET_ENTRY_FLOAT,0,0},
		{"boat_side_fric_scale",	offsetof(VehicleParameter, m_boat_side_fric_scale),			ASSET_ENTRY_FLOAT,0,0},
		{"boat_forward_fric_scale",	offsetof(VehicleParameter, m_boat_forward_fric_scale),		ASSET_ENTRY_FLOAT,0,0},
		{"boat_vertical_fric_scale",offsetof(VehicleParameter, m_boat_forward_fric_scale),		ASSET_ENTRY_FLOAT,0,0},
		{"jump_force",				offsetof(VehicleParameter, m_jump_force),					ASSET_ENTRY_FLOAT,0,0},
		{"tire_fric_side_max",		offsetof(VehicleParameter, m_tire_fric_side_max),			ASSET_ENTRY_FLOAT,0,0},
		{"drive_on_walls",			offsetof(VehicleParameter, m_drive_on_walls),				ASSET_ENTRY_BOOL,0,0},
		{"linear_drag_scale",		offsetof(VehicleParameter, m_linear_drag_scale),			ASSET_ENTRY_FLOAT,0,0},
		{"angular_drag_scale",		offsetof(VehicleParameter, m_angular_drag_scale),			ASSET_ENTRY_FLOAT,0,0},
		{0, 0, (AssetEntryType)0,0,0}
	};

	AssetEntry PVehDrivebySound[] = {
		{"p_drive_by_sound_apex",	offsetof(VehicleDriveBySound, apex),	ASSET_ENTRY_INT,0,0},
		{"p_drive_by_sound_name",	offsetof(VehicleDriveBySound, name),	ASSET_ENTRY_STRING,0,0},
		{"p_drive_by_sound_alias",	offsetof(VehicleDriveBySound, alias),	ASSET_ENTRY_UNSIGNED_INT,0,0},
		{0, 0, (AssetEntryType)0,0,0}
	};

	AssetEntry SVehDrivebySound[] = {
		{"s_drive_by_sound_apex",	offsetof(VehicleDriveBySound, apex),	ASSET_ENTRY_INT,0,0},
		{"s_drive_by_sound_name",	offsetof(VehicleDriveBySound, name),	ASSET_ENTRY_STRING,0,0},
		{"s_drive_by_sound_alias",	offsetof(VehicleDriveBySound, alias),	ASSET_ENTRY_UNSIGNED_INT,0,0},
		{0, 0, (AssetEntryType)0,0,0}
	};

	AssetEntry VehEngineSoundOnload[] = {
		{"engine_onload_sound_name",	offsetof(VehicleEngineSound, name),			ASSET_ENTRY_STRING,0,0},
		{"engine_onload_sound_alias",	offsetof(VehicleEngineSound, alias),		ASSET_ENTRY_UNSIGNED_INT,0,0},
		{"engine_onload_sound_param1_",	offsetof(VehicleEngineSound, params[0]),	ASSET_ENTRY_FLOAT,0,0},
		{"engine_onload_sound_param2_",	offsetof(VehicleEngineSound, params[1]),	ASSET_ENTRY_FLOAT,0,0},
		{"engine_onload_sound_param3_",	offsetof(VehicleEngineSound, params[2]),	ASSET_ENTRY_FLOAT,0,0},
		{"engine_onload_sound_param4_",	offsetof(VehicleEngineSound, params[3]),	ASSET_ENTRY_FLOAT,0,0},
		{"engine_onload_sound_param5_",	offsetof(VehicleEngineSound, params[4]),	ASSET_ENTRY_FLOAT,0,0},
		{0, 0, (AssetEntryType)0,0,0}
	};

	AssetEntry VehEngineSoundOffload[] = {
		{"engine_offload_sound_name",		offsetof(VehicleEngineSound, name),			ASSET_ENTRY_STRING,0,0},
		{"engine_offload_sound_alias",		offsetof(VehicleEngineSound, alias),		ASSET_ENTRY_UNSIGNED_INT,0,0},
		{"engine_offload_sound_param1_",	offsetof(VehicleEngineSound, params[0]),	ASSET_ENTRY_FLOAT,0,0},
		{"engine_offload_sound_param2_",	offsetof(VehicleEngineSound, params[1]),	ASSET_ENTRY_FLOAT,0,0},
		{"engine_offload_sound_param3_",	offsetof(VehicleEngineSound, params[2]),	ASSET_ENTRY_FLOAT,0,0},
		{"engine_offload_sound_param4_",	offsetof(VehicleEngineSound, params[3]),	ASSET_ENTRY_FLOAT,0,0},
		{"engine_offload_sound_param5_",	offsetof(VehicleEngineSound, params[4]),	ASSET_ENTRY_FLOAT,0,0},
		{0, 0, (AssetEntryType)0,0,0}
	};

	AssetEntry VehEngineGearEntries[] = {
		{"min_rpm",		offsetof(VehicleGearData, minRPM),	ASSET_ENTRY_FLOAT,0,0},
		{"max_rpm",		offsetof(VehicleGearData, maxRPM),	ASSET_ENTRY_FLOAT,0,0},
		{"ratio",		offsetof(VehicleGearData, ratio),	ASSET_ENTRY_FLOAT,0,0},
		{0, 0, (AssetEntryType)0,0,0}
	};

	AssetEntry VehEngineEntries[] = {
		{"idle_rpms",						offsetof(VehicleEngine, idleRpms),						ASSET_ENTRY_FLOAT,0,0},
		{"max_rpms",						offsetof(VehicleEngine, maxRpms),						ASSET_ENTRY_FLOAT,0,0},
		{"max_torque",						offsetof(VehicleEngine, maxTorque),						ASSET_ENTRY_FLOAT,0,0},
		{"braking_coeff",					offsetof(VehicleEngine, brakingCoeff),					ASSET_ENTRY_FLOAT,0,0},
		{"engine_onload_fadein_start",		offsetof(VehicleEngine, onLoadFadeInStart),				ASSET_ENTRY_FLOAT,0,0},
		{"engine_onload_fadein_end",		offsetof(VehicleEngine, onLoadFadeInEnd),				ASSET_ENTRY_FLOAT,0,0},
		{"engine_offload_fadeout_start",	offsetof(VehicleEngine, offLoadFadeOutStart),			ASSET_ENTRY_FLOAT,0,0},
		{"engine_offload_fadeout_end",		offsetof(VehicleEngine, offLoadFadeOutEnd),				ASSET_ENTRY_FLOAT,0,0},
		{"engine_load_scale",				offsetof(VehicleEngine, loadScale),						ASSET_ENTRY_FLOAT,0,0},
		{"engine_load_smoothing",			offsetof(VehicleEngine, loadSmoothing),					ASSET_ENTRY_FLOAT,0,0},
		{"engine_throttle_lag",				offsetof(VehicleEngine, throttleLag),					ASSET_ENTRY_FLOAT,0,0},
		{"engine_pitch_scale",				offsetof(VehicleEngine, pitchScale),					ASSET_ENTRY_FLOAT,0,0},
		{"1",								offsetof(VehicleEngine, onload[0]),						ASSET_ENTRY_EMBEDDED_REVERSED,			VehEngineSoundOnload,0},
		{"2",								offsetof(VehicleEngine, onload[1]),						ASSET_ENTRY_EMBEDDED_REVERSED,			VehEngineSoundOnload,0},
		{"3",								offsetof(VehicleEngine, onload[2]),						ASSET_ENTRY_EMBEDDED_REVERSED,			VehEngineSoundOnload,0},
		{"4",								offsetof(VehicleEngine, onload[3]),						ASSET_ENTRY_EMBEDDED_REVERSED,			VehEngineSoundOnload,0},
		{"5",								offsetof(VehicleEngine, onload[4]),						ASSET_ENTRY_EMBEDDED_REVERSED,			VehEngineSoundOnload,0},
		{"1",								offsetof(VehicleEngine, offload[0]),					ASSET_ENTRY_EMBEDDED_REVERSED,			VehEngineSoundOffload,0},
		{"2",								offsetof(VehicleEngine, offload[1]),					ASSET_ENTRY_EMBEDDED_REVERSED,			VehEngineSoundOffload,0},
		{"3",								offsetof(VehicleEngine, offload[2]),					ASSET_ENTRY_EMBEDDED_REVERSED,			VehEngineSoundOffload,0},
		{"4",								offsetof(VehicleEngine, offload[3]),					ASSET_ENTRY_EMBEDDED_REVERSED,			VehEngineSoundOffload,0},
		{"5",								offsetof(VehicleEngine, offload[4]),					ASSET_ENTRY_EMBEDDED_REVERSED,			VehEngineSoundOffload,0},
		{"num_gears",						offsetof(VehicleEngine, numGears),						ASSET_ENTRY_INT,0,0},
		{"loopLastGear",					offsetof(VehicleEngine, loopLastGear),					ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"gear1_",							offsetof(VehicleEngine, gears[0]),						ASSET_ENTRY_EMBEDDED,					VehEngineGearEntries,0},
		{"gear2_",							offsetof(VehicleEngine, gears[1]),						ASSET_ENTRY_EMBEDDED,					VehEngineGearEntries,0},
		{"gear3_",							offsetof(VehicleEngine, gears[2]),						ASSET_ENTRY_EMBEDDED,					VehEngineGearEntries,0},
		{"gear4_",							offsetof(VehicleEngine, gears[3]),						ASSET_ENTRY_EMBEDDED,					VehEngineGearEntries,0},
		{"gear5_",							offsetof(VehicleEngine, gears[4]),						ASSET_ENTRY_EMBEDDED,					VehEngineGearEntries,0},
		{"gear6_",							offsetof(VehicleEngine, gears[5]),						ASSET_ENTRY_EMBEDDED,					VehEngineGearEntries,0},
		{"gear7_",							offsetof(VehicleEngine, gears[6]),						ASSET_ENTRY_EMBEDDED,					VehEngineGearEntries,0},
		{"gear8_",							offsetof(VehicleEngine, gears[7]),						ASSET_ENTRY_EMBEDDED,					VehEngineGearEntries,0},
		{"gear9_",							offsetof(VehicleEngine, gears[8]),						ASSET_ENTRY_EMBEDDED,					VehEngineGearEntries,0},
		{"gear10_",							offsetof(VehicleEngine, gears[9]),						ASSET_ENTRY_EMBEDDED,					VehEngineGearEntries,0},
		{0, 0, (AssetEntryType)0,0,0}
	};

	AssetEntry VehAntennaEntries[] = {
		{"spring_k",	offsetof(VehicleAntenna, springK),	ASSET_ENTRY_FLOAT,0,0},
		{"damp",		offsetof(VehicleAntenna, damp),		ASSET_ENTRY_FLOAT,0,0},
		{"length",		offsetof(VehicleAntenna, length),	ASSET_ENTRY_FLOAT,0,0},
		{"gravity",		offsetof(VehicleAntenna, gravity),	ASSET_ENTRY_FLOAT,0,0},
		{0, 0, (AssetEntryType)0,0,0}
	};

	AssetEntry VehicleDefEntries[] = {
		{"type",									offsetof(VehicleDef, type),									ASSET_ENTRY_ENUM_SHORT,			(AssetEntry*)s_vehicleClassNames,	NUM_VEHICLE_TYPES},
		{"remoteControl",							offsetof(VehicleDef, remoteControl),						ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"bulletDamage",							offsetof(VehicleDef, bulletDamage),							ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"armorPiercingDamage",						offsetof(VehicleDef, armorPiercingDamage),					ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"grenadeDamage",							offsetof(VehicleDef, grenadeDamage),						ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"projectileDamage",						offsetof(VehicleDef, projectileDamage),						ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"projectileSplashDamage",					offsetof(VehicleDef, projectileSplashDamage),				ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"heavyExplosiveDamage",					offsetof(VehicleDef, heavyExplosiveDamage),					ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"cameraMode",								offsetof(VehicleDef, cameraMode),							ASSET_ENTRY_ENUM_SHORT,			(AssetEntry*)s_vehicleCameraModes,	VEH_MODE_MAX},
		{"autoRecenterOnAccel",						offsetof(VehicleDef, autoRecenterOnAccel),					ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"thirdPersonDriver",						offsetof(VehicleDef, thirdPersonDriver),					ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"thirdPersonUseVehicleRoll",				offsetof(VehicleDef, thirdPersonUseVehicleRoll),			ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"thirdPersonCameraPitchVehicleRelative",	offsetof(VehicleDef, thirdPersonCameraPitchVehicleRelative),ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"thirdPersonCameraHeightWorldRelative",	offsetof(VehicleDef, thirdPersonCameraHeightWorldRelative),	ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"thirdPersonCameraRange",					offsetof(VehicleDef, thirdPersonCameraRange),				ASSET_ENTRY_FLOAT,0,0},
		{"thirdPersonCameraMinPitchClamp",			offsetof(VehicleDef, thirdPersonCameraMinPitchClamp),		ASSET_ENTRY_FLOAT,0,0},
		{"thirdPersonCameraMaxPitchClamp",			offsetof(VehicleDef, thirdPersonCameraMaxPitchClamp),		ASSET_ENTRY_FLOAT,0,0},
		{"thirdPersonCameraHeight",					offsetof(VehicleDef, thirdPersonCameraHeight),				ASSET_ENTRY_EMBEDDED,			MinMax_FloatEntry,0},
		{"thirdPersonCameraPitch",					offsetof(VehicleDef, thirdPersonCameraPitch),				ASSET_ENTRY_EMBEDDED,			MinMax_FloatEntry,0},
		{"cameraAlwaysAutoCenter",					offsetof(VehicleDef, cameraAlwaysAutoCenter),				ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"cameraAutoCenterLerpRate",				offsetof(VehicleDef, cameraAutoCenterLerpRate),				ASSET_ENTRY_FLOAT,0,0},
		{"cameraAutoCenterMaxLerpRate",				offsetof(VehicleDef, cameraAutoCenterMaxLerpRate),			ASSET_ENTRY_FLOAT,0,0},
		{"thirdPersonCameraSpringDistance",			offsetof(VehicleDef, thirdPersonCameraSpringDistance),		ASSET_ENTRY_FLOAT,0,0},
		{"thirdPersonCameraSpringTime",				offsetof(VehicleDef, thirdPersonCameraSpringTime),			ASSET_ENTRY_FLOAT,0,0},
		{"thirdPersonCameraHandbrakeTurnRateInc",	offsetof(VehicleDef, thirdPersonCameraHandbrakeTurnRateInc),ASSET_ENTRY_FLOAT,0,0},
		{"cameraFOV",								offsetof(VehicleDef, cameraFOV),							ASSET_ENTRY_FLOAT,0,0},
		{"cameraRollFraction",						offsetof(VehicleDef, cameraRollFraction),					ASSET_ENTRY_FLOAT,0,0},
		{"tagPlayerOffset",							offsetof(VehicleDef, tagPlayerOffset),						ASSET_ENTRY_EMBEDDED,			OffsetEntry,0},
		{"killcamCollision",						offsetof(VehicleDef, killcamCollision),						ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"killcamDist",								offsetof(VehicleDef, killcamDist),							ASSET_ENTRY_FLOAT,0,0},
		{"killcamZDist",							offsetof(VehicleDef, killcamZDist),							ASSET_ENTRY_FLOAT,0,0},
		{"killcamMinDist",							offsetof(VehicleDef, killcamMinDist),						ASSET_ENTRY_FLOAT,0,0},
		{"killcamZTargetOffset",					offsetof(VehicleDef, killcamZTargetOffset),					ASSET_ENTRY_FLOAT,0,0},
		{"killcamFOV",								offsetof(VehicleDef, killcamFOV),							ASSET_ENTRY_FLOAT,0,0},
		{"killcamNearBlur",							offsetof(VehicleDef, killcamNearBlur),						ASSET_ENTRY_EMBEDDED,			BlurEntry,0},
		{"killcamFarBlur",							offsetof(VehicleDef, killcamFarBlur),						ASSET_ENTRY_EMBEDDED,			BlurEntry,0},
		{"isDrivable",								offsetof(VehicleDef, isDrivable),							ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"numberOfSeats",							offsetof(VehicleDef, numberOfSeats),						ASSET_ENTRY_INT,0,0},
		{"numberOfGunners",							offsetof(VehicleDef, numberOfGunners),						ASSET_ENTRY_INT,0,0},
		{"seatSwitchOrder",							offsetof(VehicleDef, seatSwitchOrder),						ASSET_ENTRY_EMBEDDED,			SeatSwitchOrderEntry,0},
		{"driverControlledGunPos",					offsetof(VehicleDef, driverControlledGunPos),				ASSET_ENTRY_INT,0,0},
		{"enterRadiusDriver",						offsetof(VehicleDef, enterRadiusDriver),					ASSET_ENTRY_FLOAT,0,0},
		{"enterRadiusGunner",						offsetof(VehicleDef, enterRadiusGunner),					ASSET_ENTRY_EMBEDDED,			EnterRadiusGunnerEntry,0},
		{"texureScrollScale",						offsetof(VehicleDef, texureScrollScale),					ASSET_ENTRY_FLOAT,0,0},
		{"wheelRotRate",							offsetof(VehicleDef, wheelRotRate),							ASSET_ENTRY_FLOAT,0,0},
		{"extraWheelRotScale",						offsetof(VehicleDef, extraWheelRotScale),					ASSET_ENTRY_FLOAT,0,0},
		{"wheelChildTakesSteerYaw",					offsetof(VehicleDef, wheelChildTakesSteerYaw),				ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"maxSpeed",								offsetof(VehicleDef, maxSpeed),								ASSET_ENTRY_FLOAT,0,0},
		{"maxSpeedVertical",						offsetof(VehicleDef, maxSpeedVertical),						ASSET_ENTRY_FLOAT,0,0},
		{"accel",									offsetof(VehicleDef, accel),								ASSET_ENTRY_FLOAT,0,0},
		{"accelVertical",							offsetof(VehicleDef, accelVertical),						ASSET_ENTRY_FLOAT,0,0},
		{"rotRate",									offsetof(VehicleDef, rotRate),								ASSET_ENTRY_FLOAT,0,0},
		{"rotAccel",								offsetof(VehicleDef, rotAccel),								ASSET_ENTRY_FLOAT,0,0},
		{"maxBodyPitch",							offsetof(VehicleDef, maxBodyPitch),							ASSET_ENTRY_FLOAT,0,0},
		{"maxBodyRoll",								offsetof(VehicleDef, maxBodyRoll),							ASSET_ENTRY_FLOAT,0,0},
		{"collisionDamage",							offsetof(VehicleDef, collisionDamage),						ASSET_ENTRY_FLOAT,0,0},
		{"collisionSpeed",							offsetof(VehicleDef, collisionSpeed),						ASSET_ENTRY_FLOAT,0,0},
		{"suspensionTravel",						offsetof(VehicleDef, suspensionTravel),						ASSET_ENTRY_FLOAT,0,0},
		{"heliCollisionScalar",						offsetof(VehicleDef, heliCollisionScalar),					ASSET_ENTRY_FLOAT,0,0},
		{"viewPitchOffset",							offsetof(VehicleDef, viewPitchOffset),						ASSET_ENTRY_FLOAT,0,0},
		{"viewInfluence",							offsetof(VehicleDef, viewInfluence),						ASSET_ENTRY_FLOAT,0,0},
		{"tiltFromAccelerationPitch",				offsetof(VehicleDef, tiltFromAccelerationPitch),			ASSET_ENTRY_FLOAT,0,0},
		{"tiltFromAccelerationRoll",				offsetof(VehicleDef, tiltFromAccelerationRoll),				ASSET_ENTRY_FLOAT,0,0},
		{"tiltFromDecelerationPitch",				offsetof(VehicleDef, tiltFromDecelerationPitch),			ASSET_ENTRY_FLOAT,0,0},
		{"tiltFromDecelerationRoll",				offsetof(VehicleDef, tiltFromDecelerationRoll),				ASSET_ENTRY_FLOAT,0,0},
		{"tiltFromVelocityPitch",					offsetof(VehicleDef, tiltFromVelocityPitch),				ASSET_ENTRY_FLOAT,0,0},
		{"tiltFromVelocityRoll",					offsetof(VehicleDef, tiltFromVelocityRoll),					ASSET_ENTRY_FLOAT,0,0},
		{"tiltSpeedPitch",							offsetof(VehicleDef, tiltSpeedPitch),						ASSET_ENTRY_FLOAT,0,0},
		{"tiltSpeedRoll",							offsetof(VehicleDef, tiltSpeedRoll),						ASSET_ENTRY_FLOAT,0,0},
		{"turretWeapon",							offsetof(VehicleDef, turretWeapon),							ASSET_ENTRY_STRING,0,0},
		{"turret",									offsetof(VehicleDef, turretViewLimits),						ASSET_ENTRY_EMBEDDED,			SpanEntry,0},
		{"turretRotRate",							offsetof(VehicleDef, turretRotRate),						ASSET_ENTRY_FLOAT,0,0},
		{"turretClampPlayerView",					offsetof(VehicleDef, turretClampPlayerView),				ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"turretLockTurretToPlayerView",			offsetof(VehicleDef, turretLockTurretToPlayerView),			ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"gunnerWeapon",							offsetof(VehicleDef, gunnerWeapon),							ASSET_ENTRY_EMBEDDED,			GunnerWeaponEntry,0},
		{"gunnerRotRate",							offsetof(VehicleDef, gunnerRotRate),						ASSET_ENTRY_FLOAT,0,0},
		{"gunnerRest0",								offsetof(VehicleDef, gunnerRestAngles[0]),					ASSET_ENTRY_EMBEDDED,			Vec2OffsetEntry,0},
		{"gunnerRest1",								offsetof(VehicleDef, gunnerRestAngles[1]),					ASSET_ENTRY_EMBEDDED,			Vec2OffsetEntry,0},
		{"gunnerRest2",								offsetof(VehicleDef, gunnerRestAngles[2]),					ASSET_ENTRY_EMBEDDED,			Vec2OffsetEntry,0},
		{"gunnerRest3",								offsetof(VehicleDef, gunnerRestAngles[3]),					ASSET_ENTRY_EMBEDDED,			Vec2OffsetEntry,0},
		{"passenger1",								offsetof(VehicleDef, passengerViewLimits[0]),				ASSET_ENTRY_EMBEDDED,			SpanEntry,0},
		{"passenger2",								offsetof(VehicleDef, passengerViewLimits[1]),				ASSET_ENTRY_EMBEDDED,			SpanEntry,0},
		{"passenger3",								offsetof(VehicleDef, passengerViewLimits[2]),				ASSET_ENTRY_EMBEDDED,			SpanEntry,0},
		{"passenger4",								offsetof(VehicleDef, passengerViewLimits[3]),				ASSET_ENTRY_EMBEDDED,			SpanEntry,0},
		{"passenger5",								offsetof(VehicleDef, passengerViewLimits[4]),				ASSET_ENTRY_EMBEDDED,			SpanEntry,0},
		{"passenger6",								offsetof(VehicleDef, passengerViewLimits[5]),				ASSET_ENTRY_EMBEDDED,			SpanEntry,0},
		{"turretSpinSndName",						offsetof(VehicleDef, turretSpinSndName),					ASSET_ENTRY_STRING,0,0},
		{"turretStopSndName",						offsetof(VehicleDef, turretStopSndName),					ASSET_ENTRY_STRING,0,0},
		{"turretSpinSndIndex",						offsetof(VehicleDef, turretSpinSndIndex),					ASSET_ENTRY_INT,0,0},
		{"turretStopSndIndex",						offsetof(VehicleDef, turretStopSndIndex),					ASSET_ENTRY_INT,0,0},
		{"wheelRoadNoiseSnd",						offsetof(VehicleDef, wheelRoadNoiseSnd),					ASSET_ENTRY_STRING,0,0},
		{"wheelSlidingSnd",							offsetof(VehicleDef, wheelSlidingSnd),						ASSET_ENTRY_STRING,0,0},
		{"wheelPeelingOutSnd",						offsetof(VehicleDef, wheelPeelingOutSnd),					ASSET_ENTRY_STRING,0,0},
		{"skidSpeedMin",							offsetof(VehicleDef, skidSpeedMin),							ASSET_ENTRY_FLOAT,0,0},
		{"skidSpeedMax",							offsetof(VehicleDef, skidSpeedMax),							ASSET_ENTRY_FLOAT,0,0},
		{"futzName",								offsetof(VehicleDef, futzName),								ASSET_ENTRY_STRING,0,0},
		{"futzBlend",								offsetof(VehicleDef, futzBlend),							ASSET_ENTRY_FLOAT,0,0},
		{"animType",								offsetof(VehicleDef, animType),								ASSET_ENTRY_INT,0,0},
		{"animSet",									offsetof(VehicleDef, animSet),								ASSET_ENTRY_STRING,0,0},
		{"scriptedAnimationEntry",					offsetof(VehicleDef, scriptedAnimationEntry),				ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"mantleAngleFront",						offsetof(VehicleDef, mantleAngleFront),						ASSET_ENTRY_FLOAT,0,0},
		{"mantleAngleBack",							offsetof(VehicleDef, mantleAngleBack),						ASSET_ENTRY_FLOAT,0,0},
		{"mantleAngleLeft",							offsetof(VehicleDef, mantleAngleLeft),						ASSET_ENTRY_FLOAT,0,0},
		{"mantleAngleRight",						offsetof(VehicleDef, mantleAngleRight),						ASSET_ENTRY_FLOAT,0,0},
		{"extraWheelLeft1",							offsetof(VehicleDef, extraWheelLeft1),						ASSET_ENTRY_SCRIPT_STRING,0,0},
		{"extraWheelRight1",						offsetof(VehicleDef, extraWheelRight1),						ASSET_ENTRY_SCRIPT_STRING,0,0},
		{"extraWheelLeft2",							offsetof(VehicleDef, extraWheelLeft2),						ASSET_ENTRY_SCRIPT_STRING,0,0},
		{"extraWheelRight2",						offsetof(VehicleDef, extraWheelRight1),						ASSET_ENTRY_SCRIPT_STRING,0,0},
		{"driverHideTag",							offsetof(VehicleDef, driverHideTag),						ASSET_ENTRY_SCRIPT_STRING,0,0},
		{"attachmentModel",							offsetof(VehicleDef, attachmentModels),						ASSET_ENTRY_EMBEDDED,			VehModelArrayEntry,0},
		{"attachmentTag",							offsetof(VehicleDef, attachmentTags),						ASSET_ENTRY_EMBEDDED,			VehScriptStringArray,0},
		{"deathAttachmentModel",					offsetof(VehicleDef, deathAttachmentModels),				ASSET_ENTRY_EMBEDDED,			VehModelArrayEntry,0},
		{"deathAttachmentTag",						offsetof(VehicleDef, deathAttachmentTags),					ASSET_ENTRY_EMBEDDED,			VehScriptStringArray,0},
		{"tracerOffsetForward",						offsetof(VehicleDef, tracerOffsetForward),					ASSET_ENTRY_FLOAT,0,0},
		{"tracerOffsetUp",							offsetof(VehicleDef, tracerOffsetUp),						ASSET_ENTRY_FLOAT,0,0},
		{"model",									offsetof(VehicleDef, model),								ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_XMODEL,0},
		{"viewModel",								offsetof(VehicleDef, viewModel),							ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_XMODEL,0},
		{"deathModel",								offsetof(VehicleDef, deathModel),							ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_XMODEL,0},
		{"enemyModel",								offsetof(VehicleDef, enemyModel),							ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_XMODEL,0},
		{"modelSwapDelay",							offsetof(VehicleDef, modelSwapDelay),						ASSET_ENTRY_FLOAT,0,0},
		{"exhaustFx",								offsetof(VehicleDef, exhaustFx),							ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_FX,0},
		{"oneExhaust",								offsetof(VehicleDef, oneExhaust),							ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"treadFx",									offsetof(VehicleDef, treadFx),								ASSET_ENTRY_EMBEDDED,			SurfaceFxEntries,0},
		{"deathFx",									offsetof(VehicleDef, deathFx),								ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_FX,0},
		{"deathFxTag",								offsetof(VehicleDef, deathFxTag),							ASSET_ENTRY_SCRIPT_STRING,0,0},
		{"deathFxSound",							offsetof(VehicleDef, deathFxSound),							ASSET_ENTRY_STRING,0,0},
		{"lightFx",									offsetof(VehicleDef, lightFxName),							ASSET_ENTRY_EMBEDDED,			VehFXArrayEntry,0},
		{"lightFxTag",								offsetof(VehicleDef, lightFxTag[0]),						ASSET_ENTRY_EMBEDDED,			VehScriptStringArray,0},
		{"friendlyLightFx",							offsetof(VehicleDef, friendlyLightFx),						ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_FX,0},
		{"friendlyLightFxTag",						offsetof(VehicleDef, friendlyLightFxTag),					ASSET_ENTRY_SCRIPT_STRING,0,0},
		{"enemyLightFx",							offsetof(VehicleDef, enemyLightFx),							ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_FX,0},
		{"enemyLightFxTag",							offsetof(VehicleDef, enemyLightFxTag),						ASSET_ENTRY_SCRIPT_STRING,0,0},
		{"radiusDamageMin",							offsetof(VehicleDef, radiusDamageMin),						ASSET_ENTRY_FLOAT,0,0},
		{"radiusDamageMax",							offsetof(VehicleDef, radiusDamageMax),						ASSET_ENTRY_FLOAT,0,0},
		{"radiusDamageRadius",						offsetof(VehicleDef, radiusDamageRadius),					ASSET_ENTRY_FLOAT,0,0},
		{"shootShock",								offsetof(VehicleDef, shootShock),							ASSET_ENTRY_STRING,0,0},
		{"shootRumble",								offsetof(VehicleDef, shootRumble),							ASSET_ENTRY_STRING,0,0},
		{"deathQuakeScale",							offsetof(VehicleDef, deathQuakeScale),						ASSET_ENTRY_FLOAT,0,0},
		{"deathQuakeDuration",						offsetof(VehicleDef, deathQuakeDuration),					ASSET_ENTRY_FLOAT,0,0},
		{"deathQuakeRadius",						offsetof(VehicleDef, deathQuakeRadius),						ASSET_ENTRY_FLOAT,0,0},
		{"rumbleType",								offsetof(VehicleDef, rumbleType),							ASSET_ENTRY_STRING,0,0},
		{"rumbleScale",								offsetof(VehicleDef, rumbleScale),							ASSET_ENTRY_FLOAT,0,0},
		{"rumbleDuration",							offsetof(VehicleDef, rumbleDuration),						ASSET_ENTRY_FLOAT,0,0},
		{"rumbleRadius",							offsetof(VehicleDef, rumbleRadius),							ASSET_ENTRY_FLOAT,0,0},
		{"rumbleBaseTime",							offsetof(VehicleDef, rumbleBaseTime),						ASSET_ENTRY_FLOAT,0,0},
		{"rumbleAdditionalTime",					offsetof(VehicleDef, rumbleAdditionalTime),					ASSET_ENTRY_FLOAT,0,0},
		{"healthDefault",							offsetof(VehicleDef, healthDefault),						ASSET_ENTRY_INT,0,0},
		{"healthMin",								offsetof(VehicleDef, healthMin),							ASSET_ENTRY_INT,0,0},
		{"healthMax",								offsetof(VehicleDef, healthMax),							ASSET_ENTRY_INT,0,0},
		{"team",									offsetof(VehicleDef, eTeam),								ASSET_ENTRY_INT,0,0},
		{"boostAccelMultiplier",					offsetof(VehicleDef, boostAccelMultiplier),					ASSET_ENTRY_INT,0,0},
		{"boostDuration",							offsetof(VehicleDef, boostDuration),						ASSET_ENTRY_FLOAT,0,0},
		{"boostSpeedIncrease",						offsetof(VehicleDef, boostSpeedIncrease),					ASSET_ENTRY_FLOAT,0,0},
		{"addToCompass",							offsetof(VehicleDef, addToCompass),							ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"addToCompassEnemy",						offsetof(VehicleDef, addToCompassEnemy),					ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"compassIcon",								offsetof(VehicleDef, compassIcon),							ASSET_ENTRY_STRING,0,0},
		{"compassIconMaterial",						offsetof(VehicleDef, compassIconMaterial),					ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_MATERIAL,0},
		{"gasButton",								offsetof(VehicleDef, gasButtonName),						ASSET_ENTRY_EMBEDDED,			VehButtonEntry,0},
		{"reverseBrakeButton",						offsetof(VehicleDef, reverseBrakeButtonName),				ASSET_ENTRY_EMBEDDED,			VehButtonEntry,0},
		{"handBrakeButton",							offsetof(VehicleDef, handBrakeButtonName),					ASSET_ENTRY_EMBEDDED,			VehButtonEntry,0},
		{"attackButton",							offsetof(VehicleDef, attackButtonName),						ASSET_ENTRY_EMBEDDED,			VehButtonEntry,0},
		{"attackSecondaryButton",					offsetof(VehicleDef, attackSecondaryButtonName),			ASSET_ENTRY_EMBEDDED,			VehButtonEntry,0},
		{"boostButton",								offsetof(VehicleDef, boostButtonName),						ASSET_ENTRY_EMBEDDED,			VehButtonEntry,0},
		{"moveUpButton",							offsetof(VehicleDef, moveUpButtonName),						ASSET_ENTRY_EMBEDDED,			VehButtonEntry,0},
		{"moveDownButton",							offsetof(VehicleDef, moveDownButtonName),					ASSET_ENTRY_EMBEDDED,			VehButtonEntry,0},
		{"switchSeatsButton",						offsetof(VehicleDef, switchSeatButtonName),					ASSET_ENTRY_EMBEDDED,			VehButtonEntry,0},
		{"steerGraph",								offsetof(VehicleDef, steerGraph),							ASSET_ENTRY_STRING,0,0},
		{"accelGraph",								offsetof(VehicleDef, accelGraph),							ASSET_ENTRY_STRING,0,0},
		{"isNitrous",								offsetof(VehicleDef, isNitrous),							ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"isFourWheelSteering",						offsetof(VehicleDef, isFourWheelSteering),					ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"useCollmap",								offsetof(VehicleDef, useCollmap),							ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"radius",									offsetof(VehicleDef, radius),								ASSET_ENTRY_FLOAT,0,0},
		{"minHeight",								offsetof(VehicleDef, minHeight),							ASSET_ENTRY_FLOAT,0,0},
		{"maxHeight",								offsetof(VehicleDef, maxHeight),							ASSET_ENTRY_FLOAT,0,0},
		{"max_fric_tilt_angle",						offsetof(VehicleDef, max_fric_tilt_angle),					ASSET_ENTRY_FLOAT,0,0},
		{"max_fric_tilt",							offsetof(VehicleDef, max_fric_tilt),						ASSET_ENTRY_FLOAT,0,0},
		{"noDirectionalDamage",						offsetof(VehicleDef, noDirectionalDamage),					ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"fakeBodyStabilizer",						offsetof(VehicleDef, fakeBodyStabilizer),					ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"vehHelicopterBoundsRadius",				offsetof(VehicleDef, vehHelicopterBoundsRadius),			ASSET_ENTRY_FLOAT,0,0},
		{"vehHelicopterDecelerationFwd",			offsetof(VehicleDef, vehHelicopterDecelerationFwd),			ASSET_ENTRY_FLOAT,0,0},
		{"vehHelicopterDecelerationSide",			offsetof(VehicleDef, vehHelicopterDecelerationSide),		ASSET_ENTRY_FLOAT,0,0},
		{"vehHelicopterDecelerationUp",				offsetof(VehicleDef, vehHelicopterDecelerationUp),			ASSET_ENTRY_FLOAT,0,0},
		{"vehHelicopterTiltFromControllerAxes",		offsetof(VehicleDef, vehHelicopterTiltFromControllerAxes),	ASSET_ENTRY_FLOAT,0,0},
		{"vehHelicopterTiltFromFwdAndYaw",			offsetof(VehicleDef, vehHelicopterTiltFromFwdAndYaw),		ASSET_ENTRY_FLOAT,0,0},
		{"vehHelicopterTiltFromFwdAndYaw_VelAtMaxTilt",offsetof(VehicleDef, vehHelicopterTiltFromFwdAndYaw_VelAtMaxTilt),ASSET_ENTRY_FLOAT,0,0},//This entry name suxxx
		{"vehHelicopterTiltMomentum",				offsetof(VehicleDef, vehHelicopterTiltMomentum),			ASSET_ENTRY_FLOAT,0,0},
		{"vehHelicopterQuadRotor",					offsetof(VehicleDef, vehHelicopterQuadRotor),				ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"vehHelicopterAccelTwardsView",			offsetof(VehicleDef, vehHelicopterAccelTwardsView),			ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"maxRotorArmMovementAngle",				offsetof(VehicleDef, maxRotorArmMovementAngle),				ASSET_ENTRY_FLOAT,0,0},
		{"maxRotorArmRotationAngle",				offsetof(VehicleDef, maxRotorArmRotationAngle),				ASSET_ENTRY_FLOAT,0,0},
		{"vehHelicopterMaintain",					offsetof(VehicleDef, vehHelicopterMaintainHeight),			ASSET_ENTRY_EMBEDDED,			VehHeliMaintainEntries,0},
		{"joltVehicle",								offsetof(VehicleDef, joltVehicle),							ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"joltVehicleDriver",						offsetof(VehicleDef, joltVehicleDriver),					ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"joltMaxTime",								offsetof(VehicleDef, joltMaxTime),							ASSET_ENTRY_FLOAT,0,0},
		{"joltTime",								offsetof(VehicleDef, joltTime),								ASSET_ENTRY_FLOAT,0,0},
		{"joltWaves",								offsetof(VehicleDef, joltWaves),							ASSET_ENTRY_FLOAT,0,0},
		{"joltIntensity",							offsetof(VehicleDef, joltIntensity),						ASSET_ENTRY_FLOAT,0,0},
		{"nitrous_",								offsetof(VehicleDef, nitrousVehParams),						ASSET_ENTRY_EMBEDDED,			VehNosEntries,0},
		{"p_driveby_sound_radius",					offsetof(VehicleDef, p_driveby_sound_radius),				ASSET_ENTRY_FLOAT,0,0},
		{"s_driveby_sound_radius",					offsetof(VehicleDef, s_driveby_sound_radius),				ASSET_ENTRY_FLOAT,0,0},
		{"1",										offsetof(VehicleDef, p[0]),									ASSET_ENTRY_EMBEDDED_REVERSED,	PVehDrivebySound,0},
		{"2",										offsetof(VehicleDef, p[1]),									ASSET_ENTRY_EMBEDDED_REVERSED,	PVehDrivebySound,0},
		{"3",										offsetof(VehicleDef, p[2]),									ASSET_ENTRY_EMBEDDED_REVERSED,	PVehDrivebySound,0},
		{"4",										offsetof(VehicleDef, p[3]),									ASSET_ENTRY_EMBEDDED_REVERSED,	PVehDrivebySound,0},
		{"5",										offsetof(VehicleDef, p[4]),									ASSET_ENTRY_EMBEDDED_REVERSED,	PVehDrivebySound,0},
		{"6",										offsetof(VehicleDef, p[5]),									ASSET_ENTRY_EMBEDDED_REVERSED,	PVehDrivebySound,0},
		{"7",										offsetof(VehicleDef, p[6]),									ASSET_ENTRY_EMBEDDED_REVERSED,	PVehDrivebySound,0},
		{"8",										offsetof(VehicleDef, p[7]),									ASSET_ENTRY_EMBEDDED_REVERSED,	PVehDrivebySound,0},
		{"9",										offsetof(VehicleDef, p[8]),									ASSET_ENTRY_EMBEDDED_REVERSED,	PVehDrivebySound,0},
		{"10",										offsetof(VehicleDef, p[9]),									ASSET_ENTRY_EMBEDDED_REVERSED,	PVehDrivebySound,0},
		{"11",										offsetof(VehicleDef, p[10]),								ASSET_ENTRY_EMBEDDED_REVERSED,	PVehDrivebySound,0},
		{"12",										offsetof(VehicleDef, p[11]),								ASSET_ENTRY_EMBEDDED_REVERSED,	PVehDrivebySound,0},
		{"13",										offsetof(VehicleDef, p[12]),								ASSET_ENTRY_EMBEDDED_REVERSED,	PVehDrivebySound,0},
		{"14",										offsetof(VehicleDef, p[13]),								ASSET_ENTRY_EMBEDDED_REVERSED,	PVehDrivebySound,0},
		{"15",										offsetof(VehicleDef, p[14]),								ASSET_ENTRY_EMBEDDED_REVERSED,	PVehDrivebySound,0},
		{"16",										offsetof(VehicleDef, p[15]),								ASSET_ENTRY_EMBEDDED_REVERSED,	PVehDrivebySound,0},
		{"17",										offsetof(VehicleDef, p[16]),								ASSET_ENTRY_EMBEDDED_REVERSED,	PVehDrivebySound,0},
		{"18",										offsetof(VehicleDef, p[17]),								ASSET_ENTRY_EMBEDDED_REVERSED,	PVehDrivebySound,0},
		{"19",										offsetof(VehicleDef, p[18]),								ASSET_ENTRY_EMBEDDED_REVERSED,	PVehDrivebySound,0},
		{"20",										offsetof(VehicleDef, p[19]),								ASSET_ENTRY_EMBEDDED_REVERSED,	PVehDrivebySound,0},
		{"1",										offsetof(VehicleDef, s[0]),									ASSET_ENTRY_EMBEDDED_REVERSED,	SVehDrivebySound,0},
		{"2",										offsetof(VehicleDef, s[1]),									ASSET_ENTRY_EMBEDDED_REVERSED,	SVehDrivebySound,0},
		{"3",										offsetof(VehicleDef, s[2]),									ASSET_ENTRY_EMBEDDED_REVERSED,	SVehDrivebySound,0},
		{"4",										offsetof(VehicleDef, s[3]),									ASSET_ENTRY_EMBEDDED_REVERSED,	SVehDrivebySound,0},
		{"5",										offsetof(VehicleDef, s[4]),									ASSET_ENTRY_EMBEDDED_REVERSED,	SVehDrivebySound,0},
		{"6",										offsetof(VehicleDef, s[5]),									ASSET_ENTRY_EMBEDDED_REVERSED,	SVehDrivebySound,0},
		{"7",										offsetof(VehicleDef, s[6]),									ASSET_ENTRY_EMBEDDED_REVERSED,	SVehDrivebySound,0},
		{"8",										offsetof(VehicleDef, s[7]),									ASSET_ENTRY_EMBEDDED_REVERSED,	SVehDrivebySound,0},
		{"9",										offsetof(VehicleDef, s[8]),									ASSET_ENTRY_EMBEDDED_REVERSED,	SVehDrivebySound,0},
		{"10",										offsetof(VehicleDef, s[9]),									ASSET_ENTRY_EMBEDDED_REVERSED,	SVehDrivebySound,0},
		{"11",										offsetof(VehicleDef, s[10]),								ASSET_ENTRY_EMBEDDED_REVERSED,	SVehDrivebySound,0},
		{"12",										offsetof(VehicleDef, s[11]),								ASSET_ENTRY_EMBEDDED_REVERSED,	SVehDrivebySound,0},
		{"13",										offsetof(VehicleDef, s[12]),								ASSET_ENTRY_EMBEDDED_REVERSED,	SVehDrivebySound,0},
		{"14",										offsetof(VehicleDef, s[13]),								ASSET_ENTRY_EMBEDDED_REVERSED,	SVehDrivebySound,0},
		{"15",										offsetof(VehicleDef, s[14]),								ASSET_ENTRY_EMBEDDED_REVERSED,	SVehDrivebySound,0},
		{"16",										offsetof(VehicleDef, s[15]),								ASSET_ENTRY_EMBEDDED_REVERSED,	SVehDrivebySound,0},
		{"17",										offsetof(VehicleDef, s[16]),								ASSET_ENTRY_EMBEDDED_REVERSED,	SVehDrivebySound,0},
		{"18",										offsetof(VehicleDef, s[17]),								ASSET_ENTRY_EMBEDDED_REVERSED,	SVehDrivebySound,0},
		{"19",										offsetof(VehicleDef, s[18]),								ASSET_ENTRY_EMBEDDED_REVERSED,	SVehDrivebySound,0},
		{"20",										offsetof(VehicleDef, s[19]),								ASSET_ENTRY_EMBEDDED_REVERSED,	SVehDrivebySound,0},
		{"doFootSteps",								offsetof(VehicleDef, doFootSteps),							ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"isSentient",								offsetof(VehicleDef, isSentient),							ASSET_ENTRY_BOOL_AS_INT,0,0},
		{"",										offsetof(VehicleDef, engine),								ASSET_ENTRY_EMBEDDED,			VehEngineEntries,0},
		{"antenna1_",								offsetof(VehicleDef, antenna[0]),							ASSET_ENTRY_EMBEDDED,			VehAntennaEntries,0},
		{"antenna2_",								offsetof(VehicleDef, antenna[1]),							ASSET_ENTRY_EMBEDDED,			VehAntennaEntries,0},
		{"csvInclude",								offsetof(VehicleDef, csvInclude),							ASSET_ENTRY_STRING,0,0},
		{"customFloat0",							offsetof(VehicleDef, customFloat[0]),						ASSET_ENTRY_FLOAT,0,0},
		{"customFloat1",							offsetof(VehicleDef, customFloat[1]),						ASSET_ENTRY_FLOAT,0,0},
		{"customFloat2",							offsetof(VehicleDef, customFloat[2]),						ASSET_ENTRY_FLOAT,0,0},
		{"customBool0",								offsetof(VehicleDef, customBool[0]),						ASSET_ENTRY_BOOL,0,0},
		{"customBool1",								offsetof(VehicleDef, customBool[1]),						ASSET_ENTRY_BOOL,0,0},
		{"customBool2",								offsetof(VehicleDef, customBool[2]),						ASSET_ENTRY_BOOL,0,0},
		{0, 0, (AssetEntryType)0,0,0}
	};

	int VehicleDef::DumpVehicleDef()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\vehicles\\", NULL);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\vehicles\\%s", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* file = fopen(outputPath, "w");
		if (!file)
		{
			return ERROR_BAD_NETPATH;
		}

		fprintf(file, "VEHICLEFILE");

		for (int i = 0; VehicleDefEntries[i].name; i++)
		{
			DumpAssetEntry(
				file,
				VehicleDefEntries[i].type,
				VehicleDefEntries[i].name,
				(void*)((int)(this) + VehicleDefEntries[i].offset),
				VehicleDefEntries[i].next,
				VehicleDefEntries[i].enumCount);
		}

		fclose(file);
		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_VehicleDefAsset_Hook;
	void Load_VehicleDefAsset(VehicleDef** input)
	{
		VehicleDef* currentVehicle = *input;
		//currentVehicle->DumpVehicleDef();

		// TODO: Add loading from disk

		auto Invoke = Load_VehicleDefAsset_Hook.Invoke<void(*)(VehicleDef**)>();
		Invoke(input);
	}

	typedef struct
	{
		const char* name;
		bool mainMem;
		bool videoMem;
		bool streamMem;
		unsigned int size;
		unsigned int alignment;
		unsigned __int8* data;

		int DumpMemoryBlock();
	} MemoryBlock;

	int MemoryBlock::DumpMemoryBlock()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\memory\\", NULL);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\memory\\%s", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* file = fopen(outputPath, "wb");
		if (!file)
		{
			return ERROR_BAD_NETPATH;
		}

		fwrite(data, 1, size, file);
		fclose(file);

		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_MemoryBlockAsset_Hook;
	void Load_MemoryBlockAsset(MemoryBlock** input)
	{
		(*input)->DumpMemoryBlock();

		// TODO: Add loading from disk

		auto Invoke = Load_MemoryBlockAsset_Hook.Invoke<void(*)(MemoryBlock**)>();
		Invoke(input);
	}

	typedef enum tracerType_t
	{
		TRACERTYPE_LASER	= 0,
		TRACERTYPE_SMOKE	= 1,

		TRACERTYPE_NUM
	} tracerType;
 
	typedef struct
	{
		const char* name;
		tracerType_t type;
		struct Material* material;
		unsigned int drawInterval;
		float speed;
		float beamLength;
		float beamWidth;
		float screwRadius;
		float screwDist;
		float fadeTime;
		float fadeScale;
		float texRepeatRate;
		float colors[5][4];

		int DumpTracerDef();
	} TracerDef;

	const char* tracerTypeNames[] =
	{
		"Laser",
		"Smoke"
	};

	AssetEntry TracerDefEntries[] = 
	{
		{ "type",				offsetof(TracerDef,type),				ASSET_ENTRY_ENUM,				(AssetEntry*)tracerTypeNames,			TRACERTYPE_NUM },
		{ "material",			offsetof(TracerDef,material),			ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_MATERIAL,		0 },
		{ "drawInterval",		offsetof(TracerDef,drawInterval),		ASSET_ENTRY_UNSIGNED_INT,		0,										0 },
		{ "speed",				offsetof(TracerDef,speed),				ASSET_ENTRY_FLOAT,				0,										0 },
		{ "beamLength",			offsetof(TracerDef,beamLength),			ASSET_ENTRY_FLOAT,				0,										0 },
		{ "beamWidth",			offsetof(TracerDef,beamWidth),			ASSET_ENTRY_FLOAT,				0,										0 },
		{ "screwRadius",		offsetof(TracerDef,screwRadius),		ASSET_ENTRY_FLOAT,				0,										0 },
		{ "screwDist",			offsetof(TracerDef,screwDist),			ASSET_ENTRY_FLOAT,				0,										0 },
		{ "fadeTime",			offsetof(TracerDef,fadeTime),			ASSET_ENTRY_FLOAT,				0,										0 },
		{ "fadeScale",			offsetof(TracerDef,fadeScale),			ASSET_ENTRY_FLOAT,				0,										0 },
		{ "texRepeatRate",		offsetof(TracerDef,texRepeatRate),		ASSET_ENTRY_FLOAT,				0,										0 },
		{ 0,					0,										(AssetEntryType)0,				0,										0 }
	};

	int TracerDef::DumpTracerDef()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\tracer\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\tracer\\%s", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* file = fopen(outputPath, "w");
		if (!file)
		{
			return ERROR_BAD_NETPATH;
		}

		fprintf(file, "TRACER");

		for (int i = 0; TracerDefEntries[i].name; i++)
		{
			DumpAssetEntry(
				file,
				TracerDefEntries[i].type,
				TracerDefEntries[i].name,
				(void*)((int)(this) + TracerDefEntries[i].offset),
				TracerDefEntries[i].next,
				TracerDefEntries[i].enumCount);
		}

		for (int i = 0; i < 5; i++)
		{
			fprintf(
				file,
				"\\color%iR\\%g\\color%iG\\%g\\color%iB\\%g\\color%iA\\%g",
				i,
				colors[i][0],
				i,
				colors[i][1],
				i,
				colors[i][2],
				i,
				colors[i][3]);
		}

		fclose(file);
		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_TracerDefAsset_Hook;
	void Load_TracerDefAsset(TracerDef** input)
	{
		TracerDef* currentTracer = *input;
		currentTracer->DumpTracerDef();

		// TODO: Add loading from disk

		auto Invoke = Load_TracerDefAsset_Hook.Invoke<void(*)(TracerDef**)>();
		Invoke(input);
	}

	typedef struct
	{
		const char* name;
		unsigned int maxSkinnedVerts;

		int DumpSkinnedVertsDef();
	} SkinnedVertsDef;

	int SkinnedVertsDef::DumpSkinnedVertsDef()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\skinnedverts\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\skinnedverts\\%s", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* file = fopen(outputPath, "w");
		if (!file)
		{
			return ERROR_BAD_NETPATH;
		}

		fwrite(name, 1, maxSkinnedVerts, file);
		fclose(file);

		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_SkinnedVertsDefAsset_Hook;
	void Load_SkinnedVertsDefAsset(SkinnedVertsDef** input)
	{
		(*input)->DumpSkinnedVertsDef();

		// TODO: Add loading from disk

		auto Invoke = Load_SkinnedVertsDefAsset_Hook.Invoke<void(*)(SkinnedVertsDef**)>();
		Invoke(input);
	}

	typedef struct
	{
		const char* name;
		int len;
		char* buffer;

		int DumpQdb();
	} Qdb;

	int Qdb::DumpQdb()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\%s", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* file = fopen(outputPath, "wb");
		if (!file)
		{
			return ERROR_BAD_NETPATH;
		}

		fwrite(buffer, 1, len, file);
		fclose(file);

		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_QdbAsset_Hook;
	void Load_QdbAsset(Qdb** input)
	{
		(*input)->DumpQdb();

		// TODO: Add loading from disk

		auto Invoke = Load_QdbAsset_Hook.Invoke<void(*)(Qdb**)>();
		Invoke(input);
	}

	typedef struct
	{
		const char* name;
		int len;
		char* buffer;

		int DumpSlug();
	} Slug;

	int Slug::DumpSlug()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\%s", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* file = fopen(outputPath, "wb");
		if (!file)
		{
			return ERROR_BAD_NETPATH;
		}

		fwrite(buffer, 1, len, file);
		fclose(file);

		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_SlugAsset_Hook;
	void Load_SlugAsset(Slug** input)
	{
		(*input)->DumpSlug();

		// TODO: Add loading from disk

		auto Invoke = Load_SlugAsset_Hook.Invoke<void(*)(Slug**)>();
		Invoke(input);
	}

	struct FootstepTableDef
	{
		const char* name;
		unsigned int sndAliasTable[32][7];

		int DumpFootstepTableDef();
	};

	int FootstepTableDef::DumpFootstepTableDef()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\fx\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\fx\\%s.csv", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* file = fopen(outputPath, "w");
		if (!file)
		{
			return ERROR_BAD_NETPATH;
		}

		for (int i = 0; i < 32; i++)
		{
			for (int j = 0; j < 7; i++)
			{
				const char* tableName = (j == 6)
					? ((i == 31) ? "" : "\n")
					: ",";

				fprintf(
					file,
					"%u%s",
					sndAliasTable[i][j],
					tableName);
			}
		}

		fclose(file);
		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_FootstepTableDefAsset_Hook;
	void Load_FootstepTableDefAsset(FootstepTableDef** input)
	{
		FootstepTableDef* oldTable = *input;
		//oldTable->DumpFootstepTableDef();

		// TODO: Add loading from disk

		auto Invoke = Load_FootstepTableDefAsset_Hook.Invoke<void(*)(FootstepTableDef**)>();
		Invoke(input);
	}

	struct FootstepFXTableDef
	{
		const char* name;
		FxEffectDef* footstepFX[0x20];

		int DumpFootstepFXTableDef();
	};

	int FootstepFXTableDef::DumpFootstepFXTableDef()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\fx\\bio\\player\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\fx\\bio\\player\\%s.csv", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* file = fopen(outputPath, "w");
		if (!file)
		{
			return ERROR_BAD_NETPATH;
		}

		for (int i = 0; i < 32; i++)
		{
			FxEffectDef* currentFx = footstepFX[i];

			const char* surfaceFxName = (currentFx)
				? currentFx->name
				: "";
			const char* emptyOrNewLine = (i == 31)
				? ""
				: "\n";

			fprintf(
				file,
				"surfaceFx%s,%s%s",
				SurfaceFxEntries[i].name,
				surfaceFxName,
				emptyOrNewLine);
		}

		fclose(file);
		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_FootstepFXTableDefAsset_Hook;
	void Load_FootstepFXTableDefAsset(FootstepFXTableDef** input)
	{
		FootstepFXTableDef* oldTable = *input;
		oldTable->DumpFootstepFXTableDef();

		// TODO: Add loading from disk

		auto Invoke = Load_FootstepFXTableDefAsset_Hook.Invoke<void(*)(FootstepFXTableDef**)>();
		Invoke(input);
	}

	struct ZBarrierBoard
	{
		struct XModel* pBoardModel;
		struct XModel* pAlternateBoardModel;
		struct XModel* pUpgradedBoardModel;
		const char* pTearAnim;
		const char* pBoardAnim;
		struct FxEffectDef* repairEffect[2];
		float repairEffectOffset[2][3];
		unsigned int boardRepairSound;
		unsigned int boardRepairHoverSound;
		unsigned int pauseAndRepeatRepairSound;
		float pauseBetweenRepSoundsMin;
		float pauseBetweenRepSoundsMax;
		unsigned __int16 zombieBoardTearStateName;
		unsigned __int16 zombieBoardTearSubStateName;
		unsigned int numRepsToPullProBoard;
	};
 
	struct ZBarrierDef
	{
		const char* name;
		unsigned int generalRepairSound[2];
		unsigned int upgradedGeneralRepairSound[2];
		unsigned int useDelayBetweenGeneralRepSounds;
		float delayBetweenGeneralRepSounds;
		unsigned int earthquakeOnRepair;
		float earthquakeMinScale;
		float earthquakeMaxScale;
		float earthquakeMinDuration;
		float earthquakeMaxDuration;
		float earthquakeRadius;
		int numBoardsInBarrier;
		unsigned int autoHideOpenPieces;
		unsigned int taunts;
		unsigned int reachThroughAttacks;
		unsigned __int16 zombieTauntAnimState;
		unsigned __int16 zombieReachThroughAnimState;
		int numAttackSlots;
		float attackSpotHorzOffset;
		ZBarrierBoard boards[6];

		int DumpZBarrierDef();
	};

	AssetEntry ZBarrierBoardEntries[] = 
	{
		{ "pBoardModel",						offsetof(ZBarrierBoard,pBoardModel),						ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_XMODEL,		0 },
		{ "pAlternateBoardModel",				offsetof(ZBarrierBoard,pAlternateBoardModel),				ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_XMODEL,		0 },
		{ "pUpgradedBoardModel",				offsetof(ZBarrierBoard,pUpgradedBoardModel),				ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_XMODEL,		0 },
		{ "pTearAnim",							offsetof(ZBarrierBoard,pTearAnim),							ASSET_ENTRY_STRING,				0,									0 },
		{ "pBoardAnim",							offsetof(ZBarrierBoard,pBoardAnim),							ASSET_ENTRY_STRING,				0,									0 },
		{ "repairEffect1",						offsetof(ZBarrierBoard,repairEffect[0]),					ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_FX,			0 },
		{ "repairEffect2",						offsetof(ZBarrierBoard,repairEffect[1]),					ASSET_ENTRY_ASSET_PTR,			(AssetEntry*)ASSET_TYPE_FX,			0 },
		{ "repairEffectOffset1X",				offsetof(ZBarrierBoard,repairEffectOffset[0]),				ASSET_ENTRY_FLOAT,				0,									0 },
		{ "repairEffectOffset1Y",				offsetof(ZBarrierBoard,repairEffectOffset[0]) + 4,			ASSET_ENTRY_FLOAT,				0,									0 },
		{ "repairEffectOffset1Z",				offsetof(ZBarrierBoard,repairEffectOffset[0]) + 8,			ASSET_ENTRY_FLOAT,				0,									0 },
		{ "repairEffectOffset2X",				offsetof(ZBarrierBoard,repairEffectOffset[1]),				ASSET_ENTRY_FLOAT,				0,									0 },
		{ "repairEffectOffset2Y",				offsetof(ZBarrierBoard,repairEffectOffset[1]) + 4,			ASSET_ENTRY_FLOAT,				0,									0 },
		{ "repairEffectOffset2Z",				offsetof(ZBarrierBoard,repairEffectOffset[1]) + 8,			ASSET_ENTRY_FLOAT,				0,									0 },
		{ "boardRepairSound",					offsetof(ZBarrierBoard,boardRepairSound),					ASSET_ENTRY_UNSIGNED_INT,		0,									0 },
		{ "boardRepairHoverSound",				offsetof(ZBarrierBoard,boardRepairHoverSound),				ASSET_ENTRY_UNSIGNED_INT,		0,									0 },
		{ "pauseAndRepeatRepairSound",			offsetof(ZBarrierBoard,pauseAndRepeatRepairSound),			ASSET_ENTRY_UNSIGNED_INT,		0,									0 },
		{ "pauseBetweenRepSoundsMin",			offsetof(ZBarrierBoard,pauseBetweenRepSoundsMin),			ASSET_ENTRY_FLOAT,				0,									0 },
		{ "pauseBetweenRepSoundsMax",			offsetof(ZBarrierBoard,pauseBetweenRepSoundsMax),			ASSET_ENTRY_FLOAT,				0,									0 },
		{ "zombieBoardTearStateName",			offsetof(ZBarrierBoard,zombieBoardTearStateName),			ASSET_ENTRY_SCRIPT_STRING,		0,									0 },
		{ "zombieBoardTearSubStateName",		offsetof(ZBarrierBoard,zombieBoardTearSubStateName),		ASSET_ENTRY_SCRIPT_STRING,		0,									0 },
		{ "numRepsToPullProBoard",				offsetof(ZBarrierBoard,numRepsToPullProBoard),				ASSET_ENTRY_UNSIGNED_INT,		0,									0 },
		{ 0,									0,															(AssetEntryType)0,				0,									0 }
	};

	AssetEntry ZBarrierDefEntries[] = 
	{
		{ "generalRepairSound1",					offsetof(ZBarrierDef,generalRepairSound[0]),				ASSET_ENTRY_UNSIGNED_INT,			0,							0 },
		{ "generalRepairSound2",					offsetof(ZBarrierDef,generalRepairSound[1]),				ASSET_ENTRY_UNSIGNED_INT,			0,							0 },
		{ "upgradedGeneralRepairSound1",			offsetof(ZBarrierDef,upgradedGeneralRepairSound[0]),		ASSET_ENTRY_UNSIGNED_INT,			0,							0 },
		{ "upgradedGeneralRepairSound2",			offsetof(ZBarrierDef,upgradedGeneralRepairSound[1]),		ASSET_ENTRY_UNSIGNED_INT,			0,							0 },
		{ "useDelayBetweenGeneralRepSounds",		offsetof(ZBarrierDef,useDelayBetweenGeneralRepSounds),		ASSET_ENTRY_UNSIGNED_INT,			0,							0 },
		{ "delayBetweenGeneralRepSounds",			offsetof(ZBarrierDef,delayBetweenGeneralRepSounds),			ASSET_ENTRY_FLOAT,					0,							0 },
		{ "earthquakeOnRepair",						offsetof(ZBarrierDef,earthquakeOnRepair),					ASSET_ENTRY_UNSIGNED_INT,			0,							0 },
		{ "earthquakeMinScale",						offsetof(ZBarrierDef,earthquakeMinScale),					ASSET_ENTRY_FLOAT,					0,							0 },
		{ "earthquakeMaxScale",						offsetof(ZBarrierDef,earthquakeMaxScale),					ASSET_ENTRY_FLOAT,					0,							0 },
		{ "earthquakeMinDuration",					offsetof(ZBarrierDef,earthquakeMinDuration),				ASSET_ENTRY_FLOAT,					0,							0 },
		{ "earthquakeMaxDuration",					offsetof(ZBarrierDef,earthquakeMaxDuration),				ASSET_ENTRY_FLOAT,					0,							0 },
		{ "earthquakeRadius",						offsetof(ZBarrierDef,earthquakeRadius),						ASSET_ENTRY_FLOAT,					0,							0 },
		{ "numBoardsInBarrier",						offsetof(ZBarrierDef,numBoardsInBarrier),					ASSET_ENTRY_INT,					0,							0 },
		{ "autoHideOpenPieces",						offsetof(ZBarrierDef,autoHideOpenPieces),					ASSET_ENTRY_UNSIGNED_INT,			0,							0 },
		{ "taunts",									offsetof(ZBarrierDef,taunts),								ASSET_ENTRY_UNSIGNED_INT,			0,							0 },
		{ "reachThroughAttacks",					offsetof(ZBarrierDef,reachThroughAttacks),					ASSET_ENTRY_UNSIGNED_INT,			0,							0 },
		{ "zombieTauntAnimState",					offsetof(ZBarrierDef,zombieTauntAnimState),					ASSET_ENTRY_SCRIPT_STRING,			0,							0 },
		{ "zombieReachThroughAnimState",			offsetof(ZBarrierDef,zombieReachThroughAnimState),			ASSET_ENTRY_SCRIPT_STRING,			0,							0 },
		{ "numAttackSlots",							offsetof(ZBarrierDef,numAttackSlots),						ASSET_ENTRY_INT,					0,							0 },
		{ "attackSpotHorzOffset",					offsetof(ZBarrierDef,attackSpotHorzOffset),					ASSET_ENTRY_FLOAT,					0,							0 },
		{ "1",										offsetof(ZBarrierDef, boards[0]),							ASSET_ENTRY_EMBEDDED_REVERSED,		ZBarrierBoardEntries,		0 },
		{ "2",										offsetof(ZBarrierDef, boards[1]),							ASSET_ENTRY_EMBEDDED_REVERSED,		ZBarrierBoardEntries,		0 },
		{ "3",										offsetof(ZBarrierDef, boards[2]),							ASSET_ENTRY_EMBEDDED_REVERSED,		ZBarrierBoardEntries,		0 },
		{ "4",										offsetof(ZBarrierDef, boards[3]),							ASSET_ENTRY_EMBEDDED_REVERSED,		ZBarrierBoardEntries,		0 },
		{ "5",										offsetof(ZBarrierDef, boards[4]),							ASSET_ENTRY_EMBEDDED_REVERSED,		ZBarrierBoardEntries,		0 },
		{ "6",										offsetof(ZBarrierDef, boards[5]),							ASSET_ENTRY_EMBEDDED_REVERSED,		ZBarrierBoardEntries,		0 },
		{ 0,										0,															(AssetEntryType)0,					0,							0 }
	};

	int ZBarrierDef::DumpZBarrierDef()
	{
		// Create the folder
		CreateDirectory("game:\\Alpha\\dump\\zbarrier\\", 0);

		Symbols::Com_Printf(0, "Attempting to dump '%s' to disk...\n", name);

		const char* outputPath = Utils::String::Va("game:\\Alpha\\dump\\zbarrier\\%s", name);
		if (Utils::FileSystem::FileExists(outputPath))
		{
			Symbols::Com_PrintWarning(0, "'%s' has already been dumped at '%s'\n", name, outputPath);
			return ERROR_DUP_NAME;
		}

		FILE* file = fopen(outputPath, "w");
		if (!file)
		{
			return ERROR_BAD_NETPATH;
		}

		fprintf(file,"ZBARRIER");

		for (int i = 0; ZBarrierDefEntries[i].name; i++)
		{
			DumpAssetEntry(
				file,
				ZBarrierDefEntries[i].type,
				ZBarrierDefEntries[i].name,
				(void*)((int)(this) + ZBarrierDefEntries[i].offset),
				ZBarrierDefEntries[i].next,
				ZBarrierDefEntries[i].enumCount);
		}

		fclose(file);
		Symbols::Com_Printf(0, "Dumped '%s' to disk...\n", name);
		return ERROR_SUCCESS;
	}

	Utils::Hook::Detour Load_ZBarrierDefAsset_Hook;
	void Load_ZBarrierDefAsset(ZBarrierDef** input)
	{
		ZBarrierDef* currentBarrier = *input;
		currentBarrier->DumpZBarrierDef();

		// TODO: Add loading from disk

		auto Invoke = Load_ZBarrierDefAsset_Hook.Invoke<void(*)(ZBarrierDef**)>();
		Invoke(input);
	}

	void RegisterHooks()
	{
		// Load_PhysPresetAsset_Hook.Create(0x823A2C68, Load_PhysPresetAsset);
		// Load_PhysConstraintsAsset_Hook.Create(0x823A2CE0, Load_PhysConstraintsAsset);
		// Load_DestructibleDefAsset_Hook.Create(0x823A2D58, Load_DestructibleDefAsset);
		// TODO: Add XAnimParts dumper
		// Load_XModelAsset_Hook.Create(0x823A2E48, Load_XModelAsset);
		// Load_MaterialAsset_Hook.Create(0x823A2EC0, Load_MaterialAsset);
		// Load_MaterialTechniqueSetAsset_Hook.Create(0x823A2FB0, Load_MaterialTechniqueSetAsset);
		// Load_GfxImageAsset_Hook.Create(0x823A3028, Load_GfxImageAsset);
		// Load_MapEntsAsset_Hook.Create(0x823A3480, Load_MapEntsAsset);
		// Load_LightDefAsset_Hook.Create(0x823A35E8, Load_LightDefAsset);
		// Load_FontAsset_Hook.Create(0x823A3660, Load_FontAsset);
		// Load_FxEffectDefAsset_Hook.Create(0x823A3950, Load_FxEffectDefAsset);
		// Loc string dumper
		// Load_WeaponVariantDefAsset_Hook.Create(0x823A38D8, Load_WeaponVariantDefAsset);
		// Load_RawFileAsset_Hook.Create(0x823A3A40, Load_RawFileAsset);
		// Load_StringTableAsset_Hook.Create(0x823A3AB8, Load_StringTableAsset);
		// Load_LeaderboardDefAsset_Hook.Create(0x823A3B38, Load_LeaderboardDefAsset);
		// Load_DDLAsset_Hook.Create(0x823A34F8, Load_DDLAsset);
		// Load_ScriptParseTreeAsset_Hook.Create(0x823A3D90, Load_ScriptParseTreeAsset);
		// Load_KeyValuePairsAsset_Hook.Create(0x823A3EF8, Load_KeyValuePairsAsset);
		// Load_VehicleDefAsset_Hook.Create(0x823A3F80, Load_VehicleDefAsset);
		// Load_MemoryBlockAsset_Hook.Create(0x823A3FF8, Load_MemoryBlockAsset);
		// Load_TracerDefAsset_Hook.Create(0x823A40E8, Load_TracerDefAsset);
		// Load_SkinnedVertsDefAsset_Hook.Create(0x823A4160, Load_SkinnedVertsDefAsset);
		// Load_QdbAsset_Hook.Create(0x823A3E08, Load_QdbAsset);
		// Load_SlugAsset_Hook.Create(0x823A3E80, Load_SlugAsset);
		// Load_FootstepTableDefAsset_Hook.Create(0x823A41D8, Load_FootstepTableDefAsset);
		// Load_FootstepFXTableDefAsset_Hook.Create(0x823A4250, Load_FootstepFXTableDefAsset);
		// Load_ZBarrierDefAsset_Hook.Create(0x823A42C8, Load_ZBarrierDefAsset);
	}

	void UnregisterHooks()
	{
		Load_PhysPresetAsset_Hook.Remove();
		Load_PhysConstraintsAsset_Hook.Remove();
		Load_DestructibleDefAsset_Hook.Remove();
		Load_XModelAsset_Hook.Remove();
		Load_MaterialAsset_Hook.Remove();
		Load_MaterialTechniqueSetAsset_Hook.Remove();
		Load_GfxImageAsset_Hook.Remove();
		Load_MapEntsAsset_Hook.Remove();
		Load_LightDefAsset_Hook.Remove();
		Load_FontAsset_Hook.Remove();
		Load_FxEffectDefAsset_Hook.Remove();
		// Loc string dumper
		Load_WeaponVariantDefAsset_Hook.Remove();
		Load_RawFileAsset_Hook.Remove();
		Load_StringTableAsset_Hook.Remove();
		Load_LeaderboardDefAsset_Hook.Remove();
		Load_DDLAsset_Hook.Remove();
		Load_ScriptParseTreeAsset_Hook.Remove();
		Load_KeyValuePairsAsset_Hook.Remove();
		Load_VehicleDefAsset_Hook.Remove();
		Load_MemoryBlockAsset_Hook.Remove();
		Load_TracerDefAsset_Hook.Remove();
		Load_SkinnedVertsDefAsset_Hook.Remove();
		Load_QdbAsset_Hook.Remove();
		Load_SlugAsset_Hook.Remove();
		Load_FootstepTableDefAsset_Hook.Remove();
		Load_FootstepFXTableDefAsset_Hook.Remove();
		Load_ZBarrierDefAsset_Hook.Remove();
	}

	void Load()
	{
		RegisterHooks();
	}

	void Unload()
	{
		UnregisterHooks();
	}
}