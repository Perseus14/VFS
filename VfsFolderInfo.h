//VfsFolderInfo.h

#ifndef VFSFOLDERINFO_H
#define VFSFOLDERINFO_H
#include<iostream>
#include<vector>
#include"VfsNodeInfo.h"

class VfsFileInfo;

class VfsFolderInfo : public VfsNodeInfo 
{
	std::string node_path;
	std::string node_name;
	std::vector<VfsNodeInfo *>file_info;
	std::vector<VfsNodeInfo *>sub_folders;
	VfsNodeInfo * parent;
public:
	//VfsFolderInfo();
	VfsFolderInfo(std::string dir_name_const, std::string path_name_const);
	virtual void addChildFolder(VfsNodeInfo * childFolder);
	virtual void addChildFile(VfsNodeInfo * childFile);
	virtual void setParentFolder(VfsNodeInfo * parentFolder);
	virtual void showFiles(std::vector<std::string> & contents);
	virtual void showFolders(std::vector<std::string> & contents);
	virtual void getHeader(HeaderRecord &hr);
	virtual std::string getPath();
	virtual std::string getName();
	virtual void load(std::string,std::string){}
};

#endif
