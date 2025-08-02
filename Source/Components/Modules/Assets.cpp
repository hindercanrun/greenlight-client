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
				Structs::ScriptParseTree* scriptParseTree = files[i].scriptParseTree;
				std::string asset_name = scriptParseTree->name;
				std::replace(asset_name.begin(), asset_name.end(), '/', '\\'); // Replace forward slashes with backslashes

				Utils::FileSystem::WriteFileToDisk(("game:\\dump\\" + asset_name).c_str(), scriptParseTree->buffer, scriptParseTree->len);
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
		static Structs::cmd_function_s Cmd_DumpLocalizedStrings_f_VAR;
		static Structs::cmd_function_s Cmd_DumpRawFiles_f_VAR;
		static Structs::cmd_function_s Cmd_DumpStringTables_f_VAR;
		static Structs::cmd_function_s Cmd_DumpScriptParseTree_f_VAR;
		static Structs::cmd_function_s Cmd_DumpKeyValuePair_f_VAR;

		void RegisterCommands()
		{
			// Okay so weirdly the game tries to reregister these every frame. So only run this function **once**.
			static bool alreadyRanFunction = FALSE;
			if (alreadyRanFunction)
			{
				alreadyRanFunction = TRUE;
				return;
			}
			alreadyRanFunction = TRUE;

			//Symbols::Cmd_AddCommand("DumpMapEnts", Cmd_DumpMapEnts_f, &Cmd_DumpMapEnts_f_VAR);
			//Symbols::Cmd_AddCommand("DumpLocalizedStrings", Cmd_DumpLocalizedStrings_f, &Cmd_DumpLocalizedStrings_f_VAR);
			//Symbols::Cmd_AddCommand("DumpRawFiles", Cmd_DumpRawFiles_f, &Cmd_DumpRawFiles_f_VAR);
			Symbols::Cmd_AddCommand("DumpStringTables", Cmd_DumpStringTables_f, &Cmd_DumpStringTables_f_VAR);
			//Symbols::Cmd_AddCommand("DumpScriptParseTree", Cmd_DumpScriptParseTree_f, &Cmd_DumpScriptParseTree_f_VAR);
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