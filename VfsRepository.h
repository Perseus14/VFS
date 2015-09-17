//VfsRepository.h

#ifndef VFSREPOSITORY_H
#define VFSREPOSITORY_H
#include<iostream>
#include<vector>
#include<map>

class VfsNodeInfo;

const int MAX_NAME_LENGTH=200;
const int MAX_PATH_LEN=200;
const int MAX_NODES=200;


enum repo{OPEN, CLOSED,NEUTRAL};

struct HeaderRecord 
{
	char node_name[MAX_NAME_LENGTH];
	int node_type;
	int offset;
	int size;
	char folder_path[MAX_PATH_LEN];
};

class VfsRepository 
{
	std::string repo_name;
	std::string repo_file_path;
	std::vector<VfsNodeInfo *> node_info;
	std::map<std::string, VfsNodeInfo *> nodes;
	repo repo_status;
	int num_nodes;
	void loadHeader();
	void saveHeader();
public:
	VfsRepository();
	void create (std::string name, std::string external_path);
	void open(std::string name, std::string external_path);
	void close();
	void makeDir(std::string dir_path, std::string dir_name);
	void list(std::string dir_name_arg, std::vector<std::string> & content);
	void copyIn(std::string ext_path,std::string int_path);
	void copyOut(std::string int_path,std::string ext_path);
};

#endif
