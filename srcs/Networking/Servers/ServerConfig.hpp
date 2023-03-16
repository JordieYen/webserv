#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <iostream>
# include <vector>
# include <map>

using std::string;
using std::vector;
using std::map;
using std::make_pair;

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

			typedef string					keyType;
			typedef vector<string>			valueType;
			typedef map<keyType, valueType>	mapType;

			LocationBlock(bool exact_match);

			void				set_directive(keyType key, valueType value);
			const valueType&	get_directive(keyType key) const;

			MatchType			get_match_type(void) const;

		private:
			mapType		_directives;
			MatchType	_match_type;
	};

	class	ServerConfig
	{
		public:
			typedef string							keyType;
			typedef vector<string>					normalValueType;
			typedef map<keyType, normalValueType>	normalMapType;
			typedef LocationBlock					locationValueType;
			typedef map<keyType, locationValueType>	locationMapType;

			ServerConfig();
			ServerConfig&	operator = (const ServerConfig &server_config_var);

			void									set_normal_directive(keyType key, normalValueType value);
			const normalValueType&					get_normal_directive(keyType key) const;

			void									set_location_directive(keyType key, locationValueType value);
			const LocationBlock::valueType&			get_location_directive(keyType key, keyType location_key) const;
			const locationMapType&					get_location_map(void) const;

		private:
			normalMapType	_normal_directives;
			locationMapType	_location_directives;
	};
}

#endif