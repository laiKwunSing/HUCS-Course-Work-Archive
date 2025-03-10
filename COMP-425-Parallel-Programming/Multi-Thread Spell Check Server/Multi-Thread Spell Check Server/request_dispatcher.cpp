//
// request_dispatcher.cpp
// ~~~~~~~~~~~~~~~~~~~
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

#include "request_dispatcher.hpp"
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include "mime_types.hpp"
#include "reply.hpp"
#include "request.hpp"

namespace http {
	namespace server {

		request_dispatcher::request_dispatcher(request_handler handler)
			: request_handler_(std::move(handler))
		{
		}

		void request_dispatcher::handle_request(request& req, reply& rep, done_callback done)
		{
			// Decode url to path.
			if (!url_decode(req))
			{
				rep = reply::stock_reply(reply::bad_request);
				done();
				return;
			}

			// Request path must be absolute and not contain "..".
			if (req.path.empty() || req.path[0] != '/'
				|| req.path.find("..") != std::string::npos)
			{
				rep = reply::stock_reply(reply::bad_request);
				done();
				return;
			}

			request_handler_(req, rep, done);

			/*
			// If path ends in slash (i.e. is a directory) then add "index.html".
			if (request_path[request_path.size() - 1] == '/')
			{
				request_path += "index.html";
			}

			// Determine the file extension.
			std::size_t last_slash_pos = request_path.find_last_of("/");
			std::size_t last_dot_pos = request_path.find_last_of(".");
			std::string extension;
			if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
			{
				extension = request_path.substr(last_dot_pos + 1);
			}

			// Open the file to send back.
			std::string full_path = doc_root_ + request_path;
			std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
			if (!is)
			{
				rep = reply::stock_reply(reply::not_found);
				return;
			}

			// Fill out the reply to be sent to the client.
			rep.status = reply::ok;
			char buf[512];
			while (is.read(buf, sizeof(buf)).gcount() > 0)
				rep.content.append(buf, is.gcount());
			rep.headers.resize(2);
			rep.headers[1].name = "Content-Type";
			rep.headers[1].value = mime_types::extension_to_type(extension);
			rep.headers[0].name = "Content-Length";
			rep.headers[0].value = std::to_string(rep.content.size());
			*/
		}

		bool request_dispatcher::url_decode(request& req)
		{
			req.path.clear();
			req.query.clear();
			req.path.reserve(req.uri.size());
			req.query.reserve(req.uri.size());

			std::string *out = &req.path;
			for (std::size_t i = 0; i < req.uri.size(); ++i)
			{
				if (req.uri[i] == '%')
				{
					if (i + 3 <= req.uri.size())
					{
						int value = 0;
						std::istringstream is(req.uri.substr(i + 1, 2));
						if (is >> std::hex >> value)
						{
							*out += static_cast<char>(value);
							i += 2;
						}
						else
						{
							return false;
						}
					}
					else
					{
						return false;
					}
				}
				else if (req.uri[i] == '+')
				{
					*out += ' ';
				}
				else if (req.uri[i] == '?')
				{
					out = &req.query;
				}
				else
				{
					*out += req.uri[i];
				}
			}
			return true;
		}

	} // namespace server
} // namespace http