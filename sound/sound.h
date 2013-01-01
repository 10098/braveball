/// @file sound/sound.h
/// @author grigoryj

#ifndef SOUND_H
#define SOUND_H

#include <bass.h>

namespace sound
{
   struct SoundData
   {
      SoundData(const void* d, size_t s): data(d), size(s){}

      const void* data;
      size_t size;
   };

   class Sound
   {
   public:
      Sound(const SoundData& data, bool loop = false, bool is_music = false);
      ~Sound();

      void play(bool restart = false);
      void pause();
      void stop();
      void setVolume(float vol);
      bool isPlaying() const;

   private:
      DWORD m_stream;
      bool m_isMusic;
   };
}

#endif // SOUND_H