#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

class Dev_Config{
private:
	std::map<string, string>config;
	std::map<string, string>::iterator it;

	std::string FrmtString(std::string &s) const{
		size_t x = 0;
		std::string buffer;
		unsigned char ch;

		while (x < s.length()){
			ch = s[x];
			if (ch == '\\'){
				if (x + 1 < s.length()){
					switch (s[x + 1])
					{
					case 'b':
						buffer.push_back('\b');
						x++;
						break;
					case 'r':
						buffer.push_back('\r');
						x++;
						break;
					case 'v':
						buffer.push_back('\v');
						x++;
						break;
					case 'n':
						buffer.push_back('\n');
						x++;
						break;
					case 't':
						buffer.push_back('\t');
						x++;
						break;
					default:
						break;
					}
				}
			}
			else{
				buffer.push_back(ch);
			}
			x++;
		}
		return buffer;
	}

	// trim left side of string
	void ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
			return !std::isspace(ch);
		}));
	}

	// trim right end of string
	void rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
			return !std::isspace(ch);
		}).base(), s.end());
	}

	// trim both sides of a string
	void trim(std::string &s) {
		ltrim(s);
		rtrim(s);
	}

	bool KeyExsits(std::string key){
		//Check for a key in a map
		it = config.find(key);

		if (it != config.end()){
			//Key found
			return true;
		}
		else{
			//Not found
			return false;
		}
	}
	std::vector<string>GetListContents(std::string key){
		std::string k = key;
		std::string val;
		std::string buffer;
		size_t n;
		std::vector<string>parts;
		int x = 0;
		unsigned char ch;

		//Uppercase k
		std::transform(k.begin(), k.end(), k.begin(), ::toupper);
		//Check if key is found
		if (KeyExsits(k)){
			//Get the string value
			val = get_string(k, "");
			//Trim it
			trim(val);
			//Check length
			if (val.length() > 1 && val[0] == '{' || val[0] == '['){
				//Erase front and back of string by one
				val.erase(0, 1);
				val.erase(val.length() - 1, val.length() - 1);
				//Check if last line has seperator
				if (val[val.length()] != ','){
					//Push seperator on string
					val.push_back(',');
					//split the string into vector.
					while (x < val.length()){
						//Get char
						ch = val[x];
						//Check if we hit a seperator.
						if (ch == ','){
							//Trim the buffer
							trim(buffer);
							//Check length before we add it to the vector
							if (buffer.length() > 0){
								//Push string on vector
								parts.push_back(buffer);
							}
							//Clear buffer.
							buffer.clear();
						}

						if (ch != ','){
							buffer.push_back(ch);
						}
						//INC x
						x++;
					}
				}
			}
			//Clear up
			val.clear();
			buffer.clear();
		}
		return parts;
	}
public:
	std::string get_string(std::string key, std::string default = ""){
		std::string k = key;
		std::string val;
		//Uppercase k
		std::transform(k.begin(), k.end(), k.begin(), ::toupper);

		if (KeyExsits(k)){
			val = config.at(k);
			//Check string length
			if (val.length() > 0){
				//Look for escape string
				if (val[0] == '"'){
					//Remove quotes
					val.erase(0, 1);
					val.erase(val.length() - 1, val.length() - 1);
					//Format the string
					val = FrmtString(val);
					//Return string
					return val;
				}
				else{
					//Return normal string
					return val;
				}
			}
			else{
				return default;
			}
		}
		else{
			return default;
		}
	}

	int get_int(std::string key, int default = 0){
		std::string k = key;
		//Uppercase k
		std::transform(k.begin(), k.end(), k.begin(), ::toupper);

		if (KeyExsits(k)){
			return std::stoi(config.at(k));
		}
		else{
			return default;
		}
	}

	float get_float(std::string key, float default = 0.0){
		std::string k = key;
		//Uppercase k
		std::transform(k.begin(), k.end(), k.begin(), ::toupper);

		if (KeyExsits(k)){
			return std::stof(config.at(k));
		}
		else{
			return default;
		}
	}

	double get_double(std::string key, double default = 0.0){
		std::string k = key;
		//Uppercase k
		std::transform(k.begin(), k.end(), k.begin(), ::toupper);

		if (KeyExsits(k)){
			return std::stod(config.at(k));
		}
		else{
			return default;
		}
	}

	bool get_bool(std::string key, bool default = false){
		std::string k = key;
		std::string val;
		bool v = false;
		//Uppercase k
		std::transform(k.begin(), k.end(), k.begin(), ::toupper);

		if (KeyExsits(k)){
			val = get_string(k, "false");
			trim(val);
			//Convert key to uppercase
			std::transform(val.begin(), val.end(), val.begin(), ::toupper);
			//Check val string
			if (val == "TRUE" || val == "YES" || val == "ON" || val == "1"){
				//Return true
				return true;
			}
			else{
				//Return false
				return false;
			}
		}
		else{
			//Return defualt
			return default;
		}
	}

	std::string get_list_item(std::string root, std::string key, std::string default = ""){
		std::vector<string>Parts;
		std::string ret;
		std::string k;
		std::string sCheckKey;
		std::string sData;
		std::map<string, string>paris;
		size_t pos;

		sCheckKey = key;
		//Get list contents
		Parts = GetListContents(root);
		//Uppercase the check key
		std::transform(sCheckKey.begin(), sCheckKey.end(), sCheckKey.begin(), ::toupper);

		for each (string s in Parts)
		{
			//Trim the string
			trim(s);
			//Check for value seperator
			pos = s.find_first_of(":");
			//Check if the seperator was found.
			if (pos != string::npos){
				//Extract the key name
				k = s.substr(0, pos);
				//Extract the key value
				sData = s.substr(pos + 1);
				//Uppercase the key name
				std::transform(k.begin(), k.end(), k.begin(), ::toupper);
				//trim the key string
				trim(k);
				//trim the data
				trim(sData);
				//Set map data with value of key
				paris[k] = sData;
			}
		}

		//Find the key
		it = paris.find(sCheckKey);
		//Check if the key was found.
		if (it != paris.end()){
			//Key found
			return paris[sCheckKey];
		}
		else{
			//Not found
			return default;
		}

		//Clear up
		paris.clear();
		Parts.clear();
	}

	std::vector<string>get_list_keys(std::string key){
		std::vector<string>Parts;
		std::vector<string>ret_parts;
		std::string KeyName;
		size_t pos;

		//Get list contents
		Parts = GetListContents(key);
		
		for each (string s in Parts)
		{
			//Trim the string
			trim(s);
			//Check for value seperator
			pos = s.find_first_of(":");
			//Check if the seperator was found.
			if (pos != string::npos){
				//Extract the key name
				KeyName = s.substr(0, pos);
				//Uppercase the key name
				std::transform(KeyName.begin(), KeyName.end(), KeyName.begin(), ::toupper);
				//trim the key string
				trim(KeyName);
				//Add key name to return vector.
				ret_parts.push_back(KeyName);
			}
		}
		//Clear parts.
		Parts.clear();
		//Return vector.
		return ret_parts;
	}

	bool list_key_exsits(std::string root, std::string key){
		std::string s_key = key;
		bool _found = false;
		//Uppercase the input key
		std::transform(s_key.begin(), s_key.end(), s_key.begin(), ::toupper);
		//Get a list of the key names
		std::vector<string>items = get_list_keys(root);
		//Look for key name in vector.
		if (std::find(items.begin(), items.end(), s_key) != items.end()){
			_found = true;
		}
		//Clear vector
		items.clear();
		//Return
		return _found;
	}

	std::string get_array_item(std::string key, int index, std::string default){
		std::vector<string>items = GetListContents(key);
		std::string ret;
		int idx = index;
		int len = items.size();

		if (idx < 0){
			idx = 0;
		}
		if (idx > (len - 1)){
			idx = (len - 1);
		}
		//Get item from vector
		ret = items[idx];
		//Trim the string
		trim(ret);
		//Clear vector
		items.clear();

		if (ret.length() == 0){
			return default;
		}
		else{
			return ret;
		}
	}

	std::vector<string>get_array_values(std::string key){
		return GetListContents(key);
	}

	int get_array_count(std::string key){
		return GetListContents(key).size();
	}

	std::vector<string>get_keys(){
		std::vector<string>tmp;
		//Return collection of keys
		for (it = config.begin(); it != config.end(); it++){
			tmp.push_back(it->first);
		}
		return tmp;
	}

	bool key_exsits(std::string key){
		std::transform(key.begin(), key.end(), key.begin(), ::toupper);
		//Find the key
		return KeyExsits(key);
	}

	bool key_is_array(std::string key){
		std::string sData;
		bool is_array = false;
		int len;
		//Extract the keys data
		sData = get_string(key);
		//Trim string
		trim(sData);
		//Get length of data
		len = sData.length();
		//Check for good length
		if (len > 1){
			if (sData[0] == '[' && sData[len - 1] == ']'){
				is_array = true;
			}
		}
		//Clear up
		sData.clear();
		return is_array;
	}

	bool key_is_list(std::string key){
		std::string sData;
		bool is_list = false;
		int len;
		//Extract the keys data
		sData = get_string(key);
		//Trim string
		trim(sData);
		//Get length of data
		len = sData.length();
		//Check for good length
		if (len > 1){
			if (sData[0] == '{' && sData[len - 1] == '}'){
				is_list = true;
			}
		}
		//Clear up
		sData.clear();
		return is_list;
	}
	
	bool open(std::string Filename){
		//Open config file.
		std::string StrLine;
		std::string key;
		std::string data;
		int xPos;
		fstream fin;

		fin.open(Filename, std::ios::in);

		if (!fin.is_open()){
			return false;
		}

		while (std::getline(fin, StrLine)){
			//Trim the line
			trim(StrLine);
			//Skip empry lines
			if (StrLine.empty()){
				continue;
			}
			//Skip comments
			if (StrLine[0] == '#'){
				continue;
			}

			if (StrLine == "/*"){
				while (StrLine != "*/"){
					std::getline(fin, StrLine);
				}
				if (StrLine == "*/"){
					continue;
				}
			}

			xPos = StrLine.find_first_of(":= \t");
			if (xPos != std::string::npos){
				key = StrLine.substr(0, xPos);
				data = StrLine.substr(xPos+ 1);
				ltrim(data);
				trim(key);
				std::transform(key.begin(), key.end(), key.begin(), ::toupper);
				//Set the maps data
				config[key] = data;
			}
		}
		//Close file.
		fin.close();

		return true;
	}

	~Dev_Config(){
		config.clear();
	}

};