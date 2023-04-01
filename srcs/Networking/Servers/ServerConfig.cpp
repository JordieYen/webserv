#include "ServerConfig.hpp"

namespace	ft
{
	LocationBlock::LocationBlock(bool exact_match)
	{
		if (exact_match)
			this->_match_type = EXACT;
		else
			this->_match_type = PREFIX;
	}

	void	LocationBlock::set_directive(keyType key, valueType value)
	{
		this->_directives.insert(make_pair(key, value));
	}

	const LocationBlock::valueType&	LocationBlock::get_directive(keyType key) const
	{
		return (this->_directives.at(key));
	}

	LocationBlock::MatchType	LocationBlock::get_match_type(void) const
	{
		return (this->_match_type);
	}

	ServerConfig::ServerConfig() {}

	ServerConfig::ServerConfig(const ServerConfig& server_config_var)
	{
		this->_normal_directives = server_config_var._normal_directives;
		this->_location_directives = server_config_var._location_directives;
	}

	void	ServerConfig::set_normal_directive(keyType key, normalValueType value)
	{
		this->_normal_directives.insert(make_pair(key, value));
	}

	const ServerConfig::normalValueType&	ServerConfig::get_normal_directive(keyType key) const
	{
		return (this->_normal_directives.at(key));
	}

	void	ServerConfig::set_location_directive(keyType key, locationValueType value)
	{
		this->_location_directives.insert(make_pair(key, value));
	}

	const LocationBlock::valueType&	ServerConfig::get_location_directive(keyType key, keyType location_key) const
	{
		return (this->_location_directives.at(key).get_directive(location_key));
	}

	const ServerConfig::locationMapType&	ServerConfig::get_location_map(void) const
	{
		return (this->_location_directives);
	}
}
