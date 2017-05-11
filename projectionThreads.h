#ifndef PROJECTIONTHREADS
#define PROJECTIONTHREADS

#include <QThread>
#include <QVector>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <shiftCalculationThreads.h>

class ProjectionCalculation:public QThread{
private:
    volatile int number;

public:
    ProjectionCalculation(int thisNumber,cv::Mat *thisImage);
    ~ProjectionCalculation();
    void run();
    QVector<QVector<CvPoint2D32f > >projectionResult;
    cv::Mat *thisImage; //grey Image
};

class DirectXProjectionCalculation:public QThread{
private:
    volatile int number;

public:
    DirectXProjectionCalculation(int number,cv::Mat *thisImage);
    ~DirectXProjectionCalculation();
    void run();
    QVector<CvPoint2D32f > result;
    cv::Mat *thisImage;
};

class DirectYProjectionCalculation:public QThread{
private:
    volatile int number;

public:
    DirectYProjectionCalculation(int number,cv::Mat *thisImage);
    ~DirectYProjectionCalculation();
    void run();
    QVector<CvPoint2D32f > result;
    cv::Mat *thisImage;
};

class DirectUProjectionCalculation:public QThread{
private:
    volatile int number;

public:
    DirectUProjectionCalculation(int number,cv::Mat *thisImage);
    ~DirectUProjectionCalculation();
    void run();
    QVector<CvPoint2D32f > result;
    cv::Mat *thisImage;
};

class DirectVProjectionCalculation:public QThread{
private:
    volatile int number;

public:
    DirectVProjectionCalculation(int number,cv::Mat *thisImage);
    ~DirectVProjectionCalculation();
    void run();
    QVector<CvPoint2D32f > result;
    cv::Mat *thisImage;
};

#endif // THREADS
