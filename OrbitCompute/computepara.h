#ifndef COMPUTEPARA_H
#define COMPUTEPARA_H

#include "VecMat.h"

class ComputePara
{
public:
    ComputePara();
    void Compare(ComputePara newPara,bool& changed1,bool& changed2,bool& changed3);
    void operator =(const ComputePara& nowPara);
    int userID;
    double etime;
    double  tecco;//eccencity
    double  targpo;//
    double  tinclo;//
    double  tmo;//
    double tno;//
    double tnodeo;//

    int PointAction;//0: point to the ground;1:guanxingdingxiang
    double xAngle;
    double yAngle;
    double zAngle;
    int rotateArray;//xuanzhuanshunxu

    Vec4d initQua;

    double cinitTime;


};

#endif // COMPUTEPARA_H
