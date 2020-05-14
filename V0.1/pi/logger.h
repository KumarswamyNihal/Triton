/*
 *
 * TODO
 * Write log() with timestamps
 */
#include <string>
#include <iostream>
#include <fstream>

#pragma once

class logger
{
    public:
    logger(std::string file_name);
    int log(char *text, int size);
    int log(std::string);

    private:
    std::ofstream outfile;

};