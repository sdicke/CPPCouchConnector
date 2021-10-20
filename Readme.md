# CPPCouchConnector

CPPCouchConnector is a C++ library, which allow you to interact with couchdb databases.

# Dependencies

* [cpr](https://github.com/libcpr/cpr)
* [jsoncons](https://github.com/danielaparker/jsoncons)

# Usage

```` 
#include <memory>
#include <cppcouchconnector/Network.hpp>
#include <cppcouchconnector/DataBaseConnection.hpp>
#include <cppcouchconnector/Server.hpp>

int main(int argc, char **argv){
	const std::string _server = "";
	const std::string _user = "";
	const std::string _password = "";
	auto network = std::make_shared<Network>(_server, "5984", _user, _password);
	auto dbc = std::make_shared<DataBaseConnection>(network);
	auto server = Server(dbc);
}
```` 
