#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

class Sound
{
    private:
        Mix_Chunk* sound;
    public:
        Sound(const char* filename);
        ~Sound();
        void play();
};

Sound::Sound(const char* filename)
{
    sound = Mix_LoadWAV(filename);
}

Sound::~Sound()
{
    Mix_FreeChunk(sound);
}

void Sound::play()
{
    Mix_PlayChannel( -1, sound, 0 );
}

#endif // SOUND_H_INCLUDED
