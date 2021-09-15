#pragma once
#include "animation.h"

class image
{
public:
	//이미지를 불러올수 있는 종류
	enum IMAGE_LOAD_KIND
	{
		LOAD_RESOURCE,		//리소스 파일로부터
		LOAD_FILE,			//이미지 파일로부터
		LOAD_EMPTY,			//빈 비트맵 이미지를 만들어야겠다
		LOAD_END
	};

	typedef struct tagImageInfo
	{
		DWORD resID;	//리소스를 통해서 이미지 처리할땐 얘를 쓴다
		IWICImagingFactory*		pWICImagingFactory;		//Windows Imaging Component를 위한 Factory 인터페이스
		IWICFormatConverter*	pWICFormatConverter;	//Format Converter
		IWICBitmapDecoder*		pWICDecoder;			//Bitmap Decoder
		IWICBitmapFrameDecode*  pWICFrameDecoder;		//프레임 단위로 디코딩할 Decoder 인터페이스
		ID2D1Bitmap*			pBitmap;				//D2D용 비트맵
		IWICBitmap*				wBitmap;				//픽셀처리용 비트맵
		HDC hMemDC;
		HBITMAP hBit;
		HBITMAP hOBit;
		float x;			//이미지 x좌표
		float y;			//이미지 y좌표
		int width;			//이미지 전체 가로크기
		int height;			//이미지 전체 세로크기
		int currentFrameX;	//현재 프레임 번호(가로)
		int currentFrameY;	//현재 프레임 번호(세로)
		int maxFrameX;		//최대 프레임 번호(가로)
		int maxFrameY;		//최대 프레임 번호(세로)
		int frameWidth;		//1프레임 당 가로크기
		int frameHeight;	//1프레임 당 세로크기
		BYTE loadType;

		tagImageInfo()
		{
			resID		= 0;
			pWICImagingFactory = nullptr;
			pWICFormatConverter = nullptr;
			pWICDecoder = nullptr;
			pWICFrameDecoder = nullptr;
			pBitmap = nullptr;
			wBitmap = nullptr;
			hMemDC		= NULL;
			hBit		= NULL;
			hOBit		= NULL;
			x			= 0;
			y			= 0;
			width		= 0;
			height		= 0;
			currentFrameX = 0;
			currentFrameY = 0;
			maxFrameX = 0;
			maxFrameY = 0;
			frameWidth = 0;
			frameHeight = 0;
			loadType = LOAD_RESOURCE;
		}
	}IMAGE_INFO, *LPIMAGE_INFO;

private:
	LPIMAGE_INFO	_imageInfo;
	LPWSTR			_fileName;		//파일 이름
	BOOL			_trans;			//특정 픽셀값을 제거할지 유무
	COLORREF		_transColor;	//제거할 픽셀값

	BLENDFUNCTION	_blendFunc;		//알파블렌드 함수
	LPIMAGE_INFO	_blendImage;	//알파블렌드 먹일 이미지

public:
	image();
	~image();


	// 일반 이미지를 파일에서 가져올 때, 파일명, 원본 너비, 원본 높이
	HRESULT D_Init(LPCWSTR pFileName, int width, int height);

	// 프레임 이미지를 파일에서 가져올 때, 파일명, 원본 너비, 원본 높이, 가로 프레임 수, 세로 프레임 수
	HRESULT D_Init(LPCWSTR pFileName, int width, int height, int frameNumX, int frameNumY);

	void release();
	const HRESULT get_RGB(int x, int y);

	void rotation_start(float destX, float destY, float angle);
	void Framerotation_start(float destX, float destY, float angle);
	void rotation_end();
	D2D1_MATRIX_3X2_F rotation_matrix(float destX, float destY, float angle);
	D2D1_MATRIX_3X2_F Frame_rotation_matrix(float destX, float destY, float angle);

	void scale_start(float destX, float destY, float x, float y);
	void Frame_scale_start(float destX, float destY, float x, float y);
	void scale_end();
	D2D1_MATRIX_3X2_F scale_matrix(float destX, float destY, float x, float y);
	D2D1_MATRIX_3X2_F Frame_scale_matrix(float destX, float destY, float x, float y);

	D2D1_MATRIX_3X2_F translation_matrix(float destX, float destY);
	D2D1_MATRIX_3X2_F Frame_trans_matrix(float destX, float destY, float x, float y);

	//원래 크기 출력 - 그려줄 x좌표, y좌표, 투명도
	void render(float destX, float destY, float opacity = 1.0f);

	//원래 크기에서 일정비율 잘라서 그리기(?) - 그려줄 x좌표, y좌표, 투명도
	void renderUI(float destX, float destY, float sourX, float sourY, float tempX, float tempY, float opacity = 1.0f);

	//원래 크기로 잘라서 가져오기 - 그려줄 x좌표, y좌표 -> 가져올 곳의 left, top, 가로 크기, 세로 크기, 투명도
	void render(float destX, float destY, float sourX, float sourY,
		int sourWidth, int sourHeight, float opacity = 1.0f);

	//사각형 좌표 -> 사각형 좌표 그리기
	void render(float destX, float destY, float destWidth, float destHeight, float sourX, float sourY, float sourWidth, float sourHeight, float opacity = 1.0f);

	//원래 크기 프레임렌더 - 그려줄 x, y 좌표, 가로 방향 프레임 인덱스, 세로 방향 프레임 인덱스, 투명도
	void FrameRender(float destX, float destY, int currentFrameX, int currentFrameY, float opacity = 1.0f);
	void FrameRender(float destX, float destY, int currentFrameX, int currentFrameY, float destWidth, float destHeight, float opacity = 1.0f);
	void aniRender(float destX, float destY, animation* ani, float opacity = 1.0f);

	//혹시나 런타임 도중에 제거할 픽셀값을 바꿔야한다면
	void setTransColor(BOOL trans, COLORREF transColor);

	void loopRender(const D2D1_RECT_F drawArea, int offSetX, int offSetY);
	void loopRender(const D2D1_RECT_F drawArea, int offSetX, int offSetY, float opacity);

	//백버퍼에 대한 접근자
	inline HDC getMemDC() { return _imageInfo->hMemDC; }

	//===============================================
	// ## 이미지 관련 접근자 설정자 ##
	//=============================================

	inline void setX(float x) { _imageInfo->x = x; }
	inline float getX() { return _imageInfo->x; }

	inline void setY(float y) { _imageInfo->y = y; }
	inline float getY() { return _imageInfo->y; }

	inline void setCenter(float x, float y)
	{
		_imageInfo->x = x - (_imageInfo->width / 2);
		_imageInfo->y = y - (_imageInfo->height / 2);
	}

	inline float getCenterX()
	{
		return _imageInfo->maxFrameX <= 0 ?
			_imageInfo->x + (_imageInfo->width / 2) :
			_imageInfo->x + (_imageInfo->frameWidth / 2);
	}

	inline float getCenterY()
	{
		return _imageInfo->maxFrameY <= 0 ?
			_imageInfo->y + (_imageInfo->height / 2) :
			_imageInfo->y + (_imageInfo->frameHeight / 2);
	}

	inline int getWidth() { return _imageInfo->width; }
	inline int getHeight() { return _imageInfo->height; }

	inline void setFrameX(int frameX)
	{
		_imageInfo->currentFrameX = frameX;

		if (frameX > _imageInfo->maxFrameX)
		{
			_imageInfo->currentFrameX = _imageInfo->maxFrameX;
		}
	}

	inline void setFrameY(int frameY)
	{
		_imageInfo->currentFrameY = frameY;

		if (frameY > _imageInfo->maxFrameY)
		{
			_imageInfo->currentFrameY = _imageInfo->maxFrameY;
		}
	}


	inline int getMaxFrameX() { return _imageInfo->maxFrameX; }
	inline int getMaxFrameY() { return _imageInfo->maxFrameY; }

	inline int getFrameX() { return _imageInfo->currentFrameX; }
	inline int getFrameY() { return _imageInfo->currentFrameY; }

	inline int getFrameWidth() { return _imageInfo->frameWidth; }
	inline int getFrameHeight() { return _imageInfo->frameHeight; }
};

