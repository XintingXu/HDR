#include <QCoreApplication>
#include <QVector>
#include <QThread>
#include <QDebug>
#include <projectionThreads.h>
#include <shiftCalculationThreads.h>
#include <QImage>
#include <QVector>
#include <opencv2/opencv.hpp>
#include <opencv2/photo.hpp>

void save(QVector<cv::Mat *> *);
cv::Mat* QImage2Mat(const QImage *);
QImage* Mat2QImage(const cv::Mat *);
bool loadImage(QVector<QImage*> *);
void calcuMaxReserve(QVector<QVector<int> >* shiftResult, QVector<QImage*> *images, QVector<QVector<QPoint> > *reserveArea);
void cutImages(QVector<QImage*> *srcImages,QVector<cv::Mat*> *destImages,QVector<QVector<QPoint> > *reserveArea);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QVector<QImage*> source;

    if(loadImage(&source)){
        qDebug() << "load done. " << source.length();

        QVector<ProjectionCalculation *> projectionCalcutions;
        QVector<ShiftCalculation *> shiftCalculations;

        for(int item = 0 ; item < source.length() ; item++){

            if(source.at(item)->byteCount() != 0){
                cv::Mat *greyMat;
                greyMat = QImage2Mat(source.at(item));
                cv::cvtColor(*greyMat,*greyMat,CV_RGB2GRAY);

                qDebug() << "start calculate :" << item;
                projectionCalcutions.append(new ProjectionCalculation(item,greyMat));

                if(item != 0){
                    shiftCalculations.append(new ShiftCalculation(projectionCalcutions.at(0),projectionCalcutions.at(item)));
                }

                delete greyMat;
            }else{
                qDebug() << "error reading file";
            }
        }

        qDebug() << "start projection Calcu.";

        QVector<ProjectionCalculation*>::iterator it;
        for(it = projectionCalcutions.begin() ; it != projectionCalcutions.end() ; it++){
            (*it)->start();
        }

        qDebug() << "start shift Calcution";

        projectionCalcutions.at(0)->wait();

        QVector<ShiftCalculation*>::iterator it2;
        for(it2 = shiftCalculations.begin() ; it2 != shiftCalculations.end() ; it2++){
            (*it2)->start();
        }

        QVector<QVector<int> > allShiftResults;

        for(it2 = shiftCalculations.begin() ; it2 != shiftCalculations.end() ; it2++){
            (*it2)->wait();
            qDebug() << "shift array : " << (*it2)->resultShiftVector;
            allShiftResults.append((*it2)->resultShiftVector);
        }

        qDebug() << "shift Calcu done";

        QVector<QVector<QPoint> > reserveArea;

        qDebug() << "calcu reserve area";

        calcuMaxReserve(&allShiftResults,&source,&reserveArea);

        qDebug() << "calcu reserve area done";

        QVector<cv::Mat*> cuttedImages;

        qDebug() << "start cut images";

        cutImages(&source,&cuttedImages,&reserveArea);

        qDebug() << "images cut done";

        save(&cuttedImages);

        for(int i = 0 ; i < shiftCalculations.length() ; i++){
            delete shiftCalculations.at(i);
        }

        for(int i = 0 ; i < projectionCalcutions.length() ; i++){
            delete projectionCalcutions.at(i);
        }

    }else{
        qDebug() << "Cannot load images";
    }

    qDebug() << "end";
    //return a.exec();
}

bool loadImage(QVector<QImage*> *result){
    QImage *source = new QImage("dog1.jpg");
    QImage *source2 = new QImage("dog2.jpg");
    QImage *source3 = new QImage("dog3.jpg");

    result->append(source);
    result->append(source2);
    result->append(source3);

    return true;
}

cv::Mat* QImage2Mat(const QImage *source){
    cv::Mat temp;
    cv::Mat targetImage;

    if(source->format() == QImage::Format_ARGB32_Premultiplied){
        qDebug() << "Format ARGB32";
        cv::Mat(source->height(),source->width(),CV_8UC4,(uchar *)source->bits(),source->bytesPerLine()).copyTo(temp);
        cv::cvtColor(temp,targetImage,CV_BGR2RGB);
    }else if(source->format() == QImage::Format_RGB888){
        qDebug() << "Format RGB888";
        cv::Mat(source->height(),source->width(),CV_8UC3,(uchar *)source->bits(),source->bytesPerLine()).copyTo(temp);
        cv::cvtColor(temp,targetImage,CV_BGR2RGB);
    }else{
        qDebug() << "Format unmatched";
        cv::Mat(source->height(),source->width(),CV_8UC4,(uchar *)source->bits(),source->bytesPerLine()).copyTo(targetImage);
    }

    //cv::imwrite("juestRead.jpg",targetImage);
    cv::Mat *greyMat = new cv::Mat;
    cv::cvtColor(targetImage,*greyMat,CV_BGRA2RGB);
    //cv::imwrite("justReadGrey.jpg",greyMat);

    return (greyMat);
}

QImage *Mat2QImage(const cv::Mat * srcImage){
    QImage *dest = new QImage((uchar *)srcImage->data, srcImage->cols, srcImage->rows, srcImage->step, QImage::Format_RGB888);
    return (dest);
}

void calcuMaxReserve(QVector<QVector<int> > *shiftResult, QVector<QImage*> *images, QVector<QVector<QPoint> > *reserveArea){
    int maxShiftX = 0, minShiftX = 0, maxShiftY = 0, minShiftY = 0;

    QVector<QPoint> realShift;

    for(int i =0 ; i < shiftResult->length() ; i++){
        int Tx1 = shiftResult->at(i).at(1);
        int Ty1 = shiftResult->at(i).at(0);

//        int Kesi_x = shiftResult->at(i).at(2);
//        int Kesi_y = shiftResult->at(i).at(3);

//        int Tx2 = Kesi_x + Kesi_y;
//        int Ty2 = Kesi_x - Kesi_y;

//        realShift.append(QPoint((Tx1 + Tx2) / 2, (Ty1 + Ty2) / 2));

        realShift.append(QPoint(Tx1, Ty1));
    }

    for(int i = 0 ; i < realShift.length() ; i++){
        int x = realShift.at(i).x();
        int y = realShift.at(i).y();

        if(x > maxShiftX){
            maxShiftX = x;
        }

        if(x < minShiftX){
            minShiftX = x;
        }

        if(y > maxShiftY){
            maxShiftY = y;
        }

        if(y < minShiftY){
            minShiftY = y;
        }
    }

    QVector<QPoint> firstImage;
    firstImage.append(QPoint(qAbs(minShiftX), qAbs(minShiftY)));
    firstImage.append(QPoint(qAbs(minShiftX), images->at(0)->width() - maxShiftY));
    firstImage.append(QPoint(images->at(0)->height() - maxShiftX, qAbs(minShiftY)));
    firstImage.append(QPoint(images->at(0)->height() - maxShiftX, images->at(0)->width() - maxShiftY));
    reserveArea->append(firstImage);

    for(int i = 1 ; i < images->length() ; i++){
        QVector <QPoint> thisImage;
        thisImage.clear();
        QVector<QPoint> firstPoints;
        firstPoints = reserveArea->first();
        QPoint thisShift = realShift.at(i - 1);
        thisImage.append(QPoint(firstPoints.at(0).x() + thisShift.x(), firstPoints.at(0).y() + thisShift.y()));
        thisImage.append(QPoint(firstPoints.at(1).x() + thisShift.x(), firstPoints.at(1).y() + thisShift.y()));
        thisImage.append(QPoint(firstPoints.at(2).x() + thisShift.x(), firstPoints.at(2).y() + thisShift.y()));
        thisImage.append(QPoint(firstPoints.at(3).x() + thisShift.x(), firstPoints.at(3).y() + thisShift.y()));
        reserveArea->append(thisImage);
    }

    qDebug() << "max Reserve Area Calcu done";
}

void cutImages(QVector<QImage*> *srcImages, QVector<cv::Mat*> *destImages, QVector<QVector<QPoint> > *reserveArea){
    if(srcImages->length() != reserveArea->length()){
        qDebug() << "number of images isn't match with reserve points";
    }else{
        for(int i = 0 ; i < srcImages->length() ; i++){
            cv::Mat *resultImage = new cv::Mat;
            cv::Mat *returned;
            returned = QImage2Mat(srcImages->at(i));
            returned->copyTo(*resultImage);
            int x_min = reserveArea->at(i).at(0).x();
            int y_min = reserveArea->at(i).at(0).y();
            int x_max = reserveArea->at(i).at(3).x();
            int y_max = reserveArea->at(i).at(3).y();

            *resultImage = (*resultImage)(cv::Range(x_min,x_max),cv::Range(y_min,y_max));

//            QString fileName;
//            fileName.sprintf("cutted%d.jpg",i);
            cv::cvtColor(*resultImage,*resultImage,CV_BGR2RGB);
//            cv::imwrite(fileName.toStdString(),*resultImage);

            destImages->append(resultImage);
            delete returned;
        }
    }
}

void save(QVector<cv::Mat*> *toSave){
    for(int i = 0 ; i < toSave->length() ; i++){
        QString fileName;
        fileName.sprintf("cutted%d.jpg",i);
        cv::imwrite(fileName.toStdString(),*toSave->at(i));
    }
}
