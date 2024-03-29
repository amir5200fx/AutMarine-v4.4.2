#pragma once
#ifndef _Global_Done_Header
#define _Global_Done_Header

#include <Standard_TypeDef.hxx>

namespace AutLib
{

	class Global_Done
	{

		/*Private Data*/

		Standard_Boolean IsDone_;

	protected:

		Standard_Boolean& Change_IsDone()
		{
			return IsDone_;
		}

	public:

		Global_Done()
			: IsDone_(Standard_False)
		{}

		Standard_Boolean IsDone() const
		{
			return IsDone_;
		}
	};
}

#endif // !_Global_Done_Header
