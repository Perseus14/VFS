//VfsFile.h
#ifndef VFSFILE_H
#define VFSFILE_H
#include<fstream>
#include<string>
//#include "VfsFileInfo.h"
class VfsFileInfo;
class VfsFile{
private:
	char data_bytes[];
	VfsFileInfo *vfsfileinfo;
public:
	void export_file(std::string ext_path,std::string repo_path,std::streamoff offset,long bytes);//container to out
	std::string saveInVFS(std::fstream&,std::string ext_path);//out to container
	//in is input parameter out is output 
	};

#endif
