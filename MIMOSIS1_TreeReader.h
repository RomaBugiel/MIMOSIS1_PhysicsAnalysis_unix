/*! @file MIMOSIS1_TreeReader.h
 *  @brief Header file for MIMOSIS1_TreeReader class.
 */

#ifndef _TREEREADER_H_      
#define _TREEREADER_H_   


#include "globals.h"
#include "cout_msg.h"
#include "data_structure.h"

/*! \class 		MIMOSIS1_TreeReader
 *  \brief 		Opens one (or more) MIMOSIS 1 input data in ROOT format and saves the vector of MIMOSIS1_Single_Run_Dataset structures.
 *  \author    	Roma Bugiel
 *  \date      	August 2020
 *  \pre       	ROOT installation
 *  \warning   	Exeception handling not added
 *	\details	This class should be adpated for particular MIMOSIS1 raw input data format. In this case it is dedicated to read in data from MIMOSIS1_DataCoverter that return the .root tree with the several of histograms. 
 * 
 * This class is able to read multiple .root MIMOSIS1 input data [for different values of analysis parameter such as pulse high, back bias and thereshold voltage], and saves the vector of MIMOSIS1_Single_Run_Dataset (data structure) which can be further provided for LabTest class for S-Curves generation.
 * The MIMOSIS1_TreeReader extracts the values of the analysis parameters basing on data provided in config_file.cfg 
 */

class MIMOSIS1_TreeReader { 
    

public:

	MIMOSIS1_TreeReader()	{};	//!< Default constructor
	~MIMOSIS1_TreeReader()	{};	//!< Destructor

	/*!  Overloading stream insertion operator. 
	 *   Lists all class members variables with current values. */
	friend std::ostream &operator<<(std::ostream &os, const MIMOSIS1_TreeReader &treereader);

	//Methods
	/*!  \brief Initializes class variables taking the values from the config_file.cfg
	 *   \details Calls extract_scan_values_list method. */
	void 	init(const char * ="");
	
	/*! \brief Prepares vector of analysis scan values.
		\details Depeding on _enable_scan value recognizes the analysis parameter scan.
	 * 	 Basing on _val_min, _val_max and _step generates the vector of scaned parameter values (starting from _val_min, adds _step to the previous value until _val_max). May add also _incl value to the vector. 
	 * 	 In the next step the list of the parameters will be used for loading input files. */
	int		extract_scan_values_list();
	
	/*!  \brief Loads the .root input files basing on values stored in v_param_values. 
	 *   \details Creates input file names using values in v_param_values, load the data and saves them to the v_MIM_int_frame vector.
	 * Names of input files are created as follows:
	 * 
	 * _input_tree_file_path + / + _input_tree_file_name_core + / + _param_X + _ + _param_X_value.root
	 * 
	 * where X depends on _enable_scan (VPH, VTH, VPP) and _param_X_value takes the values stored in v_param_values. 
	 * 	 \return Number of scan value; size of the v_param_values*/
	void	load_intput_files();
	
	/*!  \return MIMOSIS1_Single_Run_Dataset structure*/
	MIMOSIS1_Single_Run_Dataset	get_data_structure();
	
	/*!  \return vector of analysis parameter values v_param_values*/
	std::vector<int> get_scan_values();
	
	/*!  \return  vector of MIMOSIS1_Single_Run_Dataset structues */
	std::vector<MIMOSIS1_Single_Run_Dataset> get_integrated_frames();

	/*!  \return  name of the parameter enabled for scan */
	std::string	get_scanned_parameter_name();

	//Variables
	std::string _param_1;	//!< Name of first analysis parameter (default VPH). Used for input/output name generation.
	std::string _param_2;	//!< Name of second analysis parameter (default VTH). Used for input/output name generation.
	std::string _param_3;	//!< Name of third analysis parameter on default VBB). Used for input/output name generation.
	
	int _param_1_value;	//!< Value of first analysis parameter (default VPH). Used for input/output name generation.
	int _param_2_value; //!< Value of second analysis parameter (default VTH). Used for input/output name generation.
	int _param_3_value; //!< Value of third analysis parameter on default VBB). Used for input/output name generation.
 
	int _enable_scan; 	//!< Determines which analysis parameters is scaned (VPH, VTH, VBB)
	int _frames_in_run; //!< \brief Number of frames in run N  \details How many frames have been integrated in input integrated frame. If negative, the default value -1 read in.
			
	int _val_min;	//!< Minimum analysis parameter value
	int _val_max;	//!< Maximum analysis parameter value
	int _step;		//!< Step of ramping the analysis parameter
	int _incl;		//!< If analysis was made for additional parameter value, it might be declared here.	
	
	int _row_start;		//!< The row address of first pixel from which analysis will start (Numbering from 0!)		
	int _row_end;		//!< The row address of last pixel on which analysis will end.			
	int _column_start;	//!< The column address of first pixel from which analysis will start (Numbering from 0!)		
	int _column_end;	//!< The column address of last pixel on which analysis will end.	

	std::string _input_tree_file_path;		//!< Path to the input files.
	/*! \brief Core of the input files names. 
	 *  \details To the core name the postfix with parameters name and values is added (as described for load_intput_files function)*/
	std::string _input_tree_file_name_core;	
	std::string _input_tree_name;			//!< Name of the input tree.
	
	std::vector< MIMOSIS1_Single_Run_Dataset > 	v_MIM_int_frame; //!< Vector of structures MIMOSIS1_Single_Run_Dataset.
	std::vector< int > 							v_param_values;	//!< Vector of analysis parameters (scan values).

	//Input file tree variables 
	unsigned short frames_in_run 	{0}; //!< Number of collected frames; how many frames were integrated

	//Input variable in ROOT Tree
	TH2D *h2_integrated_frame_matrix; 	//!< Integrated hit map of N frames plotted for entire matrix.
	TH2D *h2_integrated_frame_part;		//!< Integrated hit map of N frames plotted for part of matrix.
	TH1D *h_fired_pixels_int_frame;		//!< 1-dim histogram  of integrated hit map (Multiplicity of pixel hits in N (=frames_in_run) frames)

private:
	
	TEnv config;

};


#endif
