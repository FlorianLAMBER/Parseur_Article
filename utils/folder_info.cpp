#include "folder_info.hpp"
#include <iostream>  
#include <string>  
#include <vector>  
#include <sstream>  
#include <fstream>

//#define DEBUG


//using namespace std;

const std::string folder_info::DEFAULT_OUTPUT = "output/";

/**
 * @brief Construct a new folder info::folder info object
 * 
 * 
 * @param inPdfFolder String who is the path to the pdf's folder.
 */
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

/**
 * @brief Construct a new folder info::folder info object
 * 
 * @param inPdfFolder String who is the path to the pdf's folder.
 * @param inOutputFolder Name/path of the ouput folder
 */
folder_info::folder_info(std::string inPdfFolder, std::string inOutputFolder)
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



/**
 *  @brief Construct a new folder info::folder info object
 * 
 * 
 * @param inPdfFolder String who is the path to the pdf's folder.
 */
folder_info::folder_info(std::string inPdfFolder, std::string inOutputFolder, std::vector<std::string> vPdf)
{
        this->pdfList = vPdf;
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


/**
 *  @brief Construct a new folder info::folder info object
 * 
 * 
 * @param inPdfFolder String who is the path to the pdf's folder.
 */
folder_info::folder_info(std::string inPdfFolder, std::vector<std::string> vPdf)
{
        this->pdfList = vPdf;
	if(inPdfFolder.back() != '/')
	{
		this->pdfFolder = inPdfFolder+"/";
		this->outputFolder = inPdfFolder + DEFAULT_OUTPUT;
	} else {
		this->pdfFolder = inPdfFolder;
		this->outputFolder = inPdfFolder + DEFAULT_OUTPUT;
	}
}


/**
 * @brief Getter of the pdfFolder
 * 
 * @return std::string path to pdfFolder
 */
std::string folder_info::get_pdfFolder()
{
	return this->pdfFolder;
}

/**
 * @brief Getter of the output folder
 * 
 * @return std::string path to the output
 */
std::string folder_info::get_outputFolder()
{
	return this->outputFolder;
}
 
/**
 * @brief Update the list pdf list
 */
void folder_info::update_pdfList()
{

	std::string cmd = "ls \"" +  this->get_pdfFolder() + "\" | grep .pdf";
	std::string cmd_output = this->exec(cmd.c_str());

	// See getline(X,T, ' ')
	std::string line;
	std::stringstream stream_cmd_output(cmd_output);
	while(getline(stream_cmd_output, line)){
		this->pdfList.push_back(line);
	}
}

/**
 * @brief Getter of the pdfList
 * 
 * @return std::vector<std::string> with all pdf avaible in the folder
 */ 
std::vector<std::string> folder_info::get_pdfList()
{
	return this->pdfList;
}

/**
 * @brief Execute a commande
 * 
 * @param const char* Commande to execute 
 * @return std::string Output of the commande
 */
std::string folder_info::exec(const char* cmd)
{
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

