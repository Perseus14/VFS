//VfsRepository.cc

#include<sys/stat.h>
#include<iostream>
#include<fstream>
#include"PathString.h"
#include"VfsFileInfo.h"
#include"VfsFolderInfo.h"
#include"VfsNodeInfo.h"
#include"VfsRepository.h"
#include"vfs_errors.h"
#include<algorithm>
using namespace std;

bool fileExists(const std::string& filename) 
{
	struct stat buf;
	if (stat(filename.c_str(), &buf) != -1) {
		return true;
	}
	return false;
}

VfsRepository::VfsRepository()
{
	repo_status=NEUTRAL;
}

void VfsRepository::create(string name, string external_path) 
{
	fstream file;
	num_nodes=0;
	repo_name=name;
	repo_status=NEUTRAL;
	repo_file_path=external_path;
	if(fileExists(repo_file_path))
	{
		file.open(repo_file_path.c_str(), ios::in | ios::out | ios::binary);
	}
	else 
	{
		file.open(repo_file_path.c_str(), ios::out|ios::binary);
		file.close();
		file.open(repo_file_path.c_str(), ios::in | ios::out | ios::binary);
	}
	if(!file.good())
		throw VFS_CREATE_01;
	file.write((char*)&num_nodes,sizeof(int));
	HeaderRecord hr;
	file.seekp(0,ios::end);
	file.write ((char*) &hr , sizeof(HeaderRecord)*MAX_NODES);
	file.clear();
	file.close();
}

void VfsRepository::open(string name, string external_path) 
{
	fstream file;
	repo_name=name;
	repo_file_path=external_path;
	if(repo_status==OPEN)
		throw "VFS_OPEN:Repository Already Open";
	repo_status=OPEN;
	file.open(repo_file_path.c_str(), ios::in | ios::out | ios::binary);
	if (!file.good())
    		throw VFS_OPEN_01;
	file.read((char*) &num_nodes, sizeof (int));
	HeaderRecord hr;
	for(int i=0;i<num_nodes;i++) 
	{
		file.read((char*) &hr, sizeof(HeaderRecord));
		if(hr.node_type==0) 
		{
			string node_name_str(hr.node_name);
			string file_path_str(hr.folder_path);
			string full_name_str;
			if(file_path_str!="/")
				full_name_str = file_path_str+"/"+node_name_str;
			else
				full_name_str = file_path_str+node_name_str;
			VfsFileInfo *temp=new VfsFileInfo (node_name_str,file_path_str);
			nodes[full_name_str]=temp;
			nodes[file_path_str]->addChildFile(temp);
			temp->setParentFolder(nodes[file_path_str]);
			temp->setOffset(hr.offset);
			temp->length(hr.size);
		}
		else 
		{ 							 //it is a folder
			string node_name_str(hr.node_name);
			string folder_path_str(hr.folder_path);
			string full_name_str;
			if(folder_path_str=="") 
			{
				makeDir("",""); 
			}
			else if (folder_path_str=="/" ) 
			{
				makeDir("",node_name_str);
			}
			else 
				makeDir(folder_path_str,node_name_str);
		}
	}
	file.clear();
	file.close();
}

void VfsRepository::close() 
{
	fstream file;

	//cout<<repo_status<<endl;
	//cout<<repo_status<<endl;
	if (repo_status != OPEN)
	{
    		throw VFS_GEN_01;
	}
	file.open(repo_file_path.c_str(), ios::in | ios::out | ios::binary);
	if (!file.good())
    		throw VFS_CLOSE_01;
	num_nodes=nodes.size();
	file.write((char*)&num_nodes,sizeof(int));
	HeaderRecord hr;
	int count=0;
	for(auto im=nodes.begin(); im!=nodes.end(); im++) 
	{
		 (im->second)->getHeader(hr); //virtual function to convert class to structure
		// cout<<hr.folder_path<<"-"<<hr.node_name<<endl;
		 file.write ((char*) &hr , sizeof(HeaderRecord));
	}
	file.clear();
	file.close();
	nodes.clear();
	repo_status = CLOSED;
}

void VfsRepository::makeDir(string dir_path,string dir_name) 
{
	auto search=nodes.find(dir_path+"/"+dir_name);
	if (repo_status == CLOSED)
    		throw VFS_GEN_01;
	if(search!=nodes.end())
		throw VFS_MAKEDIR_02;
	int temp_count=0;
	string full_path_str = dir_path+"/"+dir_name;
	auto im=nodes.begin();
	HeaderRecord hr;
	if(dir_path=="" && dir_name=="") 
	{
		temp_count=1;
		VfsFolderInfo * newFolder = new VfsFolderInfo("/","");
		nodes["/"]=newFolder;
	}
	else if (dir_path=="" && dir_name!="") 
	{
		temp_count=1;
		dir_path="/";
		for(im=nodes.begin(); im!=nodes.end(); im++) 
		{
			if(dir_path==(im->first)) 
			{
				VfsFolderInfo * newFolder = new VfsFolderInfo(dir_name,"/");
				
				newFolder->setParentFolder(im->second);
				im->second->addChildFolder(newFolder);
				nodes[dir_path+dir_name]=newFolder;
				break;
			}
		}
	}
	else 
	{
		for(im=nodes.begin(); im!=nodes.end(); im++) 
		{
			if(dir_path==(im->first)) 
			{
				temp_count=1;
				VfsFolderInfo * newFolder = new VfsFolderInfo(dir_name,dir_path); 
				newFolder->setParentFolder(im->second); 
				im->second->addChildFolder(newFolder);
				nodes[full_path_str]=newFolder;
				break;
			}
		}	
	}
	if(temp_count==0) 
	{
		throw VFS_MAKEDIR_01;
	}
}

void VfsRepository::list(string dir_name_arg, vector<string> & content) 
{
	if (repo_status == CLOSED)
    		throw VFS_GEN_01;
	auto search=nodes.find(dir_name_arg);
	if(search!=nodes.end())
		{
			nodes[dir_name_arg]->showFiles(content);
			nodes[dir_name_arg]->showFolders(content);
		}
	else
    		throw VFS_LISTDIR_01;	
	sort(content.begin(),content.end());
}

void VfsRepository::copyIn(string ext_path,string int_path)
{
	if (repo_status == CLOSED)
    		throw VFS_GEN_01;
	auto search=nodes.find(int_path);
	if(!fileExists(ext_path))
		throw VFS_COPYIN_03;
	if(search!=nodes.end())
		throw VFS_COPYIN_02;
	PathString P(int_path,"/");
	string path=P.excludeLast();
	//cout<<path<<endl;
	string name=P.getLast();
	//cout<<name<<endl;
	if(path=="")
		path="/";
	search=nodes.find(path);
	if(search!=nodes.end())
		{
			VfsFileInfo* file= new VfsFileInfo(name,path);
			nodes[path]->addChildFile(file);
		//	cout<<"Child Added"<<endl;
			file->setParentFolder(nodes[path]);
		//	cout<<"Parent Added"<<endl;
			file->save(ext_path,repo_file_path);	
			nodes[int_path]=file;
		//	cout<<"File Saved"<<endl; 
		}
	else
		throw VFS_COPYIN_01;
}

void VfsRepository::copyOut(string int_path,string ext_path)
{
	if (repo_status == CLOSED)
    		throw VFS_GEN_01;
	auto search = nodes.find(int_path);
	if(search==nodes.end())
		throw VFS_COPYOUT_01;
	nodes[int_path]->load(ext_path,repo_file_path);
	//cout<<ext_path<<" "<<repo_file_path<<endl;
}	
