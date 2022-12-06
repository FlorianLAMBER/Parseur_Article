#include <string>
#include <iostream>
#include "appInfo.hpp"



const std::string appInfo::PROGNAME=" Article_handler ";
//const std::string appInfo::FILE_NAME= __FILE__;
const std::string appInfo::RELEASE="Rev 0.1a | Last update 09 nov 2022";
const std::string appInfo::AUTHOR="Temkaieva Svitlana, Lambert Florian, Lohyer Jérémie, Maxime Jullien, Aubertin Emmanuel";

const std::string appInfo::COPYRIGHT="(c) 2022-2023 "+ AUTHOR + "\nLicence: https://github.com/Fliche/Parseur_Article/\n";

void appInfo::print_release() {
    std::cout << RELEASE << std::endl << COPYRIGHT << std::endl;
}


void appInfo::print_usage() 
{
        std::cout << std::endl << appInfo::PROGNAME << std::endl;
        std::cout << "Usage:" << appInfo::PROGNAME << "& [-i | --input] & [-x | --xml] & [-t | --txt] | [-h | --help] | [-v | --version] | [-o | --output]  "<< std::endl;
        std::cout << "\t\t-h  Help" << std::endl;
        std::cout << "\t\t-v  Version" << std::endl;
        std::cout << "\t\t-i  Input folder, in this folder you may have all your pdf" << std::endl;
        std::cout << "\t\t-o  Output folder (./output/ by default)" << std::endl;
        std::cout << "\t\t-t  Output will be in txt" << std::endl;
        std::cout << "\t\t-x  Output will be in xml)" << std::endl;
        std::cout << std::endl << "Example : ./" << appInfo::PROGNAME << " -i /PATH/TO/ALL/PDF -o /PATH/TO/OUTPUT" << std::endl;
        std::cout << std::endl << "DESCRIPTION :" << std::endl;
        std::cout << std::endl << "This app use the pdftotext technologies for transform a science based article" << std::endl;
        std::cout << std::endl << "into a more readable txt or xml file." << std::endl;
}

void appInfo::print_help() {
        appInfo::print_release();
        appInfo::print_usage();
        std::cout << std::endl;
        exit(0);    
}
