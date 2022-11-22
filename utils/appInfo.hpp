#include <iostream>
#include <string>

class appInfo
{
    private:
        static const std::string PROGNAME;
        //static const std::string FILE_NAME;
        static const std::string RELEASE;
        static const std::string AUTHOR;
        static const std::string COPYRIGHT;

    public:
        void static print_release();
        void static print_usage();
        void static print_help();
};