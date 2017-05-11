#ifndef SHIFTCALCULATIONTHREADS
#define SHIFTCALCULATIONTHREADS

#include <projectionThreads.h>
#include <QVector>
#include <opencv2/opencv.hpp>

class ProjectionCalculation;

class ShiftCalculation:public QThread{
private:
    ProjectionCalculation * firstProjection;
    ProjectionCalculation * thisProjection;

public:
    ShiftCalculation(ProjectionCalculation * first,ProjectionCalculation * second);
    ~ShiftCalculation();
    void run();
    QVector <int > resultShiftVector;
};


class ShiftCalculationD : public QThread{
private:
    QVector<CvPoint2D32f> firstDProjectionResult;
    QVector<CvPoint2D32f> thisDProjectionResult;
    int searchRange;

public:
    ShiftCalculationD(const QVector<CvPoint2D32f> *,const QVector<CvPoint2D32f>*);
    ~ShiftCalculationD();
    void run();
    int resultKesi;
};

#endif // SHIFTCALCULATIONTHREADS
