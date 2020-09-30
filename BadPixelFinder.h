/*! @file BadPixelFinder.h
 *  @brief Header file for BadPixelFinder class.
 */

#ifndef _BPF_H_      
#define _BPF_H_   

#include "globals.h"
#include "cout_msg.h"
#include "data_structure.h"
#include "Matrix.h"
#include <fstream>


/*! \class 		BadPixelFinder
 *  \brief 		Mainly creates a pixel mask map
 *  \author    	Roma Bugiel
 *  \date      	September 2020
 *  \pre       	ROOT installation
 *  \warning   	Exeception handling not added
 *	\details	Calculates the fake rate for each pixel, creates the debug histograms for bad pixels and fill the global #h2_masked_pixel_matrix.
 * In principle there will be two ways (and the third being merged version of these two) of selecting masked pixels.
 * The first method is just read the list from input .txt file.
 * The second way is to find these pixels, when they fake rate exceedess #_fake_rate_con (number of responses per number of frames) that is setup by the user in config_file.cfg. For that one has to provide a proper input data file (with run parameters for which pixels should not fire).
 */


class BadPixelFinder { 
    
	public:

		BadPixelFinder()	{}; //!< Default constructor
		
		BadPixelFinder(Matrix *matrix) :
			_MIMOSIS1{matrix} {};
			
		~BadPixelFinder() {}; //!< Destructor
		
		friend std::ostream &operator<<(std::ostream &os, const BadPixelFinder &bpf);

		//Variables
		Matrix 	*_MIMOSIS1;
		TH1D*	_h_fake_rate;

		//Methods 
		/*! \brief Mainly initialized class members
		 * 	\details Calls init_histo() and load_masked_pixels_file() */
		void init(const char * config_file);
			
		/*! \brief Initializes histograms
		 * 	*/
		void init_histo();
		
		/*! \brief loads file with the list of pixel to be masked
		 * 	\details it is maximum value of (fired / number of frames) ratio for dry run (run with parameters where pixel should not fire), for which pixels is not set as a bad pixel and masked.
		 * */
		void load_masked_pixel_file();
		
		TH1D *get_fake_rate_histo(); //!< \return #_h_fake_rate 1-dim histogram
		
	private:
	
		int _nb_of_row;	
		int _nb_of_column;
		int _row_start;			
		int _row_end;			
		int _column_start;		
		int _column_end;	
		
		int _frames_in_run; //!< \brief Number of frames in run N  \details How many frames have been integrated in input integrated frame. If negative, the default value -1 read in.

		
		TEnv 		config;
		std::string _input_file_masked_pixel_list; //!< text file with the list of pixels to be masked

		
		/*! \brief Condition for fake rate fraction
		 * 	\details it is maximum value of (fired / number of frames) ratio for dry run (run with parameters where pixel should not fire), for which pixels is not set as a bad pixel and masked.
		 * */
		double _fake_rate_con;
		
};


#endif
