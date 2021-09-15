#include "stdafx.h"
#include "tile.h"

//멤버 이니셜라이즈
tile::tile()
	: _totalCost(0), _costFromStart(0),
	_costToGoal(0), _parentNode(NULL),
	_idX(0), _idY(0), _isOpen(true)
{
}

tile::~tile()
{

}

HRESULT tile::init(int idX, int idY)
{
	_center = PointMake(idX * TILESIZE + (TILESIZE / 2),
		idY * TILESIZE + (TILESIZE / 2));

	_idX = idX;
	_idY = idY;
	_index.current_frameX = _index.current_frameY = 0;
	_index.object_frameX = _index.object_frameX = 0;
	current_tilePage = -1;
	current_objectPage = -1;

	rc = D2DMANAGER->RectMakeCenter(_center.x, _center.y, TILESIZE, TILESIZE);

	swprintf_s(str, L"(%d,%d)", _idX, _idY);

	_img = NULL;
	_objectImg = NULL;

	_count = 0;

	return S_OK;
}

void tile::release()
{

}

void tile::update()
{
	if (_attribute == tile_Attribute::WATER_ROW || _attribute == tile_Attribute::WATER_COL)
	{
		_count += TIMEMANAGER->getElapsedTime();
	}

	if (_attribute == tile_Attribute::WATER_ROW && _count > 0.1f)
	{
		_index.current_frameX += 1;
		_count = 0;
	}
	if (_attribute == tile_Attribute::WATER_COL && _count > 0.1f)
	{
		_index.current_frameY -= 1;
		_count = 0;
	}

	if (_attribute == tile_Attribute::WATER_ROW && _index.current_frameX > 14)
	{
		_index.current_frameX = 0;
	}

	if (_attribute == tile_Attribute::WATER_COL && _index.current_frameY < 9)
	{
		_index.current_frameY = 14;
	}
}

void tile::render()
{

	if (KEYMANAGER->isToggleKey(VK_TAB))
	{
		//D2DMANAGER->pRenderTarget->DrawRectangle(rc, D2DMANAGER->_mybrush);
		D2DMANAGER->Rectangle(D2DMANAGER->_mybrush, rc);
		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->_mybrush, L"메이플스토리", 8.0f, str, rc.left, rc.top + 4, rc.left + 40, rc.top + 40);
	}

	if (_img != NULL)
	{
		//_img->Frame_scale_start(rc.left, rc.top, 1.5f, 1.5f);
		_img->FrameRender(rc.left, rc.top, _index.current_frameX, _index.current_frameY);
		//_img->scale_end();
	}

	if (_objectImg != NULL)
	{
		//_objectImg->Frame_scale_start(rc.left, rc.top, 1.5f, 1.5f);
		_objectImg->FrameRender(rc.left, rc.top, _index.object_frameX, _index.object_frameY);
		//_objectImg->scale_end();
	}
}
