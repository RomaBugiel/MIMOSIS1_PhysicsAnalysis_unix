/*! @file data_structure.h
 *  @brief Containts a internal data structure about single run.
 */



#ifndef _DATASTRUCTURE_H_        
#define _DATASTRUCTURE_H_  

#include "globals.h"

// -----------------   Header --------------------------


/*! \brief Structure holding the compressed data of single run
 *  \details It is a internal data storage. 
 */
struct MIMOSIS1_Single_Run_Dataset
//struct MIMOSIS1_Integrated_Frame
{
	/*! \brief MIMOSIS1 integrated frame (integrated hit map over N frames)
	 * \details 2-dim map corresponding to (part) pixel matrix. Each pixel holds the number of hits (fires) in N (=nb_of_frames) frames.*/
	TH2D *h2_hit_map {nullptr};
	
	/*! \brief Number of aquired frames in run (N)*/
	int nb_of_frames {0};
	
	/*! \brief List of run analysis parameters
	 * \details Stores the information about pulse high, threshold voltage and back bias values for this particular dataset 
	 * */
	std::map<std::string, int> run_param;
};

#endif
