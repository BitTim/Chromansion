#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "variables.h"

struct Config
{
	int screen_res[2];
	bool fullscreen;		
};


void set_config(Config config)
{
	screen_size[0] = config.screen_res[0];
	screen_size[1] = config.screen_res[1];
	fullscreen = config.fullscreen;

	visible_tiles[0] = (float)screen_size[0] / (float)tile_size[0];
	visible_tiles[1] = (float)screen_size[1] / (float)tile_size[1];
}

std::vector<std::string> interpret_conf(Config config)
{
	std::vector<std::string> parsed_config;
	std::string line;
	
	line = "screenres_w: ";
	line.append(std::to_string(config.screen_res[0]));
	parsed_config.push_back(line);
	
	line = "screenres_h: ";
	line.append(std::to_string(config.screen_res[1]));
	parsed_config.push_back(line);

	line = "fullscreen: ";
	line.append(config.fullscreen ? "true" : "false");
	parsed_config.push_back(line);

	return parsed_config;
}

int write_config(const char* path, Config config)
{
	std::ofstream config_file;
	config_file.open(path, std::ofstream::trunc);

	for(std::string line : interpret_conf(config))
	{
		config_file << line;
	}

	config_file.close();
}

int interpret_line(std::vector<std::string> parsed_line, Config& config)
{
	if(parsed_line.size() > 2 || parsed_line.size() < 2) return -1;
	
	if(parsed_line[0] == "screenres_w") config.screen_res[0] = std::stoi(parsed_line[1]);
	if(parsed_line[0] == "screenres_h") config.screen_res[1] = std::stoi(parsed_line[1]);

	if(parsed_line[0] == "fullscreen")  config.fullscreen = (parsed_line[1] == "true");
	return 0;
}

int load_config(const char* path)
{
	Config config;

	std::ifstream config_file;
	config_file.open(path);

	std::string line;
	std::string word;
	std::vector<std::string> parsed_line;
	int num_lines = 0;

	while(config_file)
	{
		std::getline(config_file, line);

		if(!config_file) break;
		config_file >> std::ws;

		num_lines++;

		std::istringstream line_stream(line);	
		parsed_line.clear();
		
		while (std::getline(line_stream, word, ':'))
   		{
			word.erase(std::remove(word.begin(), word.end(),' '), word.end());
   			parsed_line.push_back(word);
		}

		if(interpret_line(parsed_line, config) == -1) return -1;
	}
	
	config_file.close();
	set_config(config);
	return 0;
}

#endif // CONFIG_MANAGER
