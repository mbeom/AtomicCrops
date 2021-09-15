#pragma once
#include "singletonBase.h"
#include <map>

#include "inc/fmod.hpp"
#pragma comment(lib, "lib/fmodex64_vc.lib")

using namespace FMOD;

#define SOUNDBUFFER 10
#define EXTRACHANNELBUFFER 5

#define TOTALSOUNDBUFFER SOUNDBUFFER + EXTRACHANNELBUFFER

class soundManager : public singletonBase<soundManager>
{
private:
	typedef map<string, Sound**>			arrSounds;
	typedef map<string, Sound**>::iterator	arrSoundsIter;

	typedef map<string, Channel**>			 arrChannels;
	typedef map<string, Channel**>::iterator arrChannelIter;

private:
	System*		_system;
	Sound**		_sound;
	Channel**	_channel;

	float _currentVolume;

	char* _song_name;
	arrSounds _mTotalSounds;
	vector<pair<string, string>> name_list;

public:
	soundManager();
	~soundManager();

	HRESULT init();
	void release();
	void update();

	void addSound(string keyName, string soundName, bool bgm, bool loop);
	string get_name(string keyName);

	unsigned int get_length(string keyName);
	unsigned int get_pos(string keyName);
	void set_pos(string keyName, unsigned int current_pos);
	void set_volume(string keyName, float volume);

	void play(string keyName, float volume = 1.0f);
	void stop(string keyName);
	void pause(string keyName);
	void resume(string keyName);

	//È£¿Á½ÃÀÌ ¾µ±îºÁ ¸¸µé¾îµÒ
	bool isPlaySound(string keyName);
	bool isPauseSound(string keyName);

	void saveVolume(float volume);
	float currentVolume();
};

