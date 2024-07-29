#include "header.hpp"

// Turn on light
void lightSwitch(const string& bridgeIp, const string& username, vector<int> lightIds, e_switch switchValue)
{
	for (int lightId : lightIds)
	{
		string url = "http://" + bridgeIp + "/api/" + username + "/lights/" + to_string(lightId) + "/state";

		// JSON data
		rapidjson::Document	document;
		document.SetObject();
		// on or off
		rapidjson::Value on(switchValue == e_switch::on);
		document.AddMember("on", on, document.GetAllocator());
		rapidjson::StringBuffer	buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		document.Accept(writer);

		string	response = makeRequest(url, "PUT", buffer.GetString());
		cout << "Light " << lightId << response << "\n";
	}
}