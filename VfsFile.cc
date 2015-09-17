#include"VfsFile.h"
#include"VfsFileInfo.h"
#include"VfsRepository.h"
#include<sys/stat.h>
#include<cstring>
using namespace std;
int CHUNK=1;
bool fileExist(const std::string& filename) 
{
	struct stat buf;
	if (stat(filename.c_str(), &buf) != -1) {
		return true;
	}
	return false;
}
std::string VfsFile::saveInVFS(fstream& file,string ext_file)
{
	fstream ext(ext_file,ios::in | ios::binary);
	if(!ext) throw "VFS_COPYIN: Error accessing source file in host";
	streamoff file_offset=file.tellp();
	//cout<<file.tellp()<<endl;
	file.seekp(0,ios::end);
	ext.read((char*)&data_bytes,CHUNK);
	file.write((char*)&data_bytes,CHUNK);
	//cout<<data_bytes;
	while(ext.read((char*)&data_bytes,CHUNK))
		{
		file.write((char*)&data_bytes,CHUNK);
	//	cout<<data_bytes;
		}
	//cout<<file.tellp()<<endl;
	int size=file.tellp()-file_offset;
	ext.clear();
	ext.close();
	return to_string(size)+"/"+to_string(file_offset);
}

void VfsFile::export_file(string ext_path,string repo_path,streamoff offset,long bytes)
{
	fstream ext_file;
	fstream repo;
	ext_file.open(ext_path,ios::out);
	ext_file.close();
	ext_file.open(ext_path,ios::in | ios::out);
	repo.open(repo_path,ios::in | ios::out | ios::binary);
	repo.seekg(offset,ios::beg);
	repo.read((char*)&data_bytes,CHUNK);
	while(ext_file.write(data_bytes,CHUNK))
	{
		if(repo.tellg()==offset+bytes)
			break;
		repo.read((char*)&data_bytes,CHUNK);
	}
	ext_file.close();
	if(!fileExist(ext_path))
		throw "VFS_COPYOUT: Unable to save file in host";
	repo.close();
}
