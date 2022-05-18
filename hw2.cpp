#include "hw2.h"
#include <fstream>
#include <iostream>

using namespace std;

Img::Img()
{
    height = 1000;
    width = 1000;
    srcImg = new int[height*width];
    x_c = -1;
    y_c = -1;
}
Img::Img(int w, int h)
{
    height = h;
    width = w;
    srcImg = new int[height*width];
    x_c = -1;
    y_c = -1;
}
Img::Img( const int* src, int w, int h)
{
    height = h;
    width = w;
    srcImg = new int[height*width];
    for(int i = 0; i < height*width; i++)
    {
        srcImg[i] = src[i];
    }
    x_c = -1;
    y_c = -1;
}
Img::~Img()
{
    width = 0;
    height = 0;
    if(srcImg != nullptr)
    {
        delete[] srcImg;
    }
}




ImageProcess::ImageProcess()
{
    srcImg = new Img();
    processedImg = new Img();
    mask = new Img(3,3);
    for(int i = 0; i < this->srcImg->height*this->srcImg->width;i++)
    {
        this->processedImg->srcImg[i] = 0;
    }
}
/* выделяется память для картинки размера w*h */
ImageProcess::ImageProcess(int w, int h)
{
    srcImg = new Img(w, h);
    processedImg = new Img(w,h);
    mask = new Img(3,3);
    for(int i = 0; i < this->srcImg->height*this->srcImg->width;i++)
    {
        this->processedImg->srcImg[i] = 0;
    }
}
/* выделяется память для картинки размера w*h и копируется картинка*/
ImageProcess::ImageProcess(const Img* img)
{
    srcImg = new Img(img->srcImg, img->width, img->height);
    srcImg = new Img(img->width, img->height);
    mask = new Img(3,3);
    for(int i = 0; i < this->srcImg->height*this->srcImg->width;i++)
    {
        this->processedImg->srcImg[i] = 0;
    }
}
/* выделяется память для картинки копируется картинка из файла*/
ImageProcess::ImageProcess(const char* fileName)
{
    ifstream fin(fileName);
    int ht;
    int wt;
    fin >> ht;
    fin >> wt;
    ImageProcess(wt, ht);
    for(int i = 0; i < ht*wt; i++)
    {
        char t;
        fin >> t;
        srcImg->srcImg[i] = int(t) - 48;
    }
    fin.close();
    for(int i = 0; i < this->srcImg->height*this->srcImg->width;i++)
    {
        this->processedImg->srcImg[i] = 0;
    }
}
ImageProcess::~ImageProcess()
{
    delete srcImg;
    delete processedImg;
    delete mask;
}


/*задание маски*/
int ImageProcess::updateMask(const Img &_mask)
{
    for(int i = 0; i < 9; i++)
    {
        mask->srcImg[i] = _mask.srcImg[i];
    }

    mask->x_c = _mask.x_c + 1; //Если брать верхнюю левую точку маски как 1;1 - "+1" убрать.
    mask->y_c = _mask.y_c + 1;
    return 0;
}

/*перезаписать исходную картинку картинкой, которая была получена в результате дилотации и/или эрозии*/
int ImageProcess::updateSrcImg()
{
//    if(processedImg->srcImg != nullptr)
//    {
//        srcImg->height = processedImg->height;
//        srcImg->width = processedImg->width;
//        for(int i = 0; i < processedImg->height*processedImg->width; i++)
//        {
//            srcImg->srcImg[i] = processedImg->srcImg[i];
//        }
//        return 0;
//    }
//    else
//    {
//        return -1;
//    }
    //srcImg->height = processedImg->height;
    //srcImg->width = processedImg->width;
    for(int i = 0; i < (srcImg->height)*(srcImg->width);i++)
    {
        srcImg->srcImg[i] = processedImg->srcImg[i];
    }
    return 0;
}

/*дилатация картинки, результат записать в processedImg*/
    /*
        input		:	srcImg = 1, обрабатывается srcImg  - результат записывается в processedImg
                        srcImg = 0, обрабатывается processedImg - результат записывается в processedImg
    */
int ImageProcess::dilatation(int srcImg)
{
    if(mask==nullptr) //Ну а вдруг
    {
        return -1;
    }
    if(srcImg == 0)
    {
        for(int i = 0; i < this->processedImg->height; i++)
        {
            for(int j = 0; j < this->processedImg->width;j++)
            {
                if(this->processedImg->srcImg[i*this->processedImg->width + j] == 1)
                {
                    for(int h = 0; h < this->mask->height; h++) //Накладываем маску
                    {
                        for(int w = 0; w < this->mask->width; w++)
                        {
                            int temph, tempw;
                            temph = (i-(this->mask->height - this->mask->y_c))+h;
                            if(this->mask->srcImg[h*this->mask->width + w] == 1)
                            {
                                tempw = (j-(this->mask->width-this->mask->x_c))+w;
                                if(this->processedImg->srcImg[temph*this->processedImg->width + tempw]==0)
                                    this->processedImg->srcImg[temph*this->processedImg->width + tempw] = 2;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        if(srcImg == 1)
        {
            for(int i = 0; i < this->srcImg->height; i++)
            {
                for(int j = 0; j < this->srcImg->width;j++)
                {
                    if(this->srcImg->srcImg[i*this->srcImg->width + j] == 1)
                    {
                        for(int h = 0; h < this->mask->height; h++) //Накладываем маску
                        {
                            for(int w = 0; w < this->mask->width; w++)
                            {
                                int temph, tempw;
                                temph = (i-(this->mask->height - this->mask->y_c))+h;
                                if(this->mask->srcImg[h*this->mask->width + w] == 1)
                                {
                                    tempw = (j-(this->mask->width-this->mask->x_c))+w;
                                    if(this->processedImg->srcImg[temph*this->processedImg->width + tempw]==0)
                                        this->processedImg->srcImg[temph*this->processedImg->width + tempw] = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for(int i = 0; i < processedImg->height*processedImg->width; i++)
    {
        if(processedImg->srcImg[i] == 2)
        {
            processedImg->srcImg[i] = 1;
        }
    }
    return 0;
}
    /*эрозия картинки, результат записать в processedImg*/
int ImageProcess::erosion(int srcImg)
{
    if(mask == nullptr) //гурдв а уН
    {
        return -1;
    }

    if(srcImg == 0)
    {
        for(int i = 0; i < this->processedImg->height; i++)
        {
            for(int j = 0; j < this->processedImg->width; j++)
            {
                if(this->processedImg->srcImg[i * this->processedImg->width + j] == this->mask->srcImg[(this->mask->y_c-1)*this->mask->width + this->mask->x_c - 1]);
                {
                    if(this->processedImg->srcImg[i * this->srcImg->width + j] == 1) //Если не закрашен, то зачем его удалять то...
                    {
                        bool ka = true;
                        for(int h = 0; h < this->mask->height; h++) //Проверяем, соответствует ли он маске
                        {
                            for(int w = 0; w < this->mask->width; w++)
                            {
                                if(this->mask->srcImg[h*this->mask->width + w] == 1)
                                {
                                    int temph, tempw;
                                    temph = (i-(this->mask->height - this->mask->y_c))+h;
                                    tempw = (j-(this->mask->width-this->mask->x_c))+w;
                                    if(this->processedImg->srcImg[temph*this->processedImg->width + tempw] == 0)
                                        ka = false;
                                }
                            }
                        }
                        if(!ka)
                        {
                            processedImg->srcImg[i * this->srcImg->width + j] = 2;
                        }
                    }
                }
            }
        }
    }
    else
    {
        if(srcImg == 1)
        {
            for(int i = 0; i < this->srcImg->height*this->srcImg->width; i++)
            {
                this->processedImg->srcImg[i] = this->srcImg->srcImg[i];
            }

            for(int i = 0; i < this->srcImg->height; i++)
            {
                for(int j = 0; j < this->srcImg->width; j++)
                {
                    if(this->srcImg->srcImg[i * this->srcImg->width + j] == this->mask->srcImg[(this->mask->y_c-1)*this->mask->width + this->mask->x_c - 1]);
                    {
                        if(this->srcImg->srcImg[i * this->srcImg->width + j] == 1) //Если не закрашен, то зачем его удалять то...
                        {
                            bool ka = true;
                            for(int h = 0; h < this->mask->height; h++) //Проверяем, соответствует ли он маске
                            {
                                for(int w = 0; w < this->mask->width; w++)
                                {
                                    if(this->mask->srcImg[h*this->mask->width + w] == 1)
                                    {
                                        int temph, tempw;
                                        temph = (i-(this->mask->height - this->mask->y_c))+h;
                                        tempw = (j-(this->mask->width-this->mask->x_c))+w;
                                        if(this->srcImg->srcImg[temph*this->processedImg->width + tempw] != this->mask->srcImg[h*this->mask->width + w])
                                            ka = false;
                                    }
                                }
                            }
                            if(!ka)
                            {
                                processedImg->srcImg[i * this->srcImg->width + j] = 2;
                            }
                        }
                    }
                }
            }
        }
    }
    for(int i = 0; i < processedImg->height*processedImg->width; i++)
    {
        if(processedImg->srcImg[i] == 2)
        {
            processedImg->srcImg[i] = 0;
        }
    }
    return 0;
}

/*
    загрузить/сохранить из файла с имененм fileName картинку
    input		:	fileName - имя файла
                    format - формат файла 	0 - последовательность 0 и 1, без перехода на новую строку
                                            1 - последовательность 0 и 1, с переходом на новую строку
*/
int ImageProcess::loadImgFromFile(const char *fileName, int format)
{
    ifstream fin(fileName);
    if(!fin.is_open())
    {
        return 1;
    }

    int ht;
    int wt;
    fin >> ht;
    fin >> wt;
    for(int i = 0; i < ht*wt; i++)
    {
        char t;
        fin >> t;
        srcImg->srcImg[i] = int(t) - 48;
    }
    fin.close();
    return 0;
}

int ImageProcess::saveImgToFile(const char* fileName, int format)
{
    ofstream fout;
    fout.open(fileName, ios_base::out);
    if(format == 1)
    {
        fout << srcImg->width << "\t" << srcImg->height << "\n";
        for(int i = 0; i < srcImg->height;i++)
        {
            for(int j = 0; j < srcImg->width; j++)
            {
                fout << srcImg->srcImg[srcImg->width * i + j];
            }
            fout << "\n";
        }
    }
    else
    {
        if(format == 0)
        {
            fout << srcImg->width << "\t" << srcImg->height << "\n";
            for(int i = 0; i < srcImg->height * srcImg->width; i++)
            {
                fout << srcImg->srcImg[i];
            }
        }
        else
        {
            fout.close();
            return 1;
        }
    }
    fout.close();
    return 0;
}
