#pragma once

#include "singleton.h"
#include <string>

namespace SCore
{

	class dataFile : public Singleton<dataFile>
	{
	protected:

		void *zip_archive;
		char *zip_archive_filename;

		dataFile( const char* dataFileName="demo.data" );
		~dataFile();

		friend class Singleton<dataFile>;

	public:

		void* getDataFileToHeap( const std::string &fileName, size_t &outFileSize );
		void addDataFileFromHeap( const std::string &filename, size_t inFileSize, void* mem );

	};

}
