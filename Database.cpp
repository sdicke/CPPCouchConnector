#include "Database.hpp"

using namespace CCC;

Database::Database(std::shared_ptr<DataBaseConnection> connection, const std::string &name) {
	this->data.connection = connection;
	this->data.name = name;
}

Document Database::createDocument(const json &content, std::string uuid) {
	cpr::Body body(content.as_string());
	if (uuid.empty()) {
		std::vector<std::string> uuids;
		const auto result = this->data.connection->uuids(1l);
		for (const auto &current : result.array_range()){
			uuids.push_back(current.as_string());
		}
		const auto uuid = uuids.at(0ul);
	}
	const auto result = this->data.connection->createDocument(this->data.name, uuid, body);
	const auto real_uuid = result["id"s].to_string();
	const auto revision = result["rev"s].to_string();
	const auto document = Document(uuid, this->data.name, revision, this->data.connection);
	return document;
}

Database::DeleteResult Database::removeDocument(Document &document) {
	const auto id = document.uuid();
	const auto result = this->data.connection->deleteDocument(this->data.name, id);
	return result;
}

std::vector<Document> Database::documents() {
	const auto name = this->data.name;
	const auto connection = this->data.connection;
	const auto result = this->data.connection->dbAllDocs(name, json());
	std::vector<Document> docs;
	for (const auto &current : result["rows"s].array_range()){
		const auto uuid = current["id"s].as_string();
		const auto revision = current["value"s]["rev"s].as_string();
		const auto temp = Document(uuid, name, revision, connection);
		docs.push_back(temp);
	}
	return docs;
}

std::vector<std::string> Database::findDocument(const Map &map){
	std::vector<std::string> result;
	const auto name = this->data.name;
	DataBaseConnection::FindOptions options;
	json selector;
	for (const auto &current : map){
		selector[current.first] = current.second;
	}
	options.selector = selector;
	auto data = this->data.connection->findDocs(name, options);
	if (data.second == DataBaseConnection::FindResult::OK){
		for (const auto &current : data.first.array_range()){
			result.push_back(current["_id"].as_string());
		}
	}
	return result;
}

std::string Database::name() {
	return this->data.name;
}

bool Database::compact(){
	return this->data.connection->compactDB(this->data.name);
}

long Database::maximalNumberOfRevisions() {
	const auto name = this->data.name;
	return this->data.connection.get()->maximalNumberOfRevisions(name);
}

bool Database::setMaximalNumberOfRevisions(const long count) {
	const auto name = this->data.name;
	return this->data.connection.get()->setMaximalNumberOfRevisions(name, count);
}
