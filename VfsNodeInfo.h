//VfsNodeInfo.h

#ifndef VFSNODEINFO_H
#define VFSNODEINFO_H
#include<iostream>
#include<vector>
#include<string>
struct HeaderRecord;
class VfsRepository;
class VfsNodeInfo 
{
	enum type {FILE_NAME,FOLDER} node_type;
	VfsRepository * repository;
public:
	VfsNodeInfo(int a);
	virtual void getHeader(HeaderRecord & header_record)=0;
	virtual void setParentFolder(VfsNodeInfo*){}
	virtual void addChildFolder(VfsNodeInfo*){}
	virtual void addChildFile(VfsNodeInfo*){}
	virtual void showFolders(std::vector<std::string>&){}
	virtual void showFiles(std::vector <std::string>&){}
	virtual std::string getPath(){}
	virtual std::string getName(){}
	virtual void load(std::string ext_path,std::string repo_file_path)=0;
};

#endif
