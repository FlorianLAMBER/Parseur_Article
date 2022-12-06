#include <string>
#include <vector>

class folder_info 
{
	private:
        static const std::string DEFAULT_OUTPUT;
        std::string pdfFolder;
        std::vector<std::string> pdfList;
        std::string outputFolder;
        
        std::string exec(const char* cmd);

    public:
        folder_info(std::string inPdfFolder);
        folder_info(std::string inPdfFolder, std::string inOutputFolder);
        folder_info(std::string inPdfFolder, std::vector<std::string> vPdf);
        folder_info(std::string inPdfFolder, std::string inOutputFolder, std::vector<std::string> vPdf);
        std::string get_pdfFolder();
        std::string get_outputFolder();
        void update_pdfList();
        std::vector<std::string> get_pdfList();
	
};
