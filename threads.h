#ifndef THREADS
#define THREADS

#include <QThread>
#include <QVector>
#include <QDebug>

class ShiftCalculation;

class ProjectionCalculation:public QThread{
private:
    volatile int number;
    QImage *thisImage;

public:
    ProjectionCalculation(int thisNumber,QImage * thisImage);
    void run();
    ProjectionCalculation *firstCalculation;
    QVector<QVector<QVector<int> > >projectionResult;
    ShiftCalculation * thisShiftCalcu;
};

class ShiftCalculation:public QThread{
private:
    ProjectionCalculation * thisImage;
    ProjectionCalculation * firstImage;

public:
    void run();
    QVector <QVector<int> > resultShiftVector;
};

class DirectXProjectionCalculation:public QThread{
private:
    volatile int number;
    QImage *thisImage;
public:
    DirectXProjectionCalculation(int number,QImage* thisImage);
    ~DirectXProjectionCalculation();
    void run();
    QVector<QVector<int> > *result;
};

class DirectYProjectionCalculation:public QThread{
private:
    volatile int number;
    QImage *thisImage;
public:
    DirectYProjectionCalculation(int number,QImage* thisImage);
    ~DirectYProjectionCalculation();
    void run();
    QVector<QVector<int> > *result;
};

class DirectUProjectionCalculation:public QThread{
private:
    volatile int number;
    QImage *thisImage;
public:
    DirectUProjectionCalculation(int number,QImage* thisImage);
    ~DirectUProjectionCalculation();
    void run();
    QVector<QVector<int> > *result;
};

class DirectVProjectionCalculation:public QThread{
private:
    volatile int number;
    QImage *thisImage;
public:
    DirectVProjectionCalculation(int number,QImage* thisImage);
    ~DirectVProjectionCalculation();
    void run();
    QVector<QVector<int> > *result;
};

#endif // THREADS
