#include "stdafx.h"
#include "animation.h"


animation::animation()
	: _frameNum(0),
	_frameWidth(0),
	_frameHeight(0),
	_loop(FALSE),
	_frameUpdateSec(0),
	_elapsedSec(0),
	_nowPlayIdx(0),
	_play(FALSE),
	_end(FALSE)
{
}


animation::~animation()
{
}

HRESULT animation::init(int totalW, int totalH, int frameW, int frameH)
{
	_frameWidth = frameW;
	int _frameNumWidth = totalW / _frameWidth;

	_frameHeight = frameH;
	int _frameNumHeight = totalH / _frameHeight;

	//총 프레임 장수를 계산한다
	_frameNum = _frameNumWidth * _frameNumHeight;

	//hoxy..모르니까 벡터는 한 번 싹 지워주고
	_frameList.clear();

	//프레임 위치 계산
	for (int i = 0; i < _frameNumHeight; ++i)
	{
		for (int j = 0; j < _frameNumWidth; j++)
		{
			POINT framePos;
			framePos.x = j * _frameWidth;
			framePos.y = i * _frameHeight;

			_frameList.push_back(framePos);
		}
	}

	//디폴트 애니메이션을 우선 둔다
	setDefPlayFrame();

	return S_OK;
}

void animation::release()
{
}

void animation::setDefPlayFrame(BOOL reverse, BOOL loop)
{
	_obj = NULL;
	_callbackFunction = NULL;
	_callbackFunctionParameter = NULL;

	_loop = loop;

	_playList.clear();

	if (reverse)
	{
		for (int i = 0; i < _frameNum; ++i)
		{
			_playList.push_back(i);
		}

		for (int i = _frameNum - 1; i > 0; --i)
		{
			_playList.push_back(i);
		}
	}
	else
	{
		for (int i = 0; i < _frameNum; ++i)
		{
			_playList.push_back(i);
		}
	}
}

void animation::setDefPlayFrame(BOOL reverse, BOOL loop, CALLBACK_FUNCTION cbFunction)
{
	_obj = NULL;
	_callbackFunction = cbFunction;
	_callbackFunctionParameter = NULL;

	_loop = loop;
	_playList.clear();

	if (reverse)
	{
		for (int i = 0; i < _frameNum; ++i)
		{
			_playList.push_back(i);
		}

		for (int i = _frameNum - 1; i > 0; --i)
		{
			_playList.push_back(i);
		}
	
	}
	else
	{
		for (int i = 0; i < _frameNum; ++i)
		{
			_playList.push_back(i);
		}
	}
}

void animation::setDefPlayFrame(BOOL reverse, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void * obj)
{
	_obj = obj;
	_callbackFunction = NULL;
	_callbackFunctionParameter = cbFunction;

	_loop = loop;

	_playList.clear();

	if (reverse)
	{
		for (int i = 0; i < _frameNum; ++i)
		{
			_playList.push_back(i);
		}

		for (int i = _frameNum - 1; i > 0; --i)
		{
			_playList.push_back(i);
		}
	}
	else
	{

		for (int i = 0; i < _frameNum; ++i)
		{
			_playList.push_back(i);
		}
	}
}

void animation::setPlayFrame(int* playArr, int arrLen, BOOL loop)
{
	_obj = NULL;
	_callbackFunction = NULL;
	_callbackFunctionParameter = NULL;

	_loop = loop;
	_playList.clear();

	for (int i = 0; i < arrLen; ++i)
	{
		_playList.push_back(playArr[i]);
	}

}

void animation::setPlayFrame(int * playArr, int arrLen, BOOL loop, CALLBACK_FUNCTION cbFunction)
{
	_obj = NULL;
	_callbackFunction = cbFunction;
	_callbackFunctionParameter = NULL;
	_loop = loop;
	_playList.clear();

	for (int i = 0; i < arrLen; ++i)
	{
		_playList.push_back(playArr[i]);
	}
}

void animation::setPlayFrame(int * playArr, int arrLen, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void * obj)
{
	_obj = obj;
	_callbackFunction = NULL;
	_callbackFunctionParameter = cbFunction;
	_loop = loop;
	_playList.clear();

	for (int i = 0; i < arrLen; ++i)
	{
		_playList.push_back(playArr[i]);
	}
}

void animation::setPlayFrame(int start, int end, BOOL reverse, BOOL loop)
{
	_obj = NULL;
	_callbackFunction = NULL;
	_callbackFunctionParameter = NULL;
	_loop = loop;
	_playList.clear();

	if (start == end)
	{
		_playList.clear();
		stop();
		return;
	}

	if (start > end)
	{
		if (reverse)
		{
			for (int i = start; i >= end; --i)
			{
				_playList.push_back(i);
			}

			for (int i = end + 1; i < start; ++i)
			{
				_playList.push_back(i);
			}
		}
		else
		{
			for (int i = start; i >= end; --i)
			{
				_playList.push_back(i);
			}
		}
	}
	else
	{
		if (reverse)
		{
			for (int i = start; i < end; ++i)
			{
				_playList.push_back(i);
			}

			for (int i = end - 1; i > start; --i)
			{
				_playList.push_back(i);
			}
		}
		else
		{
			for (int i = start; i < end; ++i)
			{
				_playList.push_back(i);
			}
		}
	}

}

void animation::setPlayFrame(int start, int end, BOOL reverse, BOOL loop, CALLBACK_FUNCTION cbFunction)
{
	_obj = NULL;
	_callbackFunction = cbFunction;
	_callbackFunctionParameter = NULL;
	_loop = loop;
	_playList.clear();

	if (start == end)
	{
		_playList.clear();
		stop();
		return;
	}

	if (start > end)
	{
		if (reverse)
		{
			for (int i = start; i >= end; --i)
			{
				_playList.push_back(i);
			}

			for (int i = end + 1; i < start; ++i)
			{
				_playList.push_back(i);
			}

		}
		else
		{
			for (int i = start; i >= end; --i)
			{
				_playList.push_back(i);
			}
		}
	}
	else
	{
		if (reverse)
		{
			for (int i = start; i < end; ++i)
			{
				_playList.push_back(i);
			}

			for (int i = end - 1; i > start; --i)
			{
				_playList.push_back(i);
			}
			
		}
		else
		{
			for (int i = start; i < end; ++i)
			{
				_playList.push_back(i);
			}
		}
	}

}

void animation::setPlayFrame(int start, int end, BOOL reverse, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void * obj)
{
	_obj = obj;
	_callbackFunction = NULL;
	_callbackFunctionParameter = cbFunction;
	_loop = loop;
	_playList.clear();

	if (start == end)
	{
		_playList.clear();
		stop();
		return;
	}

	if (start > end)
	{
		if (reverse)
		{
			for (int i = start; i >= end; --i)
			{
				_playList.push_back(i);
			}

			for (int i = end + 1; i < start; ++i)
			{
				_playList.push_back(i);
			}
			
		}
		else
		{
			for (int i = start; i >= end; --i)
			{
				_playList.push_back(i);
			}
		}
	}
	else
	{
		if (reverse)
		{
			for (int i = start; i < end; ++i)
			{
				_playList.push_back(i);
			}

			for (int i = end - 1; i > start; --i)
			{
				_playList.push_back(i);
			}
		}
		else
		{
			for (int i = start; i < end; ++i)
			{
				_playList.push_back(i);
			}
		}
	}

}

void animation::setFPS(int framePerSec)
{
	_frameUpdateSec = 1.0f / framePerSec;
}

void animation::frameUpdate(float elapsedTime)
{
	if (_play)
	{
		_elapsedSec += elapsedTime;

		if (_elapsedSec >= _frameUpdateSec)
		{
			_elapsedSec -= _frameUpdateSec;
			_nowPlayIdx++;

			if (_nowPlayIdx == _playList.size())
			{
				if (_loop) _nowPlayIdx = 0;
				else
				{
					_end = true;
					if (_obj == NULL)
					{
						if (_callbackFunction != NULL) _callbackFunction();
					}
					else
					{
						_callbackFunctionParameter(_obj);
					}

					_nowPlayIdx--;
					_play = FALSE;
				}
			}
		}
	}
}

void animation::start()
{
	_play = TRUE;
	_nowPlayIdx = 0;
	_end = false;
}

void animation::stop()
{
	_play = FALSE;
	_nowPlayIdx = 0;
}

void animation::pause()
{
	_play = FALSE;
}

void animation::resume()
{
	_play = TRUE;
}

bool animation::isCheck(int num)
{
	if (_nowPlayIdx == _playList.size() - num)
	{


		return true;
	}

	return false;
}