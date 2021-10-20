#pragma once

#include <memory>
#include <string>
#include <vector>

#include <jsoncons/json.hpp>

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

			Database(std::shared_ptr<DataBaseConnection> connection, const std::string &name);
			Document createDocument(const json &content, std::string uuid = ""s);
			DeleteResult removeDocument(Document &document);
			std::vector<Document> documents();
			void findDocument(const std::string &name);
			std::string name();
			bool compact();
			long maximalNumberOfRevisions();
			bool setMaximalNumberOfRevisions(const long count);
	};
}

