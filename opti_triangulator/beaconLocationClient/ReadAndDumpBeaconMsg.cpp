
#include "stdafx.h"
#include "triangulator/beaconSettings.h"
#include "triangulator/triangulatorCommand.h"

#include <iostream>


using namespace std;

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
	b.X = 0.0f;

	int stuff = 8 - p->beaconValues.size(); 


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
	cout <<"beacon file: "<< envfilename << endl;


	printf("/data\nf-4\tf4\nf-5\tf5\nf0\tf3\n/end_data");
	printf("\n");
	printf("/data\n");

	fprintf(fe,"/data\nf-4\tf4\nf-5\tf5\nf0\tf3\n/end_data");
	
	printf("\n");
	
	//now beaconfile
	fprintf(fb,"/data\n");

	for (int i = 0; i < stuff; i++)
	{
		printf("f0\tf0\tf0\tf0\n");
		fprintf(fb,"f0\tf0\tf0\tf0\n");
	}

	for (triangulator::beaconSetting b : p->beaconValues)
	{
		printf("f%c\t\tf%.2f\t\tf%.2f\t\tf%.2f\n", b.beaconName.c_str()[1], b.X, b.Y, b.Z);
		fprintf(fb,"f%c\t\tf%.2f\t\tf%.2f\t\tf%.2f\n", b.beaconName.c_str()[1], b.X, b.Y, b.Z);
	}
	printf("/end_data\n");
	fprintf(fb,"/end_data\n");


	fclose(fe);
	fclose(fb);


}
#endif