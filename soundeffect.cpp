#include "soundeffect.h"

SoundEffect::SoundEffect():lastSound(-1)
{

}

SoundEffect::~SoundEffect()
{
    deleteSounds();
}
void SoundEffect::addSound(const char *fname, float volume, bool infinite)
{
    QSoundEffect * effect = new QSoundEffect;;
    effect->setSource(QUrl::fromLocalFile(QString(":sounds/") + fname));
    effect->setVolume(static_cast<double>(volume));
    if (infinite)
        effect->setLoopCount(QSoundEffect::Infinite);
    sounds.append(effect);
}
void SoundEffect::playSound(int _isound)
{
    if (_isound >= sounds.size())
            return;
    if (_isound <= lastSound && playerBusy())
        return;
    if (lastSound>=0)
        sounds[lastSound]->stop();
    sounds[_isound]->play();
    lastSound = _isound;
}

void SoundEffect::stop()
{
    if (lastSound>=0)
        sounds[lastSound]->stop();
}
bool SoundEffect::playerBusy()
{
    if (lastSound < 0)
        return false;
    if (!sounds[lastSound]->isPlaying())
    {
        lastSound = -1;
        return false;
    }
    return true;
}

void SoundEffect::deleteSounds()
{
    for (int i=0; i < sounds.size(); i++)
    {
        delete sounds[i];
    }
    sounds.clear();

}
