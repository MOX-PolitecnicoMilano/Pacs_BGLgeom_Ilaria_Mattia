/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/* 
   Copyright (C) 2016 Luca Formaggia

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*!
	@file	mesh_generator.hpp
	@author	Luca Formaggia
	@date	Jan, 2017
	@brief	Utilities to generate a 1D mesh, definitions
	
	@note	Code from the example of the PACS course
*/

#include <stdexcept>
#include <algorithm>
#include <boost/range/numeric.hpp>
#include "rk45.hpp"
#include "mesh_generators.hpp"


namespace BGLgeom {

	Mesh1D::Mesh1D(Domain1D const & d,  unsigned int const &n) : myDomain(d) {
		Uniform g(d,n);
		myNodes=g();
	}

	Mesh1D::Mesh1D(Domain1D const & d, unsigned int const &n,std::function<double (double const & x)> generatingFunction) :
		myDomain(d)	{
			VariableSize g(d,generatingFunction,n);
			myNodes=g();
	}

	double Mesh1D::h() const {
		std::vector<double> tmp(myNodes.size()-1);
		std::adjacent_difference(myNodes.begin(),myNodes.end(),tmp.begin());
		return *std::max_element(tmp.begin(),tmp.end());
	}

	void Mesh1D::reset(OneDMeshGenerator const & mg){
		myDomain=mg.getDomain();
		myNodes=mg();
	}

	MeshNodes Uniform::operator()() const {
		auto const n = this->M_num_elements;
		auto const a = this->M_domain.left();
		auto const b = this->M_domain.right();
		if (n==0) throw std::runtime_error("At least two elements");
		MeshNodes mesh(n+1);
		double h = (b-a)/static_cast<double>(n);
		for(std::size_t i=0; i<n; ++i) mesh[i] = a+h*static_cast<double>(i);
		mesh[n] = b;
		return mesh;
	}	//Uniform::operator()

	MeshNodes VariableSize::operator()() const {
		// calculate dy/dx = h^-1(x)
		MeshNodes mesh;
		auto const t0 = this->M_domain.left();
		auto const T  = this->M_domain.right();
		auto const y0 = 0;
		auto const h_max = (T-t0)/4.;
		auto const h_initial = h_max/100.;
		double constexpr final_error = 1e-2;
		int status;
		std::size_t maxSteps = 20000;
		auto solution = ODE::rk45( [this](double const & x,double const &){return 1./this->M_h(x);},
		   						   t0,T,y0,h_initial,h_max,final_error,status,maxSteps);
		auto lastValue = solution.back().second;
		// make it an integer
		std::size_t numElements = std::max(static_cast<std::size_t>(std::round(lastValue)), static_cast<std::size_t>(2));
		if(numElements > M_num_elements) throw std::runtime_error("VariableSize: too many elements");
		// rescale
		using pDouble = std::pair<double,double>;
		double scaling = numElements/lastValue;
		std::transform(solution.begin(), solution.end(), solution.begin(),
					   [scaling](pDouble const & x)
					   {
					     return std::make_pair(x.first,x.second*scaling);
					   }
					   );
		mesh.reserve(numElements+1); // I need to store the nodes
		mesh.push_back(M_domain.left()); // first node
		// Now the internal nodes
		auto pos = solution.cbegin()+1;
		for (std::size_t i=1; i<numElements; ++i){
			// find_if finds the first element satisfying the predicate
			auto found = std::find_if(pos,solution.cend(),
						  [i](pDouble const & value)
						  {
						    return value.second > i;
						  }
						  );
			if (found == solution.end()) throw std::runtime_error("variableSize: Something wrong: cannot find node!");
			pos        = found-1;
			auto xpos1 = pos->first;
			auto ypos1 = pos->second;
			auto xpos2 = found->first;
			auto ypos2 = found->second;
			// Linear interpolation
			// Division by zero should not happen. But.. just in case
			if((ypos2-ypos1) == 0) throw std::runtime_error("variableSize: something wring in the spacing function h");
			auto xpos = (xpos1*(ypos2-i) + xpos2*(i-ypos1)) / (ypos2-ypos1);
			mesh.push_back(xpos);
		}	//for
		// The last node
		mesh.push_back(M_domain.right());
		return mesh;
	}	//VariableSize::operator()

}	//BGLgeom
