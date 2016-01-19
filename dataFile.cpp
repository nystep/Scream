#include "dataFile.h"
#include "miniz.c"
#include <string.h>

namespace SCore {

	//void *dataFile::zip_archive;
	//char *dataFile::zip_archive_filename;

	dataFile::dataFile( const char* dataFileName )
	{
		remove( dataFileName );
		zip_archive = malloc( sizeof(mz_zip_archive) );
		memset( zip_archive, 0, sizeof(mz_zip_archive) );
		mz_zip_reader_init_file( (mz_zip_archive*) zip_archive, dataFileName, 0 );
		zip_archive_filename = strdup( dataFileName );
	}

	dataFile::~dataFile()
	{
		mz_zip_reader_end( (mz_zip_archive*) zip_archive );
		free( zip_archive );
	}

	void* dataFile::getDataFileToHeap( const std::string &fileName, size_t &outFileSize )
	{
		return mz_zip_reader_extract_file_to_heap( (mz_zip_archive*) zip_archive, fileName.c_str(), &outFileSize, 0 );
	}

	void dataFile::addDataFileFromHeap( const std::string &filename, size_t inFileSize, void* mem )
	{
		mz_zip_add_mem_to_archive_file_in_place( zip_archive_filename, filename.c_str(), mem, inFileSize, NULL, 0, MZ_BEST_COMPRESSION);
	}

}
