#include <projectionThreads.h>
#include <QImage>
#include <QRgb>
#include <QtMath>
#include <opencv2/opencv.hpp>

ProjectionCalculation::ProjectionCalculation(int thisNumber, cv::Mat *thisImage){
    this->number = thisNumber;
    this->thisImage = new cv::Mat;
    thisImage->copyTo(*this->thisImage);
}

ProjectionCalculation::~ProjectionCalculation(){
    delete this->thisImage;
}

void ProjectionCalculation::run(){
    qDebug() << "projection thread in:" << this->currentThread() << " start";

    DirectXProjectionCalculation calcuX(this->number,this->thisImage);
    DirectYProjectionCalculation calcuY(this->number,this->thisImage);
    DirectUProjectionCalculation calcuU(this->number,this->thisImage);
    DirectVProjectionCalculation calcuV(this->number,this->thisImage);

    calcuX.start();
    calcuY.start();
    calcuU.start();
    calcuV.start();

    calcuX.wait();
    calcuY.wait();
    calcuU.wait();
    calcuV.wait();

    this->projectionResult.push_back(calcuX.result);
    this->projectionResult.push_back(calcuY.result);
    this->projectionResult.push_back(calcuU.result);
    this->projectionResult.push_back(calcuV.result);

    qDebug() << "projection thread in:" << this->currentThread() << " stop";
}

DirectXProjectionCalculation::DirectXProjectionCalculation(int number, cv::Mat *thisImage){
    this->number = number;
    this->thisImage = new cv::Mat;
    thisImage->copyTo(*this->thisImage);
}

DirectYProjectionCalculation::DirectYProjectionCalculation(int number, cv::Mat *thisImage){
    this->number = number;
    this->thisImage = new cv::Mat;
    thisImage->copyTo(*this->thisImage);
}

DirectUProjectionCalculation::DirectUProjectionCalculation(int number, cv::Mat *thisImage){
    this->number = number;
    this->thisImage = new cv::Mat;
    thisImage->copyTo(*this->thisImage);
}

DirectVProjectionCalculation::DirectVProjectionCalculation(int number, cv::Mat *thisImage){
    this->number = number;
    this->thisImage = new cv::Mat;
    thisImage->copyTo(*this->thisImage);
}

DirectXProjectionCalculation::~DirectXProjectionCalculation(){
    delete this->thisImage;
}

DirectYProjectionCalculation::~DirectYProjectionCalculation(){
    delete this->thisImage;
}

DirectUProjectionCalculation::~DirectUProjectionCalculation(){
    delete this->thisImage;
}

DirectVProjectionCalculation::~DirectVProjectionCalculation(){
    delete this->thisImage;
}


void DirectYProjectionCalculation::run(){
    qDebug() << "projectionY thread in:" << this->currentThread() << " start";

    if(this->thisImage->empty()){
        qDebug() << "NULL image in Direct Y projection";
    }else{
        cv::Mat1d *targetImage = new cv::Mat1d;
        cv::Mat *temp = new cv::Mat;
        this->thisImage->copyTo(*temp);
        cv::normalize(*temp,*targetImage,0.0,1.0,cv::NORM_MINMAX);

        qDebug() << "Y image location :" << targetImage;

        for(int i = 1 ; i < targetImage->rows ; ++i){
            double sum = 0.0;
            cv::Mat1d rows1 = targetImage->row(i);
            cv::Mat1d rows2 = targetImage->row(i - 1);
            double first,second;
            int count = 0;

            for(int y = 0 ; y < targetImage->cols ; ++y){
                first = rows1.at<double>(y);
                second = rows2.at<double>(y);

                sum += (first - second) * (first - second);
                ++count;
            }

            this->result.push_back(cvPoint2D32f(sum, (double)count));
        }

        delete targetImage;
        delete temp;
    }

    qDebug() << "projectionY thread in:" << this->currentThread() << " stop";
}

void DirectXProjectionCalculation::run(){
    qDebug() << "projectionX thread in:" << this->currentThread() << " start";

    if(this->thisImage->empty()){
        qDebug() << "NULL image in Direct X projection";
    }else{
        cv::Mat1d *targetImage = new cv::Mat1d;
        cv::Mat *temp = new cv::Mat;
        this->thisImage->copyTo(*temp);
        cv::normalize(*temp,*targetImage,0.0,1.0,cv::NORM_MINMAX);

        qDebug() << "X image location :" << targetImage;

        //        qDebug() << "X cols = " << targetImage.cols;

        for(int i = 1 ; i < targetImage->cols ; ++i){
            //            qDebug() << "X: i = " << i;

            double sum = 0.0;
            cv::Mat1d cols1 = targetImage->col(i);
            cv::Mat1d cols2 = targetImage->col(i - 1);
            double first,second;
            int count = 0;

            for(int x = 0 ; x < targetImage->rows ; ++x){
                first = cols1.at<double>(x);
                second = cols2.at<double>(x);

                sum += (first - second) * (first - second);
                ++count;
            }

            this->result.push_back(cvPoint2D32f(sum, count));
        }

        delete targetImage;
        delete temp;
    }

    qDebug() << "projectionX thread in:" << this->currentThread() << " stop";
}

void DirectUProjectionCalculation::run(){
    qDebug() << "projectionU thread in:" << this->currentThread() << " start";

    if(this->thisImage->empty()){
        qDebug() << "NULL image in Direct U projection";
    }else{
        cv::Mat1d *targetImage = new cv::Mat1d;
        cv::Mat *temp = new cv::Mat;
        this->thisImage->copyTo(*temp);
        cv::normalize(*temp,*targetImage,0.0,1.0,cv::NORM_MINMAX);

        qDebug() << "U image location :" << targetImage;

        int max_i = (targetImage->cols + targetImage->rows) / 2;
        for(int i = 1 ; i < max_i ; ++i){
            double sum = 0.0;
            int count = 0;
            double first,second;
            for(int x = 1 ; x < targetImage->rows ; ++x){
                int y = i * 2 -x;
                if(y >= 1 && y < targetImage->cols){
                    first = targetImage->at<double>(x,y);
                    second = targetImage->at<double>(x - 1,y - 1);

                    sum += (first - second) * (first - second);
                    count ++;
                }
            }

            this->result.push_back(cvPoint2D32f(sum, (double)count));
        }

        delete targetImage;
        delete temp;
    }

    qDebug() << "projectionU thread in:" << this->currentThread() << " stop";
}

void DirectVProjectionCalculation::run(){
    qDebug() << "projectionV thread in:" << this->currentThread() << " start";

    if(this->thisImage->empty()){
        qDebug() << "NULL image in Direct V projection";
    }else{
        cv::Mat1d *targetImage = new cv::Mat1d;
        cv::Mat *temp = new cv::Mat;
        this->thisImage->copyTo(*temp);
        cv::normalize(*temp,*targetImage,0.0,1.0,cv::NORM_MINMAX);
        qDebug() << "V image location :" << targetImage;

        int max_i = targetImage->rows - 1;
        int min_i = (targetImage->rows + 1 - targetImage->cols) / 2;
        for(int i = min_i ; i < max_i ; ++i){
            double sum = 0.0;
            int count = 0;
            double first,second;
            for(int x = 0 ; x < targetImage->rows - 1 ; ++x){
                int y = x + targetImage->rows - 2 * i;

                if(y >= 1 && y < targetImage->cols){
                    first = targetImage->at<double>(x,y);
                    second = targetImage->at<double>(x + 1, y - 1);

                    sum += (first - second) * (first - second);
                    count ++;
                }
            }

            this->result.push_back(cvPoint2D32f(sum, (double)count));
        }

        delete targetImage;
        delete temp;
    }

    qDebug() << "projectionV thread in:" << this->currentThread() << " stop";
}
