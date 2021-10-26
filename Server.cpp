#include "Server.hpp"

using namespace CCC;

Server::Server(std::shared_ptr<DataBaseConnection> connection) {
	this->data.connection = connection;
}

bool Server::up() {
	return this->data.connection->up();
}

Server::Meta Server::meta(){
	Meta result;
	auto mdata = this->data.connection->meta();
	result.uuid = mdata["uuid"s].as_string();
	result.version = mdata["version"s].as_string();
	return result;
}

std::vector<Database> Server::databases(const DataBaseConnection::AllDBsOptiions &options) {
	const auto result = this->data.connection->allDBs(options);
	std::vector<Database> dbs;
	for (const auto &current : result.array_range()) {
		const auto temp = Database(this->data.connection, current.as_string());
		dbs.push_back(temp);
	}
	return dbs;
}

bool Server::databaseExists(const std::string &name) {
	return this->data.connection->dbExists(name);
}

std::vector<std::string> Server::uuids(long count) {
	std::vector<std::string> uuids;
	const auto result = this->data.connection->uuids(count);
	for (const auto &current : result["uuids"].array_range()){
		uuids.push_back(current.as_string());
	}
	return  uuids;
}

std::string Server::uuid() {
	return this->uuids(1).at(0);
}

std::string Server::favicon() {
	return this->data.connection->favicon();
}

std::optional<DBC::CreateDatabaseResult> Server::createDatabase(const std::string &name) {
	return this->data.connection->createdDB(name);
}

Server::DeleteDatabaseResult Server::deleteDatabase(const std::string &name) {
	return this->data.connection->deleteDB(name);
}
