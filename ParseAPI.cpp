#include "stdafx.h"
#include "ParseAPI.h"



size_t writer(char *data, size_t size, size_t nmemb, std::string *writerData)	//WRITEFUNCTION 구현
{
	if (writerData == NULL)
		return 0;

	writerData->append(data, size*nmemb);	//지정된 string에 data를 덧붙인다.
	return size * nmemb;
}


int ParseAPI::signIn( std::string id, std::string pass)
{
	// ?username=betatest&password=betatest
	std::string html;	
	std::string api_url = "https://api.parse.com/1/login";
	std::string id_pass = "?username=";
	id_pass.append(id);
	id_pass.append("&password=");
	id_pass.append(pass);
	api_url.append(id_pass);


    curl_easy_setopt(curl, CURLOPT_URL, api_url.c_str());
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);			//redirect된 경우 경로를 따라 들어가도록 설정
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);			//여기 지정된 포인터를 WRITEFUNCTION에 넘겨준다.
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);		//writer함수 지정

	
	curl_slist* responseHeaders = NULL ;
	responseHeaders = curl_slist_append( responseHeaders , APP_ID ) ;
	responseHeaders = curl_slist_append( responseHeaders , API_KEY ) ;
	
	
	curl_easy_setopt( curl , CURLOPT_HTTPHEADER , responseHeaders ) ;


	  const CURLcode rc = curl_easy_perform(curl);
    if( rc != CURLE_OK ) {

        std::cout << "Error from cURL: " << curl_easy_strerror(rc) << std::endl;
		return -1;
    }
	else
	{
		int validToken = html.find("sessionToken");
		int validID =  html.find(id);

		std::cout << html.c_str() << std::endl;
		if(validToken > 0 && validID > 0)
			return 0;
		else
			return -1;
	}
}



int ParseAPI::getData( CString* noticeArr ,  std::string apiURL)
{
	std::string html;	
	
	//std::string api_url = "https://api.parse.com/1/classes/Notice/ULL6ulFvu3";
	//std::string id_pass = "?username=betatest&password=betatest";
	//api_url.append(id_pass);
	
	

    curl_easy_setopt(curl, CURLOPT_URL, apiURL.c_str());
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);			//redirect된 경우 경로를 따라 들어가도록 설정
	//curl_easy_setopt(curl, CURLOPT_HTTPGET, true);	
   // curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);			//여기 지정된 포인터를 WRITEFUNCTION에 넘겨준다.
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);		//writer함수 지정

	//curl_easy_setopt( curl, CURLOPT_USERPWD, "betatest:betatest" );
	curl_slist* responseHeaders = NULL ;
	responseHeaders = curl_slist_append( responseHeaders , APP_ID ) ;
	responseHeaders = curl_slist_append( responseHeaders , API_KEY ) ;
	
	
	curl_easy_setopt( curl , CURLOPT_HTTPHEADER , responseHeaders ) ;


	const CURLcode rc = curl_easy_perform(curl);

	TCHAR temp[2048]={0,};
	MultiByteToWideChar(CP_UTF8, 0, html.c_str(), strlen(html.c_str()), temp, 2048);
	*noticeArr = temp; 

	//szUniCode = 0x002be8ec L"{\"content\":\"안녕하세요\",\"createdAt\":\"2015-03-04T04:23:02.594Z\",\"objectId\":\"ULL6ulFvu3\",\"updatedAt\":\"2015-03-04T04:23:02.594Z\"}\n"
	  
    if( rc != CURLE_OK ) {
        std::cout << "Error from cURL: " << curl_easy_strerror(rc) << std::endl;
		return -1;
    }
	else 
		return 0;

}