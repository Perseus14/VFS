//VfsFileInfo.h

#ifndef VFSFILEINFO_H
#define VFSFILEINFO_H
#include<iostream>
#include<fstream>
#include"VfsNodeInfo.h"
class VfsFile;
class VfsFolderInfo;
struct HeaderRecord;
class VfsFileInfo : public VfsNodeInfo 
{
	std::streamoff file_offset;
	long file_bytes;
	VfsNodeInfo * folder_info;
	std::string node_name;
	std::string node_path;
public:
	VfsFileInfo(std::string file_name_const,std::string file_path);
	virtual void getHeader(HeaderRecord & header_record);
	virtual void setParentFolder(VfsNodeInfo*);
	virtual std::string getName();
	virtual std::string getPath();
	void save(std::string ext_path,std::string repo_file_path);
	virtual void load(std::string ext_path,std::string repo_file_path);
	void setOffset(std::streamoff offset){ file_offset = offset;}
	void length(int x){file_bytes=x;}
};

#endif



