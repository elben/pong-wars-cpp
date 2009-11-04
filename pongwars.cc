/*
 * Copyright (c) 2006 Elben Shira
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */   

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include <string>
#include <iostream>
#include <cstdio> // for printf() call

#include "libpongwars/Image.h"
#include "libpongwars/Text.h"
#include "libpongwars/Timer.h"
#include "libpongwars/Sound.h"
#include "libpongwars/Music.h"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char* WINDOW_TITLE = "Pong Wars";
const int DEFAULT_VEL = 10;
const int DEFAULT_BALL_VEL = 5;
const int FRAMES_PER_SECOND = 60;
const int BALL_SIZE = 17;
const int SPECIAL2 = 5;
int Image::id = 0;
void draw(Image img, SDL_Surface* imgSurface, SDL_Surface* screenSurface);
SDL_Surface *load_image(std::string filename);
bool checkCollision(Image &img1, Image &img2);
bool collidedTopBottom(Image &img1, Image &img2);
void ai(Image &b, Image &p);
bool menu(SDL_Surface* screenSurface);

int xVel = DEFAULT_BALL_VEL;
int yVel = DEFAULT_BALL_VEL;
int velP1 = DEFAULT_VEL;
int velP2 = DEFAULT_VEL;
int useAI = 0;
int special2Ready1 = 0;
int special2Ready2 = 0;
bool special21 = false;
bool special22 = false;

int main(int argc, char **argv)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);

  Sound hit("resources/hit1.wav");
  Sound score("resources/score1.wav");
  Sound special1SFX("resources/special1.wav");
  Music music("resources/music1.mp3");
  Music special2Music("resources/special2Music.mp3");

  SDL_Surface* screen = SDL_SetVideoMode(
      WINDOW_WIDTH, WINDOW_HEIGHT, 0,
      SDL_HWSURFACE | SDL_DOUBLEBUF);
  SDL_WM_SetCaption(WINDOW_TITLE, 0);
  SDL_Event event;
  TTF_Font* font = TTF_OpenFont("resources/gunplay.ttf", 32);
  char temp[20];  //for displaying score
  int gameRunning = 1;
  Timer fps;
  Timer fpsCalc;
  int frame = 0;
  bool capFPS = true;
  bool newRound = true;

  int p1 = 0;
  int p2 = 0;

  SDL_Color foregroundColor = { 255, 255, 255 };
  //SDL_Color backgroundColor = { 0, 0, 255 };

  //SDL_Surface* textSurface = TTF_RenderText_Solid(font, "=== PONG WARS! ===", foregroundColor);
  SDL_Surface* fpsSurface;
  SDL_Surface* score1Surface = TTF_RenderText_Solid(font, "Player 1: ",
      foregroundColor);
  SDL_Surface* score2Surface = TTF_RenderText_Solid(font, "Player 2: ",
      foregroundColor);
  SDL_Surface* p1s;
  SDL_Surface* p2s;
  SDL_Surface* special2ReadyText = TTF_RenderText_Solid(font,
      "Special 2 READY!", foregroundColor);

  SDL_Rect textLocation = { WINDOW_WIDTH/2-140, 10, 0, 0 };
  SDL_Rect fpsLocation = { WINDOW_WIDTH/2-100, WINDOW_HEIGHT-40, 0, 0 };
  SDL_Rect p1label_rect = {10,10,0,0};
  SDL_Rect p2label_rect = {WINDOW_WIDTH-130,10,0,0};
  SDL_Rect p1Score = {10,60,0,0};
  SDL_Rect p2Score = {WINDOW_WIDTH-50,50,0,0};
  SDL_Rect special2ReadyTextPos1 = {10,70,0,0};
  SDL_Rect special2ReadyTextPos2 = {WINDOW_WIDTH-60,70,0,0};

  //Ball init
  Image ball("Ball", 0, 0, 300, 100, BALL_SIZE, BALL_SIZE);
  SDL_Surface* ballSurface = load_image("resources/ball1.bmp");
  SDL_SetColorKey(ballSurface, SDL_SRCCOLORKEY,
      SDL_MapRGB(ballSurface->format, 255, 0, 255));

  //player 1
  Image paddle1("Paddle", 0, 0, 0, WINDOW_HEIGHT/2, 16, 64);
  SDL_Surface* paddle1Surface = load_image("resources/paddle1.png");
  SDL_SetColorKey(paddle1Surface, SDL_SRCCOLORKEY,
      SDL_MapRGB(paddle1Surface->format, 255, 0, 255));
  bool qPressed = false;
  bool aPressed = false;
  bool special11 = true;

  //player 2
  Image paddle2("Paddle", 0, 0, WINDOW_WIDTH-12, WINDOW_HEIGHT/2, 16, 64);
  SDL_Surface* paddle2Surface = load_image("resources/paddle2.png");
  SDL_SetColorKey(paddle2Surface, SDL_SRCCOLORKEY,
      SDL_MapRGB(paddle2Surface->format, 255, 0, 255));
  bool upPressed = false;
  bool downPressed = false;
  bool special12 = true;

  //Background init
  Image bg("Background", 0, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  SDL_Surface* bgSurface = load_image("resources/bg1.jpg");
  Image left("Left Wall", 0, 0, 12, 0, 0, WINDOW_HEIGHT);
  Image right("Right Wall", 0, 0, WINDOW_WIDTH-12, 0, 0, WINDOW_HEIGHT);

  music.play();

  if(!menu(screen))
    goto quit;

  while (gameRunning)
  {
    if(newRound)
    {
      SDL_Delay(1000);

      frame = 0;
      fpsCalc.start();
      newRound = false;
    }

    fps.start();

    if (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        gameRunning = 0;
      }
    }

    SDLKey keyPressed = event.key.keysym.sym;

    if (event.type == SDL_KEYDOWN)
    {

      switch (keyPressed)
      {
        case SDLK_ESCAPE:
          //if(!menu(screen))
          gameRunning = false;
          break;
        case SDLK_w:
          qPressed = true;
          break;
        case SDLK_s:
          aPressed = true;
          break;
        case SDLK_d:
          if(special11)
          {
            xVel+=2;
            if(xVel == 0)
              xVel++;
            special11 = false;
            special1SFX.play();
          }
          break;
        case SDLK_a:
          if(special2Ready1 >= SPECIAL2)
          {
            special21 = true;
            special2Ready1 = 0;
            music.stop();
            special2Music.play();
          }
          break;
        case SDLK_UP:
          if(useAI == 0)
            upPressed = true;
          break;
        case SDLK_DOWN:
          if(useAI == 0)
            downPressed = true;
          break;
        case SDLK_RIGHT:
          if(special12)
          {
            xVel-=2;
            if(xVel == 0)
              xVel--;
            special12 = false;
            special1SFX.play();
          }
          break;
        case SDLK_LEFT:
          if(special2Ready2 >= SPECIAL2)
          {
            special22 = true;
            special2Ready2 = 0;
            music.stop();
            special2Music.play();
          }
      }
    }
    else if(event.type == SDL_KEYUP)
    {
      switch(keyPressed)
      {
        case SDLK_w:
          qPressed = false;
          break;
        case SDLK_s:
          aPressed = false;
          break;
        case SDLK_UP:
          upPressed = false;
          break;
        case SDLK_DOWN:
          downPressed = false;
          break;
      }
    }

    if(qPressed && paddle1.getY() > 0)
      paddle1.setY(paddle1.getY() - velP1);
    if(aPressed && paddle1.getY() < WINDOW_HEIGHT-64)
      paddle1.setY(paddle1.getY() + velP1);
    if(upPressed && paddle2.getY() > 0)
      paddle2.setY(paddle2.getY() - velP2);
    if(downPressed && paddle2.getY() < WINDOW_HEIGHT-64)
      paddle2.setY(paddle2.getY() + velP2);

    ball.setX(ball.getX() + xVel);

    if(checkCollision(ball, paddle1))
    {
      hit.play();

      ball.setX(ball.getX() - xVel);  //get it out of the way
      if(aPressed && yVel <= 0)   //paddle and ball opp directions
      {
        yVel++;
        yVel *= -1;
      }
      else if(qPressed && yVel > 0)
      {
        yVel--;
        yVel *= -1;
      }

      if(qPressed && yVel < 0) //paddle and ball same direction
        yVel--;
      else if(aPressed && yVel > 0)
        yVel++;


      xVel *= -1;
    }
    if(checkCollision(ball, paddle2))
    {
      hit.play();

      ball.setX(ball.getX() - xVel);  //get it out of the way
      if(downPressed && yVel < 0)   //paddle and ball opp directions
      {
        yVel++;
        yVel *= -1;
      }
      else if(upPressed && yVel > 0)
      {
        yVel--;
        yVel *= -1;
      }

      if(upPressed && yVel < 0) //paddle and ball same direction
        yVel--;
      else if(downPressed && yVel > 0)
        yVel++;


      xVel *= -1;
    }
    if(checkCollision(ball, left))
    {
      score.play();
      p2++;
      special2Ready2++;

      if(special21 || special22) //+2 to winner, -1 to loser
      {
        p2++;
        p1--;
        special21 = false;
        special22 = false;
        special2Music.stop();
        music.play();
      }

      xVel = DEFAULT_BALL_VEL;
      yVel = DEFAULT_BALL_VEL;

      special12 = true;   //reset special 1

      ball.setX(WINDOW_WIDTH/2 - 8);
      ball.setY(WINDOW_HEIGHT/2);

      newRound = true;
    }
    if(checkCollision(right, ball))
    {
      score.play();
      p1++;
      special2Ready1++;

      if(special21 || special22) //+2 to winner, -1 to loser
      {
        p2--;
        p1++;
        special21 = false;
        special22 = false;
        special2Music.stop();
        music.play();
      }

      xVel = -DEFAULT_BALL_VEL;
      yVel = DEFAULT_BALL_VEL;

      special11 = true;   //reset special 1

      ball.setX(WINDOW_WIDTH/2 - 8);
      ball.setY(WINDOW_HEIGHT/2);

      newRound = true;
    }

    ball.setY(ball.getY() + yVel);
    if(yVel == 0) yVel = 2;

    if(ball.getY() < 0 || ball.getY() > WINDOW_HEIGHT-ball.getH() ||
        checkCollision(ball, paddle1) || checkCollision(ball, paddle2))
    {
      ball.setY(ball.getY() - yVel);  //get it out of the way
      yVel *= -1;
    }

    //AI stuff
    if(useAI != 0)
      ai(ball, paddle2);

    //refresh screen
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

    sprintf(temp, "FPS: %.2f", frame/(fpsCalc.get_ticks()/1000.f));
    fpsSurface = TTF_RenderText_Solid(font, temp, foregroundColor);

    sprintf(temp, "%d", p1);
    p1s = TTF_RenderText_Solid(font, temp, foregroundColor);

    sprintf(temp, "%d", p2);
    p2s = TTF_RenderText_Solid(font, temp, foregroundColor);

    //draw all images
    draw(bg, bgSurface, screen);
    draw(ball, ballSurface, screen);
    draw(paddle1, paddle1Surface, screen);
    draw(paddle2, paddle2Surface, screen);
    //SDL_BlitSurface(textSurface, NULL, screen, &textLocation);  //text tile
    SDL_BlitSurface(fpsSurface, NULL, screen, &fpsLocation);  //text tile
    SDL_BlitSurface(score1Surface, NULL, screen, &p1label_rect);  //"player 1"
    SDL_BlitSurface(score2Surface, NULL, screen, &p2label_rect);  //"player 2"
    SDL_BlitSurface(p1s, NULL, screen, &p1Score);  //player 1 score
    SDL_BlitSurface(p2s, NULL, screen, &p2Score);  //player 2 score
    if(special2Ready1 >= SPECIAL2) SDL_BlitSurface(special2ReadyText,
        NULL, screen, &special2ReadyTextPos1);
    if(special2Ready2 >= SPECIAL2) SDL_BlitSurface(special2ReadyText,
        NULL, screen, &special2ReadyTextPos2);

    frame++;
    if(capFPS)
    {
      while(fps.get_ticks() < 1000 / FRAMES_PER_SECOND){ }
      //SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks() - 5);
    }

    SDL_Flip(screen);

    SDL_FreeSurface(p1s);
    SDL_FreeSurface(p2s);
    SDL_FreeSurface(fpsSurface);
    p1s = NULL;
    p2s = NULL;
    fpsSurface = NULL;
  }

quit:
  //SDL_FreeSurface(screen);
  SDL_FreeSurface(ballSurface);
  SDL_FreeSurface(score1Surface);
  SDL_FreeSurface(score2Surface);
  SDL_FreeSurface(paddle1Surface);
  SDL_FreeSurface(paddle2Surface);
  SDL_FreeSurface(bgSurface);
  screen = NULL;
  ballSurface = NULL;
  score1Surface = NULL;
  score2Surface = NULL;
  paddle1Surface = NULL;
  paddle2Surface = NULL;
  bgSurface = NULL;

  TTF_Quit();
  SDL_Quit();
  return 0;
}

void draw(Image img, SDL_Surface* imgSurface, SDL_Surface* screenSurface)
{
  SDL_Rect src = img.getSrc();
  SDL_Rect dest = img.getDest();
  SDL_BlitSurface(imgSurface, &src, screenSurface, &dest);
}

SDL_Surface *load_image(std::string filename)
{
  SDL_Surface* loadedImage = NULL;
  SDL_Surface* optimizedImage = NULL;

  loadedImage = IMG_Load(filename.c_str());

  if(loadedImage != NULL)
  {
    optimizedImage = SDL_DisplayFormat(loadedImage);
    SDL_FreeSurface(loadedImage);
  }

  return optimizedImage;
}

bool checkCollision(Image &img1, Image &img2)
{
  int left1 = img1.getX();
  int left2 = img2.getX();
  int right1 = img1.getX() + img1.getW();
  int right2 = img2.getX() + img2.getW();
  int top1 = img1.getY();
  int top2 = img2.getY();
  int bottom1 = img1.getY() + img1.getH();
  int bottom2 = img2.getY() + img2.getH();

  if(left1 >= right2) return false;
  if(right1 <= left2) return false;
  if(top1 >= bottom2) return false;
  if(bottom1 <= top2) return false;

  return true;
}


bool collidedTopBottom(Image &img1, Image &img2)
{
  int top1 = img1.getY();
  int top2 = img2.getY();
  int bottom1 = img1.getY() + img1.getH();
  int bottom2 = img2.getY() + img2.getH();
  if(checkCollision(img1, img2))
  {
    if(top1 >= bottom2) return false;
    if(bottom1 <= top2) return false;

    return true;
  }
  else return false;
}

void ai(Image &b, Image &p)
{
  int py = p.getY();
  int by = b.getY();
  int vel;

  if(xVel > 0)    //easy, medium, perfect: 4, 5
    vel = 5;
  else            //easy, medium, perfect: 2, 3
    vel = 3;

  if(yVel > 0 && py > by && py > 0)
  {
    p.setY(p.getY()-vel);
  }
  else if(yVel > 0 && py < by && py+64 < WINDOW_HEIGHT)
  {
    p.setY(p.getY()+vel);
  }
  else if(yVel < 0 && py < by && py+64 < WINDOW_HEIGHT)
  {
    p.setY(p.getY()+vel);
  }
  else if(yVel < 0 && py > by && py > 0)
  {
    p.setY(p.getY()-vel);
  }
}

bool menu(SDL_Surface* screenSurface)
{
  bool choosen = false;
  Image menu("Menu", 0, 0, 0, 0, 640, 480);
  SDL_Surface* menuSurface = load_image("resources/menu.jpg");
  draw(menu, menuSurface, screenSurface);
  SDL_Flip(screenSurface);
  SDL_Event event;
  while(!choosen)
  {
    SDL_PollEvent(&event);
    SDLKey menuPress = event.key.keysym.sym;
    if (event.type == SDL_KEYDOWN)
    {

      switch (menuPress)
      {
        case SDLK_ESCAPE:
          return false;
          break;
        case SDLK_1:
          useAI = 0;
          choosen = true;
          break;
        case SDLK_2:
          useAI = 1;
          choosen = true;
          break;
        case SDLK_3:
          useAI = 2;
          choosen = true;
          break;
        case SDLK_4:
          useAI = 3;
          choosen = true;
          break;
      }
    }
  }

  return true;
}
