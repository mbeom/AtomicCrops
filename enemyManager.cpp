#include "stdafx.h"
#include "enemyManager.h"
#include "player.h"
#include "tool.h"

HRESULT enemyManager::init(int num)
{
	IMAGEMANAGER->addImage("basic_bullet", L"image/bullet/EnemyBasicBullet.png", 32, 32);
	IMAGEMANAGER->addFrameImage("monster_dead", L"image/enemy/PlaceholderCorpse.png", 440, 176, 5, 2);
	IMAGEMANAGER->addFrameImage("monster_dead_small", L"image/enemy/PlaceholderCorpseSmall.png", 440, 176, 5, 2);

	EFFECTMANAGER->addEffect("monster_dead", "monster_dead", 440, 176, 88, 88, 4.0f, 0.05f, 10);
	EFFECTMANAGER->addEffect("monster_dead_small", "monster_dead_small", 440, 176, 88, 88, 4.0f, 0.05f, 10);

	update_count = 0.0f;

	_enemy_pool.clear();
	reset_list.clear();
	_vTotalList.clear();

	_vTotalList.reserve(_map->get_tileNumX() * _map->get_tileNumY() + 1);

	for (int i = 0; i < _map->get_OnlytileInfo().size(); i++)
	{
		tile* _tile = new tile;
		_tile->init(_map->get_OnlytileInfo()[i]->getIdX(), _map->get_OnlytileInfo()[i]->getIdY());
		_tile->setAttribute(_map->get_OnlytileInfo()[i]->getAttribute());
		_vTotalList.push_back(_tile);
	}
	
	_vOpenList.clear();
	_vCloseList.clear();

	zone_num.reserve(num);

	return S_OK;
}

void enemyManager::release()
{
}

void enemyManager::update()
{
	if (update_count >= 0)
		update_count += TIMEMANAGER->getElapsedTime();

	if (update_count > 1.5f)
	{
		find_player();
		find_path();
		update_count = 0;
	}
	
	boss_only();

	for (_pool_iter = _enemy_pool.begin(); _pool_iter != _enemy_pool.end(); ++_pool_iter)
	{
		if ((*_pool_iter)->get_hit() && !(*_pool_iter)->get_find_player())
		{
			(*_pool_iter)->set_find_player(true);
			find_path();
			update_count = 0;
		}

		(*_pool_iter)->update();

		D2D1_RECT_F player = _player->get_playerRect();
		D2D1_RECT_F enemy = (*_pool_iter)->get_rect();

		if (INTERSECTION_RECT(temp, player, enemy))
		{
			(*_pool_iter)->set_arrived(true);
		}
		else
			(*_pool_iter)->set_arrived(false);
	}

	for (int i = 0; i < _enemy_pool.size(); i++)
	{
		if (_enemy_pool[i]->get_hp() <= 0)
		{
			reset();
		}

		if (_enemy_pool[i]->get_hp() < 0 && _enemy_pool[i]->get_kind() == enemy_kind::BOSS && _enemy_pool[i]->get_dead())
		{
			_enemy_pool.erase(_enemy_pool.begin() + i);
			break;
		}

		if (_enemy_pool[i]->get_groupNumber() == 1 && TIMEMANAGER->world_time > 330)
		{
			_enemy_pool.erase(_enemy_pool.begin() + i);
			break;
		}
	}
}

void enemyManager::render()
{
	for (int i = 0; i < _vCloseList.size(); i++)
	{
		WCHAR str[128];
		swprintf_s(str, L"Attribute: %d", _vCloseList[i]->getAttribute());
		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->_mybrush, L"메이플스토리", 8.0f, str, _vCloseList[i]->getRectangle().left, _vCloseList[i]->getRectangle().top + 4, _vCloseList[i]->getRectangle().left + 40, _vCloseList[i]->getRectangle().top + 40);
	}
	
	for (_pool_iter = _enemy_pool.begin(); _pool_iter != _enemy_pool.end(); ++_pool_iter)
	{
		(*_pool_iter)->render();
	}

}

void enemyManager::set_rabbits(float x, float y)
{
	enemy* _rabbit = new rabbit;
	_rabbit->init(x, y);
	_enemy_pool.push_back(_rabbit);
}

void enemyManager::set_maple(float x, float y)
{
	enemy* _enemy = new maple;
	_enemy->init(x, y);
	_enemy_pool.push_back(_enemy);
}

void enemyManager::set_scorpion(float x, float y)
{
	enemy* _enemy = new scorpion;
	_enemy->init(x, y);
	_enemy_pool.push_back(_enemy);
}

void enemyManager::set_boss(float x, float y)
{
	enemy* _enemy = new boss;
	_enemy->init(x, y);
	_enemy_pool.push_back(_enemy);
}

void enemyManager::set_rabbits(float x, float y, int groupCode)
{
	enemy* _rabbit = new rabbit;
	_rabbit->init(x, y);
	_enemy_pool.push_back(_rabbit);

	group_num.push_back(groupCode);
}

void enemyManager::set_rabbits(float x, float y, bool is_find)
{
	enemy* _rabbit = new rabbit;
	_rabbit->init(x, y, is_find);

	_enemy_pool.push_back(_rabbit);
}

void enemyManager::find_path()
{
	for (int i = 0; i < _enemy_pool.size(); i++)
	{
		if (!_enemy_pool[i]->get_find_player()) continue;
		if (_enemy_pool[i]->get_kind() == enemy_kind::BOSS) continue;

		if (_enemy_pool[i]->get_kind() == enemy_kind::MAPLE)
		{
			_enemy_pool[i]->set_destination(_player->get_playerInfo().rc);
			continue;
		}
		reset();

		int player_tileX = _player->get_playerInfo().x / TILESIZE;
		int player_tileY = _player->get_playerInfo().y / TILESIZE;

		int index = player_tileX + player_tileY * TILEX;

		_startTile = _vTotalList[index];
		_startTile->setAttribute(tile_Attribute::END);

		int enemy_tileX = _enemy_pool[i]->get_rect().left / TILESIZE;
		int enemy_tileY = _enemy_pool[i]->get_rect().top / TILESIZE;

		int enemy_index = enemy_tileX + enemy_tileY * TILEX;

		if (index == enemy_index)
		{
			reset();
			continue;
		}

		_endtile = _vTotalList[enemy_index];
		_endtile->setAttribute(tile_Attribute::START);

		_currentTile = _endtile;

		reset_list.push_back(index);
		reset_list.push_back(enemy_index);

		if (!_FindPath)
			pathFinder(_currentTile);

		if (_vCloseList.size() != 0 && _FindPath)
		{
			_enemy_pool[i]->vector_clear();
			_enemy_pool[i]->set_index(_vCloseList.size() - 1);
			_enemy_pool[i]->set_destination(_player->get_playerInfo().rc);
			for (int j = 0; j < _vCloseList.size(); j++)
			{
				D2D1_RECT_F test = _vCloseList[j]->getRectangle();
				_enemy_pool[i]->get_list().push_back(_vCloseList[j]->getRectangle());
			}
		}
		if (_vCloseList.size() == 0 && _FindPath)
		{
			_enemy_pool[i]->set_destination(_player->get_playerInfo().rc);
		}
	}
}

void enemyManager::reset()
{
	_FindPath = false;
	_vOpenList.clear();
	_vCloseList.clear();

	for (int i = 0; i < reset_list.size(); i++)
	{
		if (_vTotalList[reset_list[i]]->getAttribute() == tile_Attribute::WALL) continue;

		if (_vTotalList[reset_list[i]]->getAttribute() == tile_Attribute::FERTILE)
			_vTotalList[reset_list[i]]->setAttribute(tile_Attribute::FERTILE);

		else if (_vTotalList[reset_list[i]]->getAttribute() == tile_Attribute::FARM)
			_vTotalList[reset_list[i]]->setAttribute(tile_Attribute::FARM);

		else
			_vTotalList[reset_list[i]]->setAttribute(tile_Attribute::GROUND);

		_vTotalList[reset_list[i]]->setParentNode(NULL);
		_vTotalList[reset_list[i]]->setIsOpen(true);
	}

	reset_list.clear();
}

void enemyManager::remove_enemy(int num)
{
	_enemy_pool.erase(_enemy_pool.begin() + num);
}

void enemyManager::check_zoneEnemy()
{
	zone_num.clear();

	int code = 0;
	int count = 0;

	for (auto i = group_num.begin(); i != group_num.end(); i++)
	{
		if (code == *i)
		{
			count += 1;
		}
		else
		{
			zone_num.push_back(count);
			code = *i;
			count = 1;
		}
	}
}

void enemyManager::find_player()
{
	for (_pool_iter = _enemy_pool.begin(); _pool_iter != _enemy_pool.end(); ++_pool_iter)
	{
		if ((*_pool_iter)->get_kind() == enemy_kind::BOSS) continue;
		if ((*_pool_iter)->get_groupNumber() == 1) continue;

		int x = (*_pool_iter)->get_rect().left + ((*_pool_iter)->get_rect().right - (*_pool_iter)->get_rect().left) / 2;
		int y = (*_pool_iter)->get_rect().top + ((*_pool_iter)->get_rect().bottom - (*_pool_iter)->get_rect().top) / 2;

		int x2 = _player->get_playerInfo().rc.left + (_player->get_playerRect().right - _player->get_playerInfo().rc.left) / 2;
		int y2 = _player->get_playerInfo().rc.top + (_player->get_playerInfo().rc.bottom - _player->get_playerInfo().rc.top) / 2;

		if (getDistance(x, y, x2, y2) < 450)
		{
			// 플레이어 발견 플래그 온
			(*_pool_iter)->set_find_player(true);
		}
		else if (getDistance(x, y, x2, y2) > 600)
		{
			(*_pool_iter)->set_find_player(false);
		}
	}
}

void enemyManager::boss_only()
{
	for (int i = 0; i < _enemy_pool.size(); i++)
	{
		if (_enemy_pool[i]->get_kind() != enemy_kind::BOSS) continue;

		_enemy_pool[i]->set_destination(_player->get_playerInfo().rc);
	}
}

vector<tile*> enemyManager::addOpenList(tile* currentTile)
{
	int startX = currentTile->getIdX() - 1;
	int startY = currentTile->getIdY() - 1;

	if (startX < 0)				startX = 0;
	if (startX  + 2 >= TILEX)	startX = TILEX - 2;
	
	if (startY < 0)				startY = 0;
	if (startY + 2 >= TILEY)	startY = TILEY - 2;
	
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			tile* node = _vTotalList[(startY * TILEX) + startX + j + (i * TILEX)];

			//예외처리
			if (!node->getIsOpen()) continue;
			if (node->getAttribute() == tile_Attribute::START) continue;
			if (node->getAttribute() == tile_Attribute::WALL) continue;
			if (node->getAttribute() == tile_Attribute::WATER_ROW) continue;
			if (node->getAttribute() == tile_Attribute::WATER_COL) continue;

			if (_vTotalList[startX + (startY + 1) * TILEX]->getAttribute() == tile_Attribute::WALL && (j % 2 == 0 && i % 2 == 0))
			{
				continue;
			}

			if (_vTotalList[(startX + 2) + (startY + 1) * TILEX]->getAttribute() == tile_Attribute::WALL && (j % 2 == 0 && i % 2 == 0))
			{
				continue;
			}

			if (_vTotalList[(startX + 1) + startY * TILEX]->getAttribute() == tile_Attribute::WALL && (j % 2 == 0 && i % 2 == 0))
			{
				continue;
			}

			if (_vTotalList[(startX + 1) + (startY + 2) * TILEX]->getAttribute() == tile_Attribute::WALL && (j % 2 == 0 && i % 2 == 0))
			{
				continue;
			}

			//현재 타일 계속 갱신해준다
			node->setParentNode(currentTile);

			reset_list.push_back((startY * TILEX) + startX + j + (i * TILEX));

			//주변 타일을 검출하면서 체크했는지 유무를 알수있게 임의의 불값 선언
			bool addObj = true;

			for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
			{
				//이미 있는 애다
				if (*_viOpenList == node)
				{
					addObj = false;
					node->setIsOpen(false);
					break;
				}
			}
			//if (node->getAttribute() != "end") node->setColor(RGB(128, 64, 28));
			//이미 체크된 애는 건너뛴다
			if (!addObj) continue;

			//갈수 있는 타일은 오픈리스트 벡터에 담아준다
			_vOpenList.push_back(node);
		}
	}

	return _vOpenList;
}

void enemyManager::pathFinder(tile* currentTile)
{
	float tempTotalCost = 5000;
	tile* tempTile = nullptr;

	//오픈리스트 벡터 안에서, 가장 빠른 경로를 뽑아낸다
	for (int i = 0; i < addOpenList(currentTile).size(); ++i)
	{
		//H 값 연산
		_vOpenList[i]->setCostToGoal(
			(abs(_startTile->getIdX() - _vOpenList[i]->getIdX()) +
				abs(_startTile->getIdY() - _vOpenList[i]->getIdY())) * 10);

		//
		POINT center1 = _vOpenList[i]->getParentNode()->getCenter();
		POINT center2 = _vOpenList[i]->getCenter();

		//타일 중점 사이에 길이가 (타일사이즈 32)보다 크냐? 크면 대각선, 아니면 상하좌우
		_vOpenList[i]->setCostFromStart((getDistance(center1.x, center1.y, center2.x, center2.y) > TILESIZE) ? 14 : 10);

		//총 경로비용 == 갈 수 있는 타일 들 중에서 총 경로 비용들
		_vOpenList[i]->setTotalCost(_vOpenList[i]->getCostToGoal() + _vOpenList[i]->getCostFromStart());

		//그렇게 뽑아낸 총 경로비용들을 임의의 경로비용과 연산해서,
		//가장 작은 값을 계속 뽑아낸다
		if (tempTotalCost > _vOpenList[i]->getTotalCost())
		{
			tempTotalCost = _vOpenList[i]->getTotalCost();
			tempTile = _vOpenList[i];
		}

		bool addObj = true;
		for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
		{
			if (*_viOpenList == tempTile)
			{
				addObj = false;
				break;
			}
		}

		_vOpenList[i]->setIsOpen(false);

		if (!addObj) continue;

		_vOpenList.push_back(tempTile);
	}

	if (tempTile->getAttribute() == tile_Attribute::END)
	{
		//최단 경로는 색칠해줘라
		_FindPath = true;
		while (currentTile->getParentNode() != NULL)
		{
			_vCloseList.push_back(currentTile);
			currentTile = currentTile->getParentNode();
		}
		return;
	}

	for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
	{
		//뽑아낸 최단 경로는 오픈리스트에서 삭제시킨다
		if (*_viOpenList == tempTile)
		{
			_viOpenList = _vOpenList.erase(_viOpenList);
			break;
		}
	}
	_currentTile = tempTile;

	pathFinder(_currentTile);
}

POINT enemyManager::PointMake(int x, int y)
{
	POINT temp;

	temp.x = x;
	temp.y = y;

	return temp;
}
