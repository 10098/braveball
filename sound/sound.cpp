#include "sound/sound.h"

namespace sound
{
   Sound::Sound(const SoundData& data, bool loop, bool is_music) :
      m_stream(is_music ? BASS_MusicLoad(true, data.data, 0, data.size, loop ? BASS_SAMPLE_LOOP : 0, 44100) : BASS_StreamCreateFile(true, data.data, 0, data.size, loop ? BASS_SAMPLE_LOOP : 0)),
      m_isMusic(is_music)
   {}

   Sound::~Sound()
   {
      if(m_isMusic)
         BASS_MusicFree(m_stream);
      else
         BASS_StreamFree(m_stream);
   }

   void Sound::play(bool restart)
   {
      BASS_ChannelPlay(m_stream, restart);
   }

   void Sound::pause()
   {
      BASS_ChannelPause(m_stream);
   }

   void Sound::stop()
   {
      BASS_ChannelStop(m_stream);
   }

   void Sound::setVolume(float vol)
   {
      BASS_ChannelSetAttribute(m_stream, BASS_ATTRIB_VOL, vol);
   }

   bool Sound::isPlaying() const
   {
      return BASS_ChannelIsActive(m_stream) == BASS_ACTIVE_PLAYING;
   }
}