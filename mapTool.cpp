#include "stdafx.h"
#include "mapTool.h"

HRESULT mapTool::init()
{

	_tool.img = new image;
	_tool.img = IMAGEMANAGER->addImage("menu", L"img/tile/menu.png", 300, 900);

	_terrain.img = new image;
	_terrain2.img = new image;
	_wall.img = new image;
	_object.img = new image;
	_object2.img = new image;
	_object3.img = new image;
	_menuBtn.img = new image;
	_menuBtn2.img = new image;
	_saveButton.img = new image;
	_loadButton.img = new image;
	_eraserBtn.img = new image;
	_currentTileBox.img = new image;

	_terrain.img = IMAGEMANAGER->addFrameImage("terrain", L"img/tile/terrain.png", 256, 512, SAMPLETILEX, SAMPLETILEY);
	_terrain2.img = IMAGEMANAGER->addFrameImage("terrain2", L"img/tile/terrain2.png", 256, 512, SAMPLETILEX, SAMPLETILEY);
	_wall.img = IMAGEMANAGER->addFrameImage("wall", L"img/tile/walls.png", 256, 512, SAMPLETILEX, SAMPLETILEY);
	_object.img = IMAGEMANAGER->addFrameImage("obj", L"img/tile/object.png", 256, 512, SAMPLETILEX, SAMPLETILEY);
	_object2.img = IMAGEMANAGER->addFrameImage("obj2", L"img/tile/object2.png", 256, 512, SAMPLETILEX, SAMPLETILEY);
	_object3.img = IMAGEMANAGER->addFrameImage("obj3", L"img/tile/object3.png", 256, 512, SAMPLETILEX, SAMPLETILEY);
	_menuBtn.img = IMAGEMANAGER->addImage("menuBtn", L"img/tile/menuButton.png", 50, 50);
	_menuBtn2.img = IMAGEMANAGER->addImage("menuBtn2", L"img/tile/menuButton2.png", 50, 50);
	_saveButton.img = IMAGEMANAGER->addImage("saveBtn", L"img/tile/save.png", 128, 36);
	_loadButton.img = IMAGEMANAGER->addImage("loadBtn", L"img/tile/load.png", 128, 36);
	_currentTileBox.img = IMAGEMANAGER->addImage("ctBox", L"img/tile/ctBOX.png", 80, 100);

	_tool.rc = D2DMANAGER->RectMake(CAMERAMANAGER->get_camera_x() + WINSIZEX + 25, CAMERAMANAGER->get_camera_y(), 300, WINSIZEY);
	_menuBtn.rc = D2DMANAGER->RectMake(_tool.rc.left + 160, _tool.rc.top + 280, 50, 50);
	_menuBtn2.rc = D2DMANAGER->RectMake(_menuBtn.rc.left + 80, _tool.rc.top + 280, 50, 50);
	_saveButton.rc = D2DMANAGER->RectMake(_tool.rc.left + 10, _menuBtn.rc.top - 100, 128, 36);
	_loadButton.rc = D2DMANAGER->RectMake(_saveButton.rc.left + 155, _menuBtn2.rc.top - 100, 128, 36);
	_currentTileBox.rc = D2DMANAGER->RectMake(_tool.rc.left + 30, _menuBtn.rc.top - 40, 80, 100);

	setup();

	_tiles->sizeX = _tiles->sizeY = 16;
	_sampleTile->sizeY = 32;
	_sampleTile->sizeX = 16;

	_tool.open = false;
	_menu = Terrain;
	_ctrSelect = CTRL_TERRAINDRAW;

	return S_OK;
}

void mapTool::release()
{
}

void mapTool::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_TAB))
	{
		if (_tool.open) _tool.open = false;
		else _tool.open = true;
	}

	if (_tool.open)
	{
		if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
		{
			for (int i = 0; i < SAMPLETILEX * SAMPLETILEY; ++i)
			{
				if (PTIN_RECT(&_sampleTile[i].rcTile, _ptMouse))
				{
					if (_tiles->sizeX + _tiles->sizeY == 32) {
						_currentTile.x = _sampleTile[i].terrainFrameX;
						_currentTile.y = _sampleTile[i].terrainFrameY;
					}
					else if (_tiles->sizeX + _tiles->sizeY == 48)
					{
						if (_tiles->sizeX == 32)
						{
							_currentTile.x = _sampleTile[i].terrainFrameX;
							_currentTile.y = _sampleTile[i].terrainFrameY;
							_ct[0].x = _sampleTile[i + 1].terrainFrameX;
							_ct[0].y = _sampleTile[i].terrainFrameY;
						}
						else
						{
							_currentTile.x = _sampleTile[i].terrainFrameX;
							_currentTile.y = _sampleTile[i].terrainFrameY;
							_ct[0].x = _sampleTile[i].terrainFrameX;
							_ct[0].y = _sampleTile[i + SAMPLETILEX].terrainFrameY;
						}
					}
					else if (_tiles->sizeX + _tiles->sizeY == 64)
					{
						if (_tiles->sizeX == 16)
						{
							_currentTile.x = _sampleTile[i].terrainFrameX;
							_currentTile.y = _sampleTile[i].terrainFrameY;
							_ct[0].x = _sampleTile[i].terrainFrameX;
							_ct[0].y = _sampleTile[i + SAMPLETILEX].terrainFrameY;
							_ct[1].x = _sampleTile[i].terrainFrameX;
							_ct[1].y = _sampleTile[i + SAMPLETILEX * 2].terrainFrameY;
						}
						else if (_tiles->sizeX == 32)
						{
							_currentTile.x = _sampleTile[i].terrainFrameX;
							_currentTile.y = _sampleTile[i].terrainFrameY;
							_ct[0].x = _sampleTile[i + 1].terrainFrameX;
							_ct[0].y = _sampleTile[i].terrainFrameY;
							_ct[1].x = _sampleTile[i].terrainFrameX;
							_ct[1].y = _sampleTile[i + SAMPLETILEX].terrainFrameY;
							_ct[2].x = _sampleTile[i + 1].terrainFrameX;
							_ct[2].y = _sampleTile[i + SAMPLETILEX].terrainFrameY;
						}
						else if (_tiles->sizeX == 48)
						{
							_currentTile.x = _sampleTile[i].terrainFrameX;
							_currentTile.y = _sampleTile[i].terrainFrameY;
							_ct[0].x = _sampleTile[i + 1].terrainFrameX;
							_ct[0].y = _sampleTile[i].terrainFrameY;
							_ct[1].x = _sampleTile[i + 2].terrainFrameX;
							_ct[1].y = _sampleTile[i].terrainFrameY;
						}
					}
					else if (_tiles->sizeX + _tiles->sizeY == 80)
					{
						if (_tiles->sizeX == 32)
						{
							_currentTile.x = _sampleTile[i].terrainFrameX;
							_currentTile.y = _sampleTile[i].terrainFrameY;
							_ct[0].x = _sampleTile[i + 1].terrainFrameX;
							_ct[0].y = _sampleTile[i].terrainFrameY;
							_ct[1].x = _sampleTile[i].terrainFrameX;
							_ct[1].y = _sampleTile[i + SAMPLETILEX].terrainFrameY;
							_ct[2].x = _sampleTile[i + 1].terrainFrameX;
							_ct[2].y = _sampleTile[i + SAMPLETILEX].terrainFrameY;
							_ct[3].x = _sampleTile[i].terrainFrameX;
							_ct[3].y = _sampleTile[i + SAMPLETILEX * 2].terrainFrameY;
							_ct[4].x = _sampleTile[i + 1].terrainFrameX;
							_ct[4].y = _sampleTile[i + SAMPLETILEX * 2].terrainFrameY;
						}
						else if (_tiles->sizeX == 48)
						{
							_currentTile.x = _sampleTile[i].terrainFrameX;
							_currentTile.y = _sampleTile[i].terrainFrameY;
							_ct[0].x = _sampleTile[i + 1].terrainFrameX;
							_ct[0].y = _sampleTile[i].terrainFrameY;
							_ct[1].x = _sampleTile[i + 2].terrainFrameX;
							_ct[1].y = _sampleTile[i].terrainFrameY;
							_ct[2].x = _sampleTile[i].terrainFrameX;
							_ct[2].y = _sampleTile[i + SAMPLETILEX].terrainFrameY;
							_ct[3].x = _sampleTile[i + 1].terrainFrameX;
							_ct[3].y = _sampleTile[i + SAMPLETILEX].terrainFrameY;
							_ct[4].x = _sampleTile[i + 2].terrainFrameX;
							_ct[4].y = _sampleTile[i + SAMPLETILEX].terrainFrameY;
						}
					}
					else if (_tiles->sizeX + _tiles->sizeY == 96)
					{
						_currentTile.x = _sampleTile[i].terrainFrameX;
						_currentTile.y = _sampleTile[i].terrainFrameY;
						_ct[0].x = _sampleTile[i + 1].terrainFrameX;
						_ct[0].y = _sampleTile[i].terrainFrameY;
						_ct[1].x = _sampleTile[i + 2].terrainFrameX;
						_ct[1].y = _sampleTile[i].terrainFrameY;
						_ct[2].x = _sampleTile[i].terrainFrameX;
						_ct[2].y = _sampleTile[i + SAMPLETILEX].terrainFrameY;
						_ct[3].x = _sampleTile[i + 1].terrainFrameX;
						_ct[3].y = _sampleTile[i + SAMPLETILEX].terrainFrameY;
						_ct[4].x = _sampleTile[i + 2].terrainFrameX;
						_ct[4].y = _sampleTile[i + SAMPLETILEX].terrainFrameY;
						_ct[5].x = _sampleTile[i].terrainFrameX;
						_ct[5].y = _sampleTile[i + SAMPLETILEX * 2].terrainFrameY;
						_ct[6].x = _sampleTile[i + 1].terrainFrameX;
						_ct[6].y = _sampleTile[i + SAMPLETILEX * 2].terrainFrameY;
						_ct[7].x = _sampleTile[i + 2].terrainFrameX;
						_ct[7].y = _sampleTile[i + SAMPLETILEX * 2].terrainFrameY;
					}
				}
			}


			for (int i = 0; i < TILEX * TILEY; ++i)
			{
				if (PTIN_RECT(&_tiles[i].rc, _ptMouse) && !PTIN_RECT(&_tool.rc, _ptMouse))
				{
					switch (_menu)
					{
					case Terrain:
					case Wall:
					case Terrain2:
						if (_tiles->sizeX + _tiles->sizeY == 32) {
							_tiles[i].terrainFrameX = _currentTile.x;
							_tiles[i].terrainFrameY = _currentTile.y;

							_tiles[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);
						}
						else if (_tiles->sizeX + _tiles->sizeY == 48)
						{
							if (_tiles->sizeX == 32)
							{
								_tiles[i].terrainFrameX = _currentTile.x;
								_tiles[i].terrainFrameY = _currentTile.y;
								_tiles[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);

								_tiles[i + 1].terrainFrameX = _ct[0].x;
								_tiles[i + 1].terrainFrameY = _ct[0].y;
								_tiles[i + 1].terrain = terrainSelect(_ct[0].x, _ct[0].y);
							}
							else
							{
								_tiles[i].terrainFrameX = _currentTile.x;
								_tiles[i].terrainFrameY = _currentTile.y;
								_tiles[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);

								_tiles[i + TILEX].terrainFrameX = _ct[0].x;
								_tiles[i + TILEX].terrainFrameY = _ct[0].y;
								_tiles[i + TILEX].terrain = terrainSelect(_ct[0].x, _ct[0].y);
							}
						}
						else if (_tiles->sizeX + _tiles->sizeY == 64)
						{
							if (_tiles->sizeX == 16)
							{
								_tiles[i].terrainFrameX = _currentTile.x;
								_tiles[i].terrainFrameY = _currentTile.y;
								_tiles[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);

								_tiles[i + TILEX].terrainFrameX = _ct[0].x;
								_tiles[i + TILEX].terrainFrameY = _ct[0].y;
								_tiles[i + TILEX].terrain = terrainSelect(_ct[0].x, _ct[0].y);

								_tiles[i + TILEX * 2].terrainFrameX = _ct[1].x;
								_tiles[i + TILEX * 2].terrainFrameY = _ct[1].y;
								_tiles[i + TILEX * 2].terrain = terrainSelect(_ct[1].x, _ct[1].y);
							}
							else if (_tiles->sizeX == 32)
							{
								_tiles[i].terrainFrameX = _currentTile.x;
								_tiles[i].terrainFrameY = _currentTile.y;
								_tiles[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);

								_tiles[i + 1].terrainFrameX = _ct[0].x;
								_tiles[i + 1].terrainFrameY = _ct[0].y;
								_tiles[i + 1].terrain = terrainSelect(_ct[0].x, _ct[0].y);

								_tiles[i + TILEX].terrainFrameX = _ct[1].x;
								_tiles[i + TILEX].terrainFrameY = _ct[1].y;
								_tiles[i + TILEX].terrain = terrainSelect(_ct[1].x, _ct[1].y);

								_tiles[i + TILEX + 1].terrainFrameX = _ct[2].x;
								_tiles[i + TILEX + 1].terrainFrameY = _ct[2].y;
								_tiles[i + TILEX + 1].terrain = terrainSelect(_ct[2].x, _ct[2].y);
							}
							else if (_tiles->sizeX == 48)
							{
								_tiles[i].terrainFrameX = _currentTile.x;
								_tiles[i].terrainFrameY = _currentTile.y;
								_tiles[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);

								_tiles[i + 1].terrainFrameX = _ct[0].x;
								_tiles[i + 1].terrainFrameY = _ct[0].y;
								_tiles[i + 1].terrain = terrainSelect(_ct[0].x, _ct[0].y);

								_tiles[i + 2].terrainFrameX = _ct[1].x;
								_tiles[i + 2].terrainFrameY = _ct[1].y;
								_tiles[i + 2].terrain = terrainSelect(_ct[1].x, _ct[1].y);
							}
						}
						else if (_tiles->sizeX + _tiles->sizeY == 80)
						{
							if (_tiles->sizeX == 32)
							{
								_tiles[i].terrainFrameX = _currentTile.x;
								_tiles[i].terrainFrameY = _currentTile.y;
								_tiles[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);

								_tiles[i + 1].terrainFrameX = _ct[0].x;
								_tiles[i + 1].terrainFrameY = _ct[0].y;
								_tiles[i + 1].terrain = terrainSelect(_ct[0].x, _ct[0].y);

								_tiles[i + TILEX].terrainFrameX = _ct[1].x;
								_tiles[i + TILEX].terrainFrameY = _ct[1].y;
								_tiles[i + TILEX].terrain = terrainSelect(_ct[1].x, _ct[1].y);

								_tiles[i + TILEX + 1].terrainFrameX = _ct[2].x;
								_tiles[i + TILEX + 1].terrainFrameY = _ct[2].y;
								_tiles[i + TILEX + 1].terrain = terrainSelect(_ct[2].x, _ct[2].y);

								_tiles[i + TILEX * 2].terrainFrameX = _ct[3].x;
								_tiles[i + TILEX * 2].terrainFrameY = _ct[3].y;
								_tiles[i + TILEX * 2].terrain = terrainSelect(_ct[3].x, _ct[3].y);

								_tiles[i + TILEX * 2 + 1].terrainFrameX = _ct[4].x;
								_tiles[i + TILEX * 2 + 1].terrainFrameY = _ct[4].y;
								_tiles[i + TILEX * 2 + 1].terrain = terrainSelect(_ct[4].x, _ct[4].y);
							}
							else if (_tiles->sizeX == 48)
							{
								_tiles[i].terrainFrameX = _currentTile.x;
								_tiles[i].terrainFrameY = _currentTile.y;
								_tiles[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);

								_tiles[i + 1].terrainFrameX = _ct[0].x;
								_tiles[i + 1].terrainFrameY = _ct[0].y;
								_tiles[i + 1].terrain = terrainSelect(_ct[0].x, _ct[0].y);

								_tiles[i + 2].terrainFrameX = _ct[1].x;
								_tiles[i + 2].terrainFrameY = _ct[1].y;
								_tiles[i + 2].terrain = terrainSelect(_ct[1].x, _ct[1].y);

								_tiles[i + TILEX].terrainFrameX = _ct[2].x;
								_tiles[i + TILEX].terrainFrameY = _ct[2].y;
								_tiles[i + TILEX].terrain = terrainSelect(_ct[2].x, _ct[2].y);

								_tiles[i + TILEX + 1].terrainFrameX = _ct[3].x;
								_tiles[i + TILEX + 1].terrainFrameY = _ct[3].y;
								_tiles[i + TILEX + 1].terrain = terrainSelect(_ct[3].x, _ct[3].y);

								_tiles[i + TILEX + 2].terrainFrameX = _ct[4].x;
								_tiles[i + TILEX + 2].terrainFrameY = _ct[4].y;
								_tiles[i + TILEX + 2].terrain = terrainSelect(_ct[4].x, _ct[4].y);
							}
						}
						else if (_tiles->sizeX + _tiles->sizeY == 96)
						{
							_tiles[i].terrainFrameX = _currentTile.x;
							_tiles[i].terrainFrameY = _currentTile.y;
							_tiles[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);

							_tiles[i + 1].terrainFrameX = _ct[0].x;
							_tiles[i + 1].terrainFrameY = _ct[0].y;
							_tiles[i + 1].terrain = terrainSelect(_ct[0].x, _ct[0].y);

							_tiles[i + 2].terrainFrameX = _ct[1].x;
							_tiles[i + 2].terrainFrameY = _ct[1].y;
							_tiles[i + 2].terrain = terrainSelect(_ct[1].x, _ct[1].y);

							_tiles[i + TILEX].terrainFrameX = _ct[2].x;
							_tiles[i + TILEX].terrainFrameY = _ct[2].y;
							_tiles[i + TILEX].terrain = terrainSelect(_ct[2].x, _ct[2].y);

							_tiles[i + TILEX + 1].terrainFrameX = _ct[3].x;
							_tiles[i + TILEX + 1].terrainFrameY = _ct[3].y;
							_tiles[i + TILEX + 1].terrain = terrainSelect(_ct[3].x, _ct[3].y);

							_tiles[i + TILEX + 2].terrainFrameX = _ct[4].x;
							_tiles[i + TILEX + 2].terrainFrameY = _ct[4].y;
							_tiles[i + TILEX + 2].terrain = terrainSelect(_ct[4].x, _ct[4].y);

							_tiles[i + TILEX * 2].terrainFrameX = _ct[5].x;
							_tiles[i + TILEX * 2].terrainFrameY = _ct[5].y;
							_tiles[i + TILEX * 2].terrain = terrainSelect(_ct[5].x, _ct[5].y);

							_tiles[i + TILEX * 2 + 1].terrainFrameX = _ct[6].x;
							_tiles[i + TILEX * 2 + 1].terrainFrameY = _ct[6].y;
							_tiles[i + TILEX * 2 + 1].terrain = terrainSelect(_ct[6].x, _ct[6].y);

							_tiles[i + TILEX * 2 + 2].terrainFrameX = _ct[7].x;
							_tiles[i + TILEX * 2 + 2].terrainFrameY = _ct[7].y;
							_tiles[i + TILEX * 2 + 2].terrain = terrainSelect(_ct[7].x, _ct[7].y);
						}
						_ctrSelect = CTRL_TERRAINDRAW;
						break;
					case Object:
					case Object2:
					case Object3:
						if (_tiles->sizeX + _tiles->sizeY == 32) {
							_tiles[i].objFrameX = _currentTile.x;
							_tiles[i].objFrameY = _currentTile.y;

							_tiles[i].obj = objSelect(_currentTile.x, _currentTile.y);
						}
						else if (_tiles->sizeX + _tiles->sizeY == 48)
						{
							if (_tiles->sizeX == 32)
							{
								_tiles[i].objFrameX = _currentTile.x;
								_tiles[i].objFrameY = _currentTile.y;
								_tiles[i].obj = objSelect(_currentTile.x, _currentTile.y);

								_tiles[i + 1].objFrameX = _ct[0].x;
								_tiles[i + 1].objFrameY = _ct[0].y;
								_tiles[i + 1].obj = objSelect(_ct[0].x, _ct[0].y);
							}
							else
							{
								_tiles[i].objFrameX = _currentTile.x;
								_tiles[i].objFrameY = _currentTile.y;
								_tiles[i].obj = objSelect(_currentTile.x, _currentTile.y);

								_tiles[i + TILEX].objFrameX = _ct[0].x;
								_tiles[i + TILEX].objFrameY = _ct[0].y;
								_tiles[i + TILEX].obj = objSelect(_ct[0].x, _ct[0].y);
							}
						}
						else if (_tiles->sizeX + _tiles->sizeY == 64)
						{
							if (_tiles->sizeX == 16)
							{
								_tiles[i].objFrameX = _currentTile.x;
								_tiles[i].objFrameY = _currentTile.y;
								_tiles[i].obj = objSelect(_currentTile.x, _currentTile.y);

								_tiles[i + TILEX].objFrameX = _ct[0].x;
								_tiles[i + TILEX].objFrameY = _ct[0].y;
								_tiles[i + TILEX].obj = objSelect(_ct[0].x, _ct[0].y);

								_tiles[i + TILEX * 2].objFrameX = _ct[1].x;
								_tiles[i + TILEX * 2].objFrameY = _ct[1].y;
								_tiles[i + TILEX * 2].obj = objSelect(_ct[1].x, _ct[1].y);
							}
							else if (_tiles->sizeX == 32)
							{
								_tiles[i].objFrameX = _currentTile.x;
								_tiles[i].objFrameY = _currentTile.y;
								_tiles[i].obj = objSelect(_currentTile.x, _currentTile.y);

								_tiles[i + 1].objFrameX = _ct[0].x;
								_tiles[i + 1].objFrameY = _ct[0].y;
								_tiles[i + 1].obj = objSelect(_ct[0].x, _ct[0].y);

								_tiles[i + TILEX].objFrameX = _ct[1].x;
								_tiles[i + TILEX].objFrameY = _ct[1].y;
								_tiles[i + TILEX].obj = objSelect(_ct[1].x, _ct[1].y);

								_tiles[i + TILEX + 1].objFrameX = _ct[2].x;
								_tiles[i + TILEX + 1].objFrameY = _ct[2].y;
								_tiles[i + TILEX + 1].obj = objSelect(_ct[2].x, _ct[2].y);
							}
							else if (_tiles->sizeX == 48)
							{
								_tiles[i].objFrameX = _currentTile.x;
								_tiles[i].objFrameY = _currentTile.y;
								_tiles[i].obj = objSelect(_currentTile.x, _currentTile.y);

								_tiles[i + 1].objFrameX = _ct[0].x;
								_tiles[i + 1].objFrameY = _ct[0].y;
								_tiles[i + 1].obj = objSelect(_ct[0].x, _ct[0].y);

								_tiles[i + 2].objFrameX = _ct[1].x;
								_tiles[i + 2].objFrameY = _ct[1].y;
								_tiles[i + 2].obj = objSelect(_ct[1].x, _ct[1].y);
							}
						}
						else if (_tiles->sizeX + _tiles->sizeY == 80)
						{
							if (_tiles->sizeX == 32)
							{
								_tiles[i].objFrameX = _currentTile.x;
								_tiles[i].objFrameY = _currentTile.y;
								_tiles[i].obj = objSelect(_currentTile.x, _currentTile.y);

								_tiles[i + 1].objFrameX = _ct[0].x;
								_tiles[i + 1].objFrameY = _ct[0].y;
								_tiles[i + 1].obj = objSelect(_ct[0].x, _ct[0].y);

								_tiles[i + TILEX].objFrameX = _ct[1].x;
								_tiles[i + TILEX].objFrameY = _ct[1].y;
								_tiles[i + TILEX].obj = objSelect(_ct[1].x, _ct[1].y);

								_tiles[i + TILEX + 1].objFrameX = _ct[2].x;
								_tiles[i + TILEX + 1].objFrameY = _ct[2].y;
								_tiles[i + TILEX + 1].obj = objSelect(_ct[2].x, _ct[2].y);

								_tiles[i + TILEX * 2].objFrameX = _ct[3].x;
								_tiles[i + TILEX * 2].objFrameY = _ct[3].y;
								_tiles[i + TILEX * 2].obj = objSelect(_ct[3].x, _ct[3].y);

								_tiles[i + TILEX * 2 + 1].objFrameX = _ct[4].x;
								_tiles[i + TILEX * 2 + 1].objFrameY = _ct[4].y;
								_tiles[i + TILEX * 2 + 1].obj = objSelect(_ct[4].x, _ct[4].y);
							}
							else if (_tiles->sizeX == 48)
							{
								_tiles[i].objFrameX = _currentTile.x;
								_tiles[i].objFrameY = _currentTile.y;
								_tiles[i].obj = objSelect(_currentTile.x, _currentTile.y);

								_tiles[i + 1].objFrameX = _ct[0].x;
								_tiles[i + 1].objFrameY = _ct[0].y;
								_tiles[i + 1].obj = objSelect(_ct[0].x, _ct[0].y);

								_tiles[i + 2].objFrameX = _ct[1].x;
								_tiles[i + 2].objFrameY = _ct[1].y;
								_tiles[i + 2].obj = objSelect(_ct[1].x, _ct[1].y);

								_tiles[i + TILEX].objFrameX = _ct[2].x;
								_tiles[i + TILEX].objFrameY = _ct[2].y;
								_tiles[i + TILEX].obj = objSelect(_ct[2].x, _ct[2].y);

								_tiles[i + TILEX + 1].objFrameX = _ct[3].x;
								_tiles[i + TILEX + 1].objFrameY = _ct[3].y;
								_tiles[i + TILEX + 1].obj = objSelect(_ct[3].x, _ct[3].y);

								_tiles[i + TILEX + 2].objFrameX = _ct[4].x;
								_tiles[i + TILEX + 2].objFrameY = _ct[4].y;
								_tiles[i + TILEX + 2].obj = objSelect(_ct[4].x, _ct[4].y);
							}
						}
						else if (_tiles->sizeX + _tiles->sizeY == 96)
						{
							_tiles[i].objFrameX = _currentTile.x;
							_tiles[i].objFrameY = _currentTile.y;
							_tiles[i].obj = objSelect(_currentTile.x, _currentTile.y);

							_tiles[i + 1].objFrameX = _ct[0].x;
							_tiles[i + 1].objFrameY = _ct[0].y;
							_tiles[i + 1].obj = objSelect(_ct[0].x, _ct[0].y);

							_tiles[i + 2].objFrameX = _ct[1].x;
							_tiles[i + 2].objFrameY = _ct[1].y;
							_tiles[i + 2].obj = objSelect(_ct[1].x, _ct[1].y);

							_tiles[i + TILEX].objFrameX = _ct[2].x;
							_tiles[i + TILEX].objFrameY = _ct[2].y;
							_tiles[i + TILEX].obj = objSelect(_ct[2].x, _ct[2].y);

							_tiles[i + TILEX + 1].objFrameX = _ct[3].x;
							_tiles[i + TILEX + 1].objFrameY = _ct[3].y;
							_tiles[i + TILEX + 1].obj = objSelect(_ct[3].x, _ct[3].y);

							_tiles[i + TILEX + 2].objFrameX = _ct[4].x;
							_tiles[i + TILEX + 2].objFrameY = _ct[4].y;
							_tiles[i + TILEX + 2].obj = objSelect(_ct[4].x, _ct[4].y);

							_tiles[i + TILEX * 2].objFrameX = _ct[5].x;
							_tiles[i + TILEX * 2].objFrameY = _ct[5].y;
							_tiles[i + TILEX * 2].obj = objSelect(_ct[5].x, _ct[5].y);

							_tiles[i + TILEX * 2 + 1].objFrameX = _ct[6].x;
							_tiles[i + TILEX * 2 + 1].objFrameY = _ct[6].y;
							_tiles[i + TILEX * 2 + 1].obj = objSelect(_ct[6].x, _ct[6].y);

							_tiles[i + TILEX * 2 + 2].objFrameX = _ct[7].x;
							_tiles[i + TILEX * 2 + 2].objFrameY = _ct[7].y;
							_tiles[i + TILEX * 2 + 2].obj = objSelect(_ct[7].x, _ct[7].y);
						}
						_ctrSelect = CTRL_OBJDRAW;
						break;

					}
				}
			}
		}

		if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
		{
			for (int i = 0; i < TILEX * TILEY; ++i)
			{
				if (PTIN_RECT(&_tiles[i].rc, _ptMouse) && !PTIN_RECT(&_tool.rc, _ptMouse))
				{
					_tiles[i].terrainFrameX = NULL;
					_tiles[i].terrainFrameY = NULL;
					_tiles[i].objFrameX = NULL;
					_tiles[i].objFrameY = NULL;

					_tiles[i].terrain = TR_END;
					_tiles[i].obj = OBJ_NONE;

					_ctrSelect = CTRL_ERASER;
					break;
				}
			}
		}

		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			if (PTIN_RECT(&_saveButton.rc, _ptMouse))
			{
				save();
			}
			if (PTIN_RECT(&_loadButton.rc, _ptMouse))
			{
				load();
			}
			if (PTIN_RECT(&_menuBtn.rc, _ptMouse))
			{
				_menu -= 1;
				if (_menu < (int)Terrain) _menu = (int)MENUEND - 1;
			}
			if (PTIN_RECT(&_menuBtn2.rc, _ptMouse))
			{
				_menu += 1;
				if (_menu >= (int)MENUEND) _menu = (int)Terrain;
			}
		}

		if (KEYMANAGER->isOnceKeyDown(VK_OEM_6))
		{
			_tiles->sizeX += 16;
			if (_tiles->sizeX > 48) _tiles->sizeX = 48;
			_sampleTile->sizeX = _sampleTile->sizeX / (_tiles->sizeX / 16);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_OEM_4))
		{
			_tiles->sizeX -= 16;
			if (_tiles->sizeX < 16) _tiles->sizeX = 16;
			_sampleTile->sizeX = _sampleTile->sizeX / (_tiles->sizeX / 16);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_OEM_PLUS))
		{
			_tiles->sizeY += 16;
			if (_tiles->sizeY > 48) _tiles->sizeY = 48;
			_sampleTile->sizeY = _sampleTile->sizeY / (_tiles->sizeY / 16);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_OEM_MINUS))
		{
			_tiles->sizeY -= 16;
			if (_tiles->sizeY < 16) _tiles->sizeY = 16;
			_sampleTile->sizeY = _sampleTile->sizeY / (_tiles->sizeY / 16);
		}
	}
	//위에는 샘플타일 선택했을때, 밑의 구문은 샘플타일을 타일에 깔때

	if (_tool.open)
	{
		if (_tool.rc.right >= CAMERAMANAGER->get_camera_x() + WINSIZEX)
		{
			_tool.rc.right -= 5;
			_tool.rc.left -= 5;
		}
		else
		{
			_tool.end = true;
		}
	}
	else
	{
		_tool.end = false;
		if (_tool.rc.left <= CAMERAMANAGER->get_camera_x() + WINSIZEX)
		{
			_tool.rc.right += 5;
			_tool.rc.left += 5;
		}
	}

	if (KEYMANAGER->isOnceKeyDown(VK_F2))
	{
		for (int i = 0; i < TILEX * TILEY; ++i)
		{
			_tiles[i].terrainFrameX = NULL;
			_tiles[i].terrainFrameY = NULL;
			_tiles[i].objFrameX = NULL;
			_tiles[i].objFrameY = NULL;
			_tiles[i].terrain = TR_END;
			_tiles[i].obj = OBJ_NONE;
		}
	}
	if (KEYMANAGER->isOnceKeyDown(VK_F3))
	{
		for (int i = 0; i < TILEX * TILEY; ++i)
		{
			_tiles[i].objFrameX = NULL;
			_tiles[i].objFrameY = NULL;
			_tiles[i].obj = OBJ_NONE;
		}
	}

	//===================
	//  렉트 위치 재정의
	//===================

	if (_tool.end)
	{
		_tool.rc = D2DMANAGER->RectMake(CAMERAMANAGER->get_camera_x() + WINSIZEX - _tool.img->getWidth(), CAMERAMANAGER->get_camera_y(), 300, WINSIZEY);
	}
	_menuBtn.rc = D2DMANAGER->RectMake(_tool.rc.left + 160, _tool.rc.top + 280, 50, 50);
	_menuBtn2.rc = D2DMANAGER->RectMake(_menuBtn.rc.left + 80, _tool.rc.top + 280, 50, 50);
	_saveButton.rc = D2DMANAGER->RectMake(_tool.rc.left + 10, _menuBtn.rc.top - 100, 128, 36);
	_loadButton.rc = D2DMANAGER->RectMake(_saveButton.rc.left + 155, _menuBtn2.rc.top - 100, 128, 36);
	_currentTileBox.rc = D2DMANAGER->RectMake(_tool.rc.left + 30, _menuBtn.rc.top - 40, 80, 100);

	for (int i = 0; i < SAMPLETILEY; ++i)
	{
		for (int j = 0; j < SAMPLETILEX; ++j)
		{
			_sampleTile[i * SAMPLETILEX + j].terrainFrameX = j;
			_sampleTile[i * SAMPLETILEX + j].terrainFrameY = i;

			_sampleTile[i * SAMPLETILEX + j].rcTile = D2DMANAGER->RectMake(
				(_tool.rc.left + 20) + j * 16,
				(_tool.rc.bottom - _terrain.img->getHeight() - 30) + i * 16,
				16,
				16);
		}
	}
	for (int i = 0; i < SAMPLETILEX * SAMPLETILEY; ++i)
	{
		if (PTIN_RECT(&_sampleTile[i].rcTile, _ptMouse))
		{
			_sampleRect = D2DMANAGER->RectMake(_sampleTile[i].rcTile.left, _sampleTile[i].rcTile.top, _tiles->sizeX, _tiles->sizeY);
		}
	}
}

void mapTool::render()
{

	for (int i = 0; i < TILEX * TILEY; ++i)
	{
		if (getDistance(CAMERAMANAGER->get_camera_x() + WINSIZEX / 2, (CAMERAMANAGER->get_camera_y() + WINSIZEY / 2) - 100, _tiles[i].rc.left, _tiles[i].rc.top) < 1000) {

			if (_tiles[i].terrain == TR_END) continue;

			if (_tiles[i].terrain == TR_GROUND)
			{
				_terrain.img->Frame_scale_start(_tiles[i].rc.left, _tiles[i].rc.top, 2.2f, 2.2f);
				_terrain.img->FrameRender(_tiles[i].rc.left + 4, _tiles[i].rc.top + 4, _tiles[i].terrainFrameX, _tiles[i].terrainFrameY);
				_terrain.img->scale_end();
			}
			if (_tiles[i].terrain == TR_WALL || _tiles[i].terrain == TR_NOT_WALL)
			{
				_wall.img->Frame_scale_start(_tiles[i].rc.left, _tiles[i].rc.top, 2.2f, 2.2f);
				_wall.img->FrameRender(_tiles[i].rc.left + 4, _tiles[i].rc.top + 4, _tiles[i].terrainFrameX, _tiles[i].terrainFrameY);
				_wall.img->scale_end();
			}
			if (_tiles[i].terrain == TR_GROUND2 || _tiles[i].terrain == TR_WALL2)
			{
				_terrain2.img->Frame_scale_start(_tiles[i].rc.left, _tiles[i].rc.top, 2.1f, 2.1f);
				_terrain2.img->FrameRender(_tiles[i].rc.left + 4, _tiles[i].rc.top + 4, _tiles[i].terrainFrameX, _tiles[i].terrainFrameY);
				_terrain2.img->scale_end();
			}
			if (_tool.open) {
				if (_tiles[i].terrain == TR_WALL2 || _tiles[i].terrain == TR_WALL)
				{
					D2DMANAGER->Rectangle(D2DMANAGER->CreateBrush(RGB(255, 0, 0)), _tiles[i].rc);
				}
			}
		}
		//if (_tiles[i].terrain == TR_WALL2)
		//{
		//	D2DMANAGER->pRenderTarget->FillRectangle(_tiles[i].rc, D2DMANAGER->CreateBrush(RGB(255, 0, 0)));
		//	D2DMANAGER->Rectangle(_tiles[i].rc);
		//}
	}
	for (int i = 0; i < TILEX * TILEY; ++i)
	{
		if (_tiles[i].obj == OBJ_NONE) continue;

		if (getDistance(CAMERAMANAGER->get_camera_x() + WINSIZEX / 2, (CAMERAMANAGER->get_camera_y() + WINSIZEY / 2) - 100, _tiles[i].rc.left, _tiles[i].rc.top) < 1000) {

			if (_tiles[i].obj == OBJ || _tiles[i].obj == OBJ_STRUCT)
			{
				_object.img->Frame_scale_start(_tiles[i].rc.left, _tiles[i].rc.top, 2.2f, 2.2f);
				_object.img->FrameRender(_tiles[i].rc.left + 4, _tiles[i].rc.top + 4, _tiles[i].objFrameX, _tiles[i].objFrameY);
				_object.img->scale_end();
			}
			if (_tiles[i].obj == OBJ2 || _tiles[i].obj == OBJ_STRUCT2)
			{
				_object2.img->Frame_scale_start(_tiles[i].rc.left, _tiles[i].rc.top, 2.2f, 2.2f);
				_object2.img->FrameRender(_tiles[i].rc.left + 4, _tiles[i].rc.top + 4, _tiles[i].objFrameX, _tiles[i].objFrameY);
				_object2.img->scale_end();
			}
			if (_tiles[i].obj == OBJ3 || _tiles[i].obj == OBJ_STRUCT3)
			{
				_object3.img->Frame_scale_start(_tiles[i].rc.left, _tiles[i].rc.top, 2.1f, 2.1f);
				_object3.img->FrameRender(_tiles[i].rc.left + 4, _tiles[i].rc.top + 4, _tiles[i].objFrameX, _tiles[i].objFrameY);
				_object3.img->scale_end();

			}
			if (_tool.open)
			{
				if (_tiles[i].obj == OBJ_STRUCT3 || _tiles[i].obj == OBJ_STRUCT2 || _tiles[i].obj == OBJ_STRUCT)
				{
					D2DMANAGER->Rectangle(D2DMANAGER->CreateBrush(RGB(255, 0, 0)), _tiles[i].rc);
				}
			}
		}
	}



	if (_tool.open)
	{
		_tool.img->render(_tool.rc.left + 5, _tool.rc.top);
		_menuBtn.img->render(_menuBtn.rc.left, _menuBtn.rc.top);
		_menuBtn2.img->render(_menuBtn2.rc.left, _menuBtn2.rc.top);
		_saveButton.img->render(_saveButton.rc.left, _saveButton.rc.top);
		_loadButton.img->render(_loadButton.rc.left, _loadButton.rc.top);
		_currentTileBox.img->render(_currentTileBox.rc.left, _currentTileBox.rc.top);

		switch (_menu)
		{
		case Terrain:
			_terrain.img->render(_tool.rc.left + 20, _tool.rc.bottom - _terrain.img->getHeight() - 30);
			_terrain.img->Frame_scale_start(_currentTileBox.rc.left + 32, _currentTileBox.rc.top + 48, 4, 4);
			_terrain.img->FrameRender(_currentTileBox.rc.left + 32, _currentTileBox.rc.top + 50, _currentTile.x, _currentTile.y);
			_terrain.img->scale_end();
			break;
		case Wall:
			_wall.img->render(_tool.rc.left + 20, _tool.rc.bottom - _terrain.img->getHeight() - 30);
			_wall.img->Frame_scale_start(_currentTileBox.rc.left + 32, _currentTileBox.rc.top + 48, 4, 4);
			_wall.img->FrameRender(_currentTileBox.rc.left + 32, _currentTileBox.rc.top + 50, _currentTile.x, _currentTile.y);
			_wall.img->scale_end();

			break;
		case Object:
			_object.img->render(_tool.rc.left + 20, _tool.rc.bottom - _terrain.img->getHeight() - 30);
			_object.img->Frame_scale_start(_currentTileBox.rc.left + 32, _currentTileBox.rc.top + 48, 4, 4);
			_object.img->FrameRender(_currentTileBox.rc.left + 32, _currentTileBox.rc.top + 50, _currentTile.x, _currentTile.y);
			_object.img->scale_end();
			break;
		case Object2:
			_object2.img->render(_tool.rc.left + 20, _tool.rc.bottom - _terrain.img->getHeight() - 30);
			_object2.img->Frame_scale_start(_currentTileBox.rc.left + 32, _currentTileBox.rc.top + 48, 4, 4);
			_object2.img->FrameRender(_currentTileBox.rc.left + 32, _currentTileBox.rc.top + 50, _currentTile.x, _currentTile.y);
			_object2.img->scale_end();
			break;
		case Terrain2:
			_terrain2.img->render(_tool.rc.left + 20, _tool.rc.bottom - _terrain.img->getHeight() - 30);
			_terrain2.img->Frame_scale_start(_currentTileBox.rc.left + 32, _currentTileBox.rc.top + 48, 4, 4);
			_terrain2.img->FrameRender(_currentTileBox.rc.left + 32, _currentTileBox.rc.top + 50, _currentTile.x, _currentTile.y);
			_terrain2.img->scale_end();
			break;
		case Object3:
			_object3.img->render(_tool.rc.left + 20, _tool.rc.bottom - _terrain.img->getHeight() - 30);
			_object3.img->Frame_scale_start(_currentTileBox.rc.left + 32, _currentTileBox.rc.top + 48, 4, 4);
			_object3.img->FrameRender(_currentTileBox.rc.left + 32, _currentTileBox.rc.top + 50, _currentTile.x, _currentTile.y);
			_object3.img->scale_end();
			break;
		}

		for (int i = 0; i < SAMPLETILEY; ++i)
		{
			for (int j = 0; j < SAMPLETILEX; ++j)
			{
				{
					D2DMANAGER->Rectangle(_sampleTile[i * SAMPLETILEX + j].rcTile);
				}
			}
		}

		D2DMANAGER->Rectangle(D2DMANAGER->CreateBrush(RGB(0, 0, 255)), _sampleRect);
	}
}

void mapTool::setup()
{
	//타일셋 먼저 세팅
	for (int i = 0; i < SAMPLETILEY; ++i)
	{
		for (int j = 0; j < SAMPLETILEX; ++j)
		{
			_sampleTile[i * SAMPLETILEX + j].terrainFrameX = j;
			_sampleTile[i * SAMPLETILEX + j].terrainFrameY = i;

			//타일셋에 렉트를 씌움
			_sampleTile[i * SAMPLETILEX + j].rcTile = D2DMANAGER->RectMake(
				(_tool.rc.left + 20) + j * 16,
				(_tool.rc.bottom - _terrain.img->getHeight() - 30) + i * 16,
				16,
				16);
		}
	}

	//타일 영역
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			_tiles[i * TILEX + j].rc = D2DMANAGER->RectMake(
				j * TILESIZE,
				i * TILESIZE,
				TILESIZE,
				TILESIZE);
		}
	}

	for (int i = 0; i < TILEX * TILEY; ++i)
	{
		_tiles[i].terrainFrameX = 0;
		_tiles[i].terrainFrameY = 10;
		_tiles[i].objFrameX = 0;
		_tiles[i].objFrameY = 0;
		_tiles[i].terrain = terrainSelect(_tiles[i].terrainFrameX, _tiles[i].terrainFrameY);
		_tiles[i].obj = OBJ_NONE;
	}
}

void mapTool::save()
{
	HANDLE file;
	DWORD write;

	file = CreateFile("1.map", GENERIC_WRITE, NULL, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &write, NULL);

	CloseHandle(file);
}

void mapTool::load()
{
	HANDLE file;
	DWORD read;

	file = CreateFile("1.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	ReadFile(file, _pos, sizeof(int) * 2, &read, NULL);

	memset(_attribute, 0, sizeof(DWORD) * TILEX * TILEY);
	memset(_pos, 0, sizeof(int) * 2);

	CloseHandle(file);


	for (int i = 0; i < TILEX * TILEY; ++i)
	{
		if (_tiles[i].terrain == TR_WALL) _attribute[i] |= ATTR_UNMOVE;
		if (_tiles[i].obj == OBJ_STRUCT) _attribute[i] |= ATTR_UNMOVE;
		if (_tiles[i].obj == OBJ_STRUCT2) _attribute[i] |= ATTR_UNMOVE;
		if (_tiles[i].obj == OBJ_WEB) _attribute[i] |= ATTR_SLOW;
		if (_tiles[i].obj == OBJ_STRUCT3) _attribute[i] |= ATTR_UNMOVE;
		if (_tiles[i].terrain == TR_WALL2) _attribute[i] |= ATTR_UNMOVE;
	}

}

void mapTool::load(int num)
{
	HANDLE file;
	DWORD read;
	switch (num)
	{
	case 0:
		file = CreateFile("village.map", GENERIC_READ, NULL, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
		ReadFile(file, _pos, sizeof(int) * 2, &read, NULL);

		memset(_attribute, 0, sizeof(DWORD) * TILEX * TILEY);
		memset(_pos, 0, sizeof(int) * 2);

		CloseHandle(file);
		break;
	case 1:
		file = CreateFile("dungeon.map", GENERIC_READ, NULL, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
		ReadFile(file, _pos, sizeof(int) * 2, &read, NULL);

		memset(_attribute, 0, sizeof(DWORD) * TILEX * TILEY);
		memset(_pos, 0, sizeof(int) * 2);

		CloseHandle(file);
		break;
	case 2:
		file = CreateFile("dungeon.map", GENERIC_READ, NULL, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
		ReadFile(file, _pos, sizeof(int) * 2, &read, NULL);

		memset(_attribute, 0, sizeof(DWORD) * TILEX * TILEY);
		memset(_pos, 0, sizeof(int) * 2);

		CloseHandle(file);
		break;
	default:
		break;
	}

	for (int i = 0; i < TILEX * TILEY; ++i)
	{
		if (_tiles[i].terrain == TR_WALL) _attribute[i] |= ATTR_UNMOVE;
		if (_tiles[i].obj == OBJ_STRUCT) _attribute[i] |= ATTR_UNMOVE;
		if (_tiles[i].obj == OBJ_STRUCT2) _attribute[i] |= ATTR_UNMOVE;
		if (_tiles[i].obj == OBJ_WEB) _attribute[i] |= ATTR_SLOW;
		if (_tiles[i].obj == OBJ_STRUCT3) _attribute[i] |= ATTR_UNMOVE;
		if (_tiles[i].terrain == TR_WALL2) _attribute[i] |= ATTR_UNMOVE;
	}
}

TERRAIN mapTool::terrainSelect(int frameX, int frameY)
{
	switch (_menu)
	{
	case Terrain:
		return TR_GROUND;
		break;
	case Wall:
		if (frameY == 22 && frameX <= 3)
		{
			return TR_NOT_WALL;
		}
		else if (frameY >= 22 && frameY <= 24 && frameX > 3)
		{
			return TR_NOT_WALL;
		}
		else if (frameY >= 25 && frameY <= 26 && frameX >= 8 && frameX <= 9)
		{
			return TR_NOT_WALL;
		}
		else if (frameY >= 25 && frameY <= 26 && frameX >= 14 && frameX <= 15)
		{
			return TR_NOT_WALL;
		}
		else if (frameY >= 25 && frameY <= 26 && frameX >= 14 && frameX <= 15)
		{
			return TR_NOT_WALL;
		}
		else if (frameY == 27 && frameX >= 4)
		{
			return TR_NOT_WALL;
		}
		else if (frameY >= 28 && frameY <= 29 && frameX == 15)
		{
			return TR_NOT_WALL;
		}
		else if (frameY >= 28 && frameY <= 29 && frameX == 12)
		{
			return TR_NOT_WALL;
		}

		return TR_WALL;
		break;
	case Terrain2:
		if (frameX >= 0 && frameX <= 3 && frameY >= 30)
		{
			return TR_GROUND2;
		}
		return TR_WALL2;
		break;
	}
}

OBJECT mapTool::objSelect(int frameX, int frameY)
{
	switch (_menu)
	{
	case Object:
		if (frameX >= 0 && frameX <= 3 && frameY >= 4 && frameY <= 7)
		{
			return OBJ_STRUCT;
		}
		else if (frameX >= 8 && frameX <= 10 && frameY == 6)
		{
			return OBJ_WEB;
		}
		else if (frameX >= 8 && frameY >= 7 && frameY <= 20)
		{
			return OBJ_WEB;
		}

		return OBJ;
		break;
	case Object2:
		if (frameX >= 0 && frameX <= 8 && frameY >= 4 && frameY <= 6)
		{
			return OBJ_STRUCT2;
		}
		else if (frameX >= 0 && frameX <= 8 && frameY >= 10 && frameY <= 11)
		{
			return OBJ_STRUCT2;
		}
		else if (frameX >= 0 && frameX <= 3 && frameY >= 12)
		{
			return OBJ_STRUCT2;
		}

		return OBJ2;
		break;
	case Object3:
		if (frameX >= 1 && frameX <= 2 && frameY >= 2 && frameY <= 3)
		{
			return OBJ3;
		}
		if (frameX >= 12 && frameY >= 15 && frameY <= 24)
		{
			return OBJ3;
		}
		if (frameX >= 0 && frameY >= 23)
		{
			return OBJ3;
		}
		if (frameX >= 2 && frameX <= 3 && frameY >= 10 && frameY <= 11)
		{
			return OBJ3;
		}
		if (frameX >= 5 && frameX <= 6 && frameY >= 12 && frameY <= 14)
		{
			return OBJ3;
		}

		return OBJ_STRUCT3;
		break;
	}

}
