#include "computepara.h"

ComputePara::ComputePara()
{

}
void ComputePara::Compare(ComputePara newPara,bool& changed1,bool& changed2,bool& changed3)
{
    double etime;
    double  tecco;//eccencity
    double  targpo;//
    double  tinclo;//
    double  tmo;//
    double tno;//
    double tnodeo;//

    if((newPara.etime!=this->etime)||(newPara.tecco !=this->tecco)||(newPara.targpo!=this->targpo)||(newPara.tinclo!=this->tinclo)||(newPara.tmo!=this->tmo)||(newPara.tno!=this->tno)||(newPara.tnodeo!=this->tnodeo))
    {
        changed1 = true;//orbit parameter changed
    }
    if((newPara.PointAction != this->PointAction)||(newPara.rotateArray != this->rotateArray)||(newPara.xAngle!= this->xAngle)||(newPara.yAngle != this->yAngle)||(newPara.zAngle != this->zAngle))
    {
        changed2 = true;//pose parameter changed
    }
    if(newPara.cinitTime != this->cinitTime)
    {
        changed3 = true;
    }

}
void ComputePara::operator =(const ComputePara& nowPara)
{
    etime = nowPara.etime;
    tecco = nowPara.tecco;
    targpo = nowPara.targpo;
    tinclo = nowPara.tinclo;
    tmo = nowPara.tmo;
    tno = nowPara.tno;
    tnodeo = nowPara.tnodeo;

    PointAction = nowPara.PointAction;
    rotateArray = nowPara.rotateArray;
    xAngle = nowPara.xAngle;
    yAngle = nowPara.yAngle;
    zAngle = nowPara.zAngle;

    cinitTime = nowPara.cinitTime;

    for(int i = 0;i<4;i++)
    {
        initQua.v[i] = nowPara.initQua.v[i];
    }



}
