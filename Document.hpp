#pragma once

#include <memory>
#include <string>
#include <vector>

#include "DataBaseConnection.hpp"

using namespace std::literals::string_literals;

namespace CCC {
	using jsoncons::json;
	class Document {
		private:
			enum class GetType {
				Content,
				Revisions
			};
			struct Arguments {
				std::string uuid;
				std::string revision = ""s;
				std::string db;
				std::shared_ptr<DataBaseConnection> connection;
			};
			Arguments data;

			json get(const GetType type, const std::string &revision = ""s);

		public:
			Document(const std::string &uuid, const std::string &db, const std::string &revision, std::shared_ptr<DataBaseConnection> connection);
			Document(const Arguments &arguments);
			std::string uuid();
			std::string latest_revision();
			std::string revision_of_content();
			void setRevision(const std::string &revision);
			json json_content();
			std::string content();
			void setContent(json json);
			std::vector<std::string> revisions();
			bool purge(std::vector<std::string> revisions);
	};
}
