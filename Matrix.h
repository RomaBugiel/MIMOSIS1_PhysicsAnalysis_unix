/*! @file Matrix.h
 *  @brief Header file for BadPixelAnalyser class.
 */

#ifndef _MATRIX_H_      
#define _MATRIX_H_   

#include "globals.h"
#include "cout_msg.h"
#include "data_structure.h"
#include "Pixel.h"

/*! \class 		Matrix
 *  \brief 		Creates matrix object
 *  \author    	Roma Bugiel
 *  \date      	September 2020
 *  \pre       	ROOT installation
 *  \warning   	Built of #Pixel
 */


class Matrix { 
    
	public:

		Matrix()	
			{
				this->init("config_file.cfg");
			}; //!< Default constructor
		
		Matrix(int nb_of_row, int nb_of_column) :
			_nb_of_row(nb_of_row), _nb_of_column(nb_of_column)
			{
				this->init("config_file.cfg");
			}; //!< Overloaded constructor; initializes #_nb_of_row and #_nb_of_column
		
		Matrix(const Matrix &source) {
			_h2_masked_pixels = new TH2D;
			*_h2_masked_pixels = *source._h2_masked_pixels;		
		} //!< deep copy, when coping Matrix obj, also copy data of histos
	
		~Matrix() {}; //!< Destructor
		
		friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix);
		
		int	   _nb_of_row	{0}; 	//!< Number of rows in the matrix
		int	   _nb_of_column{0}; 	//!< Number of columns in the matrix
		int	   _nb_of_pixels{_nb_of_row*_nb_of_column}; 	//!< Number of all pixels in the matrix.

		double _mean 		{0.0}; 	//!< Pixel noise
		double _noise 		{0.0}; 	//!< Pixel mean from ERF Function
		int	   _bad_pixels	{0}; 	//!< Bad pixels are excluded from the analysis.	
		
		std::vector< std::vector <Pixel> > _v2_matrix;	//!< map of pixels
		
		TH2D*					_h2_masked_pixels;	//!< map of masked pixels
		std::vector< Pixel >	_v_masked_pixels;				//!< list of masked pixels

		//-----------------------
		
		void init(const char * config_file);
		void list_mask_pixels();
		
	private:
		
		TEnv config;

};


#endif
