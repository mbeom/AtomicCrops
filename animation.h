#pragma once
#include <vector>

typedef void(*CALLBACK_FUNCTION)(void);
typedef void(*CALLBACK_FUNCTION_PARAMETER)(void*);

//보이드 포인터
//비어있는 곳을 가르킨다? -> 무엇이든 다 가르킬수있다
//다만 내가 가르킨 것이 무엇인지 형을 지정해야 사용가능

class animation
{
public:
	//프레임 리스트
	typedef vector<POINT> _vFrameList;

	//재생목록
	typedef vector<int> _vPlayList;

private:
	int			_frameNum;			//프레임 갯수

	_vFrameList _frameList;
	_vPlayList	_playList;

	int			_frameWidth;		
	int			_frameHeight;

	float		_frameUpdateSec;	//프레임 업데이트
	float		_elapsedSec;		//틱

	DWORD		_nowPlayIdx;		//현재 플레이 인덱스
	BOOL		_play;				//재생 여부
	BOOL		_loop;
	BOOL		_end;

	void*						_obj;
	CALLBACK_FUNCTION			_callbackFunction;
	CALLBACK_FUNCTION_PARAMETER _callbackFunctionParameter;

public:
	animation();
	~animation();

	HRESULT init(int totalW, int totalH, int frameW, int frameH);
	void release();

	//디폴트 애니메이션 재생
	void setDefPlayFrame(BOOL reverse = FALSE, BOOL loop = FALSE );
	void setDefPlayFrame(BOOL reverse, BOOL loop, CALLBACK_FUNCTION cbFunction);
	void setDefPlayFrame(BOOL reverse, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj);
	
	//배열로 담아서 애니메이션 재생
	void setPlayFrame(int* playArr, int arrLen, BOOL loop = FALSE);
	void setPlayFrame(int* playArr, int arrLen, BOOL loop, CALLBACK_FUNCTION cbFunction);
	void setPlayFrame(int* playArr, int arrLen, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj);
		
	//구간 애니메이션 재생
	void setPlayFrame(int start, int end, BOOL reverse = FALSE, BOOL loop = false);
	void setPlayFrame(int start, int end, BOOL reverse, BOOL loop, CALLBACK_FUNCTION cbFunction);
	void setPlayFrame(int start, int end, BOOL reverse, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj);

	void setFPS(int framePerSec);

	void frameUpdate(float elapsedTime);

	void start();
	void stop();
	void pause();
	void resume();
	bool isCheck(int num);

	inline BOOL isPlay() { return _play; }
	inline BOOL isEnd() { return _end; }

	inline POINT getFramePos() { return _frameList[_playList[_nowPlayIdx]]; }

	inline int getFrameWidth() { return _frameWidth; }
	inline int getFrameHeight() { return _frameHeight; }

};

