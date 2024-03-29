#pragma once
#ifndef _Dir2d_Header
#define _Dir2d_Header

#include <gp_Dir2d.hxx>
#include <Vec2d.hxx>

namespace AutLib
{

	class Dir2d
		: public gp_Dir2d
	{

		/*Private Data*/

	public:

		static const Dir2d null;

		Dir2d()
		{}

		Dir2d
		(
			const gp_XY& Coord
		)
			: gp_Dir2d(Coord)
		{}

		Dir2d
		(
			const Vec2d& V
		)
			: gp_Dir2d(V)
		{}

		Dir2d
		(
			const Standard_Real Xv, 
			const Standard_Real Yv
		)
			: gp_Dir2d(Xv, Yv)
		{}
	};
}

#endif // !_Dir2d_Header
