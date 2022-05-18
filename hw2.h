#ifndef HW2_H
#define HW2_H
//Дилотация - исходная картинка = контур. Эррозия для контура не нужна. Но алгоритм нужен.
struct Img
{
    int* srcImg;
    int width;
    int height;
    int x_c;
    int y_c; // координаты центра для опорного элемента
    Img();
    Img(int w, int h);
    Img( const int* src, int w, int h);
    ~Img();
};

class ImageProcess {
private :
    Img* srcImg;
    Img* processedImg;
    Img* mask;
    //здесь можно объявлять другие переменные и функции
public :
    //здесь нельзя объявлять функции и переменные
    /**/
    ImageProcess();
    /* выделяется память для картинки размера w*h */
    ImageProcess(int w, int h);
    /* выделяется память для картинки размера w*h и копируется картинка*/
    ImageProcess(const Img* img);
    /* выделяется память для картинки копируется картинка из файла*/
    ImageProcess(const char* fileName); //Игнорировать символы /n и /r
    ~ImageProcess();

    /*задание маски*/
    int updateMask(const Img& _mask);
    /*перезаписать исходную картинку картинкой, которая была получена в результате дилотации и/или эрозии*/
    int updateSrcImg();
    /*дилатация картинки, результат записать в processedImg*/
        /*
            input		:	srcImg = 1, обрабатывается srcImg  - результат записывается в processedImg
                            srcImg = 0, обрабатывается processedImg - результат записывается в processedImg
        */
        int dilatation(int srcImg = 1);	//исправить на dilatation
        /*эрозия картинки, результат записать в processedImg*/
        int erosion(int srcImg = 1);
    /*
        загрузить/сохранить из файла с имененм fileName картинку
        input		:	fileName - имя файла
                        format - формат файла 	0 - последовательность 0 и 1, без перехода на новую строку
                                                1 - последовательность 0 и 1, с переходом на новую строку
    */
    int loadImgFromFile(const char* fileName, int format = 1);
    int saveImgToFile(const char* fileName, int format = 1);
};

#endif // HW2_H


