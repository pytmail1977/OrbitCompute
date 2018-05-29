
#include "VecMat.h"

#include "dbaccess.h"
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>

#include "computepara.h"
#include "gSatWrapper.h"
#include "func.h"
//Global variant

ComputePara curPara;
int userID;
DBAcess access;

extern "C" {
void printC(){
	printf("here is output of c++\n\n\n");
}

}

void ComputeTime(int satID,double mjLat,double mjLong,int& mjYear,int& mjMonth,int& mjDay,int& mjHour,int& mjMinute,int& mjSecond,int& mjmsecond,double& minLat,double& minLon)
{
    //read the configure file
    access.getParaFromDB(satID,curPara);
    char tmpStr[10];
    sprintf(tmpStr,"%05d",satID);

    std::string satIDStr  = tmpStr;
    gSatWrapper * pSatWrapper = new gSatWrapper(satIDStr,curPara.etime,curPara.tecco,curPara.targpo,curPara.tinclo,curPara.tmo,curPara.tno,curPara.tnodeo);

    int timestep = 60;//timestep = 1 minute = 60seconds;
    double nowJD = curPara.cinitTime;
    double latDis = 0.5;
    double longDis = 0.5;


     int tyear,tmonth,tday,thour,tminute,tsecond;


     pSatWrapper->setEpoch(nowJD);
     Vec3d position                 = pSatWrapper->getTEMEPos();
     Vec3d velocity                 = pSatWrapper->getTEMEVel();
     Vec3d latLongSubPointPosition  = pSatWrapper->getSubPoint();
     double height            = latLongSubPointPosition[2]; // km
     double lat = latLongSubPointPosition[0];
     double lon = latLongSubPointPosition[1];
     int maxStep = 60*24*365;//one year
     int step = 0;
     while((fabs(lat - mjLat)>latDis)||(fabs(lon - mjLong)>longDis))
     {
         nowJD = nowJD + timestep*1.0/(24*60*60);
         pSatWrapper->setEpoch(nowJD);
         position                 = pSatWrapper->getTEMEPos();
         velocity                 = pSatWrapper->getTEMEVel();
         latLongSubPointPosition  = pSatWrapper->getSubPoint();
         height            = latLongSubPointPosition[2]; // km
         lat = latLongSubPointPosition[0];
         lon = latLongSubPointPosition[1];
         step++;
         if(step>maxStep)
         {
             break;
         }
         if((fabs(lat - mjLat)<latDis)&&(fabs(lon - mjLong)<longDis))
         {
             break;
         }
     }
      if(step<maxStep)
      {
             double newNowJD;
          double tt = 5*1.0/(60*24);
          double newTimeStep = 1.0/(60*60*24);//0.5second

          double minSum = 2;
          double minNewNowJD;

          for(newNowJD = nowJD -tt; newNowJD<=nowJD + tt; newNowJD = newNowJD + newTimeStep)
          {
              pSatWrapper->setEpoch(newNowJD);
              position                 = pSatWrapper->getTEMEPos();
              velocity                 = pSatWrapper->getTEMEVel();
              latLongSubPointPosition  = pSatWrapper->getSubPoint();
              height            = latLongSubPointPosition[2]; // km
              lat = latLongSubPointPosition[0];
              lon = latLongSubPointPosition[1];

              double latDis = fabs(lat - mjLat);
              double lonDis = fabs(lon - mjLong);
              double sumDis = latDis + lonDis;
              if(sumDis<minSum)
              {
                  minSum = sumDis;
                  minNewNowJD = newNowJD;
                  minLat = lat;
                  minLon = lon;
              }
          }
      getDateFromJulianDay(minNewNowJD,&mjYear,&mjMonth,&mjDay);
      getTimeFromJulianDay(minNewNowJD,&mjHour,&mjMinute,&mjSecond,&mjmsecond);

      }
     else
     {
       printf("No Possible time");
     }
}

void getDateFromJulianDay(const double jd, int *yy, int *mm, int *dd)
{
	/*
	 * This algorithm is taken from
	 * "Numerical Recipes in c, 2nd Ed." (1992), pp. 14-15
	 * and converted to integer math.
	 * The electronic version of the book is freely available
	 * at http://www.nr.com/ , under "Obsolete Versions - Older
	 * book and code versions.
	 */

	static const long JD_GREG_CAL = 2299161;
	static const int JB_MAX_WITHOUT_OVERFLOW = 107374182;
	long julian;

	julian = (long)floor(jd + 0.5);

	long ta, jalpha, tb, tc, td, te;

	if (julian >= JD_GREG_CAL)
	{
		jalpha = (4*(julian - 1867216) - 1) / 146097;
		ta = julian + 1 + jalpha - jalpha / 4;
	}
	else if (julian < 0)
	{
		ta = julian + 36525 * (1 - julian / 36525);
	}
	else
	{
		ta = julian;
	}

	tb = ta + 1524;
	if (tb <= JB_MAX_WITHOUT_OVERFLOW)
	{
		tc = (tb*20 - 2442) / 7305;
	}
	else
	{
		tc = (long)(((unsigned long long)tb*20 - 2442) / 7305);
	}
	td = 365 * tc + tc/4;
	te = ((tb - td) * 10000)/306001;

	*dd = tb - td - (306001 * te) / 10000;

	*mm = te - 1;
	if (*mm > 12)
	{
		*mm -= 12;
	}
	*yy = tc - 4715;
	if (*mm > 2)
	{
		--(*yy);
	}
	if (julian < 0)
	{
		*yy -= 100 * (1 - julian / 36525);
	}
}

void getTimeFromJulianDay(const double julianDay, int *hour, int *minute, int *second, int *millis)
{
	double frac = julianDay - (floor(julianDay));
	double secs = frac * 24.0 * 60.0 * 60.0 + 0.0001; // add constant to fix floating-point truncation error
	int s = (int)floor(secs);

	*hour = ((s / (60 * 60))+12)%24;
	*minute = (s/(60))%60;
	*second = s % 60;
	if(millis)
	{
		*millis = (int)floor((secs - floor(secs)) * 1000.0);
	}
}

double DateTimetoJD(int year,int month,int day,int hour,int minute,int second)
{

	double JD = 0;
	JD =367.0 * year - floor((7 * (year + floor((month + 9) / 12.0))) * 0.25) +floor( 275 * month / 9.0 ) + day + 1721013.5 +((second / 60.0 + minute) / 60.0 +hour) / 24.0;
	return JD;
}
