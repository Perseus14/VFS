//VfsFolderInfo.cc

#include<iostream>
#include<cstring>
#include<string>
#include"VfsRepository.h"
#include"VfsFileInfo.h"
#include"VfsFolderInfo.h"
using namespace std;

//VfsFolderInfo::VfsFolderInfo() : VfsNodeInfo("/",1) {
//}

VfsFolderInfo::VfsFolderInfo(string dir_name, string path_name): VfsNodeInfo(1), node_path(path_name),node_name(dir_name)  { }

void VfsFolderInfo::getHeader(HeaderRecord & hr) 
{
	hr.node_type=1;
	hr.offset=0;
	hr.size=0;
	strcpy(hr.folder_path,node_path.c_str());
	strcpy(hr.node_name,node_name.c_str());

}

void VfsFolderInfo::addChildFile(VfsNodeInfo * childFile) 
{
	file_info.push_back(childFile);
}

void VfsFolderInfo::addChildFolder(VfsNodeInfo * childFolder) 
{
	sub_folders.push_back(childFolder);
}

void VfsFolderInfo::setParentFolder(VfsNodeInfo * parentFolder) 
{
	parent = parentFolder;
}

void VfsFolderInfo::showFiles(vector<string> & contents) 
{
	for(int i=0; i<file_info.size() ; i++)
	{
		string temp_path = file_info[i]->getPath();
		string temp_name = file_info[i]->getName();
		string full_name_str;
		if(temp_path!="/") 
			full_name_str = temp_path+"/"+temp_name;
		else
			full_name_str = temp_path+temp_name;
		contents.push_back(full_name_str);
		
	}	
}

void VfsFolderInfo::showFolders(vector<string> & contents) 
{
	for(int i=0; i<sub_folders.size(); i++) 
	{
		string temp_path = sub_folders[i]->getPath();
		string temp_name = sub_folders[i]->getName();
		string full_name_str;
		if(temp_path!="/") 
			full_name_str = temp_path+"/"+temp_name+"/";
		else
			full_name_str = temp_path+temp_name+"/";
		contents.push_back(full_name_str);
	}
}
	
string VfsFolderInfo::getPath() 
{
	return node_path;
}
string VfsFolderInfo::getName()
{
	return node_name;
}
