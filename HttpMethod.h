#ifndef HTTPMETHOD_H
#define HTTPMETHOD_H
#include <string>


enum class HttpMethod {
	GET,
	POST,
	PUT,
	DELETE,
	UNKNOWN
};

HttpMethod StringToHttpMethod(const std::string&);

std::string HttpMethodToString(const HttpMethod&);

#endif //HTTPMETHOD_H
