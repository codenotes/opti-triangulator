#include "stdafx.h"


using namespace tinyxml2;
using namespace std;

const std::string XML_PATH1 = R"(C:\Users\gbrill\Source\Repos\ROSIndigo\ROSIndigoDLL\Localization\test\test_ekf_localization_node_test_bag1.test)";


void getGlobalParam(tinyxml2::XMLDocument & doc)
{

	for (tinyxml2::XMLElement* child = doc.FirstChildElement("launch")->FirstChildElement("param");
		child != 0; child = child->NextSiblingElement("param"))
	{
		if (child->ToElement()->Attribute("name"))
			std::cout << "name= " << child->ToElement()->Attribute("name") << std::endl;

	}

}

void getNodes(tinyxml2::XMLDocument & doc)
{
	for (tinyxml2::XMLElement* child = doc.FirstChildElement("launch")->FirstChildElement("node");
		child != 0; child = child->NextSiblingElement("node"))
	{
		if (child->ToElement()->Attribute("pkg"))
			std::cout << "pkg = " << child->ToElement()->Attribute("pkg") << std::endl;
		if (child->ToElement()->Attribute("name"))
			std::cout << "name = " << child->ToElement()->Attribute("name") << std::endl;
	}
}

void getNodeParam(char * nodename, tinyxml2::XMLDocument & doc)
{

	for (tinyxml2::XMLElement* child = doc.FirstChildElement("launch")->FirstChildElement("node");
		child != 0; child = child->NextSiblingElement("node"))
	{
		string n = child->ToElement()->Attribute("name");

		if (n == nodename)
		{

			if (child->ToElement()->Attribute("pkg"))
				std::cout << "pkg = " << child->ToElement()->Attribute("pkg") << std::endl;
			if (child->ToElement()->Attribute("name"))
				std::cout << "name = " << child->ToElement()->Attribute("name") << std::endl;

		}
	}


}



void getNodeROSParam(char * nodename, tinyxml2::XMLDocument & doc, boost::unordered_map<std::string, std::string> & results, const char * paramname=0)
{

	for (tinyxml2::XMLElement* child = doc.FirstChildElement("launch")->FirstChildElement("node");
		child != 0; child = child->NextSiblingElement("node"))
	{
		string n = child->ToElement()->Attribute("name");

		if (n == nodename)
		{

			for (tinyxml2::XMLElement* child2 = child->FirstChildElement("rosparam");
				child2 != 0; child2 = child2->NextSiblingElement("rosparam"))
			{
				if (paramname != 0)
					if (strcmp(paramname,  child2->ToElement()->Attribute("param"))) //if not a match
						continue;


				if (child2->ToElement()->Attribute("param") ) //param is really 'name' as it has the name of the parameter in it.  Value is in the XML element
				{
			//		std::cout << "param= " << child2->ToElement()->Attribute("param") << std::endl;
					results[child2->ToElement()->Attribute("param")] = child2->ToElement()->GetText();
				//	cout << child2->ToElement()->GetText() << endl;
				}

			}

			/*		if (child->ToElement()->Attribute("pkg"))
			std::cout << "pkg = " << child->ToElement()->Attribute("pkg") << std::endl;
			if (child->ToElement()->Attribute("name"))
			std::cout << "name = " << child->ToElement()->Attribute("name") << std::endl;*/

		}
	}


}

void TestParse()
{
//	tinyxml2::XMLDocument doc;
//	boost::unordered_map<std::string, std::string> results;
//
//	if (doc.LoadFile(XML_PATH1.c_str()) == tinyxml2::XML_NO_ERROR)
//	{
//		//getGlobalParam(doc);
//		//getNodes(doc);
//		getNodeROSParam("ekf_localization_node_test_bag_test1", doc, results, "process_noise_covariance");
//	}
//
//
////	cout << results.size();
//	cout << results["process_noise_covariance"];
//
//	/*string s = results["odom0_config"];
//
//	string t = "[true, true, true,"
//		"false, false, false, "
//		"false, false, false, "
//		"false, false, false, "
//		"false, false, false]";*/
//
//	YAML::Node node = YAML::Load(results["process_noise_covariance"].c_str());
//	assert(node.IsSequence());  
//
//	cout << node[0].as<float>() << " " << node[1].as<float> ()<< endl;



}