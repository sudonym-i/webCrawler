#include <iostream>
#include <string>
#include <fstream>
#include <curl/curl.h>
#include <sstream>

//points that detemine what char to start and end reads at
//for example <a> , </a>
const int IND_SIZE = 32;
const char START[IND_SIZE] = "<h1>";
const char END[IND_SIZE] = "</h1>";

bool equal_str(char *&one, const char two[IND_SIZE]);

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output);

std::string *breakpoints(std::string &html_content, const char start[IND_SIZE], const char end[IND_SIZE]);

int main(int argc, char* argv[]) {

    std::ofstream outfile("output.csv");
    
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
        std::string *parsed = breakpoints(*html_content, START, END);
        
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Output the HTML content of the page
            std::cout << "\nParsing website html..." << std::endl;
            std::cout << "\nPreview: " << *parsed << std::endl;

            outfile << *parsed;
        }
        // Clean up LOCAL
        curl_easy_cleanup(curl);
        delete parsed;
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


std::string *breakpoints(std::string &html_content, const char start[IND_SIZE], const char end[IND_SIZE]){
    char *current = &html_content[0];
    std::string *inside = new std::string;
    while(*current != '\0'){

        if( equal_str(current, start) ){
            while( !(equal_str(current, end)) ){
                (*inside).push_back(*current);
                current++;
            }
            (*inside).push_back(',');
        }
    current++;
    }
    return inside;
}

bool equal_str(char *&one, const char two[IND_SIZE]){
    int i = 0;
    while(*one != '\0' && two[i] != '\0'){
        if(*one != two[i]){
            return false;
        }
        i++;
        one++;
    }
    return true;
}