
#include "stdafx.h"
#include "triangulator/beaconSettings.h"
#include "triangulator/triangulatorCommand.h"


#define MAX_BEACONS
#define BOUNDARY_BUFFER 1


#include <iostream>


using namespace std;

using namespace triangulator;



struct findThird
{
	bool biasPositive = false; //decides whether to get positive or negative solution...

	triangulator::beaconSettings::ConstPtr _pb;

	findThird(triangulator::beaconSettings::ConstPtr &known) :_pb(known)
	{}

	void setKnown(triangulator::beaconSettings::ConstPtr &known)
	{
		_pb = known;

	}

	triangulator::beaconSetting getBeacon(int ID)
	{

		for (auto b : _pb->beaconValues)
		{
			
			if (boost::lexical_cast<int>(b.beaconID) == ID)
			{
				printf("looking at:%f or %s\t %f %f\n", b.beaconID, b.beaconName.c_str(), b.X, b.Y);
				return b;
			}

		}

		throw "Beacon non existant...";
		

	}

	void calcThird(triangulator::beaconSetting & findB, 
		int ref1,float dist1, int ref2, float dist2)
	{
		
		calcThird(findB, getBeacon(ref1),dist1, getBeacon(ref2),dist2);

	}


	//float getLocThird(double *x, double *y, double *xprime, double *yprime)
	//{
	//	calcThird(ref1X, ref1Y, ref1R, ref2X, ref2Y, ref2R, x, y,  xprime,  yprime);
	//}


	void calcThird(triangulator::beaconSetting & findB, const triangulator::beaconSetting &refB1, 
		float dist1, const triangulator::beaconSetting &refB2, float dist2)
	{
		double x, y, xprime, yprime;

		calcThird(refB1.X, refB1.Y, dist1, refB2.X, refB2.Y, dist2,
			&x, &y, &xprime, &yprime);

		printf("result:%s      %3.2f %3.2f %3.2f %3.2f\n",("B"+ boost::lexical_cast<string>(findB.beaconID)).c_str(), x, y, xprime, yprime);

		findB.beaconName = "B"+boost::lexical_cast<string>(findB.beaconID);


		if (biasPositive)
		{
			findB.X = x;
			findB.Y = y;
		}
		else
		{
			findB.X = xprime;
			findB.Y = yprime;
		}

		//or could be prime here, not sure yet.

	}

	int calcThird(double x0, double y0, double r0,
		double x1, double y1, double r1,
		double *xi, double *yi,
		double *xi_prime, double *yi_prime)
	{
		double a, dx, dy, d, h, rx, ry;
		double x2, y2;

		/* dx and dy are the vertical and horizontal distances between
		* the circle centers.
		*/
		dx = x1 - x0;
		dy = y1 - y0;

		/* Determine the straight-line distance between the centers. */
		//d = sqrt((dy*dy) + (dx*dx));
		d = hypot(dx, dy); // Suggested by Keith Briggs

		/* Check for solvability. */
		if (d > (r0 + r1))
		{
			/* no solution. circles do not intersect. */
			return 0;
		}
		if (d < fabs(r0 - r1))
		{
			/* no solution. one circle is contained in the other */
			return 0;
		}

		/* 'point 2' is the point where the line through the circle
		* intersection points crosses the line between the circle
		* centers.
		*/

		/* Determine the distance from point 0 to point 2. */
		a = ((r0*r0) - (r1*r1) + (d*d)) / (2.0 * d);

		/* Determine the coordinates of point 2. */
		x2 = x0 + (dx * a / d);
		y2 = y0 + (dy * a / d);

		/* Determine the distance from point 2 to either of the
		* intersection points.
		*/
		h = sqrt((r0*r0) - (a*a));

		/* Now determine the offsets of the intersection points from
		* point 2.
		*/
		rx = -dy * (h / d);
		ry = dx * (h / d);

		/* Determine the absolute intersection points. */
		*xi = x2 + rx;
		*xi_prime = x2 - rx;
		*yi = y2 + ry;
		*yi_prime = y2 - ry;

		return 1;
	}



};

#ifdef READANDDUMP
int _tmain(int argc, _TCHAR* argv[])
{
	ros::init(argc, argv, "beaconTracker");


	ros::NodeHandle n;
	
	ros::console::set_logger_level("roscpp_internal", ros::console::Level::Info);
	ros::console::set_logger_level("BaseNonNamedLogger", ros::console::Level::Info);

	ros::console::notifyLoggerLevelsChanged();


	triangulator::beaconSettingsConstPtr p =
		ros::topic::waitForMessage< triangulator::beaconSettings>("beaconPublish");
	
	
	triangulator::beaconSetting b;
	b.beaconID = 0.0f;
	b.X = 0.0f;
	b.Y = 0.0f;
	b.Z = 0.0f;

	

	//custom third beacon calculation
	findThird cf(p);

	

	//end limits for mocap area
	//X is 7.9 meters
	//Y is 9.34 m
	//Z is 3.04m
	//therefore
	/*

	f-4	f4
	f-5 f5
	f0	f3

	*/
	/// data
	//	f - 1.91 f1.91
	//	f - 2.56 f2.56
	//	f0  f3
	//	/ end_data

	std::string envfilename, beaconfilename;

	envfilename = argv[1];
	envfilename +=argv[2];
	envfilename += "EnvLimits.txt";

	beaconfilename = argv[1];
	beaconfilename += argv[2];
	beaconfilename += "Beacons.txt";


	FILE * fe = fopen(envfilename.c_str(),"w");
	FILE * fb = fopen(beaconfilename.c_str(),"w");

	cout <<"env file: "<< envfilename << endl;
	cout << "beacon file: " << beaconfilename << endl;


	
	
	

	
	//unknown beacons here
	//this will perform calculations
	triangulator::beaconSetting b7;
	b7.beaconID = 7.0f;
	b7.Z = 1.12f;
	triangulator::beaconSetting b8;
	b8.beaconID = 8.0f;
	b8.Z = 1.88f;
	vector<beaconSetting> unknowns;
	
	//YOUR SETTINGS HERE!!!
	/**these are real measured settings done with a meter*/
	cf.calcThird(b8, 5, 8.85, 6, 5.53);
	cf.calcThird(b7, 5, 10.9, 6, 6.78);

	unknowns.push_back(b7);
	unknowns.push_back(b8);

	//end unknown calculation


	int stuff = MAX_BEACONS - p->beaconValues.size() + unknowns.size();


	for (int i = 0; i < stuff; i++)
	{
		printf("f0\tf0\tf0\tf0\n");
		fprintf(fb,"f0\tf0\tf0\tf0\n");
	}


	float biggestX=0, biggestY=0;

 
	printf("received from mocap:\n");
	//now beaconfile
	fprintf(fb, "/data\n");
	printf("/data\n");
	for (triangulator::beaconSetting b : p->beaconValues)
	{

		printf("f%c\t\tf%.2f\t\tf%.2f\t\tf%.2f\n", b.beaconName.c_str()[1], b.X, b.Y, b.Z);
		fprintf(fb,"f%c\t\tf%.2f\t\tf%.2f\t\tf%.2f\n", b.beaconName.c_str()[1], b.X, b.Y, b.Z);

		if (b.X > biggestX) biggestX = b.X;
		if (b.Y > biggestY) biggestY = b.Y;
	}

	//printf("calculated unknowns:\n");
	for (triangulator::beaconSetting b : unknowns)
	{

		printf("f%c\t\tf%.2f\t\tf%.2f\t\tf%.2f\n", b.beaconName.c_str()[1], b.X, b.Y, b.Z);
		fprintf(fb, "f%c\t\tf%.2f\t\tf%.2f\t\tf%.2f\n", b.beaconName.c_str()[1], b.X, b.Y, b.Z);

		if (b.X > biggestX) biggestX = b.X;
		if (b.Y > biggestY) biggestY = b.Y;
	}


	printf("/end_data\n");
	fprintf(fb,"/end_data\n");


	//environmental limits stuff
	
	/*
	/ data
		f - 4     f4
		f - 5     f5
		f0      f3
		/ end_data
	*/
	//old hardcoded env
	/*printf("/data\nf-4\tf4\nf-5\tf5\nf0\tf3\n/end_data");
	printf("\n");
	printf("/end_data\n");
	fprintf(fe, "/data\nf-4\tf4\nf-5\tf5\nf0\tf3\n/end_data");
	printf("\n");
*/
	printf("environment file (calculated):\n");
	printf("/data\nf%f\tf%f\nf%f\tf%f\nf%f\tf%f", biggestX*-1 + BOUNDARY_BUFFER, biggestX - BOUNDARY_BUFFER, biggestY*-1 + BOUNDARY_BUFFER, biggestY - BOUNDARY_BUFFER, 0.0f, 3.0f);
	printf("\n");
	printf("/end_data\n");
	fprintf(fe,"/data\nf%f\tf%f\nf%f\tf%f\nf%f\tf%f", biggestX*-1 + BOUNDARY_BUFFER, biggestX - BOUNDARY_BUFFER, biggestY*-1 + BOUNDARY_BUFFER, biggestY - BOUNDARY_BUFFER, 0.0f, 3.0f);
	fprintf(fe, "\n/end_data");
	printf("\n");






	fclose(fe);
	fclose(fb);


}
#endif