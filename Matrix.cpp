
/*! @file Matrix.cpp
 *  @brief Source file for Matrix class.
 */
#include "Matrix.h"

std::ostream &operator<<(std::ostream &os, const Matrix &matrix) 
{
	os << std::setw(30) << " Matrix is: " 			<< std::endl;
	os << std::setw(30) << "_nb_of_row: 	" << matrix._nb_of_row		<< std::endl;
	os << std::setw(30) << "_nb_of_column: 	" << matrix._nb_of_column	<< std::endl;
	os << std::setw(30) << "_nb_of_pixels:	" << matrix._nb_of_pixels	<< std::endl;
	os << std::setw(30) << "_mean:			" << matrix._mean			<< std::endl;
	os << std::setw(30) << "_noise:			" << matrix._noise			<< std::endl;
	os << std::setw(30) << "_bad_pixels: 	" << matrix._bad_pixels		<< std::endl;
	
	/*os << std::setw(30) << "				" << matrix.	<< std::endl;
	os << std::setw(30) << "" << matrix.	<< std::endl;
	os << std::setw(30) << "" << matrix.	<< std::endl;
	os << std::setw(30) << "" << matrix.	<< std::endl;
	os << std::setw(30) << "" << matrix.	<< std::endl;
	os << std::setw(30) << "" << matrix.	<< std::endl;
	os << std::setw(30) << "" << matrix.	<< std::endl;
	os << std::setw(30) << "" << matrix.	<< std::endl;*/
	
										
	return os;                          
}


void Matrix::init(const char * config_file) 
{
	std::cout << "Calling matrix init" << std::endl;
	
	if(config.ReadFile(config_file, kEnvUser) < 0)
	{ 
		MSG(ERR, "Config file not loaded --> exit." );
		exit(0);
	}
	else
	{             
		_nb_of_row		=	config.GetValue("_nb_of_row", 0);
		_nb_of_column	=	config.GetValue("_nb_of_column", 0);
		_mean			=	config.GetValue("_mean", 0.);
		_noise			=	config.GetValue("_noise", 0.);
		_bad_pixels		=	config.GetValue("_bad_pixels", 0);                       
	}
	
	_h2_masked_pixels = new TH2D("_h2_masked_pixels", "Bad pixels map", _nb_of_column, 0, _nb_of_column, _nb_of_row, 0, _nb_of_row);
	
}


void Matrix::list_mask_pixels()
{
	std::cout << "List of masked pixels: " << std::endl;
	std::cout << "\t There is: " << _v_masked_pixels.size() << " pixels marked as bad for this matrix." << std::endl;

	for(auto &i : _v_masked_pixels) 
	{	
		std::cout << i._pixel_address.first << "\t" << i._pixel_address.second << std::endl;
	}

}
