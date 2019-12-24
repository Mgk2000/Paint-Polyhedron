#ifndef SOUNDEFFECT_H
#define SOUNDEFFECT_H
#include <QSoundEffect>

class SoundEffect
{
public:
    SoundEffect();
    ~SoundEffect();
    void addSound(const char *fname, float volume, bool infinite = false);
    QList <QSoundEffect*> sounds;
    void playSound(int _isound);
    void stop();
private:
    bool playerBusy();
    int lastSound;
    void deleteSounds();
};

#endif // SOUNDEFFECT_H
