#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <iostream>
# include <vector>
# include <map>

namespace	ft
{
	class	LocationBlock
	{
		public:
			enum	MatchType
			{
				PREFIX,
				EXACT
			};

			typedef std::string						keyType;
			typedef std::vector<std::string>		valueType;
			typedef std::map<keyType, valueType>	mapType;

			LocationBlock(bool exact_match);

			void			set_directive(keyType key, valueType value);
			const valueType	&get_directive(keyType key) const;

			MatchType		get_match_type(void) const;

		private:
			mapType		_directives;
			MatchType	_match_type;
	};

	class	ServerConfig
	{
		public:
			typedef std::string								keyType;
			typedef std::vector<std::string>				normalValueType;
			typedef std::map<keyType, normalValueType>		normalMapType;
			typedef ft::LocationBlock						locationValueType;
			typedef std::map<keyType, locationValueType>	locationMapType;

			ServerConfig();
			ServerConfig	&operator = (const ServerConfig &server_config_var);

			void					set_normal_directive(keyType key, normalValueType value);
			const normalValueType	&get_normal_directive(keyType key) const;

			void					set_location_directive(keyType key, locationValueType value);
			const locationValueType	&get_location_directive(keyType key);

		private:
			normalMapType	_normal_directives;
			locationMapType	_location_directives;
	};
}

#endif