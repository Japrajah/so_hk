#include "Hooks.hh"
#include "../memory/Addr.hh"
#include "../memory/minhook/include/MinHook.h"
#include "BigWorld.hh"
#ifdef DEBUG
#include <cstdio>
#include <algorithm>
#include <filesystem>


auto oZipHelpers_aesDecrypt = (BinaryFile *(*__cdecl)(int a1, BinaryBlock *a2, unsigned __int8 a3))0;

inline std::string cleanSlashes( std::string filename)
{
	std::string result = filename;
	std::string::size_type slashpos;
	// Clean up any backslashes
	std::replace(result.begin(), result.end(), '\\', '/');
	if ((slashpos = result.find("//", 0)) != std::string::npos)
	{
		// Clean up any duplicate slashes
		do
		{
			result.replace(slashpos, 2, "/");
			slashpos = result.find("//", slashpos);
		} while (slashpos != std::string::npos);
	}
	// Get rid of any leading slashes
	if (result[0] == '/')
		result.erase(0, 1);

	// Get rid of any trailing slashes
	if (!result.empty() && result[result.length() - 1] == '/')
	{
		result.erase(result.length() - 1, 1);
	}
	//	if ( result != filename )
	//		TRACE_MSG( "cleanSlashes: %s => %s\n", filename.c_str(), result.c_str() );

	return result;
}



BinaryFile* __cdecl ResMgr_ZipHelpers_aesDecrypt(int a1, BinaryBlock* in_file, unsigned __int8 a3)
{
	auto result = oZipHelpers_aesDecrypt(a1, in_file, a3);
		BinaryBlock* temfix = result->block;
		if (temfix)
		{

			auto path = std::filesystem::path("Dump\/");
			auto filename = std::string(cleanSlashes(result->file_name->c_str()));
			auto filepak = std::filesystem::path(result->zip_name->c_str());
				path += filepak.filename();
				path += "/";
				path += filename;
		
			if (!std::filesystem::exists(path.parent_path()))
				std::filesystem::create_directories(path.parent_path());

			if (!std::filesystem::exists(path))
			{
				std::cout << path << " \n size: " << temfix->FullSize() << "\n";
				auto file = _wfsopen(path.wstring().c_str(), L"w+b", '@');
			
				if (file)
				{
					_fwrite_nolock(temfix->DATA_, 1, temfix->size , file);
				/*	for (BinaryBlock* current_block = temfix; current_block; current_block = current_block->parrentBlock)
					{
						_fwrite_nolock(current_block->DATA_, 1, current_block->size -10, file);
					};*/
					
					_fclose_nolock(file);
				}

		

			}
		}

	return result;
}

#endif

void Hooks::Setup()
{
#ifdef DEBUG


	/*MH_Initialize();
	CreateHook(&Addr::pfn_aes_decrypt, &ResMgr_ZipHelpers_aesDecrypt, &oZipHelpers_aesDecrypt);
	MH_EnableHook(0);*/

#endif // DEBUG
}

