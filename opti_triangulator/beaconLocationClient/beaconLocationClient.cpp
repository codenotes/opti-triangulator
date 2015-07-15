// beaconLocationClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "triangulator/beaconSettings.h"

#pragma comment(lib, "ROSIndigoDLL.lib")

using namespace triangulator;


void chatterCallback(const triangulator::beaconSettings & msg)
{
	
	
//	ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int _tmain(int argc, _TCHAR* argv[])
{

	ros::init(argc, argv, "beaconTracker");

	/**
	* NodeHandle is the main access point to communications with the ROS system.
	* The first NodeHandle constructed will fully initialize this node, and the last
	* NodeHandle destructed will close down the node.
	*/
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("beaconPublish", 1000, chatterCallback);

	ros::spin();

	return 0;
}

