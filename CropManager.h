#pragma once
#include "gameNode.h"
#include "Crop.h"
#include "Bean.h"
#include "Potato.h"
#include "Cabbage.h"
#include "Heart.h"
#include "Pumpkin.h"

class CropManager : public gameNode
{
private:
	vector<Crop*> _cropList;
	vector<Crop*> _harvestList;

public:
	CropManager() {};
	~CropManager() {};

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void set_bean(float x, float y, int idx, int idy);
	void set_potato(float x, float y, int idx, int idy);
	void set_cabbage(float x, float y, int idx, int idy);
	void set_heart(float x, float y, int idx, int idy);
	void set_pumpkin(float x, float y, int idx, int idy);
	void check_square();
	void add_harvestedCrop(Crop* _crop) { _harvestList.push_back(_crop); }
	void remove_crop(int i) { _cropList.erase(_cropList.begin() + i); }

	vector<Crop*> get_cropList() { return _cropList; }
	vector<Crop*> get_harvestList() { return _harvestList; }
};

