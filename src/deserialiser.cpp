#include <iostream>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// given command line arguments try to deduce 
std::string get_filepath(const int &argc, char **argv)
{
    if (argc >= 1)
    {
        std::string filename{argv[1]};
        return filename;        
    }
    else
        throw std::runtime_error("No input file provided.");
}

int main(int argc, char **argv)
{
    std::string input_filepath{get_filepath(argc, argv)};

    std::fstream input_file{input_filepath, std::ios_base::in};
    if (!input_file)
    {
        std::string error{"No input file found: "};
        error += input_filepath;
        throw std::runtime_error(error);
    }

    json input = json::parse(input_file);

    std::cout << "This json contains #fields: " << input.size() << std::endl;
    return 0;
}