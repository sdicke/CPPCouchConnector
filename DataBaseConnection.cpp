#include <regex>

#include "DataBaseConnection.hpp"

using namespace CCC;
using namespace std::literals;

typedef Network::Method Method;

DataBaseConnection::DataBaseConnection(std::shared_ptr<Network> network) {
	this->data.network = network;
}

std::string DataBaseConnection::bool_to_string(const bool value) const noexcept {
	if (value) {
		return "true"s;
	}
	else {
		return "false"s;
	}
}

std::string DataBaseConnection::buildQuery(map kvs) const {
	std::string result;
	bool first = true;
	const auto separator = "="s;
	for (const auto &current : kvs) {
		if (current.second.empty()) {
			continue;
		}
		std::string firstChar = "&"s;
		if (first) {
			firstChar = "?"s;
			first = false;
		}
		result += firstChar + current.first + separator + current.second;
	}
	return result;
}

json DataBaseConnection::meta()  {
	const auto result = this->data.network->perform("/"s, Method::GET);
	const auto json = json::parse(result.text);
	return json;
}

json DataBaseConnection::allDBs(const DataBaseConnection::AllDBsOptiions &options)  {
	auto url = "_all_dbs"s;
	map options_map;
	if (options.descending) {
		options_map.insert(std::make_pair("descending"s, bool_to_string(options.descending)));
	}
	if (options.limit > 0) {
		options_map.insert(std::make_pair("limit"s, bool_to_string(options.limit)));
	}
	if (options.skip > 0) {
		options_map.insert(std::make_pair("skip"s, bool_to_string(options.skip)));
	}
	if (!options.startkey.empty()) {
		options_map.insert(std::make_pair("startkey"s, options.startkey));
	}
	url += this->buildQuery(options_map);
	const auto result = this->data.network->perform(url, Method::GET);
	json json;
	if (result.status_code == 200) {
		json = json::parse(result.text);
	}
	return json;
}

bool DataBaseConnection::up()  {
	const auto result = this->data.network->perform("_up"s, Method::GET);
	if (result.status_code == 200) {
		return true;
	}
	return false;
}

json DataBaseConnection::uuids(unsigned long count)  {
	const std::string url = "_uuids?count="s + std::to_string(count);
	const auto result = this->data.network->perform(url, Method::GET);
	if (result.status_code == 400) {
		count = (2/3) * count;
		return this->uuids(count);
	}
	json json;
	if (result.status_code == 200) {
		json = json::parse(result.text);
	}
	return json;
}

std::string DataBaseConnection::favicon()  {
	const auto result = this->data.network->perform("favicon.ico"s, Method::GET);
	return result.text;
}

bool DataBaseConnection::dbExists(const std::string &name) {
	const auto result = this->data.network->perform(name, Method::HEAD);
	if (result.status_code == 200) {
		return true;
	}
	return false;
}

json DataBaseConnection::getDB(const std::string &name)  {
	const auto result = this->data.network->perform(name, Method::GET);
	//long size.active //Bytes
	//long sizes.external Uncompressed
	const auto json = json::parse(result.text);
	return json;
}

std::optional<DBC::CreateDatabaseResult> DataBaseConnection::createdDB(const std::string &name) {
	const std::regex regex("^[a-z][a-z0-9_$()+/-]*$"s);
	const auto match = std::regex_match(name, regex);
	if (match){
		const auto result = this->data.network->perform(name, Method::PUT);
		return static_cast<CreateDatabaseResult>(result.status_code);
	}
	else {
		return {};
	}
}

DataBaseConnection::DeleteDatabaseResult DataBaseConnection::deleteDB(const std::string &name) {
	const auto result = this->data.network->perform(name, Method::DELETE);
	return static_cast<DeleteDatabaseResult>(result.status_code);
}

json DataBaseConnection::changeDocument(const std::string &name, const std::string &uuid, const std::string revision, const json &content) {
	auto url = name + "/"s + uuid;
	if (!revision.empty()) {
		url += "?rev="s + revision;
	}
	const auto upload_content = cpr::Body(content.as_string().data());
	const auto result = this->data.network->perform(url, Method::PUT, upload_content);
	const auto text = result.text;
	return json::parse(text);
}

json DataBaseConnection::createDocument(const std::string &name, const std::string &uuid, const cpr::Body &content) {
	return this->changeDocument(name, uuid, ""s, json::parse(content.data()));
}

json DataBaseConnection::updateDocument(const std::string &name, const std::string &uuid, const std::string &revision, const json &content) {
	return this->changeDocument(name, uuid, revision, content);
}

json DataBaseConnection::dbAllDocs(const std::string &name, const json &selectors)  {
	const auto url = name + "/_all_docs"s;
	cpr::Response result;
	if (selectors.empty() ) {
		result = this->data.network->perform(url, Method::GET);
	}
	else {
		cpr::Body payload(selectors.at("keys"s).as_string().c_str());
		result = this->data.network->perform(url, Method::POST, payload);
	}
	json json;
	if (result.status_code == 200) {
		json = json::parse(result.text);
	}
	return json;
}

std::pair<json, DataBaseConnection::FindResult> DataBaseConnection::findDocs(const std::string &name, const FindOptions &options) {
	const auto url = name + "_find"s;
	json arguments;
	arguments["selector"s] = options.selector;
	arguments["limit"s] = options.limit;
	arguments["sort"s] = options.sort;
	arguments["fields"s] = options.fields;
	const auto result = this->data.network->perform(url, Method::POST);
	json json;
	if (result.status_code == 200) {
		json = json::parse(result.text);
	}
	FindResult fresult = static_cast<FindResult>(result.status_code);
	return std::make_pair(json, fresult);
}

json DataBaseConnection::dbDoc(const std::string &name, const std::string &uuid, const GetOptions &options)  {
	auto url = name + "/"s + uuid;
	map options_map;
	if (options.latest) {
		options_map.insert(std::make_pair("latest"s, bool_to_string(options.latest)));
	}
	if (options.revs) {
		options_map.insert(std::make_pair("revs"s, bool_to_string(options.revs)));
	}
	if (!options.rev.empty()) {
		options_map.insert(std::make_pair("rev"s, options.rev));
	}
	url += this->buildQuery(options_map);
	const auto result = this->data.network->perform(url, Method::GET);
	json json;
	if (result.status_code == 200) {
		json = json::parse(result.text);
	}
	return json;
}

DataBaseConnection::DocInfoResult DataBaseConnection::docInfo(const std::string &name, const std::string &uuid) {
	const auto url = name + uuid;
	const auto result = this->data.network->perform(url, Method::HEAD);
	return static_cast<DocInfoResult>(result.status_code);
}

DataBaseConnection::DeleteDocumentResult DataBaseConnection::deleteDocument(const std::string &name, const std::string &uuid) {
	const auto url = name + "/"s + uuid;
	auto result = this->data.network->perform(url, Method::DELETE);
	return static_cast<DeleteDocumentResult>(result.status_code);
}

bool DataBaseConnection::compactDB(const std::string &name){
	const auto path = name + "/_compact"s;
	const auto result = this->data.network.get()->perform(path, Method::POST, {});
	return result.status_code == 202;
}

bool DataBaseConnection::purgeDoc(const std::string &name, json list){
	auto path =  name + "/_purge"s;
	auto body = cpr::Body(list.as_string());
	auto result = this->data.network->perform(path, Method::POST, body);
	auto status = result.status_code;
	if (status == 201 || status == 202){
		return true;
	}
	else {
		return false;
	}
}

long DataBaseConnection::maximalNumberOfRevisions(const std::string &name){
	long count = 0;
	const auto path = name + "_revs_limit"s;
	auto result = this->data.network.get()->perform(path, Method::GET, {});
	count = std::stol(result.text);
	return count;
}

bool DataBaseConnection::setMaximalNumberOfRevisions(const std::string &name, const long count){
	const auto path = name + "_revs_limit"s;
	const auto countString = std::to_string(count);
	const auto body = cpr::Body(countString);
	auto result = this->data.network.get()->perform(path, Method::PUT, body);
	return result.status_code == 200;
}
