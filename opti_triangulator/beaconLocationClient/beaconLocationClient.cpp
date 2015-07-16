// beaconLocationClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "triangulator/beaconSettings.h"
#include "triangulator/triangulatorCommand.h"

#pragma comment(lib, "ROSIndigoDLL.lib")

using namespace triangulator;
triangulator::beaconSettings gBeaconSettings;
bool gGotReading = false;


void beaconCallback(const triangulator::beaconSettings & msg)
{
	

	gBeaconSettings = msg;
	gGotReading = true;



//	ROS_INFO("I heard: [%s]", msg->data.c_str());
}

enum TriangulatorDSCommands { GET_LOCALIZATION = 6, GET_VOLTAGE = 3, SET_ENV_LIMITS = 15, SET_BEACON_LIST = 16 };

int _tmain(int argc, _TCHAR* argv[])
{


	ros::init(argc, argv, "beaconTracker");


	ros::NodeHandle n;
	//ros::Subscriber sub = n.subscribe("beaconPublish", 1000, beaconCallback);
	std::string topic = "beaconPublish";
	triangulator::beaconSettingsConstPtr p=ros::topic::waitForMessage<triangulator::beaconSettings>(topic);
	//
	ros::ServiceClient client = n.serviceClient<triangulator::triangulatorCommand>("triangulatorCmd");//this is important as it is the name of the service we are looking for
	triangulator::triangulatorCommand srv;

	
	srv.request.triCommand.bSettings.beaconValues = p->beaconValues;
	srv.request.triCommand.commandID = TriangulatorDSCommands::SET_BEACON_LIST;

	if (client.call(srv))
	{
	
	}
	else
	{

	}

	ros::spin();

	return 0;
}

