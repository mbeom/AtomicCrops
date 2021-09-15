#include "stdafx.h"
#include "playGround.h"


playGround::playGround()
{
}


playGround::~playGround()
{
}

HRESULT playGround::init()
{
	gameNode::init(true);

	int move[] = { 20, 21, 22, 23 };

	IMAGEMANAGER->addImage("black_screen", L"image/UI/black_screen.png", 1600, 900);
	IMAGEMANAGER->addFrameImage("loading", L"image/UI/loadingAnimation.png", 440, 440, 5, 5);
	IMAGEMANAGER->addImage("cursor", L"image/UI/cursor.png", 34, 34);

	KEYANIMANAGER->addArrayFrameAnimation("loading", "loading", move, 4, 8.0f, true);
	SOUNDMANAGER->addSound("title", "sound/타이틀.mp3", true, true);
	SOUNDMANAGER->addSound("town", "sound/타운.mp3", true, true);
	SOUNDMANAGER->addSound("farm", "sound/밭.mp3", true, true);
	SOUNDMANAGER->addSound("jungle", "sound/정글.mp3", true, true);
	SOUNDMANAGER->addSound("desert", "sound/사막.mp3", true, true);

	_tool = new tool;
	//_tool->init();
	
	_player = new player;
	_player->LinkItemClass(_item);

	_item = new ItemManager;
	_item->LinkPlayerClass(_player);

	_em = new enemyManager;
	_em->LinkPlayerClass(_player);
	_em->LinkToolClass(_tool);

	_UI = new UI;
	_UI->LinkPlayerClass(_player);

	_crop = new CropManager;
	_crop->init();

	_object = new ObjectManager;

	_collision = new collision;
	_collision->Link_enemyClass(_em);
	_collision->Link_PlayerClass(_player);
	_collision->Link_ToolClass(_tool);
	_collision->Link_CropClass(_crop);
	_collision->Link_ObjectClass(_object);
	_collision->Link_UIClass(_UI);
	_collision->LinkItemClass(_item);

	_title = new title;

	_base = new base;
	_base->LinkToolClass(_tool);
	_base->LinkItemClass(_item);
	_base->LinkPlayerClass(_player);
	_base->LinkObjectClass(_object);
	_base->LinkCollisionClass(_collision);
	_base->LinkUIClass(_UI);

	_world = new world;
	_world->LinkToolClass(_tool);
	_world->LinkPlayerClass(_player);
	_world->LinkEnemyClass(_em);
	_world->LinkCollisionClass(_collision);
	_world->LinkCropClass(_crop);
	_world->LinkObjectClass(_object);
	_world->LinkItemClass(_item);
	_world->LinkUIClass(_UI);
	
	SCENEMANAGER->addScene("base", _base);
	SCENEMANAGER->addScene("world", _world);
	SCENEMANAGER->addScene("title", _title);
	SCENEMANAGER->addScene("maptool", _tool);

	SCENEMANAGER->changeScene("title");

	return S_OK;
}

void playGround::release()
{
	gameNode::release();
}

void playGround::update()
{
	gameNode::update();

	SCENEMANAGER->update();

}

void playGround::render()
{
	D2DMANAGER->BeginDraw();
	/************************************************************************************************************************/


	SCENEMANAGER->render();



	/************************************************************************************************************************/
	D2DMANAGER->EndDraw();
}
