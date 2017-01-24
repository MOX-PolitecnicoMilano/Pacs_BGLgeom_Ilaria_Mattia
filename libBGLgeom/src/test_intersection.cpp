/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	test_intersection.cpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Little test to show how works the code that compute intersection
			provided by prof. Formaggia
	
	We compute an intersection between two linear edges. The coordinates
	of the extremes points for the two edges can be easily changed, to 
	see how the output changes
*/

#include "intersections2D.hpp"
#include "point.hpp"
#include "linear_geometry.hpp"
#include <iostream>

using namespace BGLgeom;

int main(){

	point<2> p1(0.0,0.0);
	point<2> p2(0.5,0.5);
	linear_geometry<2> edge1(p1,p2);

	point<2> p3(0.5,0.5);
	point<2> p4(1.0,1.0);
	linear_geometry<2> edge2(p3,p4);

	Intersection I = compute_intersection(edge1,edge2);
	std::cout << I << std::endl;
	
	return 0;
}
