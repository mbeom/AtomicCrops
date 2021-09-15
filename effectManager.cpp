#include "stdafx.h"
#include "effectManager.h"
#include "effect.h"


effectManager::effectManager()
{
}

effectManager::~effectManager()
{
}

HRESULT effectManager::init()
{
	return S_OK;
}

void effectManager::release()
{
	iterTotalEffect vIter;
	iterEffect mIter;

	vIter = _vTotalEffect.begin();

	for (; vIter != _vTotalEffect.end(); ++vIter)
	{
		for (mIter = vIter->begin(); mIter != vIter->end();)
		{
			if (mIter->second.size() != NULL)
			{
				iterEffects vArrIter = mIter->second.begin();
				for (vArrIter; vArrIter != mIter->second.end();)
				{
					if (*vArrIter)
					{
						(*vArrIter)->release();
						delete *vArrIter;
						vArrIter = mIter->second.erase(vArrIter);
					}
					else
						++vArrIter;
				}
				mIter->second.clear();
			}
			else ++mIter;
		}
	}

}

void effectManager::update()
{
	iterTotalEffect vIter;
	iterEffect mIter;

	for (vIter = _vTotalEffect.begin(); vIter != _vTotalEffect.end(); ++vIter)
	{
		for (mIter = vIter->begin(); mIter != vIter->end(); ++mIter)
		{
			iterEffects vArrIter;
			for (vArrIter = mIter->second.begin(); vArrIter != mIter->second.end(); ++vArrIter)
			{
				(*vArrIter)->update();
			}
		}
	}
}

void effectManager::render()
{
	iterTotalEffect vIter;
	iterEffect mIter;

	for (vIter = _vTotalEffect.begin(); vIter != _vTotalEffect.end(); ++vIter)
	{
		for (mIter = vIter->begin(); mIter != vIter->end(); ++mIter)
		{
			iterEffects vArrIter;
			for (vArrIter = mIter->second.begin(); vArrIter != mIter->second.end(); ++vArrIter)
			{
				(*vArrIter)->render();
			}
		}
	}

}

void effectManager::addEffect(string effectName, const char* imageName, int imageWidth, int imageHeight, int effectWidth, int effectHeight, int fps, float elapsed, int buffer)
{
	image*		img;
	arrEffects	vEffectBuffer;
	arrEffect	mArrEffect;

	char temp[128]; // 1바이트
	strcpy_s(temp, sizeof(temp),imageName);

	wchar_t img_name[128]; // 2 바이트
	MultiByteToWideChar(0, 0, temp, sizeof(temp), img_name, sizeof(img_name)/2);
	LPCWSTR image_name = img_name;

	if (IMAGEMANAGER->findImage(imageName))
	{
		img = IMAGEMANAGER->findImage(imageName);
	}
	else
	{
		img = IMAGEMANAGER->addFrameImage(imageName, image_name, imageWidth, imageHeight, imageWidth/effectWidth, imageHeight/effectHeight);
	}
	

	for (int i = 0; i < buffer; ++i)
	{
		vEffectBuffer.push_back(new effect);
		vEffectBuffer[i]->init(img, effectWidth, effectHeight, fps, elapsed);
	}

	mArrEffect.insert(pair<string, arrEffects>(effectName, vEffectBuffer));

	_vTotalEffect.push_back(mArrEffect);

}

void effectManager::play(string effectName, int x, int y)
{
	iterTotalEffect vIter;
	iterEffect		mIter;

	for (vIter = _vTotalEffect.begin(); vIter != _vTotalEffect.end(); ++vIter)
	{
		for (mIter = vIter->begin(); mIter != vIter->end(); ++mIter)
		{
			if (!(mIter->first == effectName)) break;

			iterEffects vArrIter;
			for (vArrIter = mIter->second.begin(); vArrIter != mIter->second.end(); ++vArrIter)
			{
				if ((*vArrIter)->getIsRunning()) continue;
				(*vArrIter)->startEffect(x, y);
				return;
			}
		}
	}
}
