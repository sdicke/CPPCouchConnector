#pragma once

#include <memory>
#include <optional>
#include <string>

#include <jsoncons/json.hpp>

#include "Network.hpp"

namespace CCC {
	using namespace std::literals::string_literals;
	using jsoncons::json;

	class DataBaseConnection {
		private:
			typedef std::unordered_map<std::string,std::string> map;

			enum Codes : long {
				OK = 200l,
				Created = 201l,
				Accepted = 202l,
				NotModified = 304l,
				BadRequest = 400l,
				Unauthorized = 401l,
				NotFound = 404l,
				Conflict = 409l,
				PreconditionFailed = 412l,
				InternalServerError = 500l
			};

			struct {
				std::shared_ptr<Network> network;
			} data;

			std::string buildQuery(map kvs) const;
			json changeDocument(const std::string &name, const std::string &uuid, const std::string revision, const json &content);
			std::string bool_to_string(const bool value) const noexcept;

		public:

			enum class CreateDatabaseResult {
				Created = Codes::Created,
				Accepted = Codes::Accepted,
				BadRequest = Codes::BadRequest,
				Unauthorized = Codes::Unauthorized,
				PreconditionFailed = Codes::PreconditionFailed
			};
			enum class DeleteDatabaseResult {
				OK = Codes::OK,
				Accepted = Codes::Accepted,
				BadReqeust = Codes::BadRequest,
				Unauthorized = Codes::Unauthorized,
				NotFound = Codes::NotFound
			};
			enum class DeleteDocumentResult {
				OK = Codes::OK,
				Accepted = Codes::Accepted,
				BadReqeust = Codes::BadRequest,
				Unauthorized = Codes::Unauthorized,
				NotFound = Codes::NotFound,
				Conflict = Codes::Conflict
			};
			enum class AddDocumentResult {
				Created = Codes::Created,
				Accepted = Codes::Accepted,
				BadRequest = Codes::BadRequest,
				Unauthorized = Codes::Unauthorized,
				NotFound = Codes::NotFound,
				Conflict = Codes::Conflict
			};
			enum class DocInfoResult {
				OK = Codes::OK,
				NotModified = Codes::NotModified,
				Unauthorized = Codes::Unauthorized,
				NotFound = Codes::NotFound
			};
			enum class FindResult {
				OK = Codes::OK,
				BadRequest = Codes::BadRequest,
				Unauthorized = Codes::Unauthorized,
				NotFound = Codes::NotFound,
				InternalServerError = Codes::InternalServerError
			};

			struct FindOptions {
				long limit = 25l;
				long skip = 0l;
				json selector;
				json fields;
				json sort;
			};
			struct GetOptions {
				bool latest = true;
				bool revs = false;
				std::string rev = ""s;
			};
			struct AllDBsOptiions {
				bool descending = false;
				long limit = 0l;
				long skip = 0l;
				std::string startkey = ""s;
			};

			DataBaseConnection(std::shared_ptr<Network> network);

			json meta();
			json allDBs(const AllDBsOptiions &options);
			bool up();
			json uuids(unsigned long count = 1ul);
			std::string favicon();
			bool dbExists(const std::string &name);
			json getDB(const std::string &name);
			json createDocument(const std::string &name, const std::string &uuid, const cpr::Body &content);
			json updateDocument(const std::string &name, const std::string &uuid, const std::string &revision, const json &content);
			std::optional<CreateDatabaseResult> createdDB(const std::string &name);
			DeleteDatabaseResult deleteDB(const std::string &name);
			json dbAllDocs(const std::string &name, const json &selectors);
			std::pair<json, FindResult> findDocs(const std::string &name, const FindOptions &options);
			json dbDoc(const std::string &name, const std::string &uuid, const GetOptions &options);
			DocInfoResult docInfo(const std::string &name, const std::string &uuid);
			DeleteDocumentResult deleteDocument(const std::string &name, const std::string &uuid);
			bool compactDB(const std::string &name);
			bool purgeDoc(const std::string &name, json list);
			long maximalNumberOfRevisions(const std::string &name);
			bool setMaximalNumberOfRevisions(const std::string &name, const long count);
	};
	typedef DataBaseConnection DBC ;
}
