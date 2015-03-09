#include <iostream>
#include <atlstr.h>
#include "curl/curl.h"

#define NOTICE "https://api.parse.com/1/classes/Notice/ULL6ulFvu3"
#define VERSION "https://api.parse.com/1/classes/Version/RopOQ76DJy"
#define APP_ID "X-Parse-Application-Id: vWnOEJ7weaXLjamEaCSkN58z1Jtn0AEWAGN7QBjP"
#define API_KEY "X-Parse-REST-API-Key: ZPY2iPVYIPO0AlvJdTotCnJNtqSBVE9015P71ix0"


class ParseAPI
{
public:
	ParseAPI()
	{
		this->curl = curl_easy_init();
		if(!curl) {
			std::cout << "Unable to initialize cURL interface" << std::endl;
		}
    
	}

	~ParseAPI()
	{
		curl_easy_cleanup(curl);
		curl_global_cleanup() ;
	}


	CURL *curl;


	

	int signIn(std::string id, std::string pass);
	int getData(CString* data , std::string apiURL);
	
};