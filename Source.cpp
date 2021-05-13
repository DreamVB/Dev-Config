#include <iostream>
#include "CfgParser.h"

using namespace std;

int main(int argc, char *argv[]){

	Dev_Config cfg;
	std::vector<string>keys;
	std::vector<string>softw;
	std::vector<string>s_nums;

	if (argc != 2){
		std::cerr << "Config file required" << std::endl;
		exit(EXIT_FAILURE);
	}
	//Open config file.
	if (!cfg.open(argv[1])){
		std::cerr << "Faild to load config file." << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << cfg.get_int("Number") << std::endl;
	std::cout << cfg.get_float("Float", 0.0) << std::endl;
	std::cout << cfg.get_double("double") << std::endl;
	std::cout << cfg.get_string("Name", "default string") << std::endl;
	std::cout << cfg.get_bool("Working", false) << std::endl;
	std::cout << cfg.get_string("FormatString") << std::endl;
	std::cout << cfg.get_string("string", "This is the dfault string") << std::endl;

	std::cout << "Software I use: " << cfg.get_list_item("software", "title", "Delphi") << std::endl;
	std::cout << std::endl << "Keys in software" << std::endl << std::endl;
	
	//Get key names of software key in cfg
	softw = cfg.get_list_keys("software");
	
	//List all keys in Software key
	for each (string key in softw)
	{
		std::cout << "Key: " << key << std::endl;
	}

	//Get a collection of all the keys in the cfg file
	keys = cfg.get_keys();
	//Set title
	std::cout << std::endl << "Keys in collection" << std::endl << std::endl;
	//List keys
	for each (string s in keys)
	{
		std::cout << "Key: " << s.c_str() << std::endl;
	}

	//Checck if numbers key is an array
	if (cfg.key_is_array("numbers")){

		//Extract a number from an array
		std::cout << "Number extracted at index 2 is " <<
			cfg.get_array_item("Numbers", 2, "101") << std::endl;

		//Get a collection of the numbers array.
		s_nums = cfg.get_array_values("numbers");

		std::cout << std::endl << "Numbers in collection calculated" << std::endl << std::endl;

		for each (string num in s_nums)
		{
			std::cout << "Number: " << stoi(num) * 2 << std::endl;
		}
	}
	else{
		std::cout << "Numbers array was not found." << std::endl;
	}

	keys.clear();
	s_nums.clear();
	softw.clear();

	return EXIT_SUCCESS;
}