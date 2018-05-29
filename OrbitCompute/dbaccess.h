#ifndef DBACCESS_H
#define DBACCESS_H

#include "computepara.h"
#include "mysql.h"
class DBAcess
{
public:
    DBAcess();
    bool sqlConnection();
    void getParaFromDB(int satID,ComputePara&  nowPara);
   // void WriteTotheDB(double nowJD, Vec3d position,Vec3d velocity,Vec3d latLongSubPointPosition,Vec4d qua,Vec3d EulerAngle);
private:
    MYSQL   mysql;
};


#endif // DBACCESS_H
