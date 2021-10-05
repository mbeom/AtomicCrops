#pragma once
#include "singletonBase.h"

class CameraManager : public singletonBase<CameraManager>
{
private:
	float mapsize_x, mapsize_y;
	float camera_x, camera_y;
	float _x, _y;

	CameraManager();
	~CameraManager();

	friend singletonBase;

public:

	D2D1_POINT_2F center_pos;
	D2D1_MATRIX_3X2_F scale_mat;
	D2D1_MATRIX_3X2_F rotation_mat;
	D2D1_MATRIX_3X2_F trans_mat;
	D2D1_MATRIX_3X2_F back_trans_mat;
	D2D1_RECT_F screen;

	bool is_leave;

	HRESULT init(int width, int height);
	HRESULT init(float x, float y, int width, int height);
	void release();

	//카메라 위치 갱신
	inline float get_camera_x() { return _x; }
	inline float get_camera_y() { return _y; }
	inline void set_camera(int x, int y) { camera_x = x, camera_y = y; }

	void updateScreen(float standardX, float standardY, float ratioX = 0.5f, float ratioY = 0.5f);

};

