#include "stdafx.h"
#include "title.h"

HRESULT title::init()
{
	ShowCursor(false);

	IMAGEMANAGER->addImage("intro", L"image/UI/SplashScreen.png", 1600, 900);
	IMAGEMANAGER->addImage("FrontLoopMenu", L"image/UI/menuScrollingFront.png", 1600, 900);
	IMAGEMANAGER->addImage("BackLoopMenu", L"image/UI/menuScrollingBack.png", 1600, 900);
	IMAGEMANAGER->addImage("Logo", L"image/UI/Logo.png", 1440, 360);
	IMAGEMANAGER->addImage("menu", L"image/UI/menuBackground.png", 432, 432);
	IMAGEMANAGER->addImage("magenta", L"image/UI/magenta.png", 1600, 900);
	IMAGEMANAGER->addImage("pumpkin", L"image/UI/linksBg.png", 399, 462);
	IMAGEMANAGER->addImage("title_mouse", L"image/UI/Hand.png", 64, 64);
	IMAGEMANAGER->addFrameImage("cabbage_mouse", L"image/UI/MainMenuCabbageSelect.png", 180, 120, 3, 2);

	CAMERAMANAGER->screen = D2DMANAGER->RectMake(0, 0, WINSIZEX, WINSIZEY);

	_intro = new Button;
	_intro->x = WINSIZEX / 2;
	_intro->y = WINSIZEY / 2;
	_intro->img = IMAGEMANAGER->findImage("intro");
	_intro->rc = D2DMANAGER->RectMakeCenter(_intro->x, _intro->y, _intro->img->getWidth(), _intro->img->getHeight());
	_intro->rgb_alpha = 0.0f;
	_intro->is_ready = false;

	_logo = new Button;
	_logo->x = WINSIZEX / 2;
	_logo->y = CAMERAMANAGER->screen.top + 300;
	_logo->img = IMAGEMANAGER->findImage("Logo");
	_logo->rc = D2DMANAGER->RectMakeCenter(_logo->x, _logo->y, _logo->img->getWidth(), _logo->img->getHeight());
	_logo->rgb_alpha = 0.9f;

	_magentaBackground = new Button;
	_magentaBackground->x = WINSIZEX / 2;
	_magentaBackground->y = WINSIZEY / 2;
	_magentaBackground->img = IMAGEMANAGER->findImage("magenta");
	_magentaBackground->rc = D2DMANAGER->RectMakeCenter(_magentaBackground->x, _magentaBackground->y, _magentaBackground->img->getWidth(), _magentaBackground->img->getHeight());
	_magentaBackground->rgb_alpha = 0.3f;

	_backLoopscreen = new Button;
	_backLoopscreen->x = WINSIZEX / 2;
	_backLoopscreen->y = WINSIZEY / 2;
	_backLoopscreen->img = IMAGEMANAGER->findImage("BackLoopMenu");
	_backLoopscreen->rc = D2DMANAGER->RectMakeCenter(_backLoopscreen->x, _backLoopscreen->y, _backLoopscreen->img->getWidth(), _backLoopscreen->img->getHeight());
	_backLoopscreen->loop_y = 1.0f;

	_FrontLoopScreen = new Button;
	_FrontLoopScreen->x = WINSIZEX / 2;
	_FrontLoopScreen->y = WINSIZEY / 2;
	_FrontLoopScreen->img = IMAGEMANAGER->findImage("FrontLoopMenu");
	_FrontLoopScreen->rc = D2DMANAGER->RectMakeCenter(_FrontLoopScreen->x, _FrontLoopScreen->y, _FrontLoopScreen->img->getWidth(), _FrontLoopScreen->img->getHeight());
	_FrontLoopScreen->loop_y = 1.0f;
	_FrontLoopScreen->rgb_alpha = 0.7f;

	_link = new Button;
	_link->img = IMAGEMANAGER->findImage("pumpkin");
	_link->x = CAMERAMANAGER->screen.right - _link->img->getWidth() / 2;
	_link->y = CAMERAMANAGER->screen.bottom - _link->img->getHeight()* 0.1;
	_link->rc = D2DMANAGER->RectMakeCenter(_link->x, _link->y, _link->img->getWidth(), _link->img->getHeight());
	_link->loop_y = 1.0f;
	_link->rgb_alpha = 0.7f;

	_menuBackground = new Button;
	_menuBackground->x = WINSIZEX / 2;
	_menuBackground->y = WINSIZEY * 0.65;
	_menuBackground->img = IMAGEMANAGER->findImage("menu");
	_menuBackground->rc = D2DMANAGER->RectMakeCenter(_menuBackground->x, _menuBackground->y, _menuBackground->img->getWidth(), _menuBackground->img->getHeight());
	_menuBackground->rgb_alpha = 0.7f;

	_menuButton = new Button;
	_menuButton->x = WINSIZEX / 2;
	_menuButton->y = WINSIZEY * 0.55;
	_menuButton->rc = D2DMANAGER->RectMakeCenter(_menuButton->x, _menuButton->y, _menuBackground->img->getWidth() * 0.3, _menuBackground->img->getHeight() * 0.2);
	_menuButton->rgb_alpha = 0.7f;
	_menuButton->string = L"플레이";

	_menuButton2 = new Button;
	_menuButton2->x = WINSIZEX / 2;
	_menuButton2->y = WINSIZEY * 0.65;
	_menuButton2->rc = D2DMANAGER->RectMakeCenter(_menuButton2->x, _menuButton2->y, _menuBackground->img->getWidth() * 0.3, _menuBackground->img->getHeight() * 0.2);
	_menuButton2->rgb_alpha = 0.7f;
	_menuButton2->string = L"맵툴";

	_menuButton3 = new Button;
	_menuButton3->x = WINSIZEX / 2;
	_menuButton3->y = WINSIZEY * 0.75;
	_menuButton3->rc = D2DMANAGER->RectMakeCenter(_menuButton3->x, _menuButton3->y, _menuBackground->img->getWidth() * 0.3, _menuBackground->img->getHeight() * 0.2);
	_menuButton3->rgb_alpha = 0.7f;
	_menuButton3->string = L"종료";

	_mouse = new Button;
	_mouse->img = IMAGEMANAGER->findImage("title_mouse");
	_mouse->x = _ptMouse.x;
	_mouse->y = _ptMouse.y;
	_mouse->rc = D2DMANAGER->RectMakeCenter(_mouse->x, _mouse->y, _mouse->img->getWidth(), _mouse->img->getHeight());
	_mouse->rgb_alpha = 0.7f;

	_cabbage = IMAGEMANAGER->findImage("cabbage_mouse");
	_cabbage2 = IMAGEMANAGER->findImage("cabbage_mouse");

	int cabbage_move[] = { 0, 1, 2, 3 };
	KEYANIMANAGER->addArrayFrameAnimation("cabbage_onMouse", "cabbage_mouse", cabbage_move, 4, 8.0, true);

	_cabbageOn = KEYANIMANAGER->findAnimation("cabbage_onMouse");
	_cabbageOn2 = KEYANIMANAGER->findAnimation("cabbage_onMouse");
	_cabbageOn->start();
	_cabbageOn2->start();

	D2DMANAGER->pRenderTarget->CreateSolidColorBrush(ColorF(1.0f, 0.58f, 0.0f, 1.0f), &orange_brush);
	D2DMANAGER->pRenderTarget->CreateSolidColorBrush(ColorF(0.8f, 1.0f, 0.0f, 1.0f), &green_brush);

	return S_OK;
}

void title::release()
{
	orange_brush->Release();
	green_brush->Release();
}

void title::update()
{
	if (!_intro->is_ready)
	{
		_intro->rgb_alpha += TIMEMANAGER->getElapsedTime() * 0.5;
	}
	
	if (_intro->is_ready && _intro->rgb_alpha > 0)
	{
		_intro->rgb_alpha += TIMEMANAGER->getElapsedTime() * -0.5;
	}

	if (_intro->rgb_alpha > 1.0f)
	{
		_intro->is_ready = true;
		SOUNDMANAGER->play("title");
	}

	if (_intro->rgb_alpha < 0)
	{
		_intro->rgb_alpha = 0;
		start = true;
	}

	if (start)
	{
		_backLoopscreen->loop_y += TIMEMANAGER->getElapsedTime() * 10;
		_FrontLoopScreen->loop_y += TIMEMANAGER->getElapsedTime() * 25;

		_mouse->x = _ptMouse.x;
		_mouse->y = _ptMouse.y;
		_mouse->rc = D2DMANAGER->RectMakeCenter(_mouse->x, _mouse->y, _mouse->img->getWidth(), _mouse->img->getHeight());

		D2D1_RECT_F temp;

		if (PTIN_RECT(&_menuButton->rc, _ptMouse))
		{
			_menuButton->is_ready = true;
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
				SCENEMANAGER->changeScene("world");
		}
		else
			_menuButton->is_ready = false;

		if (PTIN_RECT(&_menuButton2->rc, _ptMouse))
		{
			_menuButton2->is_ready = true;
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
				SCENEMANAGER->changeScene("maptool");
		}
		else
			_menuButton2->is_ready = false;

		if (PTIN_RECT(&_menuButton3->rc, _ptMouse))
		{
			_menuButton3->is_ready = true;
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
				PostQuitMessage(0);
		}
		else
			_menuButton3->is_ready = false;

	}

	CAMERAMANAGER->updateScreen(WINSIZEX/2, WINSIZEY/2);
	KEYANIMANAGER->update();
}

void title::render()
{
	if(!start)
		_intro->img->render(_intro->rc.left, _intro->rc.top, _intro->rgb_alpha);

	if (start)
	{
		_backLoopscreen->img->loopRender(_backLoopscreen->rc, 1.0f, _backLoopscreen->loop_y);
		_magentaBackground->img->render(_magentaBackground->rc.left, _magentaBackground->rc.top, _magentaBackground->rgb_alpha);
		_FrontLoopScreen->img->loopRender(_FrontLoopScreen->rc, 1.0f, _FrontLoopScreen->loop_y, _FrontLoopScreen->rgb_alpha);
		_logo->img->render(_logo->rc.left, _logo->rc.top, _logo->rgb_alpha);
		_link->img->render(_link->rc.left, _link->rc.top, _link->rgb_alpha);
		_menuBackground->img->render(_menuBackground->rc.left, _menuBackground->rc.top, _menuBackground->rgb_alpha);

		if (_menuButton->is_ready)
		{
			D2DMANAGER->DrawTextD2DCenter(green_brush, L"메이플스토리", 40, _menuButton->string.c_str(), _menuButton->rc.left, _menuButton->rc.top, _menuButton->rc.right, _menuButton->rc.bottom);
			_cabbage->aniRender(_menuButton->rc.left - _cabbage->getFrameWidth(), _menuButton->rc.top, _cabbageOn);
			_cabbage2->aniRender(_menuButton->rc.right, _menuButton->rc.top, _cabbageOn2);
		}
		else
			D2DMANAGER->DrawTextD2DCenter(orange_brush, L"메이플스토리", 40, _menuButton->string.c_str(), _menuButton->rc.left, _menuButton->rc.top, _menuButton->rc.right, _menuButton->rc.bottom);
		
		if (_menuButton2->is_ready)
		{
			D2DMANAGER->DrawTextD2DCenter(green_brush, L"메이플스토리", 40, _menuButton2->string.c_str(), _menuButton2->rc.left, _menuButton2->rc.top, _menuButton2->rc.right, _menuButton2->rc.bottom);
			_cabbage->aniRender(_menuButton2->rc.left - _cabbage->getFrameWidth(), _menuButton2->rc.top, _cabbageOn);
			_cabbage2->aniRender(_menuButton2->rc.right, _menuButton2->rc.top, _cabbageOn2);
		}
		else
			D2DMANAGER->DrawTextD2DCenter(orange_brush, L"메이플스토리", 40, _menuButton2->string.c_str(), _menuButton2->rc.left, _menuButton2->rc.top, _menuButton2->rc.right, _menuButton2->rc.bottom);
		
		if (_menuButton3->is_ready)
		{
			D2DMANAGER->DrawTextD2DCenter(green_brush, L"메이플스토리", 40, _menuButton3->string.c_str(), _menuButton3->rc.left, _menuButton3->rc.top, _menuButton3->rc.right, _menuButton3->rc.bottom);
			_cabbage->aniRender(_menuButton3->rc.left - _cabbage->getFrameWidth(), _menuButton3->rc.top, _cabbageOn);
			_cabbage2->aniRender(_menuButton3->rc.right, _menuButton3->rc.top, _cabbageOn2);
		}
		else
			D2DMANAGER->DrawTextD2DCenter(orange_brush, L"메이플스토리", 40, _menuButton3->string.c_str(), _menuButton3->rc.left, _menuButton3->rc.top, _menuButton3->rc.right, _menuButton3->rc.bottom);
	
		_mouse->img->render(_mouse->rc.left, _mouse->rc.top);
	}
}