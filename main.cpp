#include <iostream>
#include <map>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sstream>

#include "HttpMethod.h"

// HTTP 상태 코드와 메시지를 매핑
const std::map<int, std::string> httpStatusMessages = {
	{200, "OK"},
	{201, "Created"},
	{204, "No Content"},
	{400, "Bad Request"},
	{401, "Unauthorized"},
	{403, "Forbidden"},
	{404, "Not Found"},
	{405, "Method Not Allowed"},
	{500, "Internal Server Error"},
	{501, "Not Implemented"},
	{503, "Service Unavailable"}
};

std::string createResponse(const std::string& contentType, const std::string& body, int statusCode = 200) {
	std::string statusText = (httpStatusMessages.find(statusCode)->second);

	return "HTTP/1.1 "+ std::to_string(statusCode) +" " + statusText + "\r\n"
			+ "Content-Type: " + contentType + "\r\n"
			+ "Content-Length: " + std::to_string(body.length()) + "\r\n"
			+ "\r\n"
			+ body;
}

std::string getBodyRawText(char buffer[1024]) {
	std::string request(buffer);
	size_t bodyStart = request.find("\r\n\r\n"); // HTTP 헤더 끝
	std::string body;
	if (bodyStart != std::string::npos) {
		body = request.substr(bodyStart + 4); // "\r\n\r\n" 이후의 데이터
	}
	return body;
}

void handleRequest(int clientSocket) {
	// 클라이언트의 요청을 읽기
	char buffer[1024];
	int bytesRead = read(clientSocket, buffer, sizeof(buffer));
	if (bytesRead <= 0) {
		return;
	}

	buffer[bytesRead] = '\0'; // null terminator 추가
	// 첫 번째 줄만 파싱하여 요청 메서드 확인
	std::istringstream requestStream(buffer);
	std::string method_string;
	requestStream >> method_string;

	const HttpMethod httpMethod = StringToHttpMethod(method_string);
	std::string response;
	switch (httpMethod) {
		case HttpMethod::GET:
			// std::cout << "GET" << std::endl;
			// HTTP 응답 헤더 작성 (GET 요청 시)
			response = createResponse("text/plain", "Hello World");
			break;
		case HttpMethod::POST: {
			// std::cout << "POST" << std::endl;
			std::string rawBody = getBodyRawText(buffer);
			// std::cout << "body: " + rawBody << std::endl;

			response = createResponse("text/plain", rawBody);
			break;
		}
		default: {
			// 미구현 HTTP 메소드에 대한 응답
			response = createResponse("text/plain", "Method Not Supported", 501);
			break;
		}
	}
	send(clientSocket, response.c_str(), response.length(), 0);
}

int main() {
	// 소켓 생성
	const int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0) {
		std::cerr << "Socket creation failed\n";
		return -1;
	}

	int opt = 1;
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	// 서버 주소 설정
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080); // 포트 8080
	serverAddress.sin_addr.s_addr = INADDR_ANY; // 모든 네트워크 인터페이스에서 수신

	// 소켓 바인딩
	if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
		std::cerr << "Bind failed\n";
		return -1;
	}

	// 클라이언트 연결 대기
	if (listen(serverSocket, 5) < 0) {
		std::cerr << "Listen failed\n";
		return -1;
	}

	std::cout << "Server is running on http://localhost:8080\n";

	while (true) {
		// 클라이언트 연결 수락
		const int clientSocket = accept(serverSocket, nullptr, nullptr);
		handleRequest(clientSocket);
		close(clientSocket);
	}

	// 서버 종료 시 소켓 닫기
	close(serverSocket);
	return 0;
}
