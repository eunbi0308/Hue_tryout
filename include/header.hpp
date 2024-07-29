#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include <curl/curl.h>
#include "./rapidjson/include/rapidjson/document.h"
#include "./rapidjson/include/rapidjson/writer.h"
#include "./rapidjson/include/rapidjson/stringbuffer.h"

using namespace std;

enum e_switch
{
    off = 0,
    on
};

string makeRequest(const string& url, const string& method, const string& data);
void lightSwitch(const string& bridgeIp, const string& username, vector<int> lightIds, e_switch switchValue);