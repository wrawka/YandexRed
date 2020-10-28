#include "stats.h"

#include <iostream>
#include <vector>

void Stats::AddMethod(string_view method) {
  if (method_stats.find(method) != method_stats.end()) {
  	method_stats.at(method)++;
	} else {
		method_stats.at("UNKNOWN")++;
	}
}

void Stats::AddUri(string_view uri) {
  if (uri_stats.find(uri) != uri_stats.end()) {
  	uri_stats.at(uri)++;
	} else {
		uri_stats.at("unknown")++;
	}
}

const map<string_view, int>& Stats::GetMethodStats() const {
  return method_stats;
}

const map<string_view, int>& Stats::GetUriStats() const {
    return uri_stats;
}

HttpRequest ParseRequest(string_view line) {
  HttpRequest return_value;
  for (string_view* out : {&return_value.method, &return_value.uri, &return_value.protocol}) {
    // remove leading whitespace if any
    line.remove_prefix(min(line.find_first_not_of(" "), line.size()));

    // evaluate next chunk size and write the data
    size_t c_count = min( line.find_first_of(" "), line.size() );
    *out = string_view(line.data(), c_count);

    // remove processed chunk from string_view
    line.remove_prefix(c_count);
  };
  return return_value;
}
