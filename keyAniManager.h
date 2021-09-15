#pragma once
#include "singletonBase.h"
#include <map>

class image;
class animation;

class keyAniManager : public singletonBase<keyAniManager>
{
private:
	typedef map<string, animation*>				arrAnimation;
	typedef map<string, animation*>::iterator	iterAnimation;

private:
	arrAnimation _mTotalAnimation;

public:
	keyAniManager();
	~keyAniManager();

	HRESULT init();
	void release();
	void update();
	void render();

	//디폴트 애니메이션
	void addDefaultFrameAnimation(string animationKeyName, const char* imageKeyName, int fps, bool reverse, bool loop);
	void addDefaultFrameAnimation(string animationKeyName, const char* imageKeyName, int fps, bool reverse, bool loop, void* cbFunction);
	void addDefaultFrameAnimation(string animationKeyName, const char* imageKeyName, int fps, bool reverse, bool loop, void* cbFunction, void* obj);
	
	//배열 애니메이션
	void addArrayFrameAnimation(string animationKeyName, const char* imageKeyName, int* arr, int arrLen, int fps, bool loop);
	void addArrayFrameAnimation(string animationKeyName, const char* imageKeyName, int* arr, int arrLen, int fps, bool loop, void* cbFunction);
	void addArrayFrameAnimation(string animationKeyName, const char* imageKeyName, int* arr, int arrLen, int fps, bool loop, void* cbFunction, void* obj);

	//구간 애니메이션
	void addCoordinateFrameAnimation(string animationKeyName, const char* imageKeyName, int start, int end, int fps, bool reverse, bool loop);
	void addCoordinateFrameAnimation(string animationKeyName, const char* imageKeyName, int start, int end, int fps, bool reverse, bool loop, void* cbFunction);
	void addCoordinateFrameAnimation(string animationKeyName, const char* imageKeyName, int start, int end, int fps, bool reverse, bool loop, void* cbFunction, void* obj);

	void start(string animationKeyName);
	void stop(string animationKeyName);
	void pause(string animationKeyName);
	void resume(string animationKeyName);

	animation* findAnimation(string animationKeyName);
	bool isEnd(string animationKeyName);
	void deleteAll();
};

