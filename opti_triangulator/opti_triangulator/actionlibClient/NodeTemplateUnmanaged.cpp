// NodeTemplateUnmanaged.cpp : Defines the entry point for the console application.

//Junk app to test things.  Has all of ROS init and boost.  I don't know if it started out as that, but that is what it is now. 

//

#include <stdio.h>
#include <tchar.h>
#include <ros/ros.h>

#include <stdio.h>
#include <tchar.h>
#include <string>
#include "tinyxml2.h"
#include <iostream>
 
// TODO: reference additional headers your program requires here
#include "legacy/tf/transform_broadcaster.h"
//#include "yaml-cpp/yaml.h"
//#include "yaml-cpp/eventhandler.h"

//void getGlobalParam(tinyxml2::XMLDocument & doc);
//void getNodes(tinyxml2::XMLDocument & doc);
//void getNodeParam(char * nodename, tinyxml2::XMLDocument & doc);
//void getNodeROSParam(char * nodename, tinyxml2::XMLDocument & doc, const char * paramname = 0);


#include <ros/serialized_message.h>
#include <topic_tools/shape_shifter.h>
#include "msgs/std_msgs/String.h"
#include "msgs/std_msgs/uint32.h"
#include <string>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>



#include "CECommandAction.h"
#include "actionlib/client/simple_action_client.h"
#include "actionlib/server/simple_action_server.h"
//const double tf::Transformer::DEFAULT_CACHE_TIME = 10.0;
#pragma comment(lib, "C:\\Users\\gbrill\\Source\\Repos\\ROSIndigo\\ROSIndigoDLL\\Debug\\ROSIndigoDLL.lib")

//using namespace tf2_ros;

typedef actionlib::SimpleActionClient<tests::CECommandAction> Client;
//typedef actionlib::SimpleActionServer<tests::CECommandAction> Server;







const std::string XML_PATH1 = R"(C:\Users\gbrill\Source\Repos\ROSIndigo\ROSIndigoDLL\Localization\test\test_ekf_localization_node_test_bag1.test)";


using namespace std;


void crap()
{



}

class CECommandClass
{
protected:

	ros::NodeHandle nh_;
	actionlib::SimpleActionServer<tests::CECommandAction> as_;
	std::string action_name_;
	// create messages that are used to published feedback/result

	tests::CECommandFeedback feedback_;
	tests::CECommandResult result_;
	//learning_actionlib::FibonacciFeedback feedback_;
	//learning_actionlib::FibonacciResult result_;

public:

	void executeCB(const tests::CECommandGoalConstPtr &goal)
	{
		printf("executing:%s\n", goal->command);

		result_.returnCode = 66;

		as_.setSucceeded(result_);

	}


	CECommandClass(std::string name) :
		as_(nh_, name, boost::bind(&CECommandClass::executeCB, this, _1), false),
		action_name_(name)
	{
		as_.start();
	}

};


void SendCommand(std::string cmd, Client &client)
{

	tests::CECommandGoal goal;
	//		chores::DoDishesGoal goal;
	// Fill in goal here

	goal.command = cmd;

	client.sendGoal(goal);

	bool finished_before_timeout = client.waitForResult(ros::Duration(5.0));

	if (finished_before_timeout)
	{

		if (client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
		{

			printf("yay!\n");
			printf("return value:%d\n", client.getResult()->returnCode);

		}

	}
	else
	{
		printf("timeout2\n");
	}

	printf("Current State: %s\n", client.getState().toString().c_str());


}


int _tmain_(int argc, _TCHAR* argv[])
{
	
		ros::init(argc, argv , "do_commands_client");


		if (ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Info)) 
		{
			ros::console::notifyLoggerLevelsChanged();
		}

//		bool b = true;

		Client client("CECommand_server", true); // true -> don't need ros::spin()
		bool b=client.waitForServer(ros::Duration(5.0));

		if (!b)
		{

			printf("timeout1\n");
			return 0;

		}



		SendCommand("booger", client);
		SendCommand("test", client);



		return 0;





	//dynamic_reconfigure::Config config;
	//dynamic_reconfigure::StrParameter s;
	//s.name = "name";
	//s.value = "somevalue";
	//config.strs.push_back(s);
	//
	//SerializeROSMessage<dynamic_reconfigure::Config>(config);

}

