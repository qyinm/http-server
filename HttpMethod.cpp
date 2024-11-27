#include "HttpMethod.h"

HttpMethod StringToHttpMethod(const std::string &method) {
	if (method == "GET") return HttpMethod::GET;
	if (method == "POST") return HttpMethod::POST;
	if (method == "PUT") return HttpMethod::PUT;
	if (method == "DELETE") return HttpMethod::DELETE;
	return HttpMethod::UNKNOWN;
}

std::string HttpMethodToString(const HttpMethod &method) {
	switch (method) {
		case HttpMethod::GET:
			return "GET";
		case HttpMethod::POST:
			return "POST";
		case HttpMethod::PUT:
			return "PUT";
		case HttpMethod::DELETE:
			return "DELETE";
		default: return "UNKNOWN";
	}
}
