#include "hello.h"

#include <algorithm>
#include <thread>
#include <chrono>

std::string Hello::sayHello(std::string & name) {
	return "Hello, " + name + "!";
}

std::string Hello::sayHelloUpper(std::string & name) {
	std::string upperCaseMessage = name;
	std::transform(upperCaseMessage.begin(), upperCaseMessage.end(),upperCaseMessage.begin(), ::toupper); 
	return "HELLO, " + upperCaseMessage + "!";
}

std::string Hello::sayHelloFormal(std::string & name) {
	//let's make this slow
	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	return "Good day, " + name + ", how are you today?";
}




