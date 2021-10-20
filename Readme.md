# CPPCouchConnector

CPPCouchConnector is a C++ library, which allow you to interact with couchdb databases.

# Dependencies

* [cpr](https://github.com/libcpr/cpr): Must be installed within the search path (/usr/local/ for example)
* [jsoncons](https://github.com/danielaparker/jsoncons): The header files must be available within the search path (/usr/local/include for example)
* [libcurl](https://github.com/curl/curl): Must be installed within the search path

# Limitations

This library implements at the moment just implements a minimal part of the API endpoints exposed from couchdb. No of the partitioning features are supported now.

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
}
```` 

# Build

GNU Make is used as the project`s build system.

```
git clone https://github.com/sdicke/CPPCouchConnector
cd CPPCouchConnector
make .
```
