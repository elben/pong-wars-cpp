#ifndef MUSIC_H_INCLUDED
#define MUSIC_H_INCLUDED

class Music
{
    private:
        Mix_Music *music;
    public:
        Music(const char* filename);
        ~Music();
        void play();
        void pause();
        void resume();
        void stop();
};

Music::Music(const char* filename)
{
    music = Mix_LoadMUS(filename);
}

Music::~Music()
{
    Mix_FreeMusic(music);
}

void Music::play()
{
    if(Mix_PlayingMusic() == 0)
        Mix_PlayMusic(music, -1);
}

void Music::pause()
{
    if(Mix_PlayingMusic() != 0)
        Mix_PauseMusic();
}

void Music::stop()
{
    if(Mix_PlayingMusic() != 0)
        Mix_HaltMusic();
}

void Music::resume()
{
    if(Mix_PlayingMusic() == 0)
        Mix_ResumeMusic();
}

#endif // MUSIC_H_INCLUDED
