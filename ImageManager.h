#pragma once
#include "singletonBase.h"
#include "image.h"
#include <map>

class ImageManager : public singletonBase<ImageManager>
{
private:
	//first - string
	//second - image*
	typedef map<string, image*>				mapImageList;
	typedef map<string, image*>::iterator	mapImageIter;

private:
	mapImageList _mImageList;

public:

	ImageManager();
	~ImageManager();

	HRESULT init();
	void release();

	image* addImage(string strKey, LPCWSTR fileName, int width, int height);

	image* addFrameImage(string strKey, LPCWSTR fileName, int width, int height, int frameX, int frameY);

	image* findImage(string strKey);

	bool deleteImage(string strKey);

	bool deleteAll();

	image* currentImage(string strKey);
};

