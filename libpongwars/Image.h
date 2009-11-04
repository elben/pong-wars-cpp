//Image class
//Provides basic Image data

#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

class Image
{
    private:
        static int id;
        std::string name;
        SDL_Rect imgSrc;
        SDL_Rect imgDest;
    public:

        //constructors
        Image();
        Image(std::string thisName, int xSrc, int ySrc, int x, int y, int w, int h);
        ~Image();

        //accessors
        int getID();
        std::string getName();
        SDL_Rect getSrc();
        SDL_Rect getDest();
        int getX();
        int getY();
        int getXsrc();
        int getYsrc();
        int getW();
        int getH();

        //modifiers
        void setName(std::string thisName);
        void setX(int thisX);
        void setY(int thisY);
        void setXsrc(int thisX);
        void setYsrc(int thisY);
        void setW(int thisW);
        void setH(int thisH);
};

Image::Image()
{
    id++;
    name = "";
}

Image::~Image()
{

}

Image::Image(std::string thisName, int xSrc, int ySrc, int x, int y, int w, int h)
{
    id++;
    name = thisName;
    imgSrc.x = xSrc;
    imgSrc.y = ySrc;
    imgDest.x = x;
    imgDest.y = y;
    imgSrc.w = imgDest.w = w;
    imgSrc.h = imgDest.h = h;
}

int Image::getID() { return id; }
SDL_Rect Image::getSrc() { return imgSrc; }
SDL_Rect Image::getDest() { return imgDest; }
int Image::getX() { return imgDest.x; }
int Image::getY() { return imgDest.y; }
int Image::getW() { return imgDest.w; }
int Image::getH() { return imgDest.h; }
int Image::getXsrc() { return imgSrc.x; }
int Image::getYsrc() { return imgSrc.y; }
void Image::setName(std::string thisName) { name = thisName; }
void Image::setX(int thisX) { imgDest.x = thisX; }
void Image::setY(int thisY) { imgDest.y = thisY; }
void Image::setXsrc(int thisX) { imgSrc.x = thisX; }
void Image::setYsrc(int thisY) { imgSrc.y = thisY; }
void Image::setW(int thisW){ imgDest.w = imgSrc.w = thisW; }
void Image::setH(int thisH){ imgDest.h = imgSrc.h = thisH; }

#endif // IMAGE_H_INCLUDED
