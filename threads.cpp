#include <threads.h>

ProjectionCalculation::ProjectionCalculation(int thisNumber, QImage *thisImage){
    this->number = thisNumber;
    this->thisImage = thisImage;
}

void ProjectionCalculation::run(){
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

    this->projectionResult.push_back(*calcuX.result);
    this->projectionResult.push_back(*calcuY.result);
    this->projectionResult.push_back(*calcuU.result);
    this->projectionResult.push_back(*calcuV.result);

    if(this->thisShiftCalcu != NULL){
        this->thisShiftCalcu->start();
    }
}

DirectXProjectionCalculation::DirectXProjectionCalculation(int number, QImage *thisImage){
    this->number = number;
    this->thisImage = thisImage;
}

DirectYProjectionCalculation::DirectYProjectionCalculation(int number, QImage *thisImage){
    this->number = number;
    this->thisImage = thisImage;
}

DirectUProjectionCalculation::DirectUProjectionCalculation(int number, QImage *thisImage){
    this->number = number;
    this->thisImage = thisImage;
}

DirectVProjectionCalculation::DirectVProjectionCalculation(int number, QImage *thisImage){
    this->number = number;
    this->thisImage = thisImage;
}

DirectXProjectionCalculation::~DirectXProjectionCalculation(){
    ;
}

DirectYProjectionCalculation::~DirectYProjectionCalculation(){
    ;
}

DirectUProjectionCalculation::~DirectUProjectionCalculation(){
    ;
}

DirectVProjectionCalculation::~DirectVProjectionCalculation(){
    ;
}


void DirectXProjectionCalculation::run(){
    ;
}

void DirectYProjectionCalculation::run(){
    ;
}

void DirectUProjectionCalculation::run(){
    ;
}

void DirectVProjectionCalculation::run(){
    ;
}
