/// @file sound/sound_system.cpp
/// @author grigoryj

#include "sound/sound_system.h"
#include <bass.h>
#include <stdexcept>

namespace sound
{
   SoundSystem::SoundSystem()
   {
      if(!BASS_Init(1, 44100, 0, 0, NULL))
      {
         throw std::runtime_error("Failed top init BASS");
      }
   }

   SoundSystem::~SoundSystem()
   {
      BASS_Free();
   }

}