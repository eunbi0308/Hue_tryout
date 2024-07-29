#include "header.hpp"

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((string*)userp)->append((char *)contents, size * nmemb);

	return size * nmemb;
}

// Making a HTTP request
string makeRequest(const string& url, const string& method, const string& data = "")
{
	// Creat a Curl handle
	CURL *curl = curl_easy_init();

	if (curl)
	{
		CURLcode res = curl_easy_perform(curl);
		
		// Set URL and options
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());

		// Set headers
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		// data for POST/PUT requests
		if (!data.empty())
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

		// Capture response
		string readBuffer;
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);

		if (res != CURLE_OK)
		{
			cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
		}
		return readBuffer;
	}
	return "";
}

int main()
{
	ifstream ifs("config.json");
	string fileContent((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
	
	rapidjson::Document	document;
	document.Parse(fileContent.c_str());

	if (document.IsObject())
	{
		// Access membsers
		string bridge_ip = document["bridge_ip"].GetString();
		string username = document["username"].GetString();
		// Access lights array
		const rapidjson::Value& lightsArr = document["lights"];
		vector<int> lights;
		for (rapidjson::SizeType i = 0; i < lightsArr.Size(); ++i)
			lights.push_back(lightsArr[i].GetInt());
		
		lightSwitch(bridge_ip, username, lights, on);
	}
	else
	{
		cerr << "Error: Config file is not valid.\n";
		return 1;
	}
	return 0;
}
