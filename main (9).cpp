#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h> // requires libcurl to be installed
#include <nlohmann/json.hpp> // requires nlohmann/json to be included in the project

using json = nlohmann::json;
using namespace std;

// Helper function to convert a JSON object to a vector of assignments
vector<string> jsonToVector(const json& jsonObject) {
  vector<string> assignments;
  for (const auto& assignment : jsonObject) {
    assignments.push_back(assignment["name"].get<string>());
  }
  return assignments;
}

// Helper function to make an API request to Canvas using libcurl
json makeApiRequest(const string& url, const string& apiKey) {
  CURL* curlHandle = curl_easy_init();

  // Set the URL and request options for the API request
  curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
  struct curl_slist* headers = nullptr;
  string authHeader = "Authorization: Bearer " + apiKey;
  headers = curl_slist_append(headers, authHeader.c_str());
  curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, headers);

  // Set up a buffer to hold the response from the API
  string responseBuffer;
  curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION,
                   [](void* data, size_t size, size_t nmemb, void* userp) -> size_t {
                     string* response = static_cast<string*>(userp);
                     response->append(static_cast
