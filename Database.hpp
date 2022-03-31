#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "DataBaseConnection.hpp"
#include "Document.hpp"

namespace CCC {
	class Database {
		private:
			struct {
				std::shared_ptr<DataBaseConnection> connection;
				std::string name;
			} data;

		public:
			typedef DataBaseConnection::DeleteDocumentResult DeleteResult;
			typedef std::unordered_map<std::string, std::string> Map;

			Database(std::shared_ptr<DataBaseConnection> connection, const std::string &name);
			Document createDocument(const json &content, std::string uuid = ""s);
			Document createDocument(const json &content);
			DeleteResult removeDocument(Document &document);
			std::vector<Document> documents();
			std::vector<std::string> findDocument(const Map &map);
			std::string name();
			bool compact();
			long maximalNumberOfRevisions();
			bool setMaximalNumberOfRevisions(const long count);
	};
}

