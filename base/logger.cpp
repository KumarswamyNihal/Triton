#include <ctime>

#include "logger.h"

logger::logger(std::string file_name)
{
    outfile.open(file_name, std::ios_base::app);
}

int logger::log(char *text, int size)
{
    if(!outfile)
        return -1;
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[20];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
    outfile.write(buffer, 19);
    outfile<<",";
    outfile.write(text, size);
    outfile<<std::endl;

    return 0;

}

int logger::log(std::string text)
{
    if(!outfile)
        return -1;
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[20];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
    outfile.write(buffer, 19);
    outfile<<",";
    outfile<<text;
    outfile<<std::endl;

    return 0;

}

