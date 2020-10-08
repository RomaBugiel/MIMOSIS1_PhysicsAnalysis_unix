
/*! @file Matrix.cpp
 *  @brief Source file for Matrix class.
 */
#include "Matrix.h"

std::ostream &operator<<(std::ostream &os, const Matrix &matrix) 
{
	os << std::setw(30) << std::endl;
	os << std::setw(30) << " Matrix is:	" 			<< std::endl;
	os << std::setw(30) << "_nb_of_row:	" 			<< matrix._nb_of_row			<< std::endl;
	os << std::setw(30) << "_nb_of_column:	" 		<< matrix._nb_of_column			<< std::endl;
	os << std::setw(30) << "_nb_of_pixels:	" 		<< matrix._nb_of_pixels			<< std::endl;
	os << std::setw(30) << "_mean:	" 				<< matrix._mean					<< std::endl;
	os << std::setw(30) << "_noise:	" 				<< matrix._noise				<< std::endl;
	os << std::setw(30) << "_bad_pixels:	" 		<< matrix._bad_pixels			<< std::endl;
	os << std::setw(30) << "_actived_selected:	" 	<< matrix._actived_selected		<< std::endl;
						
	return os;                          
}

/*
 * init
 */
 
void Matrix::init(const char * config_file) 
{
	if(config.ReadFile(config_file, kEnvUser) < 0)
	{ 
		MSG(ERR, "[MAT] Config file not loaded --> exit." );
		exit(0);
	}
	else
	{             
		_nb_of_row		=	config.GetValue("_nb_of_row", 0);
		_nb_of_column	=	config.GetValue("_nb_of_column", 0);                    
	}
	
	_h2_masked_pixels	 = new TH2D("_h2_masked_pixels", 	"Bad pixels map", _nb_of_column, 0, _nb_of_column, _nb_of_row, 0, _nb_of_row);
	_h2_activated_pixels = new TH2D("_h2_activated_pixels", "Active selected pixels", _nb_of_column, 0, _nb_of_column, _nb_of_row, 0, _nb_of_row);

}

/*
 * list_mask_pixels
 */

void Matrix::list_mask_pixels()
{
	MSG(INFO, "[MAT] There is: " + std::to_string(_v_masked_pixels.size()) + " pixels marked as bad for this matrix.");
	std::cout << "List of masked pixels: " << std::endl;

	for(auto &i : _v_masked_pixels) 
	{	
		std::cout << i._pixel_address.first << "\t" << i._pixel_address.second << std::endl;
	}

}

/*
 * list_active_pixels
 */

void Matrix::list_active_pixels()
{	
	MSG(INFO, "[MAT] There is: " + std::to_string(_v_activated_pixels.size()) + " pixels selected to analysis");
	std::cout << "List of activated pixels: " << std::endl;

	for(auto &i : _v_activated_pixels) 
	{	
		std::cout << i._pixel_address.first << "\t" << i._pixel_address.second << std::endl;
	}

}
