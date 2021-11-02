#pragma once

#include <memory>

#include "Database.hpp"
#include "DataBaseConnection.hpp"

namespace CCC {
	class Server {
		private:
			struct {
				std::shared_ptr<DataBaseConnection> connection;
			} data;

		public:
			typedef DataBaseConnection::CreateDatabaseResult CreateDatabaseResult;
			typedef DataBaseConnection::DeleteDatabaseResult DeleteDatabaseResult;

			struct Meta {
				std::string uuid;
				std::string version;
			};

			Server(std::shared_ptr<DataBaseConnection> connection);
			Meta meta();
			bool up();
			std::vector<Database> databases(const DataBaseConnection::AllDBsOptiions &options = {});
			bool databaseExists(const std::string &name);
			std::vector<std::string> uuids(const long count = 25l);
			std::string uuid();
			std::string favicon();
			std::optional<DBC::CreateDatabaseResult> createDatabase(const std::string &name);
			DeleteDatabaseResult deleteDatabase(const std::string &name);
	};
}
