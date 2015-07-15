// beaconLocationClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "triangulator/beaconSettings.h"
#include "triangulator/triangulatorCommand.h"

#pragma comment(lib, "ROSIndigoDLL.lib")

using namespace triangulator;
triangulator::beaconSettings gBeaconSettings;


void beaconCallback(const triangulator::beaconSettings & msg)
{
	

	gBeaconSettings = msg;



//	ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int _tmain(int argc, _TCHAR* argv[])
{


	ros::init(argc, argv, "beaconTracker");


	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("beaconPublish", 1000, beaconCallback);

	ros::ServiceClient client = n.serviceClient<triangulator::triangulatorCommand>("triangulatorCmd");//this is important as it is the name of the service we are looking for
	triangulator::triangulatorCommand srv;

	

	ros::spin();

	return 0;
}

