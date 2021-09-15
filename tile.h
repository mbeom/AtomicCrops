#pragma once

#define TILESIZE 44

#define TILEX 200	//가로 타일 갯수
#define TILEY 120	//세로 타일 갯수

enum class tile_Attribute
{
	GROUND,
	FARM,
	FERTILE,
	WALL,
	WATER_ROW,
	WATER_COL,
	START,

	END
};

enum class tile_Season
{
	SPRING,
	SPRING_END,
	SUMMER,
	FALL,
	WINTER,

	SEASON_NUM
};

struct Image_index
{
	int current_frameX;
	int current_frameY;
	int object_frameX;
	int object_frameY;
};

class tile
{
private:
	int _idX;
	int _idY;
	int current_tilePage;
	int current_objectPage;

	float _count;

	Image_index _index;

	POINT _center;
	RECT _rc;

	D2D1_RECT_F rc;

	image* _img;
	image* _objectImg;

	float _totalCost;
	float _costFromStart;
	float _costToGoal;

	bool _isOpen;

	tile* _parentNode; //제일 상위 타일 (계속 갱신되는 타일)

	tile_Attribute _attribute;
	tile_Season _season;

	WCHAR str[128];

public:
	tile();
	~tile();

	HRESULT init(int idX, int idY);
	void release();
	void update();
	void render();

	int getIdX() { return _idX; }
	int getIdY() { return _idY; }

	inline void setIdX(int val) { _idX = val; }
	inline void setIdY(int val) { _idY = val; }

	RECT getRect() { return _rc; }
	D2D1_RECT_F getRectangle() { return rc; }

	void set_Tilepage(int val) { current_tilePage = val; }
	int get_Tilepage() { return current_tilePage; }

	void set_Objectpage(int val) { current_objectPage = val; }
	int get_Objectpage() { return current_objectPage; }

	void set_image(image* img) { _img = img; }
	void set_objectImage(image* img) { _objectImg = img; }

	//============ 편리를 위한 접근자 & 설정자 ===============

	void setCenter(POINT center) { _center = center; }
	POINT getCenter() { return _center; }

	void setAttribute(tile_Attribute str) { _attribute = str; }
	tile_Attribute getAttribute() { return _attribute; }

	void setSeason(tile_Season str) { _season = str; }
	tile_Season getSeason() { return _season; }

	void setTileIndex(int x, int y) { _index.current_frameX = x; _index.current_frameY = y; }
	void setObjectIndex(int x, int y) { _index.object_frameX = x; _index.object_frameY = y; }
	Image_index getIndexInfo() { return _index; }

	void setTotalCost(float totalCost) { _totalCost = totalCost; }
	float getTotalCost() { return _totalCost; }

	void setCostFromStart(float costFromStart) { _costFromStart = costFromStart; }
	float getCostFromStart() { return _costFromStart; }

	void setCostToGoal(float costToGoal) { _costToGoal = costToGoal; }
	float getCostToGoal() { return _costToGoal; }

	void setParentNode(tile* t) { _parentNode = t; }
	tile* getParentNode() { return _parentNode; }

	void setIsOpen(bool isOpen) { _isOpen = isOpen; }
	bool getIsOpen() { return _isOpen; }
};

