# Server
## Introduction
The Server class represents the base capabilities of the Couchdb server. 
## Construction
The class has one constructor, which is just called with a shared pointer initialized with an DataBaseConnection object.
``` Server(std::shared_ptr<DataBaseConnection> connection);```
## Functions to retrieve basic information
### meta
``` Meta meta();``` 
This function returns a struct of the type Meta. Meta structs contain to fields: uuid and version. This function allows to identify the represented database server.
### up 
``` bool up();``` 
This function inform you if the server is up and able to serve you.
### uuids
``` std::vector<std::string> uuids(const long count = 25l);``` 
This function returns a vector of UUIDs you can use to name documents.
### uuid
``` std::string uuid();``` 
This function is a wrapper upon uuids and returns a single UUID.
### favicon
``` std::string favicon();``` 
This function returns the favicon of the server, is often empty. The data contained are binary data. You should consider this if you use it.
## Functions regarding databases
### databaseExists
``` bool databaseExists(const std::string &name);``` 
This function checks if the database with the name name exists on the database server.
### createDatabase
``` std::optional<DBC::CreateDatabaseResult> createDatabase(const std::string &name);```
This functions creates a new database on the server and returns a Database object if successfully conducted. 
### deleteDatabase
``` DeleteDatabaseResult deleteDatabase(const std::string &name);``` 
This function is there to delete the database with the name name. It returns the status code from the server. If it return value represents the value 200 or 202, it was totally or at least partially successful.
### databases
``` std::vector<Database> databases(const DataBaseConnection::AllDBsOptiions &options = {});``` 
This function returns a list of databases stored on the server. The options struct allows to specify some options regarding to the sorting and the contained databases. If options is empty just a list of all databases with no special order is returned. The return value is a vector of the databases.