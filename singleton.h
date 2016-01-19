#pragma once

namespace SCore {

	// Meyer's singleton
	template<typename T> class Singleton
	{
	public:
		static T& Instance()
		{
			static T singleInstance;
			return singleInstance;
		}
	};

}
