/*! @file Pixel.h
 *  @brief Header file for BadPixelAnalyser class.
 */

#ifndef _PIXEL_H_      
#define _PIXEL_H_   

#include "globals.h"
#include "cout_msg.h"
#include "data_structure.h"


/*! \class 		Pixel
 *  \brief 		Creates pixel object
 *  \author    	Roma Bugiel
 *  \date      	September 2020
 *  \pre       	ROOT installation
 *  \warning   	Exeception handling not added
 */


class Pixel { 
    
	public:

		Pixel()	{}; //!< Default constructor
		
		/*! \brief Overloaded constructor 
		 * 	\details Initializes all pixel global variables
		 */
		Pixel(std::pair<int, int> pixel_address, bool is_masked, bool is_bad, bool is_noisy, bool is_border ) :
			_pixel_address(pixel_address), _is_masked(is_masked), _is_bad(is_bad), _is_noisy(is_noisy), _is_border(is_border)
			{}; 

		/*! \brief Overloaded constructor 
		 * 	\details Initializes only pixel address
		 */
		Pixel(std::pair<int, int> pixel_address ) :
			_pixel_address(pixel_address)
			{}; 
	
		~Pixel() {}; //!< Destructor
		
		friend std::ostream &operator<<(std::ostream &os, const Pixel &pixel);

		double _mean 		{0.0}; 	//!< Pixel noise
		double _noise 		{0.0}; 	//!< Pixel mean from ERF Function
		double _fake_rate	{0.0}; 	//!< Pixel fake rate
		
		
		std::pair<int, int> _pixel_address; //!< pair holding pixel address (first = column, second = row)

		const int &_column 	= _pixel_address.first; 	//!< alias made for possibility of calling also Pixel::_column, instead Pixel::_pixel_address.first
		const int &_row 	= _pixel_address.second; 	//!< alias made for possibility of calling also Pixel::_row, instead Pixel::_pixel_address.second
		const int &_x 		= _pixel_address.first;		//!< alias made for possibility of calling also Pixel::_x, instead Pixel::_pixel_address.first
		const int &_y 		= _pixel_address.second; 	//!< alias made for possibility of calling also Pixel::_y, instead Pixel::_pixel_address.second

		bool _is_masked	{false}; //!< true if pixel masked
		bool _is_bad 	{false}; //!< ture if bad pixel
		bool _is_noisy	{false}; //!< true if noisy pixel
		bool _is_border {false}; //!< true if on matrix border 
		
		private:
};	



#endif
