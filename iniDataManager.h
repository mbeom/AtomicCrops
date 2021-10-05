#pragma once
#include "singletonBase.h"
#include <vector>
#include <fstream>

struct tagIniData
{
	const char* title;
	const char* value;
};

class iniDataManager : public singletonBase<iniDataManager>
{
private:
	typedef vector<tagIniData>						arrIniData;
	typedef vector<tagIniData>::iterator			arrIniDataIter;

	typedef vector<arrIniData>						arrIniDatas;
	typedef vector<arrIniData>::iterator			arrIniDatasIter;

	typedef vector<string>							arrLoadIniData;
	typedef vector<string>::iterator				arrLoadIniDataIter;

	typedef vector<arrLoadIniData>					arrLoadIniDatas;
	typedef vector<arrLoadIniData>::iterator		arrLoadIniDatasIter;

private:
	arrIniDatas _vIniData;

	arrLoadIniData vLoadIniData;

	arrLoadIniDatas _viLoadIniData;

	int currentFileNum;
	int lastFileNum;
	//char currentFile[256];

	int _listNum;

	iniDataManager();
	~iniDataManager();

	friend singletonBase;

public:
	HRESULT init();
	void release();

	int listNum();
	float readData(int num, const char* title);
	void addData(const char* title, const char* value);
	void iniSave();
	void soltData();
	void deleteData(int num);
	void currentData(int num);
	int currentDataNum() { return currentFileNum; }
	void updateData(int num, const char* title, float value);

	char* loadDataString(const char* fileName, const char* subject, const char* title);

	int loadDataInterger(const char* fileName, const char* subject, const char* title);

};


