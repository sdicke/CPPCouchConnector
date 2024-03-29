# CPPCouchConnector

CPPCouchConnector is a C++ library, which allow you to interact with couchdb databases.

Please not that this library is in a alpha state at the moment.

# Dependencies

The used library cpr needs some depencies, which development files must be located within the search path for a succesful build of CPPCouchConnector:
* libcurl
* libopenssl

# JSON processing
For JSON processing the library  [jsoncons](https://github.com/danielaparker/jsoncons) is used. It is linked as submodule within this repository.

# Limitations

This library implements at the moment just implements a minimal part of the API endpoints exposed from couchdb. No of the partitioning features are supported now.

# Usage

```cpp
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
```

# Build

GNU Make is used as the project`s build system.

```
git clone https://github.com/sdicke/CPPCouchConnector
cd CPPCouchConnector
git submodule update --init
cmake -S external/cpr -B external/cpr
make external/cpr
cd ../..
make .
```
If you want to keep it simpler, you can execute the `build.sh` script.

If you want to create a smaller binary, you can use the make target nodebug alternatively.
