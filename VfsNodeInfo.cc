//VfsNodeInfo.cc

#include<iostream>
#include"VfsRepository.h"
#include"VfsNodeInfo.h"
using namespace std;

VfsNodeInfo::VfsNodeInfo(int a)
{
	if(a==0)
		node_type=FILE_NAME;
	else
		node_type=FOLDER;
	
}



