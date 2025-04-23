#ifndef SOUND_H
#define SOUND_H

#include <AudioToolbox/AudioToolbox.h>

class Sound
{
private:
    SystemSoundID wallSound;
    SystemSoundID appleSound;

public:
    Sound();
    ~Sound();
    
    void playWallSound();
    void playAppleSound();
};

#endif // SOUND_H 