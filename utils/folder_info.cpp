#include "folder_info.hpp"
#include <iostream>  
#include <string>  
#include <vector>  
#include <sstream>  
#include <fstream>

//#define DEBUG

std::string PROGNAME="AppName";
std::string FILE_NAME= __FILE__;
std::string RELEASE="Revision 0.1a | Last update 09 nov 2022";
std::string AUTHOR="Aubertin Emmanuel";
std::string COPYRIGHT="(c) 2022 "+ AUTHOR + " from https://athomisos.fr";

void print_release() {
    std::cout << RELEASE << COPYRIGHT << std::endl;
}

//using namespace std;

const std::string folder_info::DEFAULT_OUTPUT = "output/";


folder_info::folder_info(std::string inPdfFolder)
{
	if(inPdfFolder.back() != '/')
	{
		this->pdfFolder = inPdfFolder+"/";
		this->outputFolder = inPdfFolder + DEFAULT_OUTPUT;
	} else {
		this->pdfFolder = inPdfFolder;
		this->outputFolder = inPdfFolder + DEFAULT_OUTPUT;
	}
	//this->pdfList = new std::vector<std::string>;
}

folder_info::folder_info(std::string inPdfFolder,std::string inOutputFolder)
{
	if(inPdfFolder.back() != '/')
        {               
                this->pdfFolder = inPdfFolder+"/";
        } else {        
                this->pdfFolder = inPdfFolder;
        }
	
	if(inOutputFolder.back() != '/')
        {
                this->outputFolder = inOutputFolder+"/";
        } else {        
                this->outputFolder = inOutputFolder;
        }
}

std::string folder_info::get_pdfFolder()
{
	return this->pdfFolder;
}

std::string folder_info::get_outputFolder()
{
	return this->outputFolder;
}
 
void folder_info::update_pdfList()
{
	std::string cmd = "ls " + this->get_pdfFolder() + " | grep .pdf";
	#ifdef DEBUG
		std::cout << "Enter inside update_pdfList() :" << std::endl;
	#endif
	std::string cmd_output = this->exec(cmd.c_str());
	#ifdef DEBUG
		std::cout << "cmd_output :" <<  cmd_output << std::endl;
	#endif

	// See getline(X,T, ' ')
	std::string line;
	std::stringstream stream_cmd_output(cmd_output);
	while(getline(stream_cmd_output, line)){
		#ifdef DEBUG
			std::cout << "Line : " << line << std::endl;
		#endif
		this->pdfList.push_back(line);
	}
}

std::vector<std::string> folder_info::get_pdfList()
{
	return this->pdfList;
}


std::string folder_info::exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
		std::cout << "ERROR in bufferisation" << std::endl;
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}
