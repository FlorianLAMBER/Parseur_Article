#include <string>
#include <iostream>
#include "folder_info.hpp"


int main()
{
	folder_info* test = new folder_info("/Users/manu/Downloads/CORPUS_TRAIN");
	std::cout << "test : " << test->get_pdfFolder() << std::endl;
	test->update_pdfList();
	std::vector<std::string> pdf_vect = test->get_pdfList();
	for (auto it = pdf_vect.begin(); it != pdf_vect.end(); it++)    
	{
        if (it != pdf_vect.begin()) 
		{
            std::cout << std::endl;
        }
        std::cout << "PDF : " << *it;
    }
	return 0;
}
