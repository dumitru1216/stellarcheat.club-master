#include "includes.h"
#include "BASS/bass.h"
#include "BASS/API.h"
#include "radionstuff.h"
std::string lol[] = {
	"http://dutchcore.fm:8100/high",
	"http://uk5.internet-radio.com:8270/",
	"http://streams.bigfm.de/bigfm-deutschland-128-mp3",
	"https://streams.bigfm.de/bigfm-deutschrap-128-mp3",
	"http://air2.radiorecord.ru:805/rr_320",
	"http://air.radiorecord.ru:805/dub_320",
	"http://air.radiorecord.ru:805/teo_320",
	"http://www.radiorecord.ru/player-mini/?st=hbass",
	"http://www.radiorecord.ru/player-mini/?st=rapclassics"
};
void radio::radiostart() {
	/*
	if (Config::Misc::RadioMain::get().Enable)
	{
		BASS_ChannelStop(stream);
		stream = NULL;
		stream = BASS_StreamCreateURL(lol[Config::Misc::RadioMain::get().radio_station].c_str(), 0, 0, NULL, 0);
	}*/


	if (Config::Misc::RadioMain::get().Enable)
	{
		static bool radioInit = false;

		if (!radioInit) {
			BASS_Init(-1, 44100, BASS_DEVICE_3D, 0, NULL);
			stream = BASS_StreamCreateURL(lol[Config::Misc::RadioMain::get().radio_station].c_str(), 0, 0, NULL, 0);
			radioInit = true;

		}

		static float vol = Config::Misc::RadioMain::get().volume;
		BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, vol);



		BASS_ChannelPlay(stream, false);



	}


	if (!Config::Misc::RadioMain::get().Enable)
	{
		BASS_ChannelStop(stream);
		stream = NULL;
	}
}
radio* radion = new radio();