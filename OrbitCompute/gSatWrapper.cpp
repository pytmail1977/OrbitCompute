/***************************************************************************
 * Name: gSatWrapper.hpp
 *
 * Description: Wrapper over gSatTEME class.
 *              This class allow use Satellite orbit calculation module (gSAt) in
 *              Stellarium 'native' mode using Stellarium objects.
 *
 ***************************************************************************/
/***************************************************************************
 *   Copyright (C) 2006 by J.L. Canales                                    *
 *   jlcanales.gasco@gmail.com                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Suite 500, Boston, MA  02110-1335, USA.             *
 ***************************************************************************/



#include "stdsat.h"
#include "mathUtils.h"
#include "VecMat.h"
#include "gSatWrapper.h"
#include "gVector.h"

//#include "StelUtils.hpp"



gSatWrapper::gSatWrapper(string designation, string tle1,string tle2)
{
	// The TLE library actually modifies the TLE strings, which is annoying (because
	// when we get updates, we want to check if there has been a change by using ==
	// with the original.  Thus we make a copy to send to the TLE library.
//	QByteArray t1(tle1.toLatin1().data()), t2(tle2.toLatin1().data());

	// Also, the TLE library expects no more than 130 characters length input.  We
	// shouldn't have sane input with a TLE longer than about 80, but just in case
	// we have a mal-formed input, we will truncate here to be safe
//	t1.truncate(130);
//	t2.truncate(130);

//	pSatellite = new gSatTEME(designation.c_str(),tle1.c_str(),tle2.c_str());
	pSatellite = 0;
//	setEpoch(StelApp::getInstance().getCore()->getJD());
}
gSatWrapper::gSatWrapper(string designation, double epochJD,double ecco,double argpo,double inclo,double mo,double no,double nodeo)
{


    pSatellite = new gSatTEME(designation.data(),
                              epochJD,ecco,argpo,inclo,mo,no,nodeo);

}

void gSatWrapper::resetPara( double epochJD,double ecco,double argpo,double inclo,double mo,double no,double nodeo)
{


    pSatellite->resetPara(epochJD,ecco,argpo,inclo,mo,no,nodeo);

}

gSatWrapper::~gSatWrapper()
{
	if (pSatellite != 0)
		delete pSatellite;
}


Vec3d gSatWrapper::getTEMEPos() const
{
	Vec3d returnedVector;
	if (pSatellite != 0)
	{
		Vec3d position = pSatellite->getPos();
		returnedVector.set(position[0], position[1], position[2]);
	}
	else
		printf("gSatWrapper::getTEMEPos Method called without pSatellite initialized");

	return returnedVector;

}


Vec3d gSatWrapper::getTEMEVel() const
{
	Vec3d returnedVector;
	if (pSatellite != 0)
	{
		Vec3d velocity = pSatellite->getVel();
        returnedVector.set(velocity[0], velocity[1], velocity[2]);
	}
	else
		printf("gSatWrapper::getTEMEVel Method called without pSatellite initialized");

	return returnedVector;

}


Vec3d gSatWrapper::getSubPoint() const
{
	Vec3d returnedVector;
	if (pSatellite != 0)
	{
		Vec3d satelliteSubPoint = pSatellite->getSubPoint();
        returnedVector.set(satelliteSubPoint[0], satelliteSubPoint[1], satelliteSubPoint[2]);
	}
	else
		printf("gSatWrapper::getTEMEVel Method called without pSatellite initialized");

	return returnedVector;
}

void gSatWrapper::setEpoch(double ai_julianDaysEpoch)
{
    epoch = ai_julianDaysEpoch;
    if (pSatellite)
		pSatellite->setEpoch(epoch);
}


/*void gSatWrapper::calcObserverECIPosition(Vec3d& ao_position, Vec3d& ao_velocity)
{

	if (epoch != lastCalcObserverECIPosition)
	{
		StelLocation loc   = StelApp::getInstance().getCore()->getCurrentLocation();

		double radLatitude = loc.latitude * KDEG2RAD;
		double theta       = epoch.toThetaLMST(loc.longitude * KDEG2RAD);
		double r;
		double c,sq;

		/* Reference:  Explanatory supplement to the Astronomical Almanac 1992, page 209-210. */
		/* Elipsoid earth model*/
		/* c = Nlat/a */
/*		c = 1/std::sqrt(1 + __f*(__f - 2)*Sqr(sin(radLatitude)));
		sq = Sqr(1 - __f)*c;

		r = (KEARTHRADIUS*c + (loc.altitude/1000))*cos(radLatitude);
		ao_position[0] = r * cos(theta);/*kilometers*/
/*		ao_position[1] = r * sin(theta);
		ao_position[2] = (KEARTHRADIUS*sq + (loc.altitude/1000))*sin(radLatitude);
		ao_velocity[0] = -KMFACTOR*ao_position[1];/*kilometers/second*/
/*		ao_velocity[1] =  KMFACTOR*ao_position[0];
/*		ao_velocity[2] =  0;

		lastCalcObserverECIPosition=epoch;
	}
}



Vec3d gSatWrapper::getAltAz() const
{
	StelLocation loc   = StelApp::getInstance().getCore()->getCurrentLocation();
	Vec3d topoSatPos;

	const double  radLatitude    = loc.latitude * KDEG2RAD;
	const double  theta          = epoch.toThetaLMST(loc.longitude * KDEG2RAD);
	const double sinRadLatitude=sin(radLatitude);
	const double cosRadLatitude=cos(radLatitude);
	const double sinTheta=sin(theta);
	const double cosTheta=cos(theta);

	// This now only updates if required.
	calcObserverECIPosition(observerECIPos, observerECIVel);

	Vec3d satECIPos  = getTEMEPos();
	Vec3d slantRange = satECIPos - observerECIPos;

	//top_s
	topoSatPos[0] = (sinRadLatitude * cosTheta*slantRange[0]
			 + sinRadLatitude* sinTheta*slantRange[1]
			 - cosRadLatitude* slantRange[2]);
	//top_e
	topoSatPos[1] = ((-1.0)* sinTheta*slantRange[0]
			 + cosTheta*slantRange[1]);

	//top_z
	topoSatPos[2] = (cosRadLatitude * cosTheta*slantRange[0]
			 + cosRadLatitude * sinTheta*slantRange[1]
			 + sinRadLatitude *slantRange[2]);

	return topoSatPos;
}

void  gSatWrapper::getSlantRange(double &ao_slantRange, double &ao_slantRangeRate) const
{
	//Vec3d observerECIPos;
	//Vec3d observerECIVel;
	calcObserverECIPosition(observerECIPos, observerECIVel);

        Vec3d satECIPos            = getTEMEPos();
        Vec3d satECIVel            = getTEMEVel();
        Vec3d slantRange           = satECIPos - observerECIPos;
        Vec3d slantRangeVelocity   = satECIVel - observerECIVel;

	ao_slantRange     = slantRange.length();
        ao_slantRangeRate = slantRange.dot(slantRangeVelocity)/ao_slantRange;
}*/

// Does the actual computation only if necessary (0.1s apart) and caches the result in a static variable.
/*void gSatWrapper::updateSunECIPos()
{
	// All positions in ECI system are positions referenced in a StelCore::EquinoxEq system centered in the earth centre
	//Vec3d observerECIPos;
	//Vec3d observerECIVel;
	calcObserverECIPosition(observerECIPos, observerECIVel);

	static const SolarSystem *solsystem = (SolarSystem*)StelApp::getInstance().getModuleMgr().getModule("SolarSystem");
	Vec3d sunEquinoxEqPos = solsystem->getSun()->getEquinoxEquatorialPos(StelApp::getInstance().getCore());

	//sunEquinoxEqPos is measured in AU. we need measure it in Km
	//Vec3d sunECIPos;
	sunECIPos.set(sunEquinoxEqPos[0]*AU, sunEquinoxEqPos[1]*AU, sunEquinoxEqPos[2]*AU);
	sunECIPos = sunECIPos + observerECIPos; //Change ref system centre



}

Vec3d gSatWrapper::getSunECIPos()
{
	if (epoch != lastSunECIepoch)
	{
		updateSunECIPos();
		lastSunECIepoch=epoch;
	}
	return sunECIPos;
}*/

// Operation getVisibilityPredict
// @brief This operation predicts the satellite visibility conditions.
/*gSatWrapper::Visibility gSatWrapper::getVisibilityPredict()
{
	Vec3d satAltAzPos = getAltAz();

	if (satAltAzPos[2] > 0)
	{
		Vec3d satECIPos = getTEMEPos();
		static const SolarSystem *solsystem = (SolarSystem*)StelApp::getInstance().getModuleMgr().getModule("SolarSystem");
		Vec3d sunAltAzPos = solsystem->getSun()->getAltAzPosGeometric(StelApp::getInstance().getCore());
		Vec3d sunECIPos = getSunECIPos();

		if (sunAltAzPos[2] > 0.0)
		{
			return RADAR_SUN;
		}
		else
		{
			double sunSatAngle = sunECIPos.angle(satECIPos);
			double Dist = satECIPos.length()*cos(sunSatAngle - (M_PI/2));

			if (Dist > KEARTHRADIUS)
			{
				return VISIBLE;
			}
			else
			{
				return RADAR_NIGHT;
			}
		}
	}
	else
		return NOT_VISIBLE;
}

double gSatWrapper::getPhaseAngle() const
{
	Vec3d sunECIPos = getSunECIPos();
	return sunECIPos.angle(getTEMEPos());
}
*/
gTime gSatWrapper::epoch;
//gTime gSatWrapper::lastSunECIepoch=0.0; // store last time of computation to avoid all-1 computations.
//gTime gSatWrapper::lastCalcObserverECIPosition;

//Vec3d gSatWrapper::sunECIPos; // enough to have this once.
//Vec3d gSatWrapper::observerECIPos;
//Vec3d gSatWrapper::observerECIVel;