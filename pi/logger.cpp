#include <ctime>
#include <string.h>
#include <sys/time.h>

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
    char buffer[80];
    timeval curTime;
    
    gettimeofday(&curTime, NULL);
    int milli = curTime.tv_usec / 1000;
    
    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);

    sprintf(buffer, "%s:%03d", buffer, milli);

    outfile.write(buffer, strlen(buffer));
    outfile<<", ";
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
    char buffer[80];
    timeval curTime;
    
    gettimeofday(&curTime, NULL);
    int milli = curTime.tv_usec / 1000;

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);

    sprintf(buffer, "%s:%03d", buffer, milli);

    outfile.write(buffer, strlen(buffer));
    outfile<<", ";
    outfile<<text;
    outfile<<std::endl;

    return 0;

}

