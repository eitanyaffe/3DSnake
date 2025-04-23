#include "Sound.h"
#include <string>

Sound::Sound() {
    // Create system sound IDs for wall and apple sounds
    CFURLRef soundURL;
    
    // Use "Pop" sound for apple (a yummy, satisfying sound)
    soundURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
        CFSTR("/System/Library/Sounds/Pop.aiff"),
        kCFURLPOSIXPathStyle, false);
    AudioServicesCreateSystemSoundID(soundURL, &appleSound);
    CFRelease(soundURL);
    
    // Use "Funk" sound for wall/game over (a fun boink sound)
    soundURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
        CFSTR("/System/Library/Sounds/Funk.aiff"),
        kCFURLPOSIXPathStyle, false);
    AudioServicesCreateSystemSoundID(soundURL, &wallSound);
    CFRelease(soundURL);
}

Sound::~Sound() {
    // Clean up system sound IDs
    AudioServicesDisposeSystemSoundID(wallSound);
    AudioServicesDisposeSystemSoundID(appleSound);
}

void Sound::playWallSound() {
    AudioServicesPlaySystemSound(wallSound);
}

void Sound::playAppleSound() {
    AudioServicesPlaySystemSound(appleSound);
} 