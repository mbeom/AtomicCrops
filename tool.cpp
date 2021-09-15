#include "stdafx.h"
#include "tool.h"

HRESULT tool::init()
{
	ShowCursor(true);

	_tiles.clear();

	_tileNumX = TILEX;
	_tileNumY = TILEY;
	_tiles.reserve(_tileNumX * _tileNumY);

	// 화면에 타일 80 x 60 (3520 x 2640) 으로 깔아줍니다.
	for (int i = 0; i < _tileNumY; i++)
	{
		for (int j = 0; j < _tileNumX; j++)
		{
			tile* temp_tile = new tile;
			temp_tile->init(j, i);

			_tiles.push_back(temp_tile);
		}
	}

	// 타일셋 이미지 갯수만큼 이미지 등록
	for (int i = 0; i < PAGE_NUM; i++)
	{
		WCHAR str[128];

		if (!_sampleImage[i])
		{
			swprintf_s(str, L"image/tile/sampleTile%d.png", i);

			_sampleImage[i] = new image;
			_sampleImage[i]->D_Init(str, 330, 330);
		}

		if (!_tileImage[i])
		{
			swprintf_s(str, L"image/tile/tileSet%d.png", i);

			_tileImage[i] = new image;
			_tileImage[i]->D_Init(str, 1320, 1320, 30, 30);
		}
	}

	IMAGEMANAGER->addFrameImage("tileset", L"image/tile/tileset0.png", 1320, 1320, 30, 30);
	IMAGEMANAGER->addFrameImage("fertile_soil", L"image/tile/Soil.png", 88, 88, 2, 2);
	IMAGEMANAGER->addFrameImage("soil_overflow_top", L"image/tile/soil_overflow_top.png", 44, 44, 1, 1);
	IMAGEMANAGER->addFrameImage("soil_overflow_left", L"image/tile/soil_overflow_left.png", 44, 44, 1, 1);
	IMAGEMANAGER->addFrameImage("soil_overflow_right", L"image/tile/soil_overflow_right.png", 44, 44, 1, 1);
	IMAGEMANAGER->addFrameImage("soil_overflow_bottom", L"image/tile/soil_overflow_bottom.png", 44, 44, 1, 1);

	// 현재 타일셋 이미지 설정
	_currentSample = _sampleImage[0];
	_page = TILE;

	// UI 박스 생성

	box[0] = D2DMANAGER->RectMake(WINSIZEX - 340, WINSIZEY - 640, 340, 640);
	box[1] = D2DMANAGER->RectMakeCenter((box[0].left + box[0].right) / 2, box[0].top + 100, 66, 66);
	box[2] = D2DMANAGER->RectMakeCenter((box[0].left + box[0].right) / 2 - 50, box[0].top + 250, 160, 160);

	if (!_buttonImage[0])
	{
		_buttonImage[0] = new image;
		_buttonImage[0]->D_Init(L"image/Rect.png", 340, 640);
	}
	if (!_buttonImage[1])
	{
		_buttonImage[1] = new image;
		_buttonImage[1] = _tileImage[0];
	}
	if (!_buttonImage[2])
	{
		_buttonImage[2] = new image;
		_buttonImage[2]->D_Init(L"image/button_left.png", 60, 20);
	}
	if (!_buttonImage[3])
	{
		_buttonImage[3] = new image;
		_buttonImage[3]->D_Init(L"image/button_right.png", 60, 20);
	}
	if (!_buttonImage[4])
	{
		_buttonImage[4] = new image;
		_buttonImage[4]->D_Init(L"image/button.png", 59, 19);
	}
	if (!_buttonImage[5])
	{
		_buttonImage[5] = new image;
		_buttonImage[5]->D_Init(L"image/button.png", 59, 19);
	}

	if (!_numboxImage[0])
	{
		_numboxImage[0] = new image;
		_numboxImage[0]->D_Init(L"image/button.png", 22, 22);
	}
	if (!_numboxImage[1])
	{
		_numboxImage[1] = new image;
		_numboxImage[1]->D_Init(L"image/button.png", 22, 22);
	}
	if (!_numboxImage[2])
	{
		_numboxImage[2] = new image;
		_numboxImage[2]->D_Init(L"image/button.png", 22, 22);
	}
	if (!_numboxImage[3])
	{
		_numboxImage[3] = new image;
		_numboxImage[3]->D_Init(L"image/button_up.png", 12, 12);
	}
	if (!_numboxImage[4])
	{
		_numboxImage[4] = new image;
		_numboxImage[4]->D_Init(L"image/button_down.png", 12, 12);
	}
	if (!_numboxImage[5])
	{
		_numboxImage[5] = new image;
		_numboxImage[5]->D_Init(L"image/button_up.png", 12, 12);
	}
	if (!_numboxImage[6])
	{
		_numboxImage[6] = new image;
		_numboxImage[6]->D_Init(L"image/button_down.png", 12, 12);
	}
	if (!_numboxImage[7])
	{
		_numboxImage[7] = new image;
		_numboxImage[7]->D_Init(L"image/button.png", 60, 20);
	}

	_sampleTiles.clear();
	_sampleTiles.reserve(900);

	// UI 박스 안 샘플 타일 RECT 생성
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			float start_x = box[0].left + 10;
			float start_y = (box[0].top + box[0].bottom) / 2 - 66;
			D2D1_RECT_F temp = D2DMANAGER->RectMake(start_x + j * 11, start_y + i * 11, 11, 11);
			_sampleTiles.push_back(temp);
		}
	}

	target = D2DMANAGER->RectMakeCenter(x, y, TILESIZE, TILESIZE);
	D2DMANAGER->pRenderTarget->CreateSolidColorBrush(ColorF(1.0f, 0.0f, 0.0f), &red_brush);
	D2DMANAGER->pRenderTarget->CreateSolidColorBrush(ColorF(0.0f, 0.0f, 1.0f, 0.3f), &blue_brush);
	D2DMANAGER->pRenderTarget->CreateSolidColorBrush(ColorF(1.0f, 1.0f, 1.0f), &white_brush);

	_count = 0;
	_row = _col = 0;
	current_ratio = 1;
	current_frameX = current_frameY = 0;
	current_tileX = _tileNumX;
	current_tileY = _tileNumY;
	x = y = point_x = point_y = 0;

	LButton_down = LButton_up = false;
	mouse_on_board = false;
	start_drag = end_drag = false;
	is_full = false;
	erase = false;

	return S_OK;
}

void tool::release()
{

	red_brush->Release();
	blue_brush->Release();
	white_brush->Release();

	_currentSample->release();

}

void tool::update()
{
	// cout << " test";
	// 버튼 클릭시 화면 이동 및 마우스 클릭버튼 설정
	move_viewpoint();

	int _tileX = _ptMouse.x / TILESIZE;
	int _tileY = _ptMouse.y / TILESIZE;

	if (_tileX >= _tileNumX) _tileX = _tileNumX;
	if (_tileY >= _tileNumY) _tileY = _tileNumY;

	if (_tileX >= 0 && _tileY >= 0 && _tileX < _tileNumX && _tileY < _tileNumY)
	{
		target = _tiles[_tileX + _tileY * _tileNumX]->getRectangle();
	}

	CAMERAMANAGER->updateScreen(x, y);

	/************************************************************ UI 화면 위치잡기 ********************************************************************************/

	box[0] = D2DMANAGER->RectMake(WINSIZEX - 340 + CAMERAMANAGER->get_camera_x(), WINSIZEY - 640 + CAMERAMANAGER->get_camera_y(), 340, 640);
	box[1] = D2DMANAGER->RectMakeCenter((box[0].left + box[0].right) / 2 , box[0].top + 100, 66, 66);
	box[2] = D2DMANAGER->RectMakeCenter((box[0].left + box[0].right) / 2 - 100, box[0].top + 225, 60, 30);
	box[3] = D2DMANAGER->RectMakeCenter((box[0].left + box[0].right) / 2 + 100, box[0].top + 225, 60, 30);
	box[4] = D2DMANAGER->RectMakeCenter((box[0].left + box[0].right) / 2 - 100, box[0].top + 175, 60, 30);
	box[5] = D2DMANAGER->RectMakeCenter((box[0].left + box[0].right) / 2 + 100, box[0].top + 175, 60, 30);

	num_box[0] = D2DMANAGER->RectMakeCenter((box[0].left + box[0].right) / 2 + 80, box[0].top + 50, 22, 22);
	num_box[1] = D2DMANAGER->RectMakeCenter((box[0].left + box[0].right) / 2 + 102, box[0].top + 50, 22, 22);
	num_box[2] = D2DMANAGER->RectMakeCenter((box[0].left + box[0].right) / 2 + 124, box[0].top + 50, 22, 22);

	num_box[3] = D2DMANAGER->RectMakeCenter((box[0].left + box[0].right) / 2 + 120, box[0].top + 85, 20, 20);
	num_box[4] = D2DMANAGER->RectMakeCenter((box[0].left + box[0].right) / 2 + 140, box[0].top + 85, 20, 20);
	num_box[5] = D2DMANAGER->RectMakeCenter((box[0].left + box[0].right) / 2 + 120, box[0].top + 105, 20, 20);
	num_box[6] = D2DMANAGER->RectMakeCenter((box[0].left + box[0].right) / 2 + 140, box[0].top + 105, 20, 20);

	num_box[7] = D2DMANAGER->RectMakeCenter((box[0].left + box[0].right) / 2 + 100, box[0].top + 135, 60, 20);

	box_click();

	if (PTIN_RECT(&box[0], _ptMouse))
	{
		mouse_on_board = true;
	}
	else
		mouse_on_board = false;

	// 샘플 이미지 칸 나누기
	for (int i = 0; i < 30; i++)
	{
		if (end_drag) break;

		for (int j = 0; j < 30; j++)
		{
			float start_x = box[0].left + 10;
			float start_y = (box[0].top + box[0].bottom) / 2 - 66;
			_sampleTiles[j + i*30] = D2DMANAGER->RectMake(start_x + j * 11, start_y + i * 11, 11, 11);

			// 샘플 RECT에 마우스가 있다면
			if (PTIN_RECT(&_sampleTiles[j + i * 30], _ptMouse))
			{
				// 왼쪽 버튼 클릭시
				if (LButton_down && _count == 1)
				{
					point_x = _ptMouse.x;
					point_y = _ptMouse.y;

					current_frameX = j;
					current_frameY = i;

					start_drag = true;
				}// 왼쪽 버튼 꾹 누를시 (isStayKeyDown) -> 파란상자 드래그해서 키우는중
				else if (LButton_down && _count > 1 && start_drag)
				{
					delete drag_and_drop;
					drag_and_drop = new D2D1_RECT_F(D2DMANAGER->RectMake(point_x, point_y, _ptMouse.x - point_x, _ptMouse.y - point_y));
				}
			}
			// 마우스 왼쪽 버튼 클릭 해제시
			if (LButton_up && drag_and_drop != NULL)
			{
				start_drag = false;
				end_drag = true;
			}
		}
	}

	// 원하는 크기만큼 복사할 사이즈가 정해졌다면 -> 파란색 상자 완성(왼쪽버튼 뗴서)
	if (end_drag)
	{
		int temp_x = -1;
		int temp_y = -1;

		D2D1_RECT_F drag = *drag_and_drop;

		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				D2D1_RECT_F tile = _sampleTiles[j + i * 30];
				// 파란색 상자와 충돌한 샘플 RECT들은 해당 인덱스를 벡터에 저장
				if (INTERSECTION_RECT(temp, drag, tile) && !is_full)
				{
					if (temp_x < j)
					{
						temp_x = j;
						_row += 1;
					}
					if (temp_y < i)
					{
						temp_y = i;
						_col += 1;
					}

					Sample_Data _data;
					_data.index = { j, i };
					_data.sample_img = _tileImage[_page];
					_IndexPool.push_back(_data);
					/*
					POINT tile_index = { j, i };
					_IndexPool.push_back(tile_index);
					*/
				}
			}
		}

		end_drag = false;
		is_full = true;

		delete drag_and_drop;
		drag_and_drop = NULL;
	}

	// 화면의 타일과 마우스 클릭시 타일 이미지 입히기
	for (int i = 0; i < _tileNumY; i++)
	{
		for (int j = 0; j < _tileNumX; j++)
		{
			_tiles[j + i * _tileNumX]->update();

			if (PTIN_RECT(&_tiles[j + i * _tileNumX]->getRectangle(), _ptMouse))
			{
				if (LButton_down && !mouse_on_board)
				{
					set_tiles(j, i);
				}
			}
		}
	}
	if (LButton_up && mouse_on_board)
	{
		mouse_on_board = false;
	}

	if (LButton_down)
	{
		_count += 1;
	}
	
	if (LButton_up)
	{
		_count = 0;
		LButton_up = false;
	}

	if (KEYMANAGER->isOnceKeyDown(VK_ESCAPE))
	{
		SCENEMANAGER->changeScene("title");
	}
}

void tool::render()
{
	// 필드 TILE 렌더
	for (int i = 0; i < _tileNumY; i++)
	{
		for (int j = 0; j < _tileNumX; j++)
		{
			_tiles[j + i * _tileNumX]->render();
		}
	}

	/***************************************토글키로 렌더 안할부분**************************************************/

	D2DMANAGER->Rectangle(red_brush, target);

	_buttonImage[0]->render(box[0].left, box[0].top);
	_currentSample->render(box[0].left + 10, (box[0].top + box[0].bottom) / 2 - 66);

	D2DMANAGER->Rectangle(box[1]);
	_buttonImage[1]->FrameRender(box[1].left, box[1].top, current_frameX, current_frameY, 66, 66);

	for (int i = 2; i < 6; i++)
	{
		_buttonImage[i]->render(box[i].left, box[i].top);
	}

	D2DMANAGER->DrawTextD2DCenter(white_brush, L"메이플스토리", 12.0f, L"Current Tile", box[1].left - 35, box[1].top - 20, box[1].left + 100, box[1].top + 40);
	D2DMANAGER->DrawTextD2DCenter(white_brush, L"메이플스토리", 12.0f, L"이전", box[2].left - 40, box[2].top, box[2].left + 100, box[2].top + 40);
	D2DMANAGER->DrawTextD2DCenter(white_brush, L"메이플스토리", 12.0f, L"다음", box[3].left - 40, box[3].top, box[3].left + 100, box[3].top + 40);
	D2DMANAGER->DrawTextD2DCenter(white_brush, L"메이플스토리", 12.0f, L"Save", box[4].left - 40, box[4].top, box[4].left + 100, box[4].top + 40);
	D2DMANAGER->DrawTextD2DCenter(white_brush, L"메이플스토리", 12.0f, L"Load", box[5].left - 40, box[5].top, box[5].left + 100, box[5].top + 40);

	for (int i = 0; i < 8; i++)
	{
		if (i >= 3 && i <= 6)
		{
			_numboxImage[i]->render(num_box[i].left, num_box[i].top + 4);
		}
		else
			_numboxImage[i]->render(num_box[i].left, num_box[i].top);
	}

	D2DMANAGER->DrawTextD2DCenter(white_brush, L"메이플스토리", 12.0f, L"1", num_box[0].left - 80, num_box[0].top + 5, num_box[0].left + 100, num_box[0].top + 40);
	D2DMANAGER->DrawTextD2DCenter(white_brush, L"메이플스토리", 12.0f, L"5", num_box[1].left - 80, num_box[1].top + 5, num_box[1].left + 100, num_box[1].top + 40);
	D2DMANAGER->DrawTextD2DCenter(white_brush, L"메이플스토리", 12.0f, L"10", num_box[2].left - 80, num_box[2].top + 5, num_box[2].left + 100, num_box[2].top + 40);
	
	WCHAR str[128];
	swprintf_s(str, L"Tile_X:  %d", current_tileX);
	D2DMANAGER->DrawTextD2DCenter(white_brush, L"메이플스토리", 12.0f, str, num_box[0].left - 100, num_box[0].top + 60, num_box[0].left + 100, num_box[0].top + 40);
	swprintf_s(str, L"Tile_Y:  %d", current_tileY);
	D2DMANAGER->DrawTextD2DCenter(white_brush, L"메이플스토리", 12.0f, str, num_box[0].left - 100, num_box[0].top + 80, num_box[0].left + 100, num_box[0].top + 60);
	D2DMANAGER->DrawTextD2DCenter(white_brush, L"메이플스토리", 12.0f, L"Create", num_box[7].left - 40, num_box[7].top + 4, num_box[7].left + 100, num_box[7].top + 40);

	// 샘플 이미지 사각형 칸으로 나눠지게 보이도록 RECT 렌더
	for (int i = 0; i < _sampleTiles.size(); i++)
	{
		D2DMANAGER->Rectangle(_sampleTiles[i]);
	}

	if (drag_and_drop != NULL)
	{
		D2D1_RECT_F temp = D2D1::RectF(drag_and_drop->left - CAMERAMANAGER->get_camera_x(), drag_and_drop->top - CAMERAMANAGER->get_camera_y(),
			drag_and_drop->right - CAMERAMANAGER->get_camera_x(), drag_and_drop->bottom - CAMERAMANAGER->get_camera_y());
		D2DMANAGER->pRenderTarget->FillRectangle(temp, blue_brush);
	}
	switch (_page)
	{
	case TILE:
		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->defaultBrush, L"메이플스토리", 12.0f, L"TILE", box[1].left - 35, box[1].bottom + 80, box[1].left + 100, box[1].bottom + 100);
		break;
	case TILE_EDGE:
		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->defaultBrush, L"메이플스토리", 12.0f, L"TILE EDGE", box[1].left - 35, box[1].bottom + 80, box[1].left + 100, box[1].bottom + 100);
		break;
	case TILE_EDGE2:
		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->defaultBrush, L"메이플스토리", 12.0f, L"TILE EDGE2", box[1].left - 35, box[1].bottom + 80, box[1].left + 100, box[1].bottom + 100);
		break;
	case TILE_EDGE3:
		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->defaultBrush, L"메이플스토리", 12.0f, L"TILE EDGE3", box[1].left - 35, box[1].bottom + 80, box[1].left + 100, box[1].bottom + 100);
		break;
	case OBJECT_TILE:
		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->defaultBrush, L"메이플스토리", 12.0f, L"Object Tile", box[1].left - 35, box[1].bottom + 80, box[1].left + 100, box[1].bottom + 100);
		break;
	case OBJECT_EDGE:
		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->defaultBrush, L"메이플스토리", 12.0f, L"Object Edge", box[1].left - 35, box[1].bottom + 80, box[1].left + 100, box[1].bottom + 100);
		break;
	case OBJECT_EDGE2:
		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->defaultBrush, L"메이플스토리", 12.0f, L"Object Edge2", box[1].left - 35, box[1].bottom + 80, box[1].left + 100, box[1].bottom + 100);
		break;
	case OBJECT_EDGE3:
		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->defaultBrush, L"메이플스토리", 12.0f, L"Object Edge3", box[1].left - 35, box[1].bottom + 80, box[1].left + 100, box[1].bottom + 100);
		break;
	case OBJECT_WALL:
		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->defaultBrush, L"메이플스토리", 12.0f, L"Object Wall", box[1].left - 35, box[1].bottom + 80, box[1].left + 100, box[1].bottom + 100);
		break;
	case OBJECT_BUILDING:
		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->defaultBrush, L"메이플스토리", 12.0f, L"Object Building", box[1].left - 35, box[1].bottom + 80, box[1].left + 100, box[1].bottom + 100);
		break;
	case LOOP:
		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->defaultBrush, L"메이플스토리", 12.0f, L"Loop", box[1].left - 35, box[1].bottom + 80, box[1].left + 100, box[1].bottom + 100);
		break;
	case PAGE_NUM:
		break;
	default:
		break;
	}

	if (_IndexPool.size() != 0)
	{
		int iter = 0;
		for (int i = 0; i < _col; i++)
		{
			for (int j = 0; j < _row; j++)
			{
				int width = _IndexPool[iter].sample_img->getFrameWidth();
				int height = _IndexPool[iter].sample_img->getFrameHeight();

				_IndexPool[iter].sample_img->FrameRender(_ptMouse.x + j * width - 11, _ptMouse.y + i * height - 11, _IndexPool[iter].index.x, _IndexPool[iter].index.y, 0.7f);
				
				iter++;
			}
		}
	}

}

void tool::move_viewpoint()
{
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		LButton_down = true;
		LButton_up = false;

		_count += 1;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	{
		LButton_down = false;
		LButton_up = true;
		
		_count = 0;
	}

	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		is_full = false;
		_IndexPool.clear();
		_row = 0;
		_col = 0;
	}

	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		x -= TILESIZE;
		if (x <= 0)
		{
			x = 0;
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		x += TILESIZE;
		if (x >= MAPSIZEX)
		{
			x = MAPSIZEX;
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		y -= TILESIZE;
		if (y <= 0)
		{
			y = 0;
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		y += TILESIZE;
		if (y > MAPSIZEY)
		{
			y = MAPSIZEY;
		}
	}

	if (KEYMANAGER->isToggleKey(VK_DELETE))
	{
		erase = true;
	}
	else
		erase = false;
}

void tool::set_tiles(int x, int y)
{
	switch (_page)
	{
	case TILE:
		if (_IndexPool.size() != 0)
		{
			int iter = 0;
			for (int i = 0; i < _col; i++)
			{
				for (int j = 0; j < _row; j++)
				{
					if (x + j + (y + i) * _tileNumX >= _tileNumX *_tileNumY) continue;

					_tiles[x + j + (y + i) * _tileNumX]->setIdX(x + j);
					_tiles[x + j + (y + i) * _tileNumX]->setIdY(y + i);
					_tiles[x + j + (y + i) * _tileNumX]->set_image(_tileImage[0]);
					_tiles[x + j + (y + i) * _tileNumX]->setTileIndex(_IndexPool[iter].index.x, _IndexPool[iter].index.y);
					_tiles[x + j + (y + i) * _tileNumX]->setAttribute(tile_Attribute::GROUND);
					_tiles[x + j + (y + i) * _tileNumX]->setSeason(tile_Season::SPRING);
					_tiles[x + j + (y + i) * _tileNumX]->set_Tilepage(0);

					if (_IndexPool[iter].index.y >= 6 && _IndexPool[iter].index.y <= 11)
					{
						_tiles[x + j + (y + i) * _tileNumX]->setAttribute(tile_Attribute::FARM);
					}

					iter++;
				}
			}
		}
		else
		{
			_tiles[x + y * _tileNumX]->setIdX(x);
			_tiles[x + y * _tileNumX]->setIdY(y);
			_tiles[x + y * _tileNumX]->set_image(_tileImage[0]);
			_tiles[x + y * _tileNumX]->setTileIndex(current_frameX, current_frameY);
			_tiles[x + y * _tileNumX]->setAttribute(tile_Attribute::GROUND);
			_tiles[x + y * _tileNumX]->setSeason(tile_Season::SPRING);
			_tiles[x + y * _tileNumX]->set_Tilepage(0);
		}

		break;

	case TILE_EDGE:
		if (_IndexPool.size() != 0)
		{
			int iter = 0;
			for (int i = 0; i < _col; i++)
			{
				for (int j = 0; j < _row; j++)
				{
					if (x + j + (y + i) * _tileNumX >= _tileNumX * _tileNumY) continue;

					_tiles[x + j + (y + i) * _tileNumX]->setIdX(x + j);
					_tiles[x + j + (y + i) * _tileNumX]->setIdY(y + i);
					_tiles[x + j + (y + i) * _tileNumX]->set_objectImage(_tileImage[1]);
					_tiles[x + j + (y + i) * _tileNumX]->setObjectIndex(_IndexPool[iter].index.x, _IndexPool[iter].index.y);
					_tiles[x + j + (y + i) * _tileNumX]->setAttribute(tile_Attribute::GROUND);
					_tiles[x + j + (y + i) * _tileNumX]->setSeason(tile_Season::SPRING);
					_tiles[x + j + (y + i) * _tileNumX]->set_Objectpage(TILE_EDGE);

					iter++;
				}
			}
		}
		else
		{
			_tiles[x + y * _tileNumX]->setIdX(x);
			_tiles[x + y * _tileNumX]->setIdY(y);
			_tiles[x + y * _tileNumX]->set_objectImage(_tileImage[1]);
			_tiles[x + y * _tileNumX]->setObjectIndex(current_frameX, current_frameY);
			_tiles[x + y * _tileNumX]->setAttribute(tile_Attribute::GROUND);
			_tiles[x + y * _tileNumX]->setSeason(tile_Season::SPRING);
			_tiles[x + y * _tileNumX]->set_Objectpage(TILE_EDGE);
		}
		break;
	case TILE_EDGE2:
		if (_IndexPool.size() != 0)
		{
			int iter = 0;
			for (int i = 0; i < _col; i++)
			{
				for (int j = 0; j < _row; j++)
				{
					if (x + j + (y + i) * _tileNumX >= _tileNumX * _tileNumY) continue;

					_tiles[x + j + (y + i) * _tileNumX]->setIdX(x + j);
					_tiles[x + j + (y + i) * _tileNumX]->setIdY(y + i);
					_tiles[x + j + (y + i) * _tileNumX]->set_objectImage(_tileImage[2]);
					_tiles[x + j + (y + i) * _tileNumX]->setObjectIndex(_IndexPool[iter].index.x, _IndexPool[iter].index.y);
					_tiles[x + j + (y + i) * _tileNumX]->setAttribute(tile_Attribute::GROUND);
					_tiles[x + j + (y + i) * _tileNumX]->setSeason(tile_Season::SPRING);
					_tiles[x + j + (y + i) * _tileNumX]->set_Objectpage(TILE_EDGE2);
					iter++;
				}
			}
		}
		else
		{
			_tiles[x + y * _tileNumX]->setIdX(x);
			_tiles[x + y * _tileNumX]->setIdY(y);
			_tiles[x + y * _tileNumX]->set_objectImage(_tileImage[2]);
			_tiles[x + y * _tileNumX]->setObjectIndex(current_frameX, current_frameY);
			_tiles[x + y * _tileNumX]->setAttribute(tile_Attribute::GROUND);
			_tiles[x + y * _tileNumX]->setSeason(tile_Season::SPRING);
			_tiles[x + y * _tileNumX]->set_Objectpage(TILE_EDGE2);
		}
		break;

	case TILE_EDGE3:
		if (_IndexPool.size() != 0)
		{
			int iter = 0;
			for (int i = 0; i < _col; i++)
			{
				for (int j = 0; j < _row; j++)
				{
					if (x + j + (y + i) * _tileNumX >= _tileNumX * _tileNumY) continue;

					_tiles[x + j + (y + i) * _tileNumX]->setIdX(x + j);
					_tiles[x + j + (y + i) * _tileNumX]->setIdY(y + i);
					_tiles[x + j + (y + i) * _tileNumX]->set_objectImage(_tileImage[3]);
					_tiles[x + j + (y + i) * _tileNumX]->setObjectIndex(_IndexPool[iter].index.x, _IndexPool[iter].index.y);
					_tiles[x + j + (y + i) * _tileNumX]->setAttribute(tile_Attribute::GROUND);
					_tiles[x + j + (y + i) * _tileNumX]->setSeason(tile_Season::SPRING);
					_tiles[x + j + (y + i) * _tileNumX]->set_Objectpage(TILE_EDGE3);
					iter++;
				}
			}
		}
		else
		{
			_tiles[x + y * _tileNumX]->setIdX(x);
			_tiles[x + y * _tileNumX]->setIdY(y);
			_tiles[x + y * _tileNumX]->set_objectImage(_tileImage[3]);
			_tiles[x + y * _tileNumX]->setObjectIndex(current_frameX, current_frameY);
			_tiles[x + y * _tileNumX]->setAttribute(tile_Attribute::GROUND);
			_tiles[x + y * _tileNumX]->setSeason(tile_Season::SPRING);
			_tiles[x + y * _tileNumX]->set_Objectpage(TILE_EDGE3);
		}
		break;

	case OBJECT_TILE:
		if (_IndexPool.size() != 0)
		{
			int iter = 0;
			for (int i = 0; i < _col; i++)
			{
				for (int j = 0; j < _row; j++)
				{
					if (x + j + (y + i) * _tileNumX >= _tileNumX * _tileNumY) continue;

					_tiles[x + j + (y + i) * _tileNumX]->setIdX(x + j);
					_tiles[x + j + (y + i) * _tileNumX]->setIdY(y + i);
					_tiles[x + j + (y + i) * _tileNumX]->set_objectImage(_tileImage[4]);
					_tiles[x + j + (y + i) * _tileNumX]->setObjectIndex(_IndexPool[iter].index.x, _IndexPool[iter].index.y);
					_tiles[x + j + (y + i) * _tileNumX]->setAttribute(tile_Attribute::GROUND);
					_tiles[x + j + (y + i) * _tileNumX]->setSeason(tile_Season::SPRING);
					_tiles[x + j + (y + i) * _tileNumX]->set_Objectpage(OBJECT_TILE);
					iter++;
				}
			}
		}
		else
		{
			_tiles[x + y * _tileNumX]->setIdX(x);
			_tiles[x + y * _tileNumX]->setIdY(y);
			_tiles[x + y * _tileNumX]->set_objectImage(_tileImage[4]);
			_tiles[x + y * _tileNumX]->setObjectIndex(current_frameX, current_frameY);
			_tiles[x + y * _tileNumX]->setAttribute(tile_Attribute::GROUND);
			_tiles[x + y * _tileNumX]->setSeason(tile_Season::SPRING);
			_tiles[x + y * _tileNumX]->set_Objectpage(OBJECT_TILE);
		}
		break;

	case OBJECT_EDGE:
		if (_IndexPool.size() != 0)
		{
			int iter = 0;
			for (int i = 0; i < _col; i++)
			{
				for (int j = 0; j < _row; j++)
				{
					if (x + j + (y + i) * _tileNumX >= _tileNumX * _tileNumY) continue;

					_tiles[x + j + (y + i) * _tileNumX]->setIdX(x + j);
					_tiles[x + j + (y + i) * _tileNumX]->setIdY(y + i);
					_tiles[x + j + (y + i) * _tileNumX]->set_objectImage(_tileImage[5]);
					_tiles[x + j + (y + i) * _tileNumX]->setObjectIndex(_IndexPool[iter].index.x, _IndexPool[iter].index.y);
					_tiles[x + j + (y + i) * _tileNumX]->setAttribute(tile_Attribute::WALL);
					_tiles[x + j + (y + i) * _tileNumX]->setSeason(tile_Season::SPRING);
					_tiles[x + j + (y + i) * _tileNumX]->set_Objectpage(OBJECT_EDGE);
					iter++;
				}
			}
		}
		else
		{
			_tiles[x + y * _tileNumX]->setIdX(x);
			_tiles[x + y * _tileNumX]->setIdY(y);
			_tiles[x + y * _tileNumX]->set_objectImage(_tileImage[5]);
			_tiles[x + y * _tileNumX]->setObjectIndex(current_frameX, current_frameY);
			_tiles[x + y * _tileNumX]->setAttribute(tile_Attribute::WALL);
			_tiles[x + y * _tileNumX]->setSeason(tile_Season::SPRING);
			_tiles[x + y * _tileNumX]->set_Objectpage(OBJECT_EDGE);
		}
		break;

	case OBJECT_EDGE2:
		if (_IndexPool.size() != 0)
		{
			int iter = 0;
			for (int i = 0; i < _col; i++)
			{
				for (int j = 0; j < _row; j++)
				{
					if (x + j + (y + i) * _tileNumX >= _tileNumX * _tileNumY) continue;

					_tiles[x + j + (y + i) * _tileNumX]->setIdX(x + j);
					_tiles[x + j + (y + i) * _tileNumX]->setIdY(y + i);
					_tiles[x + j + (y + i) * _tileNumX]->set_objectImage(_tileImage[6]);
					_tiles[x + j + (y + i) * _tileNumX]->setObjectIndex(_IndexPool[iter].index.x, _IndexPool[iter].index.y);
					_tiles[x + j + (y + i) * _tileNumX]->setAttribute(tile_Attribute::WALL);
					_tiles[x + j + (y + i) * _tileNumX]->setSeason(tile_Season::SPRING);
					_tiles[x + j + (y + i) * _tileNumX]->set_Objectpage(OBJECT_EDGE2);
					iter++;
				}
			}
		}
		else
		{
			_tiles[x + y * _tileNumX]->setIdX(x);
			_tiles[x + y * _tileNumX]->setIdY(y);
			_tiles[x + y * _tileNumX]->set_objectImage(_tileImage[6]);
			_tiles[x + y * _tileNumX]->setObjectIndex(current_frameX, current_frameY);
			_tiles[x + y * _tileNumX]->setAttribute(tile_Attribute::WALL);
			_tiles[x + y * _tileNumX]->setSeason(tile_Season::SPRING);
			_tiles[x + y * _tileNumX]->set_Objectpage(OBJECT_EDGE2);
		}
		break;

	case OBJECT_EDGE3:
		if (_IndexPool.size() != 0)
		{
			int iter = 0;
			for (int i = 0; i < _col; i++)
			{
				for (int j = 0; j < _row; j++)
				{
					if (x + j + (y + i) * _tileNumX >= _tileNumX * _tileNumY) continue;

					_tiles[x + j + (y + i) * _tileNumX]->setIdX(x + j);
					_tiles[x + j + (y + i) * _tileNumX]->setIdY(y + i);
					_tiles[x + j + (y + i) * _tileNumX]->set_objectImage(_tileImage[7]);
					_tiles[x + j + (y + i) * _tileNumX]->setObjectIndex(_IndexPool[iter].index.x, _IndexPool[iter].index.y);
					_tiles[x + j + (y + i) * _tileNumX]->setAttribute(tile_Attribute::WALL);
					_tiles[x + j + (y + i) * _tileNumX]->setSeason(tile_Season::SPRING);
					_tiles[x + j + (y + i) * _tileNumX]->set_Objectpage(OBJECT_EDGE3);
					iter++;
				}
			}
		}
		else
		{
			_tiles[x + y * _tileNumX]->setIdX(x);
			_tiles[x + y * _tileNumX]->setIdY(y);
			_tiles[x + y * _tileNumX]->set_objectImage(_tileImage[7]);
			_tiles[x + y * _tileNumX]->setObjectIndex(current_frameX, current_frameY);
			_tiles[x + y * _tileNumX]->setAttribute(tile_Attribute::WALL);
			_tiles[x + y * _tileNumX]->setSeason(tile_Season::SPRING);
			_tiles[x + y * _tileNumX]->set_Objectpage(OBJECT_EDGE3);
		}
		break;

	case OBJECT_WALL:
		if (_IndexPool.size() != 0)
		{
			int iter = 0;
			for (int i = 0; i < _col; i++)
			{
				for (int j = 0; j < _row; j++)
				{
					if (x + j + (y + i) * _tileNumX >= _tileNumX * _tileNumY) continue;

					_tiles[x + j + (y + i) * _tileNumX]->setIdX(x + j);
					_tiles[x + j + (y + i) * _tileNumX]->setIdY(y + i);
					_tiles[x + j + (y + i) * _tileNumX]->set_objectImage(_tileImage[8]);
					_tiles[x + j + (y + i) * _tileNumX]->setObjectIndex(_IndexPool[iter].index.x, _IndexPool[iter].index.y);
					_tiles[x + j + (y + i) * _tileNumX]->setAttribute(tile_Attribute::WALL);
					_tiles[x + j + (y + i) * _tileNumX]->setSeason(tile_Season::SPRING);
					_tiles[x + j + (y + i) * _tileNumX]->set_Objectpage(OBJECT_WALL);
					iter++;
				}
			}
		}
		else
		{
			_tiles[x + y * _tileNumX]->setIdX(x);
			_tiles[x + y * _tileNumX]->setIdY(y);
			_tiles[x + y * _tileNumX]->set_objectImage(_tileImage[8]);
			_tiles[x + y * _tileNumX]->setObjectIndex(current_frameX, current_frameY);
			_tiles[x + y * _tileNumX]->setAttribute(tile_Attribute::WALL);
			_tiles[x + y * _tileNumX]->setSeason(tile_Season::SPRING);
			_tiles[x + y * _tileNumX]->set_Objectpage(OBJECT_WALL);
		}
		break;

	case OBJECT_BUILDING:
		if (_IndexPool.size() != 0)
		{
			int iter = 0;
			for (int i = 0; i < _col; i++)
			{
				for (int j = 0; j < _row; j++)
				{
					if (x + j + (y + i) * _tileNumX >= _tileNumX * _tileNumY) continue;

					_tiles[x + j + (y + i) * _tileNumX]->setIdX(x + j);
					_tiles[x + j + (y + i) * _tileNumX]->setIdY(y + i);
					_tiles[x + j + (y + i) * _tileNumX]->set_objectImage(_tileImage[9]);
					_tiles[x + j + (y + i) * _tileNumX]->setObjectIndex(_IndexPool[iter].index.x, _IndexPool[iter].index.y);
					_tiles[x + j + (y + i) * _tileNumX]->setAttribute(tile_Attribute::WALL);
					_tiles[x + j + (y + i) * _tileNumX]->setSeason(tile_Season::SPRING);
					_tiles[x + j + (y + i) * _tileNumX]->set_Objectpage(OBJECT_BUILDING);
					iter++;
				}
			}
		}
		else
		{
			_tiles[x + y * _tileNumX]->setIdX(x);
			_tiles[x + y * _tileNumX]->setIdY(y);
			_tiles[x + y * _tileNumX]->set_objectImage(_tileImage[9]);
			_tiles[x + y * _tileNumX]->setObjectIndex(current_frameX, current_frameY);
			_tiles[x + y * _tileNumX]->setAttribute(tile_Attribute::WALL);
			_tiles[x + y * _tileNumX]->setSeason(tile_Season::SPRING);
			_tiles[x + y * _tileNumX]->set_Objectpage(OBJECT_BUILDING);
		}
		break;

	case LOOP:
		if (_IndexPool.size() != 0)
		{
			int iter = 0;
			for (int i = 0; i < _col; i++)
			{
				for (int j = 0; j < _row; j++)
				{
					if (x + j + (y + i) * _tileNumX >= _tileNumX * _tileNumY) continue;

					_tiles[x + j + (y + i) * _tileNumX]->setIdX(x + j);
					_tiles[x + j + (y + i) * _tileNumX]->setIdY(y + i);
					_tiles[x + j + (y + i) * _tileNumX]->set_image(_tileImage[10]);
					_tiles[x + j + (y + i) * _tileNumX]->setTileIndex(_IndexPool[iter].index.x, _IndexPool[iter].index.y);
					_tiles[x + j + (y + i) * _tileNumX]->setSeason(tile_Season::SPRING);
					_tiles[x + j + (y + i) * _tileNumX]->set_Tilepage(LOOP);

					if (_IndexPool[iter].index.y <= 6)
					{
						_tiles[x + j + (y + i) * _tileNumX]->setAttribute(tile_Attribute::WATER_ROW);
					}
					else
						_tiles[x + j + (y + i) * _tileNumX]->setAttribute(tile_Attribute::WATER_COL);

					iter++;
				}
			}
		}
		else
		{
			_tiles[x + y * _tileNumX]->setIdX(x);
			_tiles[x + y * _tileNumX]->setIdY(y);
			_tiles[x + y * _tileNumX]->set_image(_tileImage[10]);
			_tiles[x + y * _tileNumX]->setTileIndex(current_frameX, current_frameY);
			_tiles[x + y * _tileNumX]->setAttribute(tile_Attribute::WALL);
			_tiles[x + y * _tileNumX]->setSeason(tile_Season::SPRING);
			_tiles[x + y * _tileNumX]->set_Tilepage(LOOP);
		}
		break;
	}

	if (erase)
	{

		_tiles[x + y * _tileNumX]->set_image(NULL);
		_tiles[x + y * _tileNumX]->setTileIndex(0, 0);
		_tiles[x + y * _tileNumX]->set_objectImage(NULL);
		_tiles[x + y * _tileNumX]->setObjectIndex(0, 0);

		_tiles[x + y * _tileNumX]->setAttribute(tile_Attribute::GROUND);
		_tiles[x + y * _tileNumX]->setSeason(tile_Season::SEASON_NUM);
		_tiles[x + y * _tileNumX]->set_Tilepage(-1);
		_tiles[x + y * _tileNumX]->set_Objectpage(-1);
	}
}

void tool::box_click()
{
	// 이전 버튼 누를시, 샘플페이지 이전으로 이동. 첫페이지면 이동불가
	if (PTIN_RECT(&box[2], _ptMouse))
	{
		if (LButton_down && _page != TILE && _count == 1)
		{
			_page = (sample_page)(_page - 1);
			_currentSample = _sampleImage[_page];
			_buttonImage[1] = _tileImage[_page];

			is_full = false;
			_IndexPool.clear();
			_row = 0;
			_col = 0;
		}
	}
	// 다음 버튼 누를시, 샘플페이지 다음으로 이동. 마지막 페이지면 이동불가
	if (PTIN_RECT(&box[3], _ptMouse))
	{
		if (LButton_down && _page != LOOP && _count == 1)
		{
			_page = (sample_page)(_page + 1);
			_currentSample = _sampleImage[_page];
			_buttonImage[1] = _tileImage[_page];

			is_full = false;
			_IndexPool.clear();
			_row = 0;
			_col = 0;
		}
	}

	if (PTIN_RECT(&box[4], _ptMouse))
	{
		if (LButton_down && _page != LOOP && _count == 1)
		{
			save();
		}
	}

	if (PTIN_RECT(&box[5], _ptMouse))
	{
		if (LButton_down && _page != LOOP && _count == 1)
		{
			load();
		}
	}

	// 타일갯수 변경 1 누를시
	if (PTIN_RECT(&num_box[0], _ptMouse))
	{
		if (LButton_down && _count == 1)
		{
			current_ratio = 1;
		}
	}

	// 타일갯수 변경 5 누를시
	if (PTIN_RECT(&num_box[1], _ptMouse))
	{
		if (LButton_down && _count == 1)
		{
			current_ratio = 5;
		}
	}
	// 타일갯수 변경 10 누를시
	if (PTIN_RECT(&num_box[2], _ptMouse))
	{
		if (LButton_down && _count == 1)
		{
			current_ratio = 10;
		}
	}
	// X 타일갯수 업 누를시
	if (PTIN_RECT(&num_box[3], _ptMouse))
	{
		if (LButton_down && _count == 1)
		{
			current_tileX += current_ratio;
		}
	}
	// X 타일갯수 다운 누를시
	if (PTIN_RECT(&num_box[4], _ptMouse))
	{
		if (LButton_down && _count == 1)
		{
			current_tileX -= current_ratio;
		}
	}
	// Y 타일갯수 업 누를시
	if (PTIN_RECT(&num_box[5], _ptMouse))
	{
		if (LButton_down && _count == 1)
		{
			current_tileY += current_ratio;
		}
	}
	// Y 타일갯수 다운 누를시
	if (PTIN_RECT(&num_box[6], _ptMouse))
	{
		if (LButton_down && _count == 1)
		{
			current_tileY -= current_ratio;
		}
	}
	// Create 버튼 누를시, 새로운 TileX * TileY 로 생성.
	if (PTIN_RECT(&num_box[7], _ptMouse))
	{
		if (LButton_down && _count == 1)
		{
			_tiles.clear();
		
			_tileNumX = current_tileX;
			_tileNumY = current_tileY;

			// 화면에 타일 _tileNumX x _tileNumY 으로 깔아줍니다.
			for (int i = 0; i < _tileNumY; i++)
			{
				for (int j = 0; j < _tileNumX; j++)
				{
					tile* temp_tile = new tile;
					temp_tile->init(j, i);

					_tiles.push_back(temp_tile);
				}
			}
		}
	}

	if (current_tileX <= 0) current_tileX = 0;
	if (current_tileY <= 0) current_tileY = 0;
	if (current_tileX >= 200) current_tileX = 200;
	if (current_tileY >= 200) current_tileY = 200;
}

void tool::save()
{
	mouse_on_board = true;

	OPENFILENAME ofn;
	char filePathSize[1028] = "";
	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = filePathSize;
	ofn.nMaxFile = sizeof(filePathSize);
	ofn.nFilterIndex = true;
	ofn.nMaxFileTitle = NULL;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrFilter = (".map");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetSaveFileName(&ofn) == FALSE) return;

	char temp[1028];
	strncpy_s(temp, strlen(ofn.lpstrFile) + 1, ofn.lpstrFile, strlen(ofn.lpstrFile));

	char* context = NULL;
	char* token = strtok_s(temp, "\\", &context);

	while (strlen(context))
	{
		token = strtok_s(NULL, "\\", &context);
	}

	HANDLE file;
	DWORD write;

	file = CreateFile(token, GENERIC_WRITE, NULL, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	for (int i = 0; i < _tiles.size(); i++)
	{
		WriteFile(file, _tiles[i], sizeof(tile), &write, NULL);
	}

	CloseHandle(file);
}

void tool::load()
{
	_loopList.clear();
	_farmList.clear();

	mouse_on_board = true;

	OPENFILENAME ofn;
	char filePathSize[1028] = "";
	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = filePathSize;
	ofn.nMaxFile = sizeof(filePathSize);
	ofn.nFilterIndex = true;
	ofn.nMaxFileTitle = NULL;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrFilter = (".map");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == FALSE) return;

	char temp[1028];
	strncpy_s(temp, strlen(ofn.lpstrFile) + 1, ofn.lpstrFile, strlen(ofn.lpstrFile));

	char* context = NULL;
	char* token = strtok_s(temp, "\\", &context);

	while (strlen(context))
	{
		token = strtok_s(NULL, "\\", &context);
	}

	HANDLE file;
	DWORD read;

	tile* save_tile = new tile[_tileNumX * _tileNumY];
	//char str[TILEX * TILEY * 6 * sizeof(char) + 1];

	file = CreateFile(token, GENERIC_READ, FALSE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//ReadFile(file, str, strlen(str), &read, NULL);

	for (int i = 0; i < _tiles.size(); i++)
	{
		ReadFile(file, _tiles[i], sizeof(tile), &read, NULL);
		
		if (_tiles[i]->get_Tilepage() != -1)
		{
			_tiles[i]->set_image(_tileImage[_tiles[i]->get_Tilepage()]);
		}

		if (_tiles[i]->get_Objectpage() != -1)
		{
			_tiles[i]->set_objectImage(_tileImage[_tiles[i]->get_Objectpage()]);
		}
	}

	CloseHandle(file);

}

void tool::load(int num)
{
	_loopList.clear();
	_farmList.clear();

	HANDLE file = NULL;
	DWORD read = NULL;

	switch (num)
	{
	case 1:
		_tileNumX = 200;
		_tileNumY = 120;

		file = CreateFile("map/1.map", GENERIC_READ, FALSE,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		break;

	case 2:
		_tileNumX = 100;
		_tileNumY = 60;

		_tiles.clear();
		
		for (int i = 0; i < _tileNumY; i++)
		{
			for (int j = 0; j < _tileNumX; j++)
			{
				tile* node = new tile;
				node->init(j, i);
				_tiles.push_back(node);
			}
		}

		file = CreateFile("map/base2.map", GENERIC_READ, FALSE,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		break;

	case 3:
		_tileNumX = 200;
		_tileNumY = 120;

		_tiles.clear();

		for (int i = 0; i < _tileNumY; i++)
		{
			for (int j = 0; j < _tileNumX; j++)
			{
				tile* node = new tile;
				node->init(j, i);
				_tiles.push_back(node);
			}
		}

		file = CreateFile("world.map", GENERIC_READ, FALSE,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	}

	for (int i = 0; i < _tiles.size(); i++)
	{
		ReadFile(file, _tiles[i], sizeof(tile), &read, NULL);

		if (_tiles[i]->get_Tilepage() != -1)
		{
			_tiles[i]->set_image(_tileImage[_tiles[i]->get_Tilepage()]);
		}

		if (_tiles[i]->get_Objectpage() != -1)
		{
			_tiles[i]->set_objectImage(_tileImage[_tiles[i]->get_Objectpage()]);
		}

		if (_tiles[i]->getAttribute() == tile_Attribute::WATER_ROW || _tiles[i]->getAttribute() == tile_Attribute::WATER_COL)
		{
			_loopList.push_back(i);
		}

		if (_tiles[i]->getAttribute() == tile_Attribute::FARM)
		{
			_farmList.push_back(i);
		}
	}

	CloseHandle(file);
}
