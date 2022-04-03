#include "UUID.h"


namespace Util
{
	void UUID::Init()
	{
		this->uuid = boost::uuids::random_generator()();
	}

	void UUID::Init(boost::uuids::uuid uuid)
	{
		this->uuid = uuid;
	}

	void UUID::Init(std::string uuid)
	{
		this->uuid = boost::lexical_cast<boost::uuids::uuid>(uuid);
	}

}