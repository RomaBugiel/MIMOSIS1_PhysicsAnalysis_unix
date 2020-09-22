/*! @file data_structure.h
 *  @brief Containts a internal data structure about single run.
 */



#ifndef _DATASTRUCTURE_H_        
#define _DATASTRUCTURE_H_  

#include "globals.h"

// -----------------   Header --------------------------


/*! \brief Structure holding the compressed data of single run
 *  \details It is a internal data storage passed between TreeReader and LabTest
 */
struct MIMOSIS1_Integrated_Frame
{
	/*! \brief main data storage - integrated hit map.
	 * \details 2-dim map corresponding to (part) pixel matrix. Each pixel holds the number of hits (fires) in N (=nb_of_frames) frames.*/
	TH2D *h2_hit_map {nullptr};
	
	/*! \brief Number of aquired frames in run*/
	int nb_of_frames {0};
	
	/*! \brief map containig masked pixels (if bin value = 1)
	 * \details The masked pixels are found by #PixelMask class. See details there*/
	TH2D *h2_masked_pixels {nullptr};
	
	/*! \brief List of run analysis parameters
	 * \details Stores the information about pulse high, threshold voltage and back bias values for this particular dataset 
	 * 
	 * 
	 * 
	 * */
	std::map<std::string, int> run_param;
};

#endif
