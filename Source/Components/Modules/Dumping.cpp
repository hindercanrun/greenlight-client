#include "../../Std_Include.h"
#include "Dumping.h"

#include "../../Utils/Hook.h"
#include "../../Utils/String.h"
#include "../../Utils/FileSystem.h"

#include <direct.h>
#include <algorithm>

#include <cstddef>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include <functional>
#include <cstdint>
#include <cstring>
#include <direct.h>
#include <fstream>
#include <sstream>
#include <xtl.h>
#include <xbox.h>
#include <xgraphics.h>
#include <io.h>
#include <xjson.h>

namespace Dumping
{
	void Cmd_DumpMapEnts_f()
	{
		Structs::XAssetHeader files[2];
		int count = Symbols::DB_GetAllXAssetOfType_FastFile(Structs::ASSET_TYPE_MAP_ENTS, files, 2);

		for (int i = 0; i < count; i++)
		{
			Structs::MapEnts* mapEnts = files[i].mapEnts;
			std::string assetName = mapEnts->name;
			std::replace(assetName.begin(), assetName.end(), '/', '\\'); // Replace forward slashes with backslashes

			const std::string extension = ".d3dbsp";
			if (assetName.size() >= extension.size() &&
				assetName.compare(assetName.size() - extension.size(),
				extension.size(), extension) == 0)
			{
				assetName.erase(assetName.size() - extension.size(), extension.size());
			}

			// Dump map ents
			std::string path = "game:\\dump\\" + assetName + ".ents";
			Utils::FileSystem::WriteFileToDisk(path.c_str(), mapEnts->entityString, mapEnts->numEntityChars);

			// Dump triggers
			const Structs::MapTriggers& trigger = mapEnts->trigger;
			std::string trigger_path = "game:\\dump\\" + assetName + ".triggers";
			std::stringstream ss;

			ss << "Trigger Count (" << trigger.count << "):\n";
			for (unsigned int j = 0; j < trigger.count; ++j)
			{
				const auto& model = trigger.models[j];
				ss << "  [" << j << "] contents=" << model.contents
				   << ", hullCount=" << model.hullCount
				   << ", firstHull=" << model.firstHull << "\n";
			}

			ss << "\nHull Count (" << trigger.hullCount << "):\n";
			for (unsigned int j = 0; j < trigger.hullCount; ++j)
			{
				const auto& hull = trigger.hulls[j];
				ss << "  [" << j << "] contents=" << hull.contents
				   << ", slabCount=" << hull.slabCount
				   << ", firstSlab=" << hull.firstSlab << "\n";
				ss << "    Bounds mid=(" << hull.bounds.midPoint[0] << ", " << hull.bounds.midPoint[1] << ", " << hull.bounds.midPoint[2]
				   << ") half=(" << hull.bounds.halfSize[0] << ", " << hull.bounds.halfSize[1] << ", " << hull.bounds.halfSize[2] << ")\n";
			}

			ss << "\nSlab Count (" << trigger.slabCount << "):\n";
			for (unsigned int j = 0; j < trigger.slabCount; ++j)
			{
				const auto& slab = trigger.slabs[j];
				ss << "  [" << j << "] dir=(" << slab.dir[0] << ", " << slab.dir[1] << ", " << slab.dir[2]
				   << "), midPoint=" << slab.midPoint
				   << ", halfSize=" << slab.halfSize << "\n";
			}

			std::string trigger_text = ss.str();
			Utils::FileSystem::WriteFileToDisk(trigger_path.c_str(), trigger_text.c_str(), static_cast<int>(trigger_text.size()));
		}
	}

	void Cmd_DumpLocalizedStrings_f()
	{
		Structs::XAssetHeader files[20000];
		int count = Symbols::DB_GetAllXAssetOfType_FastFile(Structs::ASSET_TYPE_LOCALIZE_ENTRY, files, 20000);

		std::map<std::string, std::string> groupedStr;

		const char* header =
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
			"CONFIG              \"C:\\\\t6\\\\main\\\\game\\\\bin\\\\StringEd.cfg\"\n"
			"FILENOTES           \"\"\n\n";

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
				buffer += header;
			}

			buffer += "REFERENCE           " + key + "\n";
			buffer += "LANG_ENGLISH        \"" + value + "\"\n\n";
		}

		for (std::map<std::string, std::string>::iterator it = groupedStr.begin(); it != groupedStr.end(); ++it)
		{
			it->second += "\nENDMARKER\n\n";

			std::string outPath = "game:\\dump\\english\\localizedstrings\\" + it->first + ".str";
			Utils::FileSystem::WriteFileToDisk(outPath.c_str(), it->second.c_str(), it->second.size());
		}
	}
	
	void Cmd_DumpRawFiles_f()
	{
		Structs::XAssetHeader files[1024];
		int count = Symbols::DB_GetAllXAssetOfType_FastFile(Structs::ASSET_TYPE_RAWFILE, files, 1024);

		for (int i = 0; i < count; i++)
		{
			Structs::RawFile* rawFile = files[i].rawfile;
			std::string asset_name = rawFile->name;
			std::replace(asset_name.begin(), asset_name.end(), '/', '\\'); // Replace forward slashes with backslashes

			Utils::FileSystem::WriteFileToDisk(("game:\\dump\\" + asset_name).c_str(), rawFile->buffer, rawFile->len);
		}
	}

	void Cmd_DumpStringTables_f()
	{
		Structs::XAssetHeader files[1024];
		int count = Symbols::DB_GetAllXAssetOfType_FastFile(Structs::ASSET_TYPE_STRINGTABLE, files, 1024);
		for (int i = 0; i < count; ++i)
		{
			Structs::StringTable* table = files[i].stringTable;
			std::string name = table->name ? table->name : "unnamed";
			std::replace(name.begin(), name.end(), '/', '\\');
			std::string outPath = "game:\\dump\\" + name;

			std::string csv;

			for (int row = 0; row < table->rowCount; ++row)
			{
				for (int col = 0; col < table->columnCount; ++col)
				{
					int index = row * table->columnCount + col;
					int cellIdx = table->cellIndex[index];

					const char* value = (cellIdx >= 0 && cellIdx < table->rowCount * table->columnCount)
						? table->values[cellIdx].string
						: "";

					std::string cell = value ? value : "";

					// Wrap in quotes and double any internal quotes
					bool needsQuotes = cell.find(',') != std::string::npos || cell.find('"') != std::string::npos;
					if (needsQuotes)
					{
						size_t pos = 0;
						while ((pos = cell.find('"', pos)) != std::string::npos)
						{
							cell.insert(pos, "\"");
							pos += 2;
						}
						csv += '"';
						csv += cell;
						csv += '"';
					}
					else
					{
						csv += cell;
					}

					if (col < table->columnCount - 1)
						csv += ',';
				}
				csv += '\n';
			}

			Utils::FileSystem::WriteFileToDisk(outPath.c_str(), csv.c_str(), csv.size());
		}
	}

	void Cmd_DumpScriptParseTree_f()
	{
		Structs::XAssetHeader files[1024];
		int count = Symbols::DB_GetAllXAssetOfType_FastFile(Structs::ASSET_TYPE_SCRIPTPARSETREE, files, 1024);

		for (int i = 0; i < count; i++)
		{
			Structs::ScriptParseTree* scriptParseTree = files[i].scriptParseTree;
			std::string asset_name = scriptParseTree->name;
			std::replace(asset_name.begin(), asset_name.end(), '/', '\\'); // Replace forward slashes with backslashes

			Utils::FileSystem::WriteFileToDisk(("game:\\dump\\" + asset_name).c_str(), scriptParseTree->buffer, scriptParseTree->len);
		}
	}

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
	static Structs::cmd_function_s Cmd_DumpLocalizedStrings_f_VAR;
	static Structs::cmd_function_s Cmd_DumpRawFiles_f_VAR;
	static Structs::cmd_function_s Cmd_DumpStringTables_f_VAR;
	static Structs::cmd_function_s Cmd_DumpScriptParseTree_f_VAR;
	static Structs::cmd_function_s Cmd_DumpKeyValuePair_f_VAR;

	void RegisterHooks()
	{
		static bool alreadyRan = false;
		if (alreadyRan)
		{
			alreadyRan = true;
			return;
		}
		alreadyRan = true;

		Symbols::Cmd_AddCommand("DumpMapEnts", Cmd_DumpMapEnts_f, &Cmd_DumpMapEnts_f_VAR);
		Symbols::Cmd_AddCommand("DumpLocalizedStrings", Cmd_DumpLocalizedStrings_f, &Cmd_DumpLocalizedStrings_f_VAR);
		Symbols::Cmd_AddCommand("DumpRawFiles", Cmd_DumpRawFiles_f, &Cmd_DumpRawFiles_f_VAR);
		Symbols::Cmd_AddCommand("DumpStringTables", Cmd_DumpStringTables_f, &Cmd_DumpStringTables_f_VAR);
		Symbols::Cmd_AddCommand("DumpScriptParseTree", Cmd_DumpScriptParseTree_f, &Cmd_DumpScriptParseTree_f_VAR);
		Symbols::Cmd_AddCommand("DumpKeyValuePair", Cmd_DumpKeyValuePair_f, &Cmd_DumpKeyValuePair_f_VAR);
	}

	void UnregisterHooks()
	{

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