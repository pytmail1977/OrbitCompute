
#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>


int main(int argc, char *argv[])
{

	int satID;

	FILE* file = fopen("conf_info.ini","r");
    if(!file)
    {
    	printf("%s\n","no conf_info.ini");
    }
	fseek(file,0,0);
	char ss[30];
	fscanf(file,"%s",ss);
	char deliminator[] = "= ";
	char* seg = 0;
	seg = strtok(ss, deliminator);
	if(0!=seg)
    {
		seg = strtok(NULL, deliminator);
		satID = atoi(seg);
    }

	fclose(file);

    double mjLat = 46 ;
    double mjLong = 142;
    int mjYear, mjMonth, mjDay,mjHour, mjMinute, mjSecond,mjmSecond;
    double minLat,minLon;
    ComputeTime(satID,mjLat,mjLong,mjYear,mjMonth, mjDay,mjHour,mjMinute,mjSecond,mjmSecond, minLat,minLon);
    printf("%04d-%02d-%02d %02d:%02d:%02d\n",mjYear,mjMonth,mjDay,mjHour,mjMinute,mjSecond);
    printf("lat:%2.3f long:%03.3f\n",minLat,minLon);
    return 0;
}
