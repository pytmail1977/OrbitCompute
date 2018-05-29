#include "dbaccess.h"

#include "computepara.h"

#include "mysql.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include "func.h"

using namespace std;
extern  ComputePara curPara;

DBAcess::DBAcess()
{
	if (!sqlConnection())
	    printf("%s\n","Error: can not connect to sql.");
}
bool DBAcess:: sqlConnection()
{
    FILE *fp = fopen("dbinfo.txt","r");
    if(!fp)
    {
    	printf("%s\n","No DB information");
    	return false;
    }
	fseek(fp,0,0);
	char DB_NAME[100];

	char USER_NAME[100];
	char USER_PASSWORD[100];
	char ipAddress[100];
	int port;
	fscanf(fp,"%s",DB_NAME);
	fscanf(fp,"%s",USER_NAME);
	fscanf(fp,"%s",USER_PASSWORD);
	fscanf(fp,"%s",ipAddress);
    fscanf(fp,"%d",&port);
	fclose(fp);

	 if (NULL==mysql_init(&mysql))
	    {    //分配和初始化MYSQL对象
		 printf("mysql_init(): %s\n", mysql_error(&mysql));
	      return false;
	    }

	    //尝试与运行在主机上的MySQL数据库引擎建立连接
	    if (NULL == mysql_real_connect(&mysql,ipAddress,USER_NAME,USER_PASSWORD,DB_NAME, port, NULL,0))
	    {
	           printf("mysql_real_connect(): %s\n", mysql_error(&mysql));
	           return false;
	    }
	    printf("%s\n","success to connect the database");
	    return true;
}

void DBAcess::getParaFromDB(int satID,ComputePara&  nowPara)
{
	char sql_gl[500];
	sprintf(sql_gl,"select * from qlzx_wx_gl where wx_bs = %d",satID);
	bool success = mysql_real_query(&mysql,sql_gl,(unsigned int)strlen(sql_gl));
	    MYSQL_RES *res=NULL;
	    MYSQL_ROW row=NULL;
 	    if(success)
	    {
	        printf("%s\n","No Datas in DataBase");
	        return;
	    }

	    res=mysql_store_result(&mysql);


	    while((row = mysql_fetch_row(res)))
	    {
	        nowPara.userID = atoi(row[1]);
	        double bcz = atof(row[4]);
	        nowPara.tno = 24*3600/(2*3.1415926*sqrt(bcz*bcz*bcz/398600.4405));
	        nowPara.targpo = atof(row[8]);
	        nowPara.tecco = atof(row[5]);
	        nowPara.tinclo = atof(row[6]);
	        nowPara.tmo = atof(row[9]);
	        nowPara.tnodeo = atof(row[7]);

	        const char* tmp = 0;//
	        tmp = row[10];
	        std::string tmpstr = tmp;
	        std::string yearstr = tmpstr.substr(0,4);
	        std::string monthstr = tmpstr.substr(5,2);
	        std::string daystr = tmpstr.substr(8,2);
	        std::string hourstr = tmpstr.substr(11,2);
	        std::string minutestr = tmpstr.substr(14,2);
	        std::string secondstr = tmpstr.substr(17,2);
	        int year,month,day,hour,minute,second;
	        year = atoi(yearstr.c_str());
	        month = atoi(monthstr.c_str());
	        day = atoi(daystr.c_str());
	        hour = atoi(hourstr.c_str());
	        minute = atoi(minutestr.c_str());
	        second = atoi(secondstr.c_str());
            nowPara.etime = DateTimetoJD(year,month,day,hour,minute,second);
	        //nowPara.etime = 2458118;


	        tmp = row[11];
	        const char deliminator[] = ", ";
	        char parse[100];
	        double q0,q1,q2,q3;
	        strcpy(parse,tmp);
	        char * seg = 0;
	        seg = strtok(parse, deliminator);
	       if(0!=seg)
	        {
	        	seg = strtok(NULL, deliminator);
	        	if(0!=seg)
	        	{
	        		q0 = atof(seg);
	        		seg = strtok(NULL, deliminator);
	        		if(0!=seg)
	        		{
	        			q1 =atof(seg);
	        			seg = strtok(NULL, deliminator);
	        			if(0!=seg)
	        			{
	        				q2 = atof(seg);
	        				seg = strtok(NULL, deliminator);
	        				if(0!=seg)
	        				{
	        					q3 = atof(seg);
	        				}
	        			}
	        		}
	        	}
	        }



	        nowPara.initQua.set(q0,q1,q2,q3) ;
	     //   nowPara.initQua.set(0,0,0,1);
	        nowPara.PointAction = atoi(row[12]);
	        nowPara.xAngle = atof(row[13]);
	        nowPara.yAngle = atof(row[14]);
	        nowPara.zAngle = atof(row[15]);
	        nowPara.rotateArray = atoi(row[16]);


	         tmp = row[17];
	         tmpstr = tmp;
	         yearstr = tmpstr.substr(0,4);
	         monthstr = tmpstr.substr(5,2);
	         daystr = tmpstr.substr(8,2);
	         hourstr = tmpstr.substr(11,2);
	         minutestr = tmpstr.substr(14,2);
	         secondstr = tmpstr.substr(17,2);
	         int initYear,initMonth,initDay,initHour,initMinute,initSecond;
	         initYear = atoi(yearstr.c_str());
	         initMonth = atoi(monthstr.c_str());
	         initDay = atoi(daystr.c_str());
	         initHour = atoi(hourstr.c_str());
	         initMinute = atoi(minutestr.c_str());
	         initSecond = atoi(secondstr.c_str());
	         nowPara.cinitTime = DateTimetoJD(initYear,initMonth,initDay,initHour,initMinute,initSecond);

	        //nowPara.cinitTime = 2458204.006944445;
	    }

	    mysql_free_result(res);

}
/*void DBAcess::WriteTotheDB(double nowJD, Vec3d position,Vec3d velocity,Vec3d latLongSubPointPosition,Vec4d qua,Vec3d EulerAngle)
{
	char gdStr[500];
	sprintf(gdStr,"{'J2000位置':'%f %f %f','速度':'%f %f %f','星下点经纬度':'%f %f'}",position.v[0],position.v[1],position.v[2],velocity.v[0],velocity.v[1],velocity.v[2],latLongSubPointPosition.v[0],latLongSubPointPosition.v[1]);
   char ztStr[500];
   sprintf(ztStr,"{'四元数q0':'%f','四元数q1':'%f','四元数q2':'%f','四元数q3':'%f','方位角':'%f','俯仰角':'%f','横滚角':'%f'}",qua.v[0],qua.v[1],qua.v[2],qua.v[3],EulerAngle.v[0],EulerAngle.v[1],EulerAngle.v[2]);

	    int year,month,day,hour,minute,second,msecond;
	    getDateFromJulianDay(nowJD,&year,&month,&day);
	    getTimeFromJulianDay(nowJD,&hour,&minute,&second,&msecond);
	    char timeStr[500];
	    sprintf(timeStr,"%d-%d-%d %d:%d:%d",year,month,day,hour,minute,second);

	    char tiaojianStr[500];
	    sprintf(tiaojianStr," where yh_bs = %d and wx_bs = %d;",curPara.userID,satID);

	    std::string str = "update qlzx_wx_zt set gd = ";
	    str = str + "\"";
	    str = str + gdStr;
	    str = str + "\",zt = ";
	    str = str + " \" ";
	    str = str + ztStr;
	    str = str + "\", xssj = ";
	    str = str + "\'";
	    str = str + timeStr;
	    str = str + "\'";
	    str = str + tiaojianStr;

	    int flag = mysql_real_query(&mysql,str.c_str(),(unsigned int)(str.length()));

	    if(flag)
	    {
	        printf("%s","Update the database failed!");
	        return;
	    }

}*/
