#pragma once
#ifndef _Global_Named_Header
#define _Global_Named_Header

#include <word.hxx>

namespace AutLib
{

	class Global_Named
	{

		/*Private Data*/

		word theName_;

	public:

		Global_Named()
		{}

		Global_Named(const word& theName)
			: theName_(theName)
		{}

		const word& Name() const
		{
			return theName_;
		}

		word& Name()
		{
			return theName_;
		}

		void SetName(const word& theName)
		{
			theName_ = theName;
		}
	};
}

#endif // !_Global_Named_Header
