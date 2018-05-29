/*
 * func.h
 *
 *  Created on: 2018骞�5���23���
 *      Author: zhangheng
 */

#ifndef FUNC_H_
#define FUNC_H_


//void ComputeTime(int satID,double mjLat,double mjLong,int& mjYear,int& mjMonth,int& mjDay,int& mjHour,int& mjMinute,int& mjSecond,int& mjmsecond,double& minLat,double& minLon);

void getDateFromJulianDay(const double jd, int *yy, int *mm, int *dd);
void getTimeFromJulianDay(const double julianDay, int *hour, int *minute, int *second, int *millis);
double DateTimetoJD(int year,int month,int day,int hour,int minute,int second);

extern "C" {
void ComputeTime(int satID,double mjLat,double mjLong,int& mjYear,int& mjMonth,int& mjDay,int& mjHour,int& mjMinute,int& mjSecond,int& mjmsecond,double& minLat,double& minLon);
void printC();
}


#endif /* FUNC_H_ */
