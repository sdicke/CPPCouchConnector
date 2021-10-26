#pragma once

#include <string>

#include <cpr/cpr.h>

namespace CCC {
	using namespace std::literals::string_literals;
	class Network {
		public:
			struct Arguments {
				std::string url;
				std::string port;
				cpr::Cookies cookies;
			};
		private:
			Arguments data;

			void createSession(const std::string &user, const std::string &password);

		public:
			enum class Method {
				GET,
				POST,
				PUT,
				PATCH,
				DELETE,
				HEAD
			};

			Network(const std::string &target, const std::string &port, const std::string &user, const std::string &password);
			Network(const Arguments &arguments);
			cpr::Response perform(const std::string &path, Method method, const cpr::Body &body = {});

		private:
			std::string buildURL(const std::string &path) const noexcept;
	};
}
