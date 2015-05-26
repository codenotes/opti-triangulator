// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

//#include "targetver.h"

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