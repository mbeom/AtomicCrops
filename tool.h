#pragma once
#include "gameNode.h"
#include "tile.h"

enum sample_page
{
	TILE,
	TILE_EDGE,
	TILE_EDGE2,
	TILE_EDGE3,
	OBJECT_TILE,
	OBJECT_EDGE,
	OBJECT_EDGE2,
	OBJECT_EDGE3,
	OBJECT_WALL,
	OBJECT_BUILDING,
	LOOP,

	PAGE_NUM
};

struct Sample_Data
{
	POINT index;
	image* sample_img;
};

class tool : public gameNode
{
private:

	int _count, point_x, point_y;
	int current_ratio, current_frameX, current_frameY;
	int current_tileX, current_tileY;
	int _tileNumX, _tileNumY;
	int _row, _col;
	float x, y;
	float width, height;
	sample_page _page;

	D2D1_RECT_F temp, target;
	D2D1_RECT_F box[6];
	D2D1_RECT_F num_box[8];
	D2D1_RECT_F* drag_and_drop;

	vector<tile*> _tiles;
	vector<D2D1_RECT_F> _sampleTiles;
	vector<Sample_Data> _IndexPool;
	vector<int> _loopList;
	vector<int> _farmList;

	image* _sampleImage[PAGE_NUM];
	image* _tileImage[PAGE_NUM];
	image* _buttonImage[6];
	image* _numboxImage[8];
	image* _currentSample;

	ID2D1SolidColorBrush* red_brush;
	ID2D1SolidColorBrush* blue_brush;
	ID2D1SolidColorBrush* white_brush;

	bool LButton_down;
	bool LButton_up;
	bool LButton_Staydown;

	bool mouse_on_board;
	bool start_drag;
	bool end_drag;
	bool is_full;
	bool erase;

public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void move_viewpoint();
	void set_tiles(int x, int y);
	void box_click();
	void save();
	void load();
	void load(int num);

	inline int get_tileNumX() { return _tileNumX; }
	inline int get_tileNumY() { return _tileNumY; }

	D2D1_RECT_F get_currentTIle() { return target; }

	vector<tile*>& get_tiles() { return _tiles; }
	vector<tile*> get_OnlytileInfo() { return _tiles; }
	vector<int>& get_loopList() { return _loopList; }
	vector<int>& get_farmList() { return _farmList; }

	
};

