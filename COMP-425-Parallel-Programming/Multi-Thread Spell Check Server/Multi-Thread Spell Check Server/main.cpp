//
// main.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2016 Gabriel Foust (gfoust at harding dot edu)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <boost/asio.hpp>
#include "server.hpp"
#include "request.hpp"
#include "reply.hpp"
#include "spell.hpp"
#include "thread_safe_queue.hpp"


// Handle request by doing spell check on query string
// Render results as JSON
void spellcheck_request(const http::server::request& req, http::server::reply& rep) {
	// Set up reply
	rep.status = http::server::reply::status_type::ok;
	rep.headers["Content-Type"] = "application/json";

	// Loop over spellcheck results
	bool first = true;
	rep.content << "[";
	for (auto& candidate : spell::spellcheck(req.query)) {
		if (first) {
			first = false;
		}
		else {
			rep.content << ", ";
		}
		rep.content << "\n  { \"word\" : \"" << candidate.word << "\",  "
			<< "\"distance\" : " << candidate.distance << " }";
	}
	rep.content << "\n]";
}

// Called by server whenever a request is received
// Must fill in reply, then call done()
void handle_request(const http::server::request& req, http::server::reply& rep, http::server::done_callback done) {
	std::cout << req.method << ' ' << req.uri << std::endl;
	if (req.path == "/spell") {
		spellcheck_request(req, rep);
	}
	else {
		rep = http::server::reply::stock_reply(http::server::reply::not_found);
	}
	done();
}

// Initialize and run server
int main()
{
	try
	{
		std::cout << "Listening on port 8000..." << std::endl;
		http::server::server s("localhost", "8000", handle_request);
		s.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "exception: " << e.what() << "\n";
	}

	return 0;
}
