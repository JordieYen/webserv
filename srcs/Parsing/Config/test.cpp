#include "ErrorChecker.hpp"

#include <fstream>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <sstream>
#include <string>



namespace ft
{
	std::vector<std::string>	split(std::string	line)
	{
		std::vector<std::string>	ret;
		std::string					space_temp;
		std::string					tab_temp;
		std::istringstream			ss(line);

		while (std::getline(ss, space_temp, ' '))
		{
			std::istringstream	tab_ss(space_temp);
			while (std::getline(tab_ss, tab_temp, '\t'))
			{
				if (!tab_temp.empty())
					ret.push_back(tab_temp);
			}
		}
		return (ret);
	}
}

int	main(int argc, char** argv)
{
	int				fd;
	std::string		myText;

	if (argc == 2)
	{
		char buffer[10000];
		fd = open(argv[1], O_RDONLY);
		read(fd, buffer, 10000);
		myText = buffer;

		std::vector<std::string>	lines;

		std::string::size_type pos = 0;
		std::string::size_type prev = 0;
		while ((pos = myText.find('\n', prev)) != std::string::npos) {
			lines.push_back(myText.substr(prev, pos - prev));
			prev = pos + 1;
		}
		lines.push_back(myText.substr(prev));

		std::vector<std::vector<std::string> >	temp;

		for (std::vector<std::string>::iterator line = lines.begin(); line != lines.end(); line++)
		{
			temp.push_back(ft::split(*line));
		}

		ft::ErrorChecker	lol;

		try
		{
			lol.check_server(temp);
		}
		catch (ft::ErrorChecker::InvalidConfigException & e)
		{
			std::cout << e.what() << std::endl;
		}
		
	}
	else
		std::cout << "nothing ran..." << std::endl;
	return (0);
}