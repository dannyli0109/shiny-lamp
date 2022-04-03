#pragma once

#include <uuid.hpp>            // uuid class
#include <uuid_generators.hpp> // generators
#include <uuid_io.hpp>         // streaming operators etc.
#include <boost/math/tools/lexical_cast.hpp>

namespace Util
{

	class UUID
	{
	public:
		void Init();
		void Init(boost::uuids::uuid uuid);
		void Init(std::string uuid);
		operator std::string() const { return boost::lexical_cast<std::string>(uuid); }
	private:
		boost::uuids::uuid uuid;
	};
}