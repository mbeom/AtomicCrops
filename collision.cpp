#include "stdafx.h"
#include "collision.h"
#include "tool.h"
#include "UI.h"

//충돌을 요구하는 객체
#include "player.h"
#include "enemyManager.h"
#include "CropManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"

HRESULT collision::init()
{
	IMAGEMANAGER->addFrameImage("harvest_effect", L"image/crop/harvest_effect.png", 213, 128, 5, 2);
	IMAGEMANAGER->addFrameImage("farm_indicator", L"image/UI/farmingIndicator.png", 40, 40, 1, 1);
	IMAGEMANAGER->addFrameImage("number", L"image/UI/Number.png", 120, 20, 10, 1);

	EFFECTMANAGER->addEffect("harvest_effect", "harvest_effect", 213, 128, 43, 64, 4.0f, 0.1f, 50);

	_indicator = IMAGEMANAGER->findImage("farm_indicator");

	if (!_fontEffect)
	{
		_fontEffect = new font_effect;
		_fontEffect->init();
	}

	mapTileX = _map->get_tileNumX();
	mapTileY = _map->get_tileNumY();

	_farm = { 2540, 500, 5300, 3000 };

	on_tile = false;

	return S_OK;
}

void collision::release()
{
}

void collision::update()
{
	D2D1_RECT_F player = _player->get_playerRect();

	current_tile_rect = _map->get_currentTIle();

	int _tileX = _ptMouse.x / TILESIZE;
	int _tileY = _ptMouse.y / TILESIZE;

	if (_tileX >= 0 && _tileY >= 0 && _tileX < mapTileX && _tileY < mapTileY)
		tile_onMouse = _map->get_tiles()[_tileX + _tileY * mapTileX];

	//플레이어 4방향 (상하좌우 타일충돌 관리함수)
	player_AND_CrossTiles();

	//플레이어 대각선 방향 타일충돌 관리함수
	player_AND_diagonalTiles();

	// 플레이어와 타일
	if(_player->get_inWorld() && INTERSECTION_RECT(temp, _farm, player))
		player_AND_tile();

	// 플레이어와 에너미 충돌처리
	player_AND_enemy();

	// 플레이어와 오브젝트 상호작용
	player_AND_object();

	// 플레이어와 작물들 상호작용
	player_AND_crops();

	// 플레이어와 아이템 상호작용
	player_AND_Item();

	// 에너미 타일충돌 (4방향)
	enemy_AND_CrossTiles();

	// 에너미 타일 대각선 충돌
	enemy_AND_diagonalTiles();

	// 작물 수확시 UI 폰트 재생
	_fontEffect->update();

	// 이펙트 업데이트
	EFFECTMANAGER->update();
}

void collision::render()
{
	if (x != 0)
	{
		D2DMANAGER->Rectangle(D2DMANAGER->defaultBrush, current_tile_rect);
	}

	if (on_tile)
	{
		//float center_x = (current_tile->getRectangle().left + current_tile->getRectangle().right) / 2 - CAMERAMANAGER->get_camera_x();
		//float center_y = (current_tile->getRectangle().top + current_tile->getRectangle().bottom) / 2 - CAMERAMANAGER->get_camera_y();
		//
		//CAMERAMANAGER->trans_mat = D2D1::Matrix3x2F::Translation(D2D1::SizeF(CAMERAMANAGER->center_pos.x - center_x, CAMERAMANAGER->center_pos.y - center_y));
		//D2DMANAGER->pRenderTarget->SetTransform(CAMERAMANAGER->scale_mat * CAMERAMANAGER->trans_mat);

		_indicator->FrameRender(current_tile->getRectangle().left, current_tile->getRectangle().top, 0, 0);
	}
	//D2DMANAGER->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		for (int i = 0; i < 2; i++)
		{
			D2DMANAGER->Rectangle(D2DMANAGER->white_brush, _map->get_tiles()[tileIndex[i]]->getRectangle());
		}

		for (int i = 0; i < 3; i++)
		{
			D2DMANAGER->Rectangle(D2DMANAGER->white_brush, _map->get_tiles()[tileIndex2[i]]->getRectangle());
		}
	}

	_fontEffect->render();

	EFFECTMANAGER->render();
}

/********************************************************** 4 방향 충돌 *******************************************************************/

void collision::player_AND_CrossTiles()
{
	D2D1_RECT_F collision;

	//int tileIndex[2];	//타일 검출에 필요한 인덱스
	int tileX, tileY;	//플레이어가 밟고 있는 타일의 인덱스

	//가상의 판정렉트에 현재 렉트를 대입해주자
	collision = _player->get_playerRect();

	tileX = collision.left / TILESIZE;
	tileY = collision.top / TILESIZE;

	switch (_player->get_playerInfo().moving_direction)
	{
	case Player_Direction::TOP:
		tileIndex[0] = tileX + tileY * mapTileX;
		tileIndex[1] = (tileX + 1) + tileY * mapTileX;
		break;
	case Player_Direction::BOTTOM:
		tileIndex[0] = (tileX + tileY * mapTileX) + mapTileX;
		tileIndex[1] = (tileX + 1 + tileY * mapTileX) + mapTileX;
		break;
	case Player_Direction::LEFT:
		tileIndex[0] = tileX + (tileY * mapTileX);
		tileIndex[1] = tileX + (tileY + 1) * mapTileX;
		break;
	case Player_Direction::RIGHT:
		tileIndex[0] = (tileX + tileY * mapTileX) + 1;
		tileIndex[1] = (tileX + (tileY + 1) * mapTileX) + 1;
		break;
	default:
		tileIndex[0] = NULL;
		tileIndex[1] = NULL;
		break;
	}

	for (int i = 0; i < 2; ++i)
	{
		if (tileIndex[i] == NULL) break;

		D2D1_RECT_F rc;
		D2D1_RECT_F tile = _map->get_tiles()[tileIndex[i]]->getRectangle();
		tile_Attribute attribute = _map->get_tiles()[tileIndex[i]]->getAttribute();

		if (attribute == tile_Attribute::WALL || attribute == tile_Attribute::WATER_COL || attribute == tile_Attribute::WATER_ROW)
			attribute = tile_Attribute::WALL;

		if (attribute == tile_Attribute::WALL && INTERSECTION_RECT(rc, tile, collision))
		{
			if ((rc.right - rc.left >= rc.bottom - rc.top) && collision.top < tile.top)
			{
				collision.bottom = tile.top;
				collision.top = collision.bottom - TILESIZE;

				_player->set_y((collision.top + collision.bottom) / 2);
			}

			else if ((rc.right - rc.left >= rc.bottom - rc.top) && collision.top >= tile.top)
			{
				collision.top = tile.bottom;
				collision.bottom = collision.top + TILESIZE;

				_player->set_y((collision.top + collision.bottom) / 2);
			}

			else if ((rc.right - rc.left <= rc.bottom - rc.top) && collision.left < tile.left)
			{
				collision.right = tile.left;
				collision.left = collision.right - TILESIZE;

				_player->set_x((collision.left + collision.right) / 2);
			}

			else if((rc.right - rc.left <= rc.bottom - rc.top) && collision.left > tile.left)
			{
				collision.left = tile.right;
				collision.right = collision.left + TILESIZE;

				_player->set_x((collision.left + collision.right) / 2);
			}
			_player->set_rect();
			break;
		}
	}
}


/********************************************************** 대각선 충돌 *******************************************************************/

void collision::player_AND_diagonalTiles()
{
	D2D1_RECT_F collision;

	//int tileIndex[3];	//타일 검출에 필요한 인덱스
	int tileX, tileY;	//플레이어가 밟고 있는 타일의 인덱스

	//가상의 판정렉트에 현재 렉트를 대입해주자
	collision = _player->get_playerRect();

	tileX = collision.left / TILESIZE;
	tileY = collision.top / TILESIZE;

	switch (_player->get_playerInfo().moving_direction)
	{
	case Player_Direction::LEFT_TOP:
		tileIndex2[0] = tileX + (tileY * mapTileX);
		tileIndex2[2] = tileX + (tileY + 1) * mapTileX;
		tileIndex2[1] = (tileX + 1) + tileY * mapTileX;
		break;
	case Player_Direction::RIGHT_TOP:
		tileIndex2[0] = (tileX + tileY * mapTileX) + 1;
		tileIndex2[2] = (tileX + (tileY + 1) * mapTileX) + 1;
		tileIndex2[1] = (tileX) + tileY * mapTileX;
		break;
	case Player_Direction::LEFT_BOTTOM:
		tileIndex2[0] = tileX + (tileY * mapTileX);
		tileIndex2[1] = tileX + (tileY + 1) * mapTileX;
		tileIndex2[2] = (tileX + 1) + (tileY + 1) * mapTileX;
		break;
	case Player_Direction::RIGHT_BOTTOM:
		tileIndex2[0] = (tileX + tileY * mapTileX) + 1;
		tileIndex2[1] = (tileX + (tileY + 1) * mapTileX) + 1;
		tileIndex2[2] = (tileX)+(tileY + 1) * mapTileX;
		break;
	default:
		tileIndex2[0] = NULL;
		tileIndex2[1] = NULL;
		tileIndex2[2] = NULL;
		break;
	}

	tile_Attribute attribute[3] = { _map->get_tiles()[tileIndex2[0]]->getAttribute(), _map->get_tiles()[tileIndex2[1]]->getAttribute(), _map->get_tiles()[tileIndex2[2]]->getAttribute() };

	for (int i = 0; i < 3; ++i)
	{
		if (tileIndex2[i] == NULL) break;

		D2D1_RECT_F temp;
		D2D1_RECT_F tile = _map->get_tiles()[tileIndex2[i]]->getRectangle();
		tile_Attribute _attribute = _map->get_tiles()[tileIndex2[i]]->getAttribute();

		if (attribute[i] == tile_Attribute::WALL || attribute[i] == tile_Attribute::WATER_COL || attribute[i] == tile_Attribute::WATER_ROW)
			attribute[i] = tile_Attribute::WALL;

		if (_attribute != tile_Attribute::WALL && _attribute != tile_Attribute::WATER_COL && _attribute != tile_Attribute::WATER_ROW) continue;

		if (INTERSECTION_RECT(temp, tile, collision))
		{
			switch (_player->get_playerInfo().moving_direction)
			{
			case Player_Direction::LEFT_TOP:

				if (attribute[2] == tile_Attribute::WALL)
				{
					if (attribute[1] == tile_Attribute::WALL)
					{
						D2D1_RECT_F tile = _map->get_tiles()[tileIndex2[0]]->getRectangle();

						collision.left = tile.right;
						collision.right = collision.left + TILESIZE;

						collision.top = tile.bottom;
						collision.bottom = collision.top + TILESIZE;

						_player->set_y((collision.top + collision.bottom) / 2);
					}
					else
					{
						collision.left = tile.right;
						collision.right = collision.left + TILESIZE;
					}

					_player->set_x((collision.left + collision.right) / 2);
				}
				else if (attribute[1] == tile_Attribute::WALL)
				{
					collision.top = tile.bottom;
					collision.bottom = collision.top + TILESIZE;

					_player->set_y((collision.top + collision.bottom) / 2);
				}
				else if (temp.right - temp.left > temp.bottom - temp.top)
				{
					collision.top = tile.bottom;
					collision.bottom = collision.top + TILESIZE;

					_player->set_y((collision.top + collision.bottom) / 2);
				}
				else if (temp.right - temp.left < temp.bottom - temp.top)
				{
					collision.left = tile.right;
					collision.right = collision.left + TILESIZE;

					_player->set_x((collision.left + collision.right) / 2);
				}
				break;

			case Player_Direction::RIGHT_TOP:

				if (attribute[2] == tile_Attribute::WALL)
				{
					if (attribute[1] == tile_Attribute::WALL)
					{
						D2D1_RECT_F tile = _map->get_tiles()[tileIndex2[0]]->getRectangle();

						collision.right = tile.left;
						collision.left = collision.right - 40;

						collision.top = tile.bottom;
						collision.bottom = collision.top + 40;

						_player->set_y((collision.top + collision.bottom) / 2);

					}
					else
					{
						collision.right = tile.left;
						collision.left = collision.right - TILESIZE;
					}

					_player->set_x((collision.left + collision.right) / 2);
				}
				else if (attribute[1] == tile_Attribute::WALL)
				{
					collision.top = tile.bottom;
					collision.bottom = collision.top + TILESIZE;

					_player->set_y((collision.top + collision.bottom) / 2);
				}
				else if (temp.right - temp.left > temp.bottom - temp.top)
				{
					collision.top = tile.bottom;
					collision.bottom = collision.top + TILESIZE;

					_player->set_y((collision.top + collision.bottom) / 2);
				}
				else if (temp.right - temp.left < temp.bottom - temp.top)
				{
					collision.right = tile.left;
					collision.left = collision.right - TILESIZE;

					_player->set_x((collision.left + collision.right) / 2);
				}
				break;

			case Player_Direction::LEFT_BOTTOM:
				if (i == 0)
				{
					collision.left = tile.right;
					collision.right = collision.left + TILESIZE;

					_player->set_x((collision.left + collision.right) / 2);
				}
				else
				{
					collision.bottom = tile.top;
					collision.top = collision.bottom - TILESIZE;

					_player->set_y((collision.top + collision.bottom) / 2);
				}
				break;

			case Player_Direction::RIGHT_BOTTOM:
				if (i == 0)
				{
					collision.right = tile.left;
					collision.left = collision.right - TILESIZE;

					_player->set_x((collision.left + collision.right) / 2);

				}
				else
				{
					collision.bottom = tile.top;
					collision.top = collision.bottom - TILESIZE;

					_player->set_y((collision.top + collision.bottom) / 2);
				}
				break;
			}
		}
	}
	_player->set_rect();
}

void collision::player_AND_tile()
{
	int player_tilex = _player->get_playerRect().left / TILESIZE;
	int player_tiley = _player->get_playerRect().top / TILESIZE;

	D2D1_RECT_F player = _map->get_tiles()[player_tilex + player_tiley * mapTileX]->getRectangle();

	if (current_tile != NULL)
	{
		D2D1_RECT_F tile = current_tile->getRectangle();

		left_tile = _map->get_tiles()[current_tile->getIdX() - 1 + current_tile->getIdY() * mapTileX];
		right_tile = _map->get_tiles()[current_tile->getIdX() + 1 + current_tile->getIdY() * mapTileX];
		top_tile = _map->get_tiles()[current_tile->getIdX() + (current_tile->getIdY() - 1) * mapTileX];
		bottom_tile = _map->get_tiles()[current_tile->getIdX() + (current_tile->getIdY() + 1) * mapTileX];

		float player_x = (player.left + player.right) / 2;
		float player_y = (player.top + player.bottom) / 2;

		float tile_x = (tile.left + tile.right) / 2;
		float tile_y = (tile.top + tile.bottom) / 2;

		// 밭 타일에서 오른쪽 버튼을 꾹 누를시, 곡괭이로 땅을 일구어서 비옥하게 만듬
		if (getDistance(player_x, player_y, tile_x, tile_y) <= TILESIZE * 1.5 && current_tile->getAttribute() == tile_Attribute::FARM)
		{
			on_tile = true;
			if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
			{
				if (_player->get_complete())
				{
					current_tile->setAttribute(tile_Attribute::FERTILE);
					current_tile->set_objectImage(IMAGEMANAGER->findImage("fertile_soil"));
					current_tile->setObjectIndex(1, 1);
					current_tile->set_Objectpage(0);

					if (left_tile->get_Objectpage() == -1)
					{
						left_tile->set_objectImage(IMAGEMANAGER->findImage("soil_overflow_left"));
						left_tile->setObjectIndex(0, 0);
						left_tile->set_Objectpage(1);
					}
					if (right_tile->get_Objectpage() == -1)
					{
						right_tile->set_objectImage(IMAGEMANAGER->findImage("soil_overflow_right"));
						right_tile->setObjectIndex(0, 0);
						right_tile->set_Objectpage(1);
					}
					if (top_tile->get_Objectpage() == -1)
					{
						top_tile->set_objectImage(IMAGEMANAGER->findImage("soil_overflow_top"));
						top_tile->setObjectIndex(0, 0);
						top_tile->set_Objectpage(1);
					}
					if (bottom_tile->get_Objectpage() == -1)
					{
						bottom_tile->set_objectImage(IMAGEMANAGER->findImage("soil_overflow_bottom"));
						bottom_tile->setObjectIndex(0, 0);
						bottom_tile->set_Objectpage(1);
					}

					_player->set_complete(false);
				}
				else
				{
					_player->set_state(Player_State::SHOVELING);
					_player->animation_setting();
				}

			}
		}

		// 밭의 근접한 타일을 검사하여서, 곡괭이로 땅을 일구어서 새로운 밭 타일을 만듬
		else if (getDistance(player_x, player_y, tile_x, tile_y) <= TILESIZE * 1.5 && current_tile->getAttribute() == tile_Attribute::GROUND)
		{
			if (left_tile->getAttribute() == tile_Attribute::FARM || right_tile->getAttribute() == tile_Attribute::FARM ||
				top_tile->getAttribute() == tile_Attribute::FARM || bottom_tile->getAttribute() == tile_Attribute::FARM ||
				left_tile->getAttribute() == tile_Attribute::FERTILE || right_tile->getAttribute() == tile_Attribute::FERTILE ||
				top_tile->getAttribute() == tile_Attribute::FERTILE || bottom_tile->getAttribute() == tile_Attribute::FERTILE)
			{
				if (_player->get_playerResource().pickaxe > 0)
				{
					on_tile = true;

					if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
					{
						if (_player->get_complete())
						{
							current_tile->setAttribute(tile_Attribute::FARM);
							current_tile->set_image(IMAGEMANAGER->findImage("tileset"));
							current_tile->setTileIndex(13, 8);
							_player->set_pickaxe(-1);
							_player->set_complete(false);
						}
						else
						{
							_player->set_state(Player_State::SHOVELING);
							_player->animation_setting();
						}

					}
				}
			}
		}

		// 일군땅에 아무것도 없다면, 농작물을 심는다.
		else if (getDistance(player_x, player_y, tile_x, tile_y) <= TILESIZE * 1.5 && current_tile->getAttribute() == tile_Attribute::FERTILE)
		{
			if (_UI->get_currentItem().quantity > 0)
			{
				on_tile = true;

				if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
				{
					if (current_tile->get_Objectpage() == 0)

						if (_player->get_complete())
						{
							current_tile->set_Objectpage(1);
							_player->get_playerResource().inventory[_UI->get_currentItemIndex()]->set_quantity(-1);
							_player->set_complete(false);

							switch (_UI->get_currentItem().kind)
							{
							case Crop_Kind::BEAN:
								_crop->set_bean(tile_x, tile_y, current_tile->getIdX(), current_tile->getIdY());
								break;

							case Crop_Kind::CABBAGE:
								_crop->set_cabbage(tile_x, tile_y, current_tile->getIdX(), current_tile->getIdY());
								break;

							case Crop_Kind::POTATO:
								_crop->set_potato(tile_x, tile_y, current_tile->getIdX(), current_tile->getIdY());
								break;

							case Crop_Kind::HEART:
								_crop->set_heart(tile_x, tile_y, current_tile->getIdX(), current_tile->getIdY());
								break;

							case Crop_Kind::PUMPKIN:
								_crop->set_pumpkin(tile_x, tile_y, current_tile->getIdX(), current_tile->getIdY());
								break;
							}
						}
						else
						{
							_player->set_state(Player_State::SHOVELING);
							_player->animation_setting();
						}
				}
			}
		}
		else
		{
			on_tile = false;
		}

		if (KEYMANAGER->isOnceKeyUp(VK_RBUTTON))
		{
			on_tile = false;
			_player->set_state(Player_State::IDLE);
			_player->animation_setting();
			_player->set_complete(false);
		}
	}

	if (_player->get_playerInfo().state != Player_State::SHOVELING) 
		current_tile = tile_onMouse;
}

void collision::player_AND_enemy()
{
	D2D1_RECT_F player = _player->get_playerInfo().rc;

	// 플레이어 불렛과 에너미 충돌
	for (int i = 0; i < _player->get_bullet()->getVBullet().size(); i++)
	{
		D2D1_RECT_F bullet = _player->get_bullet()->getVBullet()[i].rc;

		for (int j = 0; j < _enemy->get_enemy().size(); j++)
		{
			D2D1_RECT_F enemy = _enemy->get_enemy()[j]->get_rect();

			if (INTERSECTION_RECT(temp, enemy, bullet))
			{
				_enemy->get_enemy()[j]->set_hit(true);
				_enemy->get_enemy()[j]->set_angle(_player->get_bullet()->getVBullet()[i].angle);
				_enemy->get_enemy()[j]->set_hp(-_player->get_playerStatus().damage);
				_player->get_bullet()->removeBullet(i);
				break;
			}
		}
	}

	// 플레이어 다람쥐 불렛과 에너미 충돌
	for (int i = 0; i < _player->get_squirrel()->getVBullet().size(); i++)
	{
		D2D1_RECT_F bullet = _player->get_squirrel()->getVBullet()[i].rc;

		for (int j = 0; j < _enemy->get_enemy().size(); j++)
		{
			D2D1_RECT_F enemy = _enemy->get_enemy()[j]->get_rect();

			if (INTERSECTION_RECT(temp, enemy, bullet))
			{
				_enemy->get_enemy()[j]->set_hit(true);
				_enemy->get_enemy()[j]->set_angle(_player->get_squirrel()->getVBullet()[i].angle);
				_enemy->get_enemy()[j]->set_hp(-_player->get_playerStatus().damage);
				_player->get_squirrel()->removeBullet(i);
				break;
			}
		}
	}

	for (int i = 0; i < _enemy->get_enemy().size(); i++)
	{
		D2D1_RECT_F enemy = _enemy->get_enemy()[i]->get_rect();

		if (_enemy->get_enemy()[i]->get_kind() == enemy_kind::BOSS) continue;

		if (_enemy->get_enemy()[i]->get_hp() < 0)
		{
			if(_enemy->get_enemy()[i]->get_kind() == enemy_kind::WORM)
				EFFECTMANAGER->play("monster_dead_small", (enemy.left + enemy.right) / 2, enemy.bottom);
			else
				EFFECTMANAGER->play("monster_dead", (enemy.left + enemy.right) / 2, enemy.bottom);
			
			_item->randomItemDrop(enemy.left, enemy.top);
			_enemy->remove_enemy(i);
		}
	}

	// 에너미 불렛과 플레이어 충돌
	for (int i = 0; i < _enemy->get_enemy().size(); i++)
	{
		for (int j = 0; j < _enemy->get_enemy()[i]->get_basic_bullet()->getVBullet().size(); j++)
		{
			D2D1_RECT_F bullet = _enemy->get_enemy()[i]->get_basic_bullet()->getVBullet()[j].rc;

			if (INTERSECTION_RECT(temp, player, bullet))
			{
				_enemy->get_enemy()[i]->get_basic_bullet()->removeBullet(j);
				_player->set_hp(-1);
				break;
			}
		}

		if (_enemy->get_enemy()[i]->get_kind() != enemy_kind::BOSS) continue;

		for (int j = 0; j < _enemy->get_enemy()[i]->get_BossBullet()->getVBullet().size(); j++)
		{
			D2D1_RECT_F bullet = _enemy->get_enemy()[i]->get_BossBullet()->getVBullet()[j].rc;

			if (INTERSECTION_RECT(temp, player, bullet))
			{
				_enemy->get_enemy()[i]->get_BossBullet()->removeBullet(j);
				_player->set_hp(-1);
				break;
			}
		}

	}
}

void collision::player_AND_object()
{
	D2D1_RECT_F player = _player->get_playerRect();

	for (int i = 0; i < _object->get_objectList().size(); i++)
	{
		if (_object->get_objectList()[i]->get_type() != object_type::WELL) continue;

		D2D1_RECT_F Object = _object->get_objectList()[i]->get_rect();

		if (INTERSECTION_RECT(temp, player, Object))
		{
			if (_player->get_water() != _player->get_playerStatus().max_water && _object->get_objectList()[i]->get_ready())
			{
				_object->get_objectList()[i]->set_touched(true);
				_player->set_water(_player->get_playerStatus().max_water);
				break;
			}
			else
				_object->get_objectList()[i]->set_touched(false);
		}
	}

	for (int i = 0; i < _object->get_objectList().size(); i++)
	{
		if (_object->get_objectList()[i]->get_type() == object_type::PLANE) continue;

		D2D1_RECT_F Object = _object->get_objectList()[i]->get_rect();

		if (INTERSECTION_RECT(temp, player, Object))
		{
			if ((temp.right - temp.left >= temp.bottom - temp.top) && player.top < Object.top)
			{
				player.bottom = Object.top;
				player.top = player.bottom - TILESIZE;

				_player->set_y((player.top + player.bottom) / 2);
			}

			else if ((temp.right - temp.left >= temp.bottom - temp.top) && player.top >= Object.top)
			{
				player.top = Object.bottom;
				player.bottom = player.top + TILESIZE;

				_player->set_y((player.top + player.bottom) / 2);
			}

			else if ((temp.right - temp.left < temp.bottom - temp.top) && player.left < Object.left)
			{
				player.right = Object.left;
				player.left = player.right - TILESIZE;

				_player->set_x((player.left + player.right) / 2);
			}

			else if ((temp.right - temp.left < temp.bottom - temp.top) && player.left > Object.left)
			{
				player.left = Object.right;
				player.right = player.left + TILESIZE;

				_player->set_x((player.left + player.right) / 2);
			}
			_player->set_rect();
		}
	}
}

void collision::player_AND_crops()
{
	D2D1_RECT_F player = _player->get_playerInfo().rc;

	for (int i = 0; i < _crop->get_cropList().size(); i++)
	{
		D2D1_RECT_F crop = _crop->get_cropList()[i]->get_rect();

		if (INTERSECTION_RECT(temp, player, crop))
		{
			// 농작물 위에 물 아이콘 표시가 있고, 플레이어가 물을 소지하고 있을시 물을 줍니다.
			if (_crop->get_cropList()[i]->get_waterNeed() && _player->get_water() != 0)
			{
				_player->set_water(_player->get_water() - 1);
				_crop->get_cropList()[i]->set_waterNeed(false);
			}

			// 농작물이 수확 가능한 상태이고 플레이어가 마우스 휠을 눌렀을시 수확합니다.
			if (_crop->get_cropList()[i]->get_stage() == Crop_stage::HARVEST || 
				_crop->get_cropList()[i]->get_stage() == Crop_stage::LARGE_HARVEST)
			{
				if (KEYMANAGER->isOnceKeyDown(VK_MBUTTON))
				{
					int tile_idx = _crop->get_cropList()[i]->get_idX();
					int tile_idy = _crop->get_cropList()[i]->get_idY();

					_player->set_gold(_crop->get_cropList()[i]->get_price());

					if (_crop->get_cropList()[i]->get_stage() == Crop_stage::HARVEST)
					{
						_fontEffect->fire(crop.left, crop.bottom, _crop->get_cropList()[i]->get_price());
						_map->get_tiles()[tile_idx + tile_idy * mapTileX]->set_objectImage(NULL);
						_map->get_tiles()[tile_idx + tile_idy * mapTileX]->setAttribute(tile_Attribute::FARM);
						_map->get_tiles()[tile_idx + tile_idy * mapTileX]->set_Objectpage(-1);

						if (_crop->get_cropList()[i]->get_kind() == Crop_Kind::HEART)
							_player->set_hp(1);
					}
					else
					{
						_fontEffect->fire((crop.left + crop.right) / 2, crop.bottom, _crop->get_cropList()[i]->get_price());
						_map->get_tiles()[tile_idx + tile_idy * mapTileX]->set_objectImage(NULL);
						_map->get_tiles()[tile_idx + 1 + tile_idy * mapTileX]->set_objectImage(NULL);
						_map->get_tiles()[tile_idx + (tile_idy + 1) * mapTileX]->set_objectImage(NULL);
						_map->get_tiles()[tile_idx + 1 + (tile_idy + 1) * mapTileX]->set_objectImage(NULL);

						_map->get_tiles()[tile_idx + tile_idy * mapTileX]->set_Objectpage(-1);
						_map->get_tiles()[tile_idx + 1 + tile_idy * mapTileX]->set_Objectpage(-1);
						_map->get_tiles()[tile_idx + (tile_idy + 1) * mapTileX]->set_Objectpage(-1);
						_map->get_tiles()[tile_idx + 1 + (tile_idy + 1) * mapTileX]->set_Objectpage(-1);

						_map->get_tiles()[tile_idx + tile_idy * mapTileX]->setAttribute(tile_Attribute::FARM);
						_map->get_tiles()[tile_idx + 1 + tile_idy * mapTileX]->setAttribute(tile_Attribute::FARM);
						_map->get_tiles()[tile_idx + (tile_idy + 1) * mapTileX]->setAttribute(tile_Attribute::FARM);
						_map->get_tiles()[tile_idx + 1 + (tile_idy + 1) * mapTileX]->setAttribute(tile_Attribute::FARM);

						if (_crop->get_cropList()[i]->get_kind() == Crop_Kind::HEART)
							_player->set_Maxhp(1);
					}

					_crop->add_harvestedCrop(_crop->get_cropList()[i]);

					if (_crop->get_cropList()[i]->get_stage() == Crop_stage::HARVEST)
						_crop->get_harvestList().back()->get_harvest()->fire((crop.left + crop.right) / 2 , (crop.top + crop.bottom) / 2);
					else
						_crop->get_harvestList().back()->get_harvest()->fire((crop.left + crop.right) / 2 + 20, (crop.top + crop.bottom) / 2);
					
					_crop->remove_crop(i);
				}
			}
		}
	}
}

// 아이템과 플레이어와의 충돌관리
void collision::player_AND_Item()
{
	D2D1_RECT_F player = _player->get_playerRect();
	float player_x = (player.left + player.right) / 2;
	float player_y = (player.top + player.bottom) / 2;

	// 필드에 드랍되는 아이템과 충돌하여 아이템 습득관리 함수
	for (int i = 0; i < _item->get_itemList().size(); i++)
	{
		// 필드에 드랍된 아이템이 아니라면 생략
		if (_item->get_itemList()[i]->get_itemObject() == NULL) continue;

		// 판매되는 상점 아이템이라면 아래 생략
		if (_item->get_itemList()[i]->get_isSelling() == true) continue;

		ItemObject* _ItemObject = _item->get_itemList()[i]->get_itemObject();

		// 아이템과 플레이어과 충돌했다면
		if (INTERSECTION_RECT(temp, player, _ItemObject->rc))
		{
			if (_item->get_itemList()[i]->get_itemType() == ItemType::PICKAXE)
			{
				_player->set_pickaxe(1);
				_item->get_itemList()[i]->removeObject();
				continue;
			}

			if (_item->get_itemList()[i]->get_itemType() == ItemType::FERTILIZER)
			{
				_player->set_fertilizer(1);
				_item->get_itemList()[i]->removeObject();
				continue;
			}

			if (_player->get_playerResource().inventory.size() == 0)
			{
				Item* newItem = NULL;

				switch (_item->get_itemList()[i]->get_seedKind())
				{
				case Crop_Kind::BEAN:
					newItem = new seed_bean;
					newItem->init();
					newItem->set_quantity(RND->getFromIntTo(1, 2));
					_player->get_playerResource().inventory.push_back(newItem);
					break;

				case Crop_Kind::CABBAGE:
					newItem = new seed_cabbage;
					newItem->init();
					newItem->set_quantity(RND->getFromIntTo(1, 2));
					_player->get_playerResource().inventory.push_back(newItem);
					break;

				case Crop_Kind::POTATO:
					newItem = new seed_potato;
					newItem->init();
					newItem->set_quantity(RND->getFromIntTo(1, 2));
					_player->get_playerResource().inventory.push_back(newItem);
					break;

				case Crop_Kind::HEART:
					newItem = new seed_heart;
					newItem->init();
					newItem->set_quantity(1);
					_player->get_playerResource().inventory.push_back(newItem);
					break;

				case Crop_Kind::PUMPKIN:
					newItem = new seed_pumpkin;
					newItem->init();
					newItem->set_quantity(RND->getFromIntTo(1, 2));
					_player->get_playerResource().inventory.push_back(newItem);
					break;
				}
				_item->get_itemList()[i]->removeObject();
				continue;
			}

			bool is_obtained = false;
			// 플레이어의 인벤토리에서 동일한 아이템이 있는지 찾는다
			for (int j = 0; j < _player->get_playerResource().inventory.size(); j++)
			{
				// 획득한 아이템이 인벤토리에 있는경우, 수량만 + 1 ~ 2
				if (_item->get_itemList()[i]->get_seedKind() == _player->get_playerResource().inventory[j]->get_seedKind())
				{
					_player->get_playerResource().inventory[j]->set_quantity(RND->getFromIntTo(1, 2));
					is_obtained = true;
				}
				else if(j == _player->get_playerResource().inventory.size() - 1 && !is_obtained)  // 아니라면 새롭게 추가해준다.
				{
					Item* newItem = NULL;

					switch (_item->get_itemList()[i]->get_seedKind())
					{
					case Crop_Kind::BEAN:
						newItem = new seed_bean;
						newItem->init();
						newItem->set_quantity(RND->getFromIntTo(1, 2));
						_player->get_playerResource().inventory.push_back(newItem);
						is_obtained = true;
						break;

					case Crop_Kind::CABBAGE:
						newItem = new seed_cabbage;
						newItem->init();
						newItem->set_quantity(RND->getFromIntTo(1, 2));
						_player->get_playerResource().inventory.push_back(newItem);
						is_obtained = true;
						break;

					case Crop_Kind::POTATO:
						newItem = new seed_potato;
						newItem->init();
						newItem->set_quantity(RND->getFromIntTo(1, 2));
						_player->get_playerResource().inventory.push_back(newItem);
						is_obtained = true;
						break;

					case Crop_Kind::HEART:
						newItem = new seed_heart;
						newItem->init();
						newItem->set_quantity(1);
						_player->get_playerResource().inventory.push_back(newItem);
						is_obtained = true;
						break;

					case Crop_Kind::PUMPKIN:
						newItem = new seed_pumpkin;
						newItem->init();
						newItem->set_quantity(RND->getFromIntTo(1, 2));
						_player->get_playerResource().inventory.push_back(newItem);
						is_obtained = true;
						break;
					}
				}
				if (is_obtained) break;
			}
			_item->get_itemList()[i]->removeObject();
		}
	}


	// 상점에서 아이템과 플레이어의 상호작용을 관리하는 함수
	for (int i = 0; i < _item->get_itemList().size(); i++)
	{
		// 판매되는 상점 아이템이 아니라면 아래 생략
		if (_item->get_itemList()[i]->get_isSelling() != true) continue;
		if (_item->get_itemList()[i]->get_itemObject() == NULL) continue;

		ItemObject* _ItemObject = _item->get_itemList()[i]->get_itemObject();
		float item_x = (_ItemObject->rc.left + _ItemObject->rc.right) / 2;
		float item_y = (_ItemObject->rc.top + _ItemObject->rc.bottom) / 2;

		// 아이템과 가까이가면 아이템 설명 UI가 뜨도록 설정합니다.
		if (getDistance(player_x, player_y, item_x, item_y) < 76)
		{
			if(player_x > _ItemObject->rc.left && player_x < _ItemObject->rc.right)
				_item->get_itemList()[i]->set_isClosed(true);
			else
				_item->get_itemList()[i]->set_isClosed(false);
		}
		else
			_item->get_itemList()[i]->set_isClosed(false);

		bool is_obtained = false;

		if (_item->get_itemList()[i]->get_isClosed() && _player->get_playerResource().gold >= _item->get_itemList()[i]->get_price())
		{
			if (KEYMANAGER->isOnceKeyDown('F'))
			{
				_player->set_gold(-_item->get_itemList()[i]->get_price());

				if (_item->get_itemList()[i]->get_itemType() == ItemType::PICKAXE)
				{
					_player->set_pickaxe(1);
					_item->remove_item(i);
					continue;
				}

				if (_item->get_itemList()[i]->get_itemType() == ItemType::FERTILIZER)
				{
					_player->set_fertilizer(1);
					_item->remove_item(i);
					continue;
				}

				if (_item->get_itemList()[i]->get_itemType() == ItemType::EQUIPMENT)
				{
					_player->set_gunType(_item->get_itemList()[i]->get_gunType());
					_item->remove_item(i);
					continue;
				}

				if (_player->get_playerResource().inventory.size() == 0) // 아니라면 새롭게 추가해준다.
				{
					Item* newItem = NULL;

					switch (_item->get_itemList()[i]->get_seedKind())
					{
					case Crop_Kind::BEAN:
						newItem = new seed_bean;
						newItem->init();
						newItem->set_quantity(2);
						_player->get_playerResource().inventory.push_back(newItem);
						is_obtained = true;
						break;

					case Crop_Kind::CABBAGE:
						newItem = new seed_cabbage;
						newItem->init();
						newItem->set_quantity(2);
						_player->get_playerResource().inventory.push_back(newItem);
						is_obtained = true;
						break;

					case Crop_Kind::POTATO:
						newItem = new seed_potato;
						newItem->init();
						newItem->set_quantity(2);
						_player->get_playerResource().inventory.push_back(newItem);
						is_obtained = true;
						break;

					case Crop_Kind::HEART:
						newItem = new seed_heart;
						newItem->init();
						newItem->set_quantity(1);
						_player->get_playerResource().inventory.push_back(newItem);
						is_obtained = true;
						break;

					case Crop_Kind::PUMPKIN:
						newItem = new seed_pumpkin;
						newItem->init();
						newItem->set_quantity(2);
						_player->get_playerResource().inventory.push_back(newItem);
						is_obtained = true;
						break;
					}
					_item->remove_item(i);
					continue;
				}

				// 플레이어의 인벤토리에서 동일한 아이템이 있는지 찾는다
				for (int j = 0; j < _player->get_playerResource().inventory.size(); j++)
				{
					// 획득한 아이템이 인벤토리에 있는경우, 수량만 + 1 ~ 2
					if (_item->get_itemList()[i]->get_seedKind() == _player->get_playerResource().inventory[j]->get_seedKind())
					{
						_player->get_playerResource().inventory[j]->set_quantity(2);
						is_obtained = true;
					}
					else if (j == _player->get_playerResource().inventory.size() - 1 && !is_obtained) // 아니라면 새롭게 추가해준다.
					{
						Item* newItem = NULL;

						switch (_item->get_itemList()[i]->get_seedKind())
						{
						case Crop_Kind::BEAN:
							newItem = new seed_bean;
							newItem->init();
							newItem->set_quantity(2);
							_player->get_playerResource().inventory.push_back(newItem);
							is_obtained = true;
							break;

						case Crop_Kind::CABBAGE:
							newItem = new seed_cabbage;
							newItem->init();
							newItem->set_quantity(2);
							_player->get_playerResource().inventory.push_back(newItem);
							is_obtained = true;
							break;

						case Crop_Kind::POTATO:
							newItem = new seed_potato;
							newItem->init();
							newItem->set_quantity(2);
							_player->get_playerResource().inventory.push_back(newItem);
							is_obtained = true;
							break;

						case Crop_Kind::HEART:
							newItem = new seed_heart;
							newItem->init();
							newItem->set_quantity(1);
							_player->get_playerResource().inventory.push_back(newItem);
							is_obtained = true;
							break;

						case Crop_Kind::PUMPKIN:
							newItem = new seed_pumpkin;
							newItem->init();
							newItem->set_quantity(2);
							_player->get_playerResource().inventory.push_back(newItem);
							is_obtained = true;
							break;
						}
					}
					if (is_obtained) break;
				}
				_item->remove_item(i);
			}
		}
	}
}

void collision::enemy_AND_CrossTiles()
{
	for (int i = 0; i < _enemy->get_enemy().size(); i++)
	{
		if (_enemy->get_enemy()[i]->get_kind() == enemy_kind::BOSS) continue;
		if (_enemy->get_enemy()[i]->get_kind() == enemy_kind::MAPLE) continue;

		enemy* enemy = _enemy->get_enemy()[i];

		D2D1_RECT_F collision;

		int tileIndex[2];	//타일 검출에 필요한 인덱스
		int tileX, tileY;	//플레이어가 밟고 있는 타일의 인덱스

		//가상의 판정렉트에 현재 렉트를 대입해주자
		collision = enemy->get_rect();

		tileX = collision.left / TILESIZE;
		tileY = collision.top / TILESIZE;

		switch (enemy->get_movingDirection())
		{
		case enemy_direction::TOP:
			tileIndex[0] = tileX + tileY * mapTileX;
			tileIndex[1] = (tileX + 1) + tileY * mapTileX;
			break;
		case enemy_direction::BOTTOM:
			tileIndex[0] = (tileX + tileY * mapTileX) + mapTileX;
			tileIndex[1] = (tileX + 1 + tileY * mapTileX) + mapTileX;
			break;
		case enemy_direction::LEFT:
			tileIndex[0] = tileX + (tileY * mapTileX);
			tileIndex[1] = tileX + (tileY + 1) * mapTileX;
			break;
		case enemy_direction::RIGHT:
			tileIndex[0] = (tileX + tileY * mapTileX) + 1;
			tileIndex[1] = (tileX + (tileY + 1) * mapTileX) + 1;
			break;
		default:
			tileIndex[0] = NULL;
			tileIndex[1] = NULL;
			break;
		}

		for (int j = 0; j < 2; ++j)
		{
			if (tileIndex[j] == NULL) break;

			D2D1_RECT_F rc;
			D2D1_RECT_F tile = _map->get_tiles()[tileIndex[j]]->getRectangle();
			tile_Attribute attribute = _map->get_tiles()[tileIndex[j]]->getAttribute();

			if (attribute == tile_Attribute::WALL || attribute == tile_Attribute::WATER_COL || attribute == tile_Attribute::WATER_ROW)
				attribute = tile_Attribute::WALL;

			if (attribute == tile_Attribute::WALL && INTERSECTION_RECT(rc, tile, collision))
			{
				if (rc.right - rc.left >= rc.bottom - rc.top && collision.top < tile.top)
				{
					collision.bottom = tile.top;
					collision.top = collision.bottom - TILESIZE;

					_enemy->get_enemy()[i]->set_y((collision.top + collision.bottom) / 2);
				}
				else if (rc.right - rc.left >= rc.bottom - rc.top && collision.top >= tile.top)
				{
					collision.top = tile.bottom;
					collision.bottom = collision.top + TILESIZE;

					_enemy->get_enemy()[i]->set_y((collision.top + collision.bottom) / 2);
				}
				else if (rc.right - rc.left < rc.bottom - rc.top && collision.left < tile.left)
				{
					collision.right = tile.left;
					collision.left = collision.right - TILESIZE;

					_enemy->get_enemy()[i]->set_x((collision.left + collision.right) / 2);
				}
				else if (rc.right - rc.left < rc.bottom - rc.top && collision.left > tile.left)
				{
					collision.left = tile.right;
					collision.right = collision.left + TILESIZE;

					_enemy->get_enemy()[i]->set_x((collision.left + collision.right) / 2);
				}
			}
			break;
		}
		_enemy->get_enemy()[i]->set_rect();
	}
}

void collision::enemy_AND_diagonalTiles()
{
	for (int j = 0; j < _enemy->get_enemy().size(); j++)
	{
		if (_enemy->get_enemy()[j]->get_kind() == enemy_kind::BOSS) continue;
		if (_enemy->get_enemy()[j]->get_kind() == enemy_kind::MAPLE) continue;

		D2D1_RECT_F collision = { _enemy->get_enemy()[j]->get_rect()};

		int tileIndex[3];	//타일 검출에 필요한 인덱스
		int tileX, tileY;	//플레이어가 밟고 있는 타일의 인덱스

		//가상의 판정렉트에 현재 렉트를 대입해주자
		enemy* enemy = _enemy->get_enemy()[j];
		collision = _enemy->get_enemy()[j]->get_rect();

		tileX = collision.left / TILESIZE;
		tileY = collision.top / TILESIZE;

		switch (enemy->get_movingDirection())
		{
		case enemy_direction::LEFT_TOP:
			tileIndex[0] = tileX + (tileY * mapTileX);
			tileIndex[2] = tileX + (tileY + 1) * mapTileX;
			tileIndex[1] = (tileX + 1) + tileY * mapTileX;
			break;
		case enemy_direction::RIGHT_TOP:
			tileIndex[0] = (tileX + tileY * mapTileX) + 1;
			tileIndex[2] = (tileX + (tileY + 1) * mapTileX) + 1;
			tileIndex[1] = (tileX)+tileY * mapTileX;
			break;
		case enemy_direction::LEFT_BOTTOM:
			tileIndex[0] = tileX + (tileY * mapTileX);
			tileIndex[1] = tileX + (tileY + 1) * mapTileX;
			tileIndex[2] = (tileX + 1) + (tileY + 1) * mapTileX;
			break;
		case enemy_direction::RIGHT_BOTTOM:
			tileIndex[0] = (tileX + tileY * mapTileX) + 1;
			tileIndex[1] = (tileX + (tileY + 1) * mapTileX) + 1;
			tileIndex[2] = (tileX)+ (tileY + 1) * mapTileX;
			break;
		default:
			tileIndex[0] = NULL;
			tileIndex[1] = NULL;
			tileIndex[2] = NULL;
			break;
		}

		tile_Attribute attribute[3] = { _map->get_tiles()[tileIndex[0]]->getAttribute(), 
										_map->get_tiles()[tileIndex[1]]->getAttribute(), 
										_map->get_tiles()[tileIndex[2]]->getAttribute() };

		for (int i = 0; i < 3; ++i)
		{
			if (tileIndex[i] == NULL) break;

			D2D1_RECT_F temp;
			D2D1_RECT_F tile = _map->get_tiles()[tileIndex[i]]->getRectangle();
			tile_Attribute _attribute = _map->get_tiles()[tileIndex[i]]->getAttribute();

			if (attribute[i] == tile_Attribute::WALL || attribute[i] == tile_Attribute::WATER_COL || attribute[i] == tile_Attribute::WATER_ROW)
				attribute[i] = tile_Attribute::WALL;

			if (_attribute != tile_Attribute::WALL && _attribute != tile_Attribute::WATER_COL && _attribute != tile_Attribute::WATER_ROW) continue;

			if (INTERSECTION_RECT(temp, tile, collision))
			{
				_enemy->get_enemy()[j]->set_isWall(true);
				switch (enemy->get_movingDirection())
				{
				case enemy_direction::LEFT_TOP:

					if (attribute[2] == tile_Attribute::WALL)
					{
						if (attribute[1] == tile_Attribute::WALL)
						{
							D2D1_RECT_F tile = _map->get_tiles()[tileIndex[0]]->getRectangle();

							collision.left = tile.right;
							collision.right = collision.left + TILESIZE;

							collision.top = tile.bottom;
							collision.bottom = collision.top + TILESIZE;

							_enemy->get_enemy()[j]->set_y((collision.top + collision.bottom) / 2);
						}
						else
						{
							collision.left = tile.right;
							collision.right = collision.left + TILESIZE;
						}

						_enemy->get_enemy()[j]->set_x((collision.left + collision.right) / 2);
					}
					else if (attribute[1] == tile_Attribute::WALL)
					{
						collision.top = tile.bottom;
						collision.bottom = collision.top + TILESIZE;

						_enemy->get_enemy()[j]->set_y((collision.top + collision.bottom) / 2);
					}
					else if (temp.right - temp.left > temp.bottom - temp.top)
					{
						collision.top = tile.bottom;
						collision.bottom = collision.top + TILESIZE;

						_enemy->get_enemy()[j]->set_y((collision.top + collision.bottom) / 2);
					}
					else if (temp.right - temp.left < temp.bottom - temp.top)
					{
						collision.left = tile.right;
						collision.right = collision.left + TILESIZE;

						_enemy->get_enemy()[j]->set_x((collision.left + collision.right) / 2);
					}
					break;

				case enemy_direction::RIGHT_TOP:

					if (attribute[2] == tile_Attribute::WALL)
					{
						if (attribute[1] == tile_Attribute::WALL)
						{
							D2D1_RECT_F tile = _map->get_tiles()[tileIndex[0]]->getRectangle();

							collision.right = tile.left;
							collision.left = collision.right - 40;

							collision.top = tile.bottom;
							collision.bottom = collision.top + 40;

							_enemy->get_enemy()[j]->set_y((collision.top + collision.bottom) / 2);

						}
						else
						{
							collision.right = tile.left;
							collision.left = collision.right - TILESIZE;
						}

						_enemy->get_enemy()[j]->set_x((collision.left + collision.right) / 2);
					}
					else if (attribute[1] == tile_Attribute::WALL)
					{
						collision.top = tile.bottom;
						collision.bottom = collision.top + TILESIZE;

						_enemy->get_enemy()[j]->set_y((collision.top + collision.bottom) / 2);
					}
					else if (temp.right - temp.left > temp.bottom - temp.top)
					{
						collision.top = tile.bottom;
						collision.bottom = collision.top + TILESIZE;

						_enemy->get_enemy()[j]->set_y((collision.top + collision.bottom) / 2);
					}
					else if (temp.right - temp.left < temp.bottom - temp.top)
					{
						collision.right = tile.left;
						collision.left = collision.right - TILESIZE;

						_enemy->get_enemy()[j]->set_x((collision.left + collision.right) / 2);
					}
					break;

				case enemy_direction::LEFT_BOTTOM:
					if (i == 0)
					{
						collision.left = tile.right;
						collision.right = collision.left + TILESIZE;

						_enemy->get_enemy()[j]->set_x((collision.left + collision.right) / 2);
					}
					else
					{
						collision.bottom = tile.top;
						collision.top = collision.bottom - TILESIZE;

						_enemy->get_enemy()[j]->set_y((collision.top + collision.bottom) / 2);
					}
					break;

				case enemy_direction::RIGHT_BOTTOM:
					if (attribute[0] == tile_Attribute::WALL)
					{
						collision.right = tile.left;
						collision.left = collision.right - TILESIZE;

						_enemy->get_enemy()[j]->set_x((collision.left + collision.right) / 2);

					}
					else
					{
						collision.bottom = tile.top;
						collision.top = collision.bottom - TILESIZE;

						_enemy->get_enemy()[j]->set_y((collision.top + collision.bottom) / 2);
					}
					break;
				}
				break;
			}
		}
		_enemy->get_enemy()[j]->set_rect();
	}

}




