#include <algorithm>

#include "Document.hpp"

using namespace CCC;

Document::Document(const std::string &uuid, const std::string &db,  const std::string &revision, std::shared_ptr<DataBaseConnection> connection) {
	this->data.uuid = uuid;
	this->data.db = db;
	this->data.revision = revision;
	this->data.connection = connection;
}

Document::Document(const Arguments &arguments) {
	this->data = arguments;
}

std::string Document::uuid() {
	return this->data.uuid;
}

json Document::json_content() {
	const auto revision = this->data.revision;
	return this->get(GetType::Content, revision);
}

std::string Document::content() {
	return this->json_content().as_string();
}

void Document::setContent(json json) {
	const auto name = this->data.db;
	const auto revision = this->revisions().back();
	const auto uuid = this->data.uuid;
	this->data.connection.get()->updateDocument(name, uuid, revision, json);
}

std::vector<std::string> Document::revisions() {
	std::vector<std::string> result;
	const auto temp = this->get(GetType::Revisions);
	const auto current = temp["_rev"s];
	result.push_back(current.as_string());
	const auto revs = temp["_revisions"s]["ids"s];
	const auto count = temp["_revisions"s]["start"].as_integer<int>();
	if (count > 1) {
		for (const auto &current : revs.array_range()){
			const auto tmp = current.as_string();
			result.push_back(tmp);
		}
	}
	std::reverse(result.begin(), result.end());
	return result;
}

json Document::get(const Document::GetType type, const std::string &revision) {
	DBC::GetOptions options;
	if (type == GetType::Content) {
		if (!revision.empty()) {
			options.rev = revision;
		}
		else {
			options.latest = true;
		}
	}
	else {
		options.revs = true;
	}
	const auto name = this->data.db;
	const auto uuid = this->data.uuid;
	const auto tmp = this->data.connection.get()->dbDoc(name, uuid, options);
	this->data.revision = tmp["_rev"s].as_string();
	return tmp;;
}

std::string Document::revision() {
	return this->data.revision;
}

void Document::setRevision(const std::string &revision) {
	this->data.revision = revision;
}

bool Document::purge(std::vector<std::string> revisions){
	json temp;
	temp[this->data.uuid] = "[]"s;
	for (const auto &current : revisions){
		temp[this->data.uuid].emplace_back(current);
	}
	return this->data.connection.get()->purgeDoc(this->data.db, temp);
}
