//
// request.hpp
// ~~~~~~~~~~~
//
// Copyright (c) 2016 Gabriel Foust (gfoust at harding dot edu)
//
// Derived from sources found at
// http://www.boost.org/doc/libs/1_61_0/doc/html/boost_asio/examples/cpp11_examples.html
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>
#include <vector>
#include <map>

namespace http {
	namespace server {

		/// A request received from a client.
		struct request
		{
			std::string method;
			std::string uri;
			std::string path;
			std::string query;
			int http_version_major;
			int http_version_minor;
			std::map<std::string, std::string> headers;
		};

	} // namespace server
} // namespace http

#endif // HTTP_REQUEST_HPP