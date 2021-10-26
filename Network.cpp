#include "Network.hpp"

using namespace CCC;

Network::Network(const std::string &target, const std::string &port, const std::string &user, const std::string &password) {
	this->data.url = target;
	this->data.port = port;
	this->createSession(user, password);
}

Network::Network(const Arguments &arguments) {
	this->data = arguments;
}

void Network::createSession(const std::string &user, const std::string &password){
	const auto payload = cpr::Payload{{"name"s, user}, {"password"s, password}};
	const auto urls = this->buildURL("_session");
	const auto url = cpr::Url(urls);
	const auto response = cpr::Post(url, payload);
	this->data.cookies = response.cookies;
}

cpr::Response Network::perform(const std::string &path, Method method, const cpr::Body &body) {
	cpr::Response response;
	const auto url_string = this->buildURL(path);
	cpr::Url url(url_string);
	const auto cookies = this->data.cookies;
	const cpr::Header header{{"Content-Type"s, "application/json"s}};
	switch (method) {
		case Network::Method::GET:
			response = cpr::Get(url, body, header, cookies);
			break;
		case Network::Method::POST:
			response = cpr::Post(url, body, header, cookies);
			break;
		case Network::Method::PUT:
			response = cpr::Put(url, body, header, cookies);
			break;
		case Network::Method::PATCH:
			response = cpr::Patch(url, body, header, cookies);
			break;
		case Network::Method::DELETE:
			response = cpr::Delete(url, body, header, cookies);
			break;
		case Network::Method::HEAD:
			response = cpr::Head(url, body, header, cookies);
			break;
	}
	return response;
}

std::string Network::buildURL(const std::string &path) const noexcept {
	return this->data.url + ":"s + this->data.port + "/"s + path;
}
