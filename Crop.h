#pragma once
#include "gameNode.h"
#include "bullets.h"

enum class Crop_stage
{
	stageA,
	stageB,
	stageC,
	stageD,
	LargeStageA,
	LargeStageB,
	LargeStageC,
	LargeStageD,
	HARVEST,
	LARGE_HARVEST
};

enum class Crop_Kind
{
	BEAN,
	CABBAGE,
	POTATO,
	PUMPKIN,

	KIND_NUM,
	HEART
};

class Crop : public gameNode
{
protected:

	image* _soil;
	image* _crop;
	image* _icon;
	harvest_effect* _harvest;

	D2D1_RECT_F _rc;
	animation* _cropAction;
	animation* _iconAction;
	Crop_stage _stage;
	Crop_Kind _kind;

	// 타일 인덱스를 저장할 변수
	int tile_idX, tile_idY;
	
	// crop 밑에 깔릴 이미지 프레임 인덱스
	int currentFrameX, currentFrameY;

	// 물 없을떄 죽어가는 이미지 프레임 보여주기 위한 인덱스
	int water_idx, water_idy;

	// 비료 몇개 먹었는지 저장하는 변수
	int fertile_num;

	// 농작물 값
	int price;

	float x, y;
	float width, height;
	float angle;
	float current_gague, max_gauge;

	float count;
	float grow_speed;
	float water_bonus;
	float fertile_bonus;

	bool water_need;

public:
	Crop() {};
	~Crop() {};

	virtual HRESULT init();
	virtual HRESULT init(float x, float y);
	void release();
	void update();
	void render();
	void soil_render();
	void crop_render();
	void icon_render();

	virtual void animation_init();
	virtual void animation_setting();
	virtual void harvest();

	inline void set_idX(int x) { tile_idX = x; }
	inline void set_idY(int y) { tile_idY = y; }

	inline int get_idX() { return tile_idX; }
	inline int get_idY() { return tile_idY; }

	inline void set_price(int val) { price = val; }
	inline int get_price() { return price; }

	inline void set_kind(Crop_Kind val) { _kind = val; }
	inline Crop_Kind get_kind() { return _kind; }

	inline void set_stage(Crop_stage val) { _stage = val; }
	inline Crop_stage get_stage() { return _stage; }

	inline void set_fertileNum(int val) { fertile_num += val; }
	inline int get_fertileNum() { return fertile_num; }

	inline void set_waterBonus(float val) { water_bonus = val; }
	inline float get_waterBonus() { return water_bonus; }

	inline void set_fertileBonus(float val) { fertile_bonus = val; }
	inline float get_fertileBonus() { return fertile_bonus; }

	inline void set_waterNeed(bool val) { water_need = val; animation_setting(); }
	inline bool get_waterNeed() { return water_need; }

	D2D1_RECT_F get_rect() { return _rc; }
	harvest_effect* get_harvest() { return _harvest; }
};

