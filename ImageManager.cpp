#include "stdafx.h"
#include "ImageManager.h"

ImageManager::ImageManager()
{
}

ImageManager::~ImageManager()
{
}

HRESULT ImageManager::init()
{
	return S_OK;
}

void ImageManager::release()
{
	deleteAll();
}

image* ImageManager::addImage(string strKey, LPCWSTR fileName, int width, int height)
{
	image* img = findImage(strKey);

	if (img) return img;

	img = new image;

	if (FAILED(img->D_Init(fileName, width, height)))
	{
		SAFE_DELETE(img);

		return nullptr;
	}

	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image* ImageManager::addFrameImage(string strKey, LPCWSTR fileName, int width, int height, int frameX, int frameY)
{
	image* img = findImage(strKey);

	if (img) return img;

	img = new image;

	if (FAILED(img->D_Init(fileName, width, height, frameX, frameY)))
	{
		SAFE_DELETE(img);

		return nullptr;
	}

	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image* ImageManager::findImage(string strKey)
{
	mapImageIter key = _mImageList.find(strKey);

	//찾았다!
	if (key != _mImageList.end())
	{
		return key->second;
	}

	return nullptr;
}

bool ImageManager::deleteImage(string strKey)
{
	mapImageIter key = _mImageList.find(strKey);

	if (key != _mImageList.end())
	{
		key->second->release();
		SAFE_DELETE(key->second);
		_mImageList.erase(key);

		return true;
	}

	return false;
}

bool ImageManager::deleteAll()
{
	mapImageIter iter = _mImageList.begin();

	for (; iter != _mImageList.end();)
	{
		//맵컨테이너안에 이미지가 있다면
		if (iter->second != nullptr)
		{
			iter->second->release();
			SAFE_DELETE(iter->second);
			iter = _mImageList.erase(iter);
		}
		else ++iter;
	}

	_mImageList.clear();

	return false;
}

image* ImageManager::currentImage(string strKey)
{
	mapImageIter key = _mImageList.find(strKey);

	if (key != _mImageList.end())
	{
		return key->second;
	}

	return nullptr;
}
