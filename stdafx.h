#pragma once

// D2D 사용을 위한 lib
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "WindowsCodecs.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "Comdlg32.lib")

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN   // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <cassert>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// D2D Header Files
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <dwrite.h>

#include <iostream>
#include <commdlg.h>
#include <list>>
#include <math.h>
#include <algorithm>
#include "commonMacroFunction.h"
#include "randomFunction.h"
#include "keyManager.h"
#include "utils.h"
#include "txtData.h"
#include "timeManager.h"
#include "effectManager.h"
#include "sceneManager.h"
#include "soundManager.h"
#include "iniDataManager.h"
#include "D2Dmanager.h"
#include "CameraManager.h"
#include "ImageManager.h"
#include "keyAniManager.h"
#include "itemManager.h"

using namespace std;
using namespace SUNFL_UTIL;
using namespace D2D1;

//==================================
// ## 매크로 처리 ## 21.04.23 ##
//==================================

#define WINNAME (LPTSTR)(TEXT("Heroes of Hammerwatch - B104"))

#define WINSTARTX 50	//윈도우 창 시작좌표 (left)
#define WINSTARTY 50	//윈도우 창 시작좌표 (top)
#define WINSIZEX 1600	//윈도우 가로크기
#define WINSIZEY 900	//윈도우 세로크기
#define MAPSIZEX 8800
#define MAPSIZEY 5280

#define WINSTYLE WS_CAPTION | WS_SYSMENU

#define RND				randomFunction::getSingleton()
#define KEYMANAGER		keyManager::getSingleton()
#define TIMEMANAGER		timeManager::getSingleton()
#define EFFECTMANAGER	effectManager::getSingleton()
#define SCENEMANAGER	sceneManager::getSingleton()
#define SOUNDMANAGER	soundManager::getSingleton()
#define KEYANIMANAGER	keyAniManager::getSingleton()
#define TXTDATA			txtData::getSingleton()
#define INIDATA			iniDataManager::getSingleton()
#define D2DMANAGER		D2Dmanager::getSingleton()
#define CAMERAMANAGER	CameraManager::getSingleton()
#define IMAGEMANAGER	ImageManager::getSingleton()
#define ITEMMANAGER		itemManager::getSingleton()
#define UIMANAGER		UiManager::getSingleton()

#define SAFE_DELETE(p) {if(p) {delete(p); (p) = nullptr;}}
#define SAFE_RELEASE(p) {if(p) {(p)->release(); (p) = NULL;}}

//==================================
// ## extern ## 21.04.28 ##
//==================================

extern HINSTANCE _hInstance;
extern HWND _hWnd;
extern POINT _ptMouse;
extern POINT _ptMouse2;