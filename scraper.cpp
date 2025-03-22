#include <iostream>
#include <string>
#include <fstream>
#include <curl/curl.h>
#include <sstream>

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output);

int main(int argc, char* argv[]) {

    std::ofstream outfile("output.txt");
    
    // Initialize curl globally
    CURL *curl;
    CURLcode res;
    std::string *html_content = new std::string;

    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if(curl) {
        // Set the URL to fetch
        std::string url = argv[1];
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set the callback function to write the response data into a string
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, html_content);

        // Perform the request
        res = curl_easy_perform(curl);
        // Check if the request was successful
        
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Output the HTML content of the page
            outfile << *html_content << std::endl;
        }
        // Clean up LOCAL
        curl_easy_cleanup(curl);

    }
    // Clean up curl globally
    delete html_content;
    curl_global_cleanup();
    outfile.close();
    return 0;
}

// Callback function to write the response data
size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

