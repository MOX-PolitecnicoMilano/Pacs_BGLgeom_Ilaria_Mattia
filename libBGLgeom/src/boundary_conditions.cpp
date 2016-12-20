#include "boundary_conditions.hpp"
#include <string>
#include <iostream>
#include <iomanip>

using namespace BGLgeom;

namespace BGLgeom{

std::istream & operator>>(std::istream & in, boundary_condition & BC){
	std::string type;
	in >> type;
	if(type == "NONE"){	// we suppose that there is a zero after the type. However, we check
		BC.type = BC_type::NONE;		
		in >> BC.value;
		if(BC.value != .0)
			BC.value = .0;
		return in;
	} else if(type == "INT"){
			BC.type = BC_type::INT;
			in >> BC.value;
			if(BC.value != .0)
				BC.value = .0;
		return in;
	} else if(type == "DIR"){
			BC.type = BC_type::DIR;
			in >> BC.value;
		return in;
	} else if(type == "NEU"){
			BC.type = BC_type::NEU;
			in >> BC.value;
		return in;
	} else if(type == "MIX"){
			BC.type = BC_type::MIX;
			in >> BC.value;
		return in;
	} else if(type == "OTHER"){
			BC.type = BC_type::OTHER;
			in >> BC.value;
		return in;
	}
	// Should not reach this
	std::cerr << "boundary_conditions: Something wrong in input operation." << std::endl;
	return in;
}	//operator>>


std::ostream & operator<<(std::ostream & out, boundary_condition const& BC){
	if(BC.type == BC_type::NONE){
		out << "BC NONE";	//se faccio << BC.type stampa solo il numero. devo fare controlli per stampare string
		return out;
	} else if(BC.type == BC_type::INT){
		out << "BC INT";
		return out;
	} else if(BC.type == BC_type::DIR){
		out << "BC DIR " << std::fixed << std::setprecision(8) << BC.value;
		return out;
	} else if(BC.type == BC_type::NEU){
		out << "BC NEU " << std::fixed << std::setprecision(8) << BC.value;
		return out;
	} else if(BC.type == BC_type::MIX){
		out << "BC MIX " << std::fixed << std::setprecision(8) << BC.value;
		return out;
	} else if(BC.type == BC_type::OTHER){
		out << "BC OTHER " << std::fixed << std::setprecision(8) << BC.value;
		return out;
	}
	// Should not reach this
	std::cerr << "boundary_conditions: something wrong in output operation." << std::endl;
	return out;
}	//operator<<

}	//BGlgeom