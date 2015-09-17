//VfsFileInfo.cc

#include<iostream>
#include<cstring>
#include"PathString.h"
#include"VfsFile.h"
#include"VfsFolderInfo.h"
#include"VfsFileInfo.h"
#include"VfsRepository.h"
using namespace std;


VfsFileInfo::VfsFileInfo(string file_name,string dir_name) :VfsNodeInfo(0),node_name(file_name),node_path(dir_name) { }

void VfsFileInfo::getHeader(HeaderRecord & hr) 
{
	hr.node_type=0;
        hr.offset=file_offset;
        hr.size=file_bytes;
        strcpy(hr.folder_path,node_path.c_str());
        strcpy(hr.node_name,node_name.c_str());
}
void VfsFileInfo::setParentFolder(VfsNodeInfo* x)
{
	folder_info=x;
}
string VfsFileInfo::getPath()
{
	return node_path;
}
string VfsFileInfo::getName()
{
	return node_name;
}

void VfsFileInfo::save(string ext_path,string repo_file_path)
{
	VfsFile* f = new VfsFile();
	fstream file;
	file.open(repo_file_path,ios::out | ios::in | ios::ate | ios::binary);
	string temp=f->saveInVFS(file,ext_path);
	PathString P1(temp,"/");
	file_bytes=stoi(P1.excludeLast());
	file_offset=stoi(P1.getLast());
	file.close();
}

void VfsFileInfo::load(string ext_path,string repo_file_path)
{
	VfsFile* f = new VfsFile();
	f->export_file(ext_path,repo_file_path,file_offset,file_bytes);
	delete f;
}
