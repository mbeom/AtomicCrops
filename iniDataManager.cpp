#include "stdafx.h"
#include "iniDataManager.h"

iniDataManager::iniDataManager()
{
}

iniDataManager::~iniDataManager()
{
}

HRESULT iniDataManager::init()
{
	_listNum = 0;

	return S_OK;
}

void iniDataManager::release()
{
}

int iniDataManager::listNum()
{
	//listNum함수에 들어올때마다 일단 깨끗이 지워주고 시작.
	vLoadIniData.clear();

	char str[256];
	char dir[256];

	_listNum = 0;

	for (int i = 0; i < 100; i++)
	{
		//중복확인 45 ~ 53
		//파일이름
		sprintf_s(dir, "\\%s%d.ini", "saveDATA", i);

		//파일경로 얻기
		GetCurrentDirectory(256, str);

		//"파일경로 + 파일이름"으로 합치기
		strncat_s(str, 256, dir, 254);

		//파일의 유무를 확인해주는 함수
		GetFileAttributes(str);

		//파일명을 가진 파일이 존재한다면
		if (INVALID_FILE_ATTRIBUTES != GetFileAttributes(str) || GetLastError() != ERROR_FILE_NOT_FOUND)
		{
			vLoadIniData.push_back(str);
		}
	}

	//푸시백된 사이즈만큼 리스트 세줌.

	for (int i = 0; i < vLoadIniData.size(); i++)
	{
		_listNum += 1;
	}


	//푸시백된 사이즈 수를 인트값으로 반환.
	return _listNum;
}

float iniDataManager::readData(int num, const char* title)
{
	char playerInfo[256];
	char file[256];

	//ini파일 안의 제목(번호)
	sprintf_s(playerInfo, "플레이어 정보");

	//listNum함수에서 푸시백된 목록 중 num번째에 있는 파일명을 가져옴.
	sprintf_s(file, "%s", vLoadIniData[num].c_str());

	//입력한 num번째의 ini파일의 정보 중, 입력한 title에 해당하는 값을 반환함.
	char data[256] = {};
	char *pos = NULL;
	float value = 0;
	bool isTrue = false;

	isTrue = GetPrivateProfileString(playerInfo, title, "", data, 256, file);

	if (!isTrue) return 0;
	else if (isTrue)
	{
		value = strtof(data, &pos);

		return value;
	}
}

void iniDataManager::addData(const char * title, const char * value)
{
	tagIniData iniData;
	iniData.title = title;
	iniData.value = value;

	arrIniData vIniData;
	vIniData.push_back(iniData);

	_vIniData.push_back(vIniData);
}

void iniDataManager::iniSave()
{
	char str1[256];
	char dir1[256];

	ZeroMemory(dir1, sizeof(dir1));

	for (int i = 0; i < 100; i++)
	{
		char str2[256];
		char dir2[256];
		char playerInfo[256];
		sprintf_s(playerInfo, "플레이어 정보");

		//중복확인 45 ~ 53
		sprintf_s(dir2, "\\%s%d.ini", "saveDATA", i);
		GetCurrentDirectory(256, str2);
		strncat_s(str2, 256, dir2, 254);

		GetFileAttributes(str2);

		//같은이름의 파일이 있다면 continue,
		if (INVALID_FILE_ATTRIBUTES != GetFileAttributes(str2)) continue;

		//세이브파일 생성 55 ~ 69
		sprintf_s(dir1, "\\%s%d.ini", "saveDATA", i);

		GetCurrentDirectory(256, str1);
		strncat_s(str1, 256, dir1, 254);

		for (int j = 0; j < _vIniData.size(); ++j)
		{

			arrIniData vData = _vIniData[j];
			WritePrivateProfileString(playerInfo, vData[0].title, vData[0].value, str1);

			vData.clear();
		}

		break;
	}

	_vIniData.clear();
}

void iniDataManager::soltData()
{
	for (int i = 0; i < 5; i++)
	{
		char str1[256];
		char dir1[256];
		char str2[256];
		char dir2[256];
		char str3[256];
		char dir3[256];

		//중복확인
		sprintf_s(dir1, "\\%s%d.ini", "saveDATA", i);
		sprintf_s(dir3, "\\%s%d.ini", "saveDATA", i + 1);
		sprintf_s(dir2, "\\%s%d.ini", "saveDATA", i + 2);
		GetCurrentDirectory(256, str1);
		strncat_s(str1, 256, dir1, 254);
		GetCurrentDirectory(256, str3);
		strncat_s(str3, 256, dir3, 254);
		GetCurrentDirectory(256, str2);
		strncat_s(str2, 256, dir2, 254);

		GetFileAttributes(str1);
		GetFileAttributes(str3);
		GetFileAttributes(str2);

		if (i == 0 && INVALID_FILE_ATTRIBUTES == GetFileAttributes(str1) && INVALID_FILE_ATTRIBUTES != GetFileAttributes(str3))
		{
			ifstream in(str3);
			ofstream out(str1);
			out << in.rdbuf();
			out.close();
			in.close();

			remove(str3);
		}

		//n번째 파일과 n+2번째 파일 사이에 파일이 있으면 continue, 없으면 n+2번째 파일의 내용을 복사해 붙여넣고 n+2번째 파일은 지움.
		if (INVALID_FILE_ATTRIBUTES != GetFileAttributes(str1) &&
			INVALID_FILE_ATTRIBUTES == GetFileAttributes(str3) &&
			INVALID_FILE_ATTRIBUTES != GetFileAttributes(str2))
		{
			ifstream in(str2);
			ofstream out(str3);
			out << in.rdbuf();
			out.close();
			in.close();

			remove(str2);

			break;
		}

	}
}

void iniDataManager::deleteData(int num)
{
	char dir[256];
	char str[256];

	sprintf_s(dir, "\\%s%d.ini", "saveDATA", num);

	//파일경로 얻기
	GetCurrentDirectory(256, str);

	//"파일경로 + 파일이름"으로 합치기
	strncat_s(str, 256, dir, 254);

	remove(str);
}

//최근 실행한 파일을 등록.
void iniDataManager::currentData(int num)
{
	currentFileNum = num;

	//char str[256];

	//sprintf_s(str, "\\%s%d.ini", "saveDATA", num);

	//GetCurrentDirectory(256, currentFile);
	//strncat_s(currentFile, 256, str, 254);
}

void iniDataManager::updateData(int currentNum, const char * title, float value)
{
	char str[256];
	char dir[256];
	char section[256];
	char valueNum[256];

	sprintf_s(section, "플레이어 정보");
	sprintf_s(valueNum, "%f", value);

	sprintf_s(dir, "\\%s%d.ini", "saveDATA", currentNum);

	GetCurrentDirectory(256, str);
	strncat_s(str, 256, dir, 254);

	WritePrivateProfileString(section, title, valueNum, str);
}

char* iniDataManager::loadDataString(const char * fileName, const char * subject, const char * title)
{
	char str[256];
	char dir[256];

	ZeroMemory(dir, sizeof(dir));
	sprintf_s(dir, "\\%s.ini", fileName);

	GetCurrentDirectory(256, str);
	strncat_s(str, 256, dir, 254);

	char data[64] = {};
	GetPrivateProfileString(subject, title, "", data, 64, str);

	return data;
}

int iniDataManager::loadDataInterger(const char * fileName, const char * subject, const char * title)
{
	char str[256];
	char dir[256];

	ZeroMemory(dir, sizeof(dir));
	sprintf_s(dir, "\\%s.ini", fileName);

	GetCurrentDirectory(256, str);
	strncat_s(str, 256, dir, 254);

	return GetPrivateProfileInt(subject, title, 0, str);
}
