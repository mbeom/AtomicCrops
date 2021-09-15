#pragma once
#include "tileNode.h"
#include "gameNode.h"

enum MENU
{
	Terrain,
	Wall,
	Object,
	Object2,
	Terrain2,
	Object3,
	MENUEND
};

class mapTool : public gameNode
{
private:
	saveButton _saveButton;
	loadButton _loadButton;
	eraserButton _eraserBtn;
	menuButton _menuBtn;
	menuButton _menuBtn2;
	CTBOX _currentTileBox;

	tagCurrentTile _currentTile;
	tagCurrentTile _ct[8];
	tagSampleTile _sampleTile[SAMPLETILEX * SAMPLETILEY];
	tagTile _tiles[TILEX * TILEY];
	DWORD _attribute[TILEX * TILEY];

	tagTOOL _tool;
	tagTile _terrain;
	tagTile _terrain2;
	tagTile _wall;
	tagTile _object;
	tagTile _object2;
	tagTile _object3;

	int _pos[2];
	int _menu;
	int _ctrSelect;
	D2D_RECT_F _sampleRect;

public:
	mapTool() {};
	~mapTool() {};

	HRESULT init();
	void release();
	void update();
	void render();

	void setup();

	void save();
	void load();
	void load(int num);

	TERRAIN terrainSelect(int frameX, int frameY);
	OBJECT objSelect(int frameX, int frameY);

	tagTile* getTile() { return _tiles; }

	DWORD* getTileAttribute() { return _attribute; }
	DWORD getObj(int num) { return _attribute[num]; }

	int getPosFirst() { return _pos[0]; }
	int getPosSecond() { return _pos[1]; }

};

