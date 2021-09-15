#include "stdafx.h"
#include "base.h"
#include "tool.h"
#include "player.h"
#include "ObjectManager.h"
#include "collision.h"
#include "UI.h"

HRESULT base::init()
{
	CAMERAMANAGER->init(1500, 1300, 100 * 44, 60 * 44);

	_tool->init();
	_tool->load(2);
	
	_map = _tool->get_tiles();

	_player->set_x(1500);
	_player->set_y(1300);
	_player->set_gunType(GunType::BASIC_GUN);
	_player->set_inWorld(false);
	//_player->init();

	_collision->init();

	_object->init();

	_object->set_NPC(1508, 965, 0);
	_object->set_NPC(1975, 970, 1);
	_object->set_NPC(2290, 970, 2);
	_object->set_NPC(1340, 1660, 3);
	_object->set_NPC(1380, 1660, 4);
	_object->set_NPC(1480, 1660, 5);
	_object->set_NPC(1008, 1490, 6);
	_object->set_NPC(1340, 1080, 6);
	_object->set_NPC(2480, 1340, 6);
	_object->set_NPC(2300, 1660, 3);
	_object->set_NPC(2450, 1660, 4);
	
	_object->set_copter(1900, 1620);
	_object->set_NPC(1840, 1660, 7);

	_item->set_seedBag(1440, 960, 0);
	_item->set_seedBag(1570, 960, 1);
	_item->set_seedBag(1620, 960, 2);

	_item->set_gun(1940, 990, 0);
	_item->set_gun(2010, 995, 1);

	_item->set_heartBag(2375, 1005, 0);
	
	_loading = IMAGEMANAGER->findImage("loading");
	_blackScreen = IMAGEMANAGER->findImage("black_screen");
	_scoreBackground = IMAGEMANAGER->findImage("score_bg");
	_animation = KEYANIMANAGER->findAnimation("loading");
	_animation->start();

	_score = 0;
	_count = 0;
	_maxScore = _player->get_score();

	start = false;

	if (SOUNDMANAGER->isPlaySound("farm"))
		SOUNDMANAGER->stop("farm");
	return S_OK;
}

void base::release()
{
}

void base::update()
{
	if (!start)
		_count += TIMEMANAGER->getElapsedTime();

	if (_count > 3.0f)
	{
		_count = 0;
		start = true;
		SOUNDMANAGER->play("town");
	}

	if (_score < _maxScore) _score += 1;

	_player->update();
	_object->update();
	_item->update();
	_collision->update();

	load();

	CAMERAMANAGER->updateScreen(_player->get_playerInfo().x, _player->get_playerInfo().y);
	_ui->update();
	
	CAMERAMANAGER->center_pos.x = _player->get_playerInfo().x - CAMERAMANAGER->get_camera_x();
	CAMERAMANAGER->center_pos.y = _player->get_playerInfo().y - CAMERAMANAGER->get_camera_y();
	CAMERAMANAGER->scale_mat = D2D1::Matrix3x2F::Scale(D2D1::SizeF(1.5f, 1.5f), CAMERAMANAGER->center_pos);
}

void base::load()
{
	for (int i = 0; i < _object->get_objectList().size(); i++)
	{
		if (_object->get_objectList()[i]->get_type() != object_type::COPTER) continue;

		float player_x = (_player->get_playerRect().left + _player->get_playerRect().right) / 2;
		float player_y = (_player->get_playerRect().top + _player->get_playerRect().bottom) / 2;

		float object_x = (_object->get_objectList()[i]->get_rect().left + _object->get_objectList()[i]->get_rect().right) / 2;
		float object_y = (_object->get_objectList()[i]->get_rect().top + _object->get_objectList()[i]->get_rect().bottom) / 2;

		if (getDistance(player_x, player_y, object_x, object_y) < 160)
		{
			if (KEYMANAGER->isOnceKeyDown('F'))
			{
				_item->get_itemList().clear();
				_object->get_objectList().clear();

				TIMEMANAGER->world_time = 0;
				SCENEMANAGER->changeScene("world");
			}
		}
	}
}

void base::render()
{
	D2DMANAGER->pRenderTarget->SetTransform(CAMERAMANAGER->scale_mat);
	
	for (int i = 0; i < _map.size(); i++)
	{
		_map[i]->render();
	}
	_object->render();
	_item->render();

	WCHAR str[128];
	swprintf_s(str, L"총 스코어: %d", _score);

	/*
	_scoreBackground->render((CAMERAMANAGER->screen.left + CAMERAMANAGER->screen.right) / 2, (CAMERAMANAGER->screen.top + CAMERAMANAGER->screen.bottom) / 2);

	D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->white_brush, L"메이플스토리", 14, str,
		CAMERAMANAGER->screen.left + 350, (CAMERAMANAGER->screen.top + CAMERAMANAGER->screen.bottom)/2,
		CAMERAMANAGER->screen.left + 550, CAMERAMANAGER->screen.top + 400);
		*/
	D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->white_brush, L"메이플스토리", 14, L" 새벽......",
		_player->get_playerInfo().rc.left + 660, _player->get_playerInfo().rc.top - 280,
		_player->get_playerInfo().rc.left + 150, _player->get_playerInfo().rc.top - 160);

	_ui->render();

	_player->render();

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

}
