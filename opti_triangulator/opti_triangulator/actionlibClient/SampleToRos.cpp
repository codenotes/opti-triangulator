//=============================================================================
// Copyright � 2014 NaturalPoint, Inc. All Rights Reserved.
// 
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall NaturalPoint, Inc. or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or Fpublishconsequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//=============================================================================

void StartROS();

/*

SampleClient.cpp

This program connects to a NatNet server, receives a data stream, and writes that data stream
to an ascii file.  The purpose is to illustrate using the NatNetClient class.

Usage [optional]:

SampleClient [ServerIP] [LocalIP] [OutputFilename]

[ServerIP]			IP address of the server (e.g. 192.168.0.107) ( defaults to local machine)
[OutputFilename]	Name of points file (pts) to write out.  defaults to Client-output.pts

*/

//#include "stdafx.h"

#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <winsock2.h>

#include "include/NatNetTypes.h"
#include "include/NatNetClient.h"

#include "geometry_msgs/PoseStamped.h"
#include <map>

#include <ros/ros.h>
#include "triangulator/beaconSettings.h"
#include "ColorCon.h"

#pragma comment(lib,"wsock32")

#pragma warning( disable : 4996 )


ros::NodeHandle * n;
ros::Publisher * ROSPubPose;
ros::Publisher beaconPublisher;

bool gbPublishersCreated = false; 

ColorCon gPrintout1;


 class stPub
{
public:

	std::string name;

	ros::Publisher publisher;
	stPub(std::string name_, std::string  topic, int qSize=10)
	{


		publisher = n->advertise<geometry_msgs::PoseStamped>(topic, qSize);
		name = name_;


	}

	stPub()
	{
		printf("should not be here!!!!");

	}


};


std::map<int, std::string> IDtoName;
std::map<int, stPub*> IDtoPublisher;




void _WriteHeader(FILE* fp, sDataDescriptions* pBodyDefs);
void _WriteFrame(FILE* fp, sFrameOfMocapData* data);
void _WriteFooter(FILE* fp);
void __cdecl DataHandler(sFrameOfMocapData* data, void* pUserData);		// receives data from the server
void __cdecl MessageHandler(int msgType, char* msg);		            // receives NatNet error mesages
void resetClient();
int CreateClient(int iConnectionType);

unsigned int MyServersDataPort = 3130;
unsigned int MyServersCommandPort = 3131;

NatNetClient* theClient;
FILE* fp;

char szMyIPAddress[128] = "";
char szServerIPAddress[128] = "";

//#pragma comment(lib, "ROSIndigoDLL.lib")
//#pragma comment(lib, "boost_system-vc140-mt-gd-1_58.lib")
//#pragma comment(lib, "boost_thread-vc140-mt-gd-1_58.lib")
////#pragma comment(lib,"boost_signals-vc140-mt-gd-1_58.lib")
////#pragma comment(lib,"boost_program_options-vc140-mt-gd-1_58.lib")
//#pragma comment(lib,"boost_regex-vc140-mt-gd-1_58.lib")

//#define TEST_MODE

#include "std_msgs/String.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int iResult;
	int iConnectionType = ConnectionType_Multicast;
	//int iConnectionType = ConnectionType_Unicast;
	StartROS();




#ifdef TEST_MODE

	geometry_msgs::PoseStamped ps;

	std_msgs::String s;
	s.data = "hi there";

	auto n = new ros::NodeHandle();
	//auto pub = n->advertise<geometry_msgs::PoseStamped>("testpose", 10);
	auto pub = n->advertise<std_msgs::String>("teststring", 10);

	ROS_INFO("this should print");

	ROS_DEBUG_NAMED("interop", "this hsould also");

	while (ros::ok())
	{
//		ps.pose.position.x += 1;
	//	pub.publish(ps);
	//	printf("publish %f\n", ps.pose.position.x);
		pub.publish(s);
		

		ros::Duration(.2).sleep();

	}


	return 0;
#endif


	// parse command line args
	if (argc>1)
	{
		strcpy(szServerIPAddress, argv[1]);	// specified on command line
		printf("Connecting to server at %s...\n", szServerIPAddress);
	}
	else
	{
		strcpy(szServerIPAddress, "");		// not specified - assume server is local machine
		printf("Connecting to server at LocalMachine\n");
	}
	if (argc>2)
	{
		strcpy(szMyIPAddress, argv[2]);	    // specified on command line
		printf("Connecting from %s...\n", szMyIPAddress);
	}
	else
	{
		strcpy(szMyIPAddress, "");          // not specified - assume server is local machine
		printf("Connecting from LocalMachine...\n");
	}

	// Create NatNet Client
	iResult = CreateClient(iConnectionType);
	if (iResult != ErrorCode_OK)
	{
		printf("Error initializing client.  See log for details.  Exiting");
		return 1;
	}
	else
	{
		printf("Client initialized and ready.\n");
	}


	// send/receive test request
	printf("[SampleClient] Sending Test Request\n");
	void* response;
	int nBytes;
	iResult = theClient->SendMessageAndWait("TestRequest", &response, &nBytes);
	if (iResult == ErrorCode_OK)
	{
		printf("[SampleClient] Received: %s", (char*)response);
	}

	// Retrieve Data Descriptions from server
	printf("\n\n[SampleClient] Requesting Data Descriptions...");
	sDataDescriptions* pDataDefs = NULL;
	int nBodies = theClient->GetDataDescriptions(&pDataDefs);
	if (!pDataDefs)
	{
		printf("[SampleClient] Unable to retrieve Data Descriptions.");
	}
	else
	{
		printf("[SampleClient] Received %d Data Descriptions:\n", pDataDefs->nDataDescriptions);
		for (int i = 0; i < pDataDefs->nDataDescriptions; i++)
		{
			printf("Data Description # %d (type=%d)\n", i, pDataDefs->arrDataDescriptions[i].type);
			if (pDataDefs->arrDataDescriptions[i].type == Descriptor_MarkerSet)
			{
				// MarkerSet
				sMarkerSetDescription* pMS = pDataDefs->arrDataDescriptions[i].Data.MarkerSetDescription;
				printf("MarkerSet Name : %s\n", pMS->szName);
				for (int i = 0; i < pMS->nMarkers; i++)
					printf("%s\n", pMS->szMarkerNames[i]);

			}
			else if (pDataDefs->arrDataDescriptions[i].type == Descriptor_RigidBody)
			{
				// RigidBody, names and IDs gathered here
				sRigidBodyDescription* pRB = pDataDefs->arrDataDescriptions[i].Data.RigidBodyDescription;
				printf("RigidBody Name : %s\n", pRB->szName);
				printf("RigidBody ID : %d\n", pRB->ID);
				printf("RigidBody Parent ID : %d\n", pRB->parentID);
				printf("Parent Offset : %3.2f,%3.2f,%3.2f\n", pRB->offsetx, pRB->offsety, pRB->offsetz);
				IDtoName[pRB->ID] = pRB->szName;
				IDtoPublisher[pRB->ID] = new stPub(pRB->szName, pRB->szName, 10);
				

				


			}
			else if (pDataDefs->arrDataDescriptions[i].type == Descriptor_Skeleton)
			{
				// Skeleton
				sSkeletonDescription* pSK = pDataDefs->arrDataDescriptions[i].Data.SkeletonDescription;
				printf("Skeleton Name : %s\n", pSK->szName);
				printf("Skeleton ID : %d\n", pSK->skeletonID);
				printf("RigidBody (Bone) Count : %d\n", pSK->nRigidBodies);
				for (int j = 0; j < pSK->nRigidBodies; j++)
				{
					sRigidBodyDescription* pRB = &pSK->RigidBodies[j];
					printf("  RigidBody Name : %s\n", pRB->szName);
					printf("  RigidBody ID : %d\n", pRB->ID);
					printf("  RigidBody Parent ID : %d\n", pRB->parentID);
					printf("  Parent Offset : %3.2f,%3.2f,%3.2f\n", pRB->offsetx, pRB->offsety, pRB->offsetz);
				}
			}
			else
			{
				printf("Unknown data type.");
				// Unknown
			}
		}

		gbPublishersCreated = true;
	}


	// Create data file for writing received stream into
	char szFile[MAX_PATH];
	char szFolder[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szFolder);
	if (argc > 3)
		sprintf(szFile, "%s\\%s", szFolder, argv[3]);
	else
		sprintf(szFile, "%s\\Client-output.pts", szFolder);
	fp = fopen(szFile, "w");
	if (!fp)
	{
		printf("error opening output file %s.  Exiting.", szFile);
		exit(1);
	}
	if (pDataDefs)
		_WriteHeader(fp, pDataDefs);

	// Ready to receive marker stream!
	printf("\nClient is connected to server and listening for data...\n");
	int c;
	bool bExit = false;
	while (c = _getch())
	{
		switch (c)
		{
		case 'q':
			bExit = true;
			break;
		case 'r':
			resetClient();
			break;
		case 'p':
			sServerDescription ServerDescription;
			memset(&ServerDescription, 0, sizeof(ServerDescription));
			theClient->GetServerDescription(&ServerDescription);
			if (!ServerDescription.HostPresent)
			{
				printf("Unable to connect to server. Host not present. Exiting.");
				return 1;
			}
			break;
		case 'f':
		{
					sFrameOfMocapData* pData = theClient->GetLastFrameOfData();
					printf("Most Recent Frame: %d", pData->iFrame);
		}
			break;
		case 'm':	                        // change to multicast
			iResult = CreateClient(ConnectionType_Multicast);
			if (iResult == ErrorCode_OK)
				printf("Client connection type changed to Multicast.\n\n");
			else
				printf("Error changing client connection type to Multicast.\n\n");
			break;
		case 'u':	                        // change to unicast
			iResult = CreateClient(ConnectionType_Unicast);
			if (iResult == ErrorCode_OK)
				printf("Client connection type changed to Unicast.\n\n");
			else
				printf("Error changing client connection type to Unicast.\n\n");
			break;


		default:
			break;
		}
		if (bExit)
			break;
	}

	// Done - clean up.
	theClient->Uninitialize();
	_WriteFooter(fp);
	fclose(fp);

	return ErrorCode_OK;
}

// Establish a NatNet Client connection
int CreateClient(int iConnectionType)
{
	// release previous server
	if (theClient)
	{
		theClient->Uninitialize();
		delete theClient;
	}

	// create NatNet client
	theClient = new NatNetClient(iConnectionType);

	// [optional] use old multicast group
	//theClient->SetMulticastAddress("224.0.0.1");

	// print version info
	unsigned char ver[4];
	theClient->NatNetVersion(ver);
	printf("NatNet Sample Client (NatNet ver. %d.%d.%d.%d)\n", ver[0], ver[1], ver[2], ver[3]);

	// Set callback handlers
	theClient->SetMessageCallback(MessageHandler);
	theClient->SetVerbosityLevel(Verbosity_None); //Verbosity_Debug);
	theClient->SetDataCallback(DataHandler, theClient);	// this function will receive data from the server

	// Init Client and connect to NatNet server
	// to use NatNet default port assigments
	int retCode = theClient->Initialize(szMyIPAddress, szServerIPAddress);
	// to use a different port for commands and/or data:
	//int retCode = theClient->Initialize(szMyIPAddress, szServerIPAddress, MyServersCommandPort, MyServersDataPort);
	if (retCode != ErrorCode_OK)
	{
		printf("Unable to connect to server.  Error code: %d. Exiting", retCode);
		return ErrorCode_Internal;
	}
	else
	{
		// print server info
		sServerDescription ServerDescription;
		memset(&ServerDescription, 0, sizeof(ServerDescription));
		theClient->GetServerDescription(&ServerDescription);
		if (!ServerDescription.HostPresent)
		{
			printf("Unable to connect to server. Host not present. Exiting.");
			return 1;
		}
		printf("[SampleClient] Server application info:\n");
		printf("Application: %s (ver. %d.%d.%d.%d)\n", ServerDescription.szHostApp, ServerDescription.HostAppVersion[0],
			ServerDescription.HostAppVersion[1], ServerDescription.HostAppVersion[2], ServerDescription.HostAppVersion[3]);
		printf("NatNet Version: %d.%d.%d.%d\n", ServerDescription.NatNetVersion[0], ServerDescription.NatNetVersion[1],
			ServerDescription.NatNetVersion[2], ServerDescription.NatNetVersion[3]);
		printf("Client IP:%s\n", szMyIPAddress);
		printf("Server IP:%s\n", szServerIPAddress);
		printf("Server Name:%s\n\n", ServerDescription.szHostComputerName);
	}

	return ErrorCode_OK;

}

// DataHandler receives data from the server
void __cdecl DataHandler(sFrameOfMocapData* data, void* pUserData)
{
	NatNetClient* pClient = (NatNetClient*)pUserData;

	if (fp)
		_WriteFrame(fp, data);

	int i = 0;

	/*printf("FrameID! : %d\n", data->iFrame);
	printf("Timestamp :  %3.2lf\n", data->fTimestamp);
	printf("Latency :  %3.2lf\n", data->fLatency);
*/
	// FrameOfMocapData params
	
	bool bIsRecording = data->params & 0x01;
	bool bTrackedModelsChanged = data->params & 0x02;
	if (bIsRecording)
		printf("RECORDING\n");
	if (bTrackedModelsChanged)
		printf("Models Changed.\n");


	// timecode - for systems with an eSync and SMPTE timecode generator - decode to values
	int hour, minute, second, frame, subframe;
	bool bValid = pClient->DecodeTimecode(data->Timecode, data->TimecodeSubframe, &hour, &minute, &second, &frame, &subframe);
	// decode to friendly string
	char szTimecode[128] = "";
	pClient->TimecodeStringify(data->Timecode, data->TimecodeSubframe, szTimecode, 128);
	
	
	//printf("Timecode : %s\n", szTimecode);

	// Other Markers

	//printf("Other Markers [Count=%d]\n", data->nOtherMarkers);
	//for (i = 0; i < data->nOtherMarkers; i++)
	//{
	//	printf("Other Marker %d : %3.2f\t%3.2f\t%3.2f\n",
	//		i,
	//		data->OtherMarkers[i][0],
	//		data->OtherMarkers[i][1],
	//		data->OtherMarkers[i][2]);
	//}

	// Rigid Bodies
	//printf("Rigid Bodies [Count=%d]\n", data->nRigidBodies);
	triangulator::beaconSetting bc;
	triangulator::beaconSettings bs;
	int beaconCount = 0;

	
	for (i = 0; i < data->nRigidBodies; i++)
	{
		geometry_msgs::PoseStamped ps;
		// params
		// 0x01 : bool, rigid body was successfully tracked in this frame
		bool bTrackingValid = data->RigidBodies[i].params & 0x01;

		//(R)ed is the color for the X - axis.
		//	(G)reen is the color for the Y - axis.
		//	(B)lue is the color for the Z - axis.

		/*
		
		In motiv, green is verticle.  In ROS green is Y
		Blue and green are reversed between motiv and rviz
		there for Y and Z are reversed, which we knew. 


		
		*/



		ps.pose.position.x = data->RigidBodies[i].x;
		ps.pose.position.y = data->RigidBodies[i].y;//height
		//ps.pose.position.y = - data->RigidBodies[i].z;
		ps.pose.position.z = data->RigidBodies[i].z;
		
		
		ps.pose.orientation.x = data->RigidBodies[i].qx;
		ps.pose.orientation.y = data->RigidBodies[i].qy;
		//ps.pose.orientation.y = -data->RigidBodies[i].qz;
		ps.pose.orientation.z = data->RigidBodies[i].qz;
		ps.pose.orientation.w = data->RigidBodies[i].qw;

		ps.header.stamp =  ros::Time::now();
		
		
		ps.header.frame_id = "map";

		bc.beaconName = IDtoName[ data->RigidBodies[i].ID  ] ;
		
		try
		{
			bc.beaconID =boost::lexical_cast<float>( &(bc.beaconName.c_str()[1]) ); //was 0, but I think this is useful
		}
		catch (...)
		{
			bc.beaconID = 0.0f;
		}
		
		
		bc.X = ps.pose.position.x;
		bc.Y = ps.pose.position.y;
		bc.Z = ps.pose.position.z;
		
		bs.beaconValues.push_back(bc);

		//gPrintout1.sprintConsole(0, 2, FOREGROUND_GREEN,
		//	"@Rigid Body [ID=%d  Name=%s Error=%3.2f  Valid=%d]", 
		//	data->RigidBodies[i].ID, bc.beaconName.c_str() , data->RigidBodies[i].MeanError, bTrackingValid);
			
		//printf("\tx\ty\tz\tqx\tqy\tqz\tqw\n"
			
		/*gPrintout1.sprintConsole(0, i+1, FOREGROUND_GREEN,
		"\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f",
			data->RigidBodies[i].x,
			data->RigidBodies[i].y,
			data->RigidBodies[i].z,
			data->RigidBodies[i].qx,
			data->RigidBodies[i].qy,
			data->RigidBodies[i].qz,
			data->RigidBodies[i].qw);*/


		printf("@Rigid Body [ID=%d  Name=%s Error=%3.2f  Valid=%d]\n", data->RigidBodies[i].ID, bc.beaconName.c_str() , data->RigidBodies[i].MeanError, bTrackingValid);
		printf("\tx\ty\tz\tqx\tqy\tqz\tqw\n");
		printf("\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\n",
			data->RigidBodies[i].x,
			data->RigidBodies[i].y,
			data->RigidBodies[i].z,
			data->RigidBodies[i].qx,
			data->RigidBodies[i].qy,
			data->RigidBodies[i].qz,
			data->RigidBodies[i].qw);



		//ROSPubPose->publish(ps);


		if (n->ok())
		{

			if (gbPublishersCreated)
			{

				IDtoPublisher[data->RigidBodies[i].ID]->publisher.publish(ps);
				
				
			}

			

			ros::spinOnce();
		}
		else
			printf("issue...\n");



		//printf("\tRigid body markers [Count=%d]\n", data->RigidBodies[i].nMarkers);
		//for (int iMarker = 0; iMarker < data->RigidBodies[i].nMarkers; iMarker++)
		//{
		//	printf("\t\t");
		//	if (data->RigidBodies[i].MarkerIDs)
		//		printf("MarkerID:%d", data->RigidBodies[i].MarkerIDs[iMarker]);
		//	if (data->RigidBodies[i].MarkerSizes)
		//		printf("\tMarkerSize:%3.2f", data->RigidBodies[i].MarkerSizes[iMarker]);
		//	if (data->RigidBodies[i].Markers)
		//		printf("\tMarkerPos:%3.2f,%3.2f,%3.2f\n",
		//		data->RigidBodies[i].Markers[iMarker][0],
		//		data->RigidBodies[i].Markers[iMarker][1],
		//		data->RigidBodies[i].Markers[iMarker][2]);
		//}
	}
	//now am done with iterating through rigidbodes, so publish the beaconSet as one big gulp
	beaconPublisher.publish(bs);

	// skeletons
	//printf("Skeletons [Count=%d]\n", data->nSkeletons);
	for (i = 0; i < data->nSkeletons; i++)
	{
		sSkeletonData skData = data->Skeletons[i];
		printf("Skeleton [ID=%d  Bone count=%d]\n", skData.skeletonID, skData.nRigidBodies);
		for (int j = 0; j< skData.nRigidBodies; j++)
		{
			sRigidBodyData rbData = skData.RigidBodyData[j];
			printf("Bone %d\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\n",
				rbData.ID, rbData.x, rbData.y, rbData.z, rbData.qx, rbData.qy, rbData.qz, rbData.qw);

			printf("\tRigid body markers [Count=%d]\n", rbData.nMarkers);
			for (int iMarker = 0; iMarker < rbData.nMarkers; iMarker++)
			{
				printf("\t\t");
				if (rbData.MarkerIDs)
					printf("MarkerID:%d", rbData.MarkerIDs[iMarker]);
				if (rbData.MarkerSizes)
					printf("\tMarkerSize:%3.2f", rbData.MarkerSizes[iMarker]);
				if (rbData.Markers)
					printf("\tMarkerPos:%3.2f,%3.2f,%3.2f\n",
					data->RigidBodies[i].Markers[iMarker][0],
					data->RigidBodies[i].Markers[iMarker][1],
					data->RigidBodies[i].Markers[iMarker][2]);
			}
		}
	}

	// labeled markers
	bool bOccluded;     // marker was not visible (occluded) in this frame
	bool bPCSolved;     // reported position provided by point cloud solve
	bool bModelSolved;  // reported position provided by model solve
//	printf("Labeled Markers [Count=%d]\n", data->nLabeledMarkers);
	for (i = 0; i < data->nLabeledMarkers; i++)
	{
		bOccluded = data->LabeledMarkers[i].params & 0x01;
		bPCSolved = data->LabeledMarkers[i].params & 0x02;
		bModelSolved = data->LabeledMarkers[i].params & 0x04;
		sMarker marker = data->LabeledMarkers[i];
//		printf("Labeled Marker [ID=%d, Occluded=%d, PCSolved=%d, ModelSolved=%d] [size=%3.2f] [pos=%3.2f,%3.2f,%3.2f]\n",
//			marker.ID, bOccluded, bPCSolved, bModelSolved, marker.size, marker.x, marker.y, marker.z);
	}

}

// MessageHandler receives NatNet error/debug messages
void __cdecl MessageHandler(int msgType, char* msg)
{
	printf("\n%s\n", msg);
}

/* File writing routines */
void _WriteHeader(FILE* fp, sDataDescriptions* pBodyDefs)
{
	int i = 0;

	if (!pBodyDefs->arrDataDescriptions[0].type == Descriptor_MarkerSet)
		return;

	sMarkerSetDescription* pMS = pBodyDefs->arrDataDescriptions[0].Data.MarkerSetDescription;

	fprintf(fp, "<MarkerSet>\n\n");
	fprintf(fp, "<Name>\n%s\n</Name>\n\n", pMS->szName);

	fprintf(fp, "<Markers>\n");
	for (i = 0; i < pMS->nMarkers; i++)
	{
		fprintf(fp, "%s\n", pMS->szMarkerNames[i]);
	}
	fprintf(fp, "</Markers>\n\n");

	fprintf(fp, "<Data>\n");
	fprintf(fp, "Frame#\t");
	for (i = 0; i < pMS->nMarkers; i++)
	{
		fprintf(fp, "M%dX\tM%dY\tM%dZ\t", i, i, i);
	}
	fprintf(fp, "\n");

}

void _WriteFrame(FILE* fp, sFrameOfMocapData* data)
{
	fprintf(fp, "%d", data->iFrame);
	for (int i = 0; i < data->MocapData->nMarkers; i++)
	{
		fprintf(fp, "\t%.5f\t%.5f\t%.5f", data->MocapData->Markers[i][0], data->MocapData->Markers[i][1], data->MocapData->Markers[i][2]);
	}
	fprintf(fp, "\n");
}

void _WriteFooter(FILE* fp)
{
	fprintf(fp, "</Data>\n\n");
	fprintf(fp, "</MarkerSet>\n");
}

void resetClient()
{
	int iSuccess;

	printf("\n\nre-setting Client\n\n.");

	iSuccess = theClient->Uninitialize();
	if (iSuccess != 0)
		printf("error un-initting Client\n");

	iSuccess = theClient->Initialize(szMyIPAddress, szServerIPAddress);
	if (iSuccess != 0)
		printf("error re-initting Client\n");


}

void StartROS()
{



	char * c[1][255];
	c[0][0] = "OPTI_ROS";
	int argc = 1;

	ros::init(argc, (char**)c, "OPTI_ROS");

#ifndef TEST_MODE
	//ros::init(argc, argv, "CECommand_server");

	if (ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Info))
	{
		ros::console::notifyLoggerLevelsChanged();
	}

	//	CECommandClass command(ros::this_node::getName());


	
	n = new ros::NodeHandle();
	beaconPublisher = n->advertise<triangulator::beaconSettings  >("beaconPublish", 10);

#endif

	//ROSPubPose = new 	ros::Publisher;
	//*ROSPubPose = n->advertise<geometry_msgs::PoseStamped>("OPTI_POS", 100);

	//gbRosInitialized = true;
	//	ros::Rate loop_rate(10);

}
