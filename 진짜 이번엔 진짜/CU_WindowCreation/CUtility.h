#pragma once
///<summary>
///Summary description for Utility.
///</summary>
class CUtility
{
private:
	static BOOL SearchFolders(char* filename, char* exeFolder, char* exeName, char fullPath[]);
public:
	static BOOL GetMediaFile(char* file, char path[]);
};

