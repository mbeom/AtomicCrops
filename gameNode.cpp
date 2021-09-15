#include "stdafx.h"
#include "gameNode.h"


gameNode::gameNode()
{
}


gameNode::~gameNode()
{
}

HRESULT gameNode::init()
{
	_hdc = GetDC(_hWnd);
	_managerInit = false;

	return S_OK;
}

HRESULT gameNode::init(bool managerInit)
{
	_hdc = GetDC(_hWnd);
	_managerInit = managerInit;

	if (_managerInit)
	{
		SetTimer(_hWnd, 1, 10, NULL);
		KEYMANAGER->init();
		TIMEMANAGER->init();
		EFFECTMANAGER->init();
		SCENEMANAGER->init();
		SOUNDMANAGER->init();
		TXTDATA->init();
		INIDATA->init();
		D2DMANAGER->Init();
		IMAGEMANAGER->init();
		KEYANIMANAGER->init();
		CAMERAMANAGER->init(MAPSIZEX, MAPSIZEY);
	}

	return S_OK;
}

void gameNode::release()
{
	if (_managerInit)
	{
		KillTimer(_hWnd, 1);
		KEYMANAGER->release();
		KEYMANAGER->releaseSingleton();

		TIMEMANAGER->release();
		TIMEMANAGER->releaseSingleton();

		EFFECTMANAGER->release();
		EFFECTMANAGER->releaseSingleton();

		SCENEMANAGER->release();
		SCENEMANAGER->releaseSingleton();

		SOUNDMANAGER->release();
		SOUNDMANAGER->releaseSingleton();

		TXTDATA->release();
		TXTDATA->releaseSingleton();

		INIDATA->release();
		INIDATA->releaseSingleton();

		D2DMANAGER->release();
		D2DMANAGER->releaseSingleton();

		KEYANIMANAGER->release();
		KEYANIMANAGER->releaseSingleton();

		CAMERAMANAGER->release();
		CAMERAMANAGER->releaseSingleton();

		IMAGEMANAGER->release();
		IMAGEMANAGER->releaseSingleton();
	}
	
	ReleaseDC(_hWnd, _hdc);
}

void gameNode::update()
{
	_ptMouse.x = static_cast<float>(LOWORD(_lparam)) + CAMERAMANAGER->get_camera_x();
	_ptMouse.y = static_cast<float>(HIWORD(_lparam)) + CAMERAMANAGER->get_camera_y();
}

void gameNode::render()
{
}

LRESULT gameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC         hdc; //얘 찐 중요합니...

	switch (iMessage)
	{
		//마우스 움직이면 여기서 메세지 발생
		case WM_MOUSEMOVE:
			_ptMouse.x = static_cast<float>(LOWORD(lParam)) + CAMERAMANAGER->get_camera_x();
			_ptMouse.y = static_cast<float>(HIWORD(lParam)) + CAMERAMANAGER->get_camera_y();

			_ptMouse2.x = static_cast<float>(LOWORD(lParam));
			_ptMouse2.y = static_cast<float>(HIWORD(lParam));

			_lparam = lParam;

		break;

			//마우스 왼쪽 클릭(눌려지고 있을때 여기)

		//case WM_KEYDOWN:

		//	switch (wParam)
		//	{
		//		case VK_ESCAPE:
		//			PostQuitMessage(0);
		//			break;
		//	}
		//	break;


			//윈도우 창 부쉬는(?) 
		case WM_DESTROY:
		//윈도우 종료함수
			PostQuitMessage(0);
			CoUninitialize();

		return 0;

	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
	
}
