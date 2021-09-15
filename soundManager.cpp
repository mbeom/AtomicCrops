#include "stdafx.h"
#include "soundManager.h"


soundManager::soundManager()
	: _system(nullptr),
	_channel(nullptr),
	_sound(nullptr)
{
}


soundManager::~soundManager()
{
}

HRESULT soundManager::init()
{
	//FMOD 엔진 초기화
	System_Create(&_system);

	_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, NULL);

	_sound = new Sound*[TOTALSOUNDBUFFER];
	_channel = new Channel*[TOTALSOUNDBUFFER];

	memset(_sound, 0, sizeof(Sound*)  * TOTALSOUNDBUFFER);
	memset(_channel, 0, sizeof(Channel*) * TOTALSOUNDBUFFER);

	_currentVolume = 0.5f;

	return S_OK;
}

void soundManager::release()
{
	_system->release();
	_system->close();

	delete[] _sound;
	delete[] _channel;
}

void soundManager::update()
{
	//FMOD 엔진 내부에 작동을 계속해서 최신화 시켜주려면 update 걸어두자
	_system->update();
}

void soundManager::addSound(string keyName, string soundName, bool bgm, bool loop)
{
	char* temp = NULL;
	char temp2[1280];
	strcpy_s(temp2, sizeof(temp2), soundName.c_str());
	temp = strtok_s(temp2, ".", &temp);
	_song_name = temp;

	if (loop)
	{
		if (bgm)
		{
			_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, NULL, &_sound[_mTotalSounds.size()]);
		}
		else
		{
			_system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, NULL, &_sound[_mTotalSounds.size()]);
		}
	}
	else
	{
		if (bgm)
		{
			_system->createStream(soundName.c_str(), FMOD_DEFAULT, NULL, &_sound[_mTotalSounds.size()]);
		}
		else
		{
			_system->createSound(soundName.c_str(), FMOD_DEFAULT, NULL, &_sound[_mTotalSounds.size()]);
		}
	}

	name_list.push_back(make_pair(keyName, _song_name));

	_mTotalSounds.insert(make_pair(keyName, &_sound[_mTotalSounds.size()]));
}


string soundManager::get_name(string keyName)
{
	vector<pair<string, string>>::iterator iter = name_list.begin();
	for (iter; iter != name_list.end(); iter++)
	{
		if (iter->first == keyName)
		{
			string temp = iter->second;
			return iter->second;
		}
	}
	return "";
}

unsigned int soundManager::get_length(string keyName)
{
	unsigned int temp = 0;
	arrSoundsIter iter = _mTotalSounds.begin();

	for (iter; iter != _mTotalSounds.end(); ++iter)
	{
		if (iter->first == keyName)
		{
			(*iter->second)->getLength(&temp, FMOD_TIMEUNIT_MS);
			return temp;
		}
	}

	return 0;
}

unsigned int soundManager::get_pos(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;
	unsigned int song_pos = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (iter->first == keyName)
		{
			_channel[count]->getPosition(&song_pos, FMOD_TIMEUNIT_MS);
			return song_pos;
		}
	}

	return song_pos;

}

void soundManager::set_pos(string keyName, unsigned int current_pos)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (iter->first == keyName)
		{
			_channel[count]->setPosition(current_pos, FMOD_TIMEUNIT_MS);
			break;
		}
	}
}

void soundManager::set_volume(string keyName, float volume)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setVolume(volume);
			break;
		}
	}
}


void soundManager::play(string keyName, float volume)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);

			_channel[count]->setVolume(volume);
			break;
		}
	}
}

void soundManager::stop(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->stop();
			break;
		}
	}
}

void soundManager::pause(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(true);
			break;
		}
	}
}

void soundManager::resume(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(false);
			break;
		}
	}
}

bool soundManager::isPlaySound(string keyName)
{
	bool isPlay;
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->isPlaying(&isPlay);
			break;
		}
	}

	return isPlay;
}

bool soundManager::isPauseSound(string keyName)
{
	bool isPause;
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->getPaused(&isPause);
			break;
		}
	}
	return isPause;
}

void soundManager::saveVolume(float volume)
{
	_currentVolume = volume;
}

float soundManager::currentVolume()
{
	return _currentVolume;
}
