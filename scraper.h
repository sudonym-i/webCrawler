#ifndef SCRAPER_H
#define SCRAPER_H
#include <string>


const int IND_SIZE = 512;

std::string *scrape(int argc, const char url_char[IND_SIZE], char start[IND_SIZE], char end[IND_SIZE]);
#endif
