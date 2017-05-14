#include <shiftCalculationThreads.h>
#include <qmath.h>

ShiftCalculation::ShiftCalculation(ProjectionCalculation *first, ProjectionCalculation *second){
    if(first != NULL){
        this->firstProjection = first;
    }else{
        this->firstProjection = NULL;
    }

    if(second != NULL){
        this->thisProjection = second;
    }else{
        this->thisProjection = NULL;
    }
}

ShiftCalculation::~ShiftCalculation(){
    this->firstProjection = NULL;
    this->thisProjection = NULL;
}

void ShiftCalculation::run(){
    qDebug() << "Shift Thread " << this->currentThread() << " calcu start.";

    if(this->firstProjection != NULL && this->thisProjection != NULL){
        if(this->thisProjection->isRunning()){
            this->thisProjection->wait();
        }

        if(this->firstProjection->isRunning()){
            this->firstProjection->wait();
        }

        QVector<QVector<CvPoint2D32f > > firstProjectionResult = this->firstProjection->projectionResult;
        QVector<QVector<CvPoint2D32f > > thisProjectionResult = this->thisProjection->projectionResult;

        ShiftCalculationD calcuX(&firstProjectionResult.at(0),&thisProjectionResult.at(0));
        ShiftCalculationD calcuY(&firstProjectionResult.at(1),&thisProjectionResult.at(1));
        ShiftCalculationD calcuU(&firstProjectionResult.at(2),&thisProjectionResult.at(2));
        ShiftCalculationD calcuV(&firstProjectionResult.at(3),&thisProjectionResult.at(3));

        calcuX.start();
        calcuY.start();
        calcuU.start();
        calcuV.start();

        calcuX.wait();
        calcuY.wait();
        calcuU.wait();
        calcuV.wait();

        this->resultShiftVector.push_back(calcuX.resultKesi);
        this->resultShiftVector.push_back(calcuY.resultKesi);
        this->resultShiftVector.push_back(calcuU.resultKesi);
        this->resultShiftVector.push_back(calcuV.resultKesi);

        qDebug() << "Shift Thread " << this->currentThread() << " calcu done.";
    }else{
        qDebug() << "Null pointer in ShiftCalculation thread " << this->currentThread();
    }
}

ShiftCalculationD::ShiftCalculationD(const QVector<CvPoint2D32f> *firstProjection, const QVector<CvPoint2D32f> *thisProjection){
    this->firstDProjectionResult = *firstProjection;
    this->thisDProjectionResult = *thisProjection;

    this->searchRange = qMax(firstDProjectionResult.length() / 4 , thisDProjectionResult.length() / 4);
}

ShiftCalculationD::~ShiftCalculationD(){
    ;
}

void ShiftCalculationD::run(){
    int suitableKesi = 0.0;
    double argmin = 0.0;

    int search_min = searchRange * (-1);
    int search_max = qAbs(searchRange);
    for(int kesi = search_min ; kesi <= search_max ; kesi++){
        double currentKesi = 0.0;
        int firstVectorLength = firstDProjectionResult.length();
        int thisVectorLength = thisDProjectionResult.length();
        for(int i = 0 ; i < firstVectorLength ; i++){
            if(i + kesi < 0 || i + kesi >= thisVectorLength){
                continue;
            }

            currentKesi += qAbs(firstDProjectionResult.at(i).x * thisDProjectionResult.at(i + kesi).y
                                - thisDProjectionResult.at(i + kesi).x * firstDProjectionResult.at(i).y);
        }

        if(kesi == search_min){
            suitableKesi = kesi;
            argmin = currentKesi;
        }else{
            if(argmin - currentKesi >= 0.0000000001){
                argmin = currentKesi;
                suitableKesi = kesi;
            }
        }
    }

    this->resultKesi = suitableKesi;
}
