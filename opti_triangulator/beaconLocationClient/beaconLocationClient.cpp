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

	std::vector<float> envLimits;

	ros::init(argc, argv, "beaconTracker");


	ros::NodeHandle n;
	//ros::Subscriber sub = n.subscribe("beaconPublish", 1000, beaconCallback);
	//std::string topic = "beaconPublish";

	n.getParam("envLimits", envLimits);

	printf("Env Limits: XMinus:%.4f XPlus:%.4f YMinus:%.4f YPlus:%.4f ZMinus:%.4f ZPlus:%.4f \n",
		envLimits[0],
		envLimits[1],
		envLimits[2],
		envLimits[3],
		envLimits[4],
		envLimits[5]
		);

	
	triangulator::beaconSettingsConstPtr p=		
		ros::topic::waitForMessage< triangulator::beaconSettings>("beaconPublish");
	////
	//

	//
	for (triangulator::beaconSetting b : p->beaconValues)
	{

		printf("%s\t\t%.5f\t\t%.5f\t\t%.5f\n", b.beaconName.c_str(), b.X, b.Y, b.Z);


	}

	
	ros::ServiceClient client = n.serviceClient<triangulator::triangulatorCommand>("triangulatorCmd");//this is important as it is the name of the service we are looking for
	triangulator::triangulatorCommand srv;
	srv.request.triCommand.bSettings.beaconValues = p->beaconValues;
	srv.request.triCommand.commandID = TriangulatorDSCommands::SET_BEACON_LIST;
	
	
	for (int i = 0; i < envLimits.size(); i++)
		srv.request.triCommand.envLimits[i] = envLimits[i];

	

	if (client.call(srv))
	{
	
	}
	else
	{

	}

	ros::spinOnce();

	return 0;
}

