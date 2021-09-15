#include "stdafx.h"
#include "world.h"
#include "tool.h"
#include "player.h"
#include "enemyManager.h"
#include "collision.h"
#include "CropManager.h"
#include "ObjectManager.h"
#include "UI.h"

HRESULT world::init()
{
	ShowCursor(false);

	CAMERAMANAGER->init(200 * 44, 120 * 44);

	TIMEMANAGER->day += 1;

	IMAGEMANAGER->addImage("sun_rise", L"image/UI/LenseFlare.png", 1600, 900);
	IMAGEMANAGER->addImage("sun_set", L"image/UI/night_screen.png", 1600, 900);
	IMAGEMANAGER->addImage("gameover", L"image/gameover.png", 1600, 900);

	_map->init();
	_tiles.reserve(TILEX * TILEY);

	_player->set_x(3952);
	_player->set_y(1744);
	_player->set_inWorld(true);

	if (TIMEMANAGER->day <= 1)
	{
		_map->load(1);
		_tiles = _map->get_tiles();

		_player->init();
		_UI->init();
	}
	else
	{
		_map->load(3);
		load();
	}

	_loopList = _map->get_loopList();

	_em->init(12);

	_collision->init();

	_object->init();
	_object->set_well(3716, 1754);

	set_enemies();

	_loading = IMAGEMANAGER->findImage("loading");
	_blackScreen = IMAGEMANAGER->findImage("black_screen");
	_sunRise = IMAGEMANAGER->findImage("sun_rise");
	_gameover = IMAGEMANAGER->findImage("gameover");
	_animation = KEYANIMANAGER->findAnimation("loading");
	_animation->start();

	_count = 0;
	rgb_alpha = 0.8f;
	time_holder = 0;
	day_time = 180;
	wave_time = 50;

	wave[0] = false;
	wave[1] = false;
	wave[2] = false;

	start = false;
	end = false;
	last_day = false;
	return_home = false;
	sun_rise = false;
	sun_set = false;

	if (SOUNDMANAGER->isPlaySound("town"))
	{
		SOUNDMANAGER->stop("town");
	}
	else if (SOUNDMANAGER->isPlaySound("title"))
	{
		SOUNDMANAGER->stop("title");
	}

	return S_OK;
}

void world::release()
{

}

void world::update()
{
	if (!start)
		_count += TIMEMANAGER->getElapsedTime();

	if (_player->get_playerStatus().current_hp <= 0)
		_count += TIMEMANAGER->getElapsedTime();

	if (_player->get_playerStatus().current_hp <= 0 && _count > 3.0f)
		end = true;

	if (end && _count > 5.0f)
	{
		TIMEMANAGER->world_time = 0;
		TIMEMANAGER->day = 0;
		SOUNDMANAGER->stop("farm");
		SCENEMANAGER->changeScene("title");
	}

	if (_count > 3.0f && !end)
	{
		_count = 0;
		start = true;
		TIMEMANAGER->world_time = 0;
		SOUNDMANAGER->play("farm");
	}

	if (TIMEMANAGER->world_time > 180 && TIMEMANAGER->world_time <= 330)
	{
		sun_set = true;
		_blackScreen = IMAGEMANAGER->findImage("sun_set");
	}
	else if (TIMEMANAGER->world_time > 330 && !sun_rise)
	{
		sun_set = false;
		sun_rise = true;
		rgb_alpha = 1.0f;
	}

	if (sun_rise || sun_set)
	{
		rgb_alpha -= TIMEMANAGER->getElapsedTime() * 0.5;
	}

	if (rgb_alpha < 0.4 && sun_set)
	{
		rgb_alpha = 0.4f;
	}

	if (rgb_alpha < 0 && sun_rise)
	{
		rgb_alpha = 0.0f;
	}

	_player->update();
	_crop->update();
	_object->update();
	_collision->update();
	_item->update();

	for (int i = 0; i < _loopList.size(); i++)
	{
		_tiles[_loopList[i]]->update();
	}

	CAMERAMANAGER->updateScreen(_player->get_playerInfo().x, _player->get_playerInfo().y);
	_UI->update();
	_em->update();

	/*
	if (KEYMANAGER->isOnceKeyDown('T'))
	{
		save();
		_em->get_enemy().clear();
		_object->get_objectList().clear();
		_map->release();

		SCENEMANAGER->changeScene("base");
	}
	*/
}

void world::render()
{
	CAMERAMANAGER->center_pos.x = _player->get_playerInfo().x - CAMERAMANAGER->get_camera_x();
	CAMERAMANAGER->center_pos.y = _player->get_playerInfo().y - CAMERAMANAGER->get_camera_y();
	CAMERAMANAGER->scale_mat = D2D1::Matrix3x2F::Scale(D2D1::SizeF(1.5f, 1.5f), CAMERAMANAGER->center_pos);
	D2DMANAGER->pRenderTarget->SetTransform(CAMERAMANAGER->scale_mat);

	for (int i = 0; i < _tiles.size(); i++)
	{
		_tiles[i]->render();
	}
	_crop->render();
	_collision->render();
	_object->render();
	_em->render();

	CAMERAMANAGER->center_pos.x = _player->get_playerInfo().x - CAMERAMANAGER->get_camera_x();
	CAMERAMANAGER->center_pos.y = _player->get_playerInfo().y - CAMERAMANAGER->get_camera_y();
	CAMERAMANAGER->scale_mat = D2D1::Matrix3x2F::Scale(D2D1::SizeF(1.5f, 1.5f), CAMERAMANAGER->center_pos);
	D2DMANAGER->pRenderTarget->SetTransform(CAMERAMANAGER->scale_mat);

	_item->render();

	count_time();

	_UI->render();

	_player->render();

	TIMEMANAGER->render(_player->get_playerInfo().x, _player->get_playerInfo().y);

	D2DMANAGER->pRenderTarget->SetTransform(CAMERAMANAGER->scale_mat);

	if (!start)
	{
		_blackScreen->render(CAMERAMANAGER->screen.left, CAMERAMANAGER->screen.top);
		_loading->aniRender((CAMERAMANAGER->screen.left + CAMERAMANAGER->screen.right) / 2 - _loading->getFrameWidth() / 2,
			(CAMERAMANAGER->screen.top + CAMERAMANAGER->screen.bottom) / 2 - _loading->getFrameHeight() / 2, _animation);

		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->white_brush, L"메이플스토리", 20, L"로딩중....",
			CAMERAMANAGER->screen.left, CAMERAMANAGER->screen.bottom - 200, CAMERAMANAGER->screen.right, CAMERAMANAGER->screen.bottom);
	}

	D2DMANAGER->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	if (sun_set)
	{
		_blackScreen->render(CAMERAMANAGER->screen.left, CAMERAMANAGER->screen.top, rgb_alpha);
	}

	if (sun_rise)
	{
		_sunRise->render(CAMERAMANAGER->screen.left, CAMERAMANAGER->screen.top, rgb_alpha);
	}

	if (end)
	{
		_gameover->render(CAMERAMANAGER->screen.left, CAMERAMANAGER->screen.top);
	}
}

void world::save()
{
	HANDLE file;
	DWORD write;

	file = CreateFile("world.map", GENERIC_WRITE, NULL, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	for (int i = 0; i < _tiles.size(); i++)
	{
		WriteFile(file, _tiles[i], sizeof(tile), &write, NULL);
	}

	CloseHandle(file);
}

void world::load()
{
	HANDLE file;
	DWORD read;

	file = CreateFile("world.map", GENERIC_READ, FALSE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	for (int i = 0; i < _tiles.size(); i++)
	{
		ReadFile(file, _tiles[i], sizeof(tile), &read, NULL);
	}

	_map->get_tiles() = _tiles;

	CloseHandle(file);
}

void world::count_time()
{
	if(start)
		time_holder += TIMEMANAGER->getElapsedTime();

	if (time_holder > 1.0f)
	{
		time_holder = 0.0f;
		TIMEMANAGER->world_time += 1;
	}

	if (TIMEMANAGER->world_time <= 180)
	{
		int min = (day_time - TIMEMANAGER->world_time) / 60;
		int sec = (day_time - TIMEMANAGER->world_time) % 60;

		WCHAR str[128];
		if(sec < 10)
			swprintf_s(str, L"%d:0%d", min, sec);
		else
			swprintf_s(str, L"%d:%d", min, sec);

		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->white_brush, L"메이플스토리", 14, str, 
		_player->get_playerInfo().rc.left + 680, _player->get_playerInfo().rc.top - 260,
		_player->get_playerInfo().rc.left + 150, _player->get_playerInfo().rc.top - 160);
	}
	else if (TIMEMANAGER->world_time > 180 && TIMEMANAGER->world_time <= 230)
	{
		int min = (wave_time - (TIMEMANAGER->world_time - 180)) / 60;
		int sec = (wave_time - (TIMEMANAGER->world_time - 180)) % 60;

		WCHAR str[128];
		if (sec < 10)
			swprintf_s(str, L": %d:0%d", min, sec);
		else
			swprintf_s(str, L": %d:%d", min, sec);

		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->white_brush, L"메이플스토리", 14, str,
			_player->get_playerInfo().rc.left + 680, _player->get_playerInfo().rc.top - 260,
			_player->get_playerInfo().rc.left + 150, _player->get_playerInfo().rc.top - 160);

		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->white_brush, L"메이플스토리", 14, L": 1 / 3",
			_player->get_playerInfo().rc.left + 660, _player->get_playerInfo().rc.top - 280,
			_player->get_playerInfo().rc.left + 150, _player->get_playerInfo().rc.top - 160);

		if (!wave[0])
		{
			_player->set_x(3952);
			_player->set_y(1744);

			wave[0] = true;
			_em->set_rabbits(2946, 668, true);
			_em->set_rabbits(3330, 668, true);
			_em->set_rabbits(3511, 2724, true);
			_em->set_rabbits(3059, 2724, true);
		}

		if (TIMEMANAGER->day >= 3 && !last_day)
		{
			last_day = true;
			_em->set_boss(4915, 1000);
		}
	}
	else if (TIMEMANAGER->world_time > 230 && TIMEMANAGER->world_time <= 280)
	{
		int min = (wave_time - (TIMEMANAGER->world_time - 230)) / 60;
		int sec = (wave_time - (TIMEMANAGER->world_time - 230));

		WCHAR str[128];
		if (sec < 10)
			swprintf_s(str, L": %d:0%d", min, sec);
		else
			swprintf_s(str, L": %d:%d", min, sec);

		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->white_brush, L"메이플스토리", 14, str,
			_player->get_playerInfo().rc.left + 680, _player->get_playerInfo().rc.top - 260,
			_player->get_playerInfo().rc.left + 150, _player->get_playerInfo().rc.top - 160);

		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->white_brush, L"메이플스토리", 14, L": 2 / 3",
			_player->get_playerInfo().rc.left + 660, _player->get_playerInfo().rc.top - 280,
			_player->get_playerInfo().rc.left + 150, _player->get_playerInfo().rc.top - 160);

		if (!wave[1])
		{
			wave[1] = true;
			_em->set_rabbits(3931, 604, true);
			_em->set_rabbits(4379, 640, true);
			_em->set_rabbits(4915, 640, true);
			_em->set_rabbits(4915, 2724, true);
			_em->set_rabbits(4543, 2724, true);
		}
	}
	else if (TIMEMANAGER->world_time > 280 && TIMEMANAGER->world_time <= 330)
	{
		int min = (wave_time - (TIMEMANAGER->world_time - 280)) / 60;
		int sec = (wave_time - (TIMEMANAGER->world_time - 280)) % 60;

		WCHAR str[128];
		if (sec < 10)
			swprintf_s(str, L": %d:0%d", min, sec);
		else
			swprintf_s(str, L": %d:%d", min, sec);

		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->white_brush, L"메이플스토리", 14, str,
			_player->get_playerInfo().rc.left + 680, _player->get_playerInfo().rc.top - 260,
			_player->get_playerInfo().rc.left + 150, _player->get_playerInfo().rc.top - 160);

		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->white_brush, L"메이플스토리", 14, L": 3 / 3",
			_player->get_playerInfo().rc.left + 660, _player->get_playerInfo().rc.top - 280,
			_player->get_playerInfo().rc.left + 150, _player->get_playerInfo().rc.top - 160);

		if (!wave[2])
		{
			wave[2] = true;
			_em->set_rabbits(2935, 940, true);
			_em->set_rabbits(4915, 993, true);
			_em->set_rabbits(3947, 2724, true);
			_em->set_rabbits(3059, 2396, true);
			_em->set_rabbits(4935, 2466, true);
		}
	}
	else if (TIMEMANAGER->world_time > 330 && TIMEMANAGER->world_time < 340)
	{
		if (!return_home && TIMEMANAGER->world_time > 331)
		{
			_object->set_plane(_player->get_playerInfo().rc.right + 200, _player->get_playerInfo().rc.top - WINSIZEY / 2);
			return_home = true;
		}

		WCHAR str[128];
		swprintf_s(str, L" 마을로 돌아가기까지 앞으로 %d초 ...", 340 - TIMEMANAGER->world_time);

		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->white_brush, L"메이플스토리", 20, str,
			CAMERAMANAGER->screen.left, CAMERAMANAGER->screen.top + WINSIZEY * 0.4,
			CAMERAMANAGER->screen.right, CAMERAMANAGER->screen.bottom);
	}
	else
	{
		save();
		_em->get_enemy().clear();
		_item->get_itemList().clear();
		_object->get_objectList().clear();
		_map->release();

		SCENEMANAGER->changeScene("base");

	}
}

void world::set_enemies()
{
	_em->set_rabbits(232.2, 202.5, 0);
	_em->set_rabbits(617, 210.7, 0);
	_em->set_rabbits(144.6, 466.7, 0);

	_em->set_rabbits(1450.1, 178.2);

	_em->set_rabbits(2354.8, 226.2, 1);
	_em->set_rabbits(2398.7, 462.6, 1);
	_em->set_rabbits(1922.2, 350.5, 1);

	_em->set_rabbits(297, 1094);
	_em->set_rabbits(269.3, 1238);

	_em->set_rabbits(1878.6, 422.2, 2);
	_em->set_rabbits(1532.8, 1133, 2);
	_em->set_rabbits(2013.2, 1129, 2);

	_em->set_rabbits(1284.7, 1861.9);

	_em->set_rabbits(203.7, 2686, 3);
	_em->set_rabbits(631.9, 2830, 3);
	_em->set_rabbits(587.9, 2553.2, 3);

	_em->set_rabbits(1925, 2853.3, 4);
	_em->set_rabbits(1853.6, 1385.8, 4);
	_em->set_rabbits(1629.9, 2645.8, 4);
	
	_em->set_rabbits(236.3, 3770.9, 5);
	_em->set_rabbits(510.2, 3890.3, 5);
	_em->set_rabbits(913.7, 3774.8, 5);
	_em->set_rabbits(965.9, 3450.3);

	_em->set_rabbits(2107.8, 1476.1);
	
	_em->set_rabbits(5920, 1168);
	_em->set_rabbits(6204, 1028);
	_em->set_rabbits(6484, 440);
	_em->set_rabbits(6116, 412);
	_em->set_rabbits(7840, 824);
	_em->set_rabbits(7840, 1066);
	_em->set_rabbits(7652, 1350);
	_em->set_rabbits(7840, 2670);
	_em->set_rabbits(7840, 3122);
	_em->set_rabbits(5839, 3174);
	_em->set_rabbits(6307, 2990);
	_em->set_rabbits(6591, 2546);
	_em->set_rabbits(6963, 2350);
	_em->set_rabbits(7355, 1982);
	_em->set_rabbits(7367, 1833);
	_em->set_rabbits(7200, 1833);
	_em->set_rabbits(7194, 1989);
	_em->set_rabbits(6123, 2217);

	_em->set_scorpion(1823, 1856);
	_em->set_scorpion(1843, 2284);
	_em->set_scorpion(1047, 2680);
	_em->set_scorpion(1044, 1276);
	_em->set_scorpion(976, 704);
	_em->set_scorpion(1877, 724);

	_em->set_maple(6225, 1162);
	_em->set_maple(6493, 510);
	_em->set_maple(7598, 174);
	_em->set_maple(8178, 490);
	_em->set_maple(7893, 2430);
	_em->set_maple(7465, 2818);
	_em->set_maple(6485, 3014);
	_em->set_maple(6905, 1918);

}
