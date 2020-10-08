#include "MIMOSIS1_TreeReader.h"

std::ostream &operator<<(std::ostream &os, const MIMOSIS1_TreeReader &treereader) 
{
	os << std::setw(30) << std::endl;
	os << std::setw(30) << " TreeReader is run with: " 		<< std::endl;
	os << std::setw(30) << "_param_1 : " 					<< treereader._param_1 			<< std::endl;
	os << std::setw(30) << "_param_2 : " 					<< treereader._param_2 			<< std::endl;
	os << std::setw(30) << "_param_3 : " 					<< treereader._param_3 			<< std::endl;
	os << std::setw(30) << "_param_1_value : " 				<< treereader._param_1_value 	<< std::endl;
	os << std::setw(30) << "_param_2_value : " 				<< treereader._param_2_value 	<< std::endl;
	os << std::setw(30) << "_param_3_value : " 				<< treereader._param_3_value 	<< std::endl;
	os << std::setw(30) << "_enable_scan : " 				<< treereader._enable_scan 		<< std::endl;
	os << std::setw(30) << "_frames_in_run : " 				<< treereader._frames_in_run 		<< std::endl;
	os << std::setw(30) << "_val_min : " 					<< treereader._val_min 			<< std::endl;
	os << std::setw(30) << "_val_max : " 					<< treereader._val_max 			<< std::endl;
	os << std::setw(30) << "_step : " 						<< treereader._step 			<< std::endl;
	os << std::setw(30) << "_incl : " 						<< treereader._incl 			<< std::endl;
	os << std::setw(30) << "_excl : " 						<< treereader._excl 			<< std::endl;
	os << std::setw(30) << "_input_tree_file_path : " 		<< treereader._input_tree_file_path 		<< std::endl;
	os << std::setw(30) << "_input_tree_file_name_core : " 	<< treereader._input_tree_file_name_core 	<< std::endl;
	os << std::setw(30) << "_input_tree_name : " 			<< treereader._input_tree_name 				<< std::endl;
	os << std::setw(30) << "_input_prefix : " 				<< treereader._input_prefix	 				<< std::endl;

										
	return os;                          
}

/*
 * Takes parameter min, max and step values and also inclusion list to prepare the list of parameter values used futher in the analysis
 */
 
int MIMOSIS1_TreeReader::extract_scan_values_list()
{	
	std::string param;
	std::vector<int> exclude_list;
	std::vector<int> include_list;
	std::vector<int>::iterator it;
	int f;
	
	switch(_enable_scan) 
	{
		case 1: { param = "Back bias"; 			break;}
		case 2: { param = "Pulse height"; 		break;}
		case 3: { param = "Threshold voltage"; 	break;}
		default:{ param = "WRONG PARAM"; MSG(WARN, "No scan enable!" ); break;}
	}
	
	
	for(double p = _val_min; p <= _val_max ; p += _step)
	{	
		v_param_values.push_back(p);
	}

	//Include files 
	std::stringstream si (_incl); 
	while ((si >> f)) include_list.push_back(f);
  
  	for(int i=0; i < (int)(include_list.size()); i++) 
	{	
		v_param_values.push_back(include_list[i]);
	}
  
 	//Exclude files 
	std::stringstream se (_excl); 
	while ((se >> f)) exclude_list.push_back(f);
	for(int i=0; i < (int)(exclude_list.size()); i++) 
	{	
		it =  find ( v_param_values.begin(), v_param_values.end(), exclude_list[i] );
		if ( it != v_param_values.end() )
		{
			v_param_values.erase(it);
		}
	}

	_val_min = *std::min_element(v_param_values.begin(), v_param_values.end());
	_val_max = *std::max_element(v_param_values.begin(), v_param_values.end());

	//for(auto &i : exclude_list) std::cout << "Exclude: " << i << std::endl;

	MSG(INFO, "\t" + param + " analysis scan is enabled from " + std::to_string(_val_min) + " to " + std::to_string(_val_max) + " in " +  TString::Itoa(v_param_values.size(),10)  + " steps."  );

	return 0;

}


/*
 * Initis variables and calls extracting parameter list
 */


void MIMOSIS1_TreeReader::init(const char * config_file)
{
	
	if(config.ReadFile(config_file, kEnvUser) < 0)
	{ 
		MSG(ERR, "Config file not loaded --> exit." );
		exit(0);
	}
	else
	{
	
		_param_1 		=	config.GetValue("_param_1", "");
		_param_2 		=   config.GetValue("_param_2", "");
		_param_3 		=   config.GetValue("_param_3", "");
		                    
		_param_1_value	=   config.GetValue("_param_1_value", 0);
		_param_2_value	=   config.GetValue("_param_2_value", 0);
		_param_3_value	=   config.GetValue("_param_3_value", 0);
	                       
		_enable_scan	=	config.GetValue("_enable_scan", 0);
		_frames_in_run	=	config.GetValue("_frames_in_run", -1);

		_val_min		=	config.GetValue("_val_min", 0);
		_val_max		=   config.GetValue("_val_max", 0);
		_step			=   config.GetValue("_step", 0);
		_incl			=   config.GetValue("_incl", "");		
		_excl			=   config.GetValue("_excl", "");			

		_input_tree_file_path		=	config.GetValue("_input_tree_file_path", "");		
		_input_tree_file_name_core 	=   config.GetValue("_input_tree_file_name_core", "");	
		_input_tree_name			=   config.GetValue("_input_tree_name", "");			
		_input_prefix				=   config.GetValue("_input_prefix", "");		
		
		_row_start				=	config.GetValue("_row_start", 0);
		_row_end				=	config.GetValue("_row_end", 0);
		_column_start			=	config.GetValue("_column_start", 0);
		_column_end				=	config.GetValue("_column_end", 0);
	}
	
	extract_scan_values_list();
}

/*
 * Load input TFiles
 */


void MIMOSIS1_TreeReader::load_intput_files()
{
	std::string inname 			{""};
	TFile 						*infile {nullptr};
	TTree 						*tree {nullptr};
	TH2D 						*h2_hit_map;	
	MIMOSIS1_Single_Run_Dataset	integrated_frame;	
	std::map<std::string, int> 	run_param;

	const int nb_of_bins_x  {(_column_end-_column_start)+1};
	const int nb_of_bins_y  {(_row_end-_row_start)+1};

	for(long unsigned int i = 0; i < v_param_values.size(); i++) 
	{	
		// Depending on which parameter scan was made, prepare a proper name and proper parameters info for data structure
		// to do: might be probably shorter
		switch( _enable_scan ) 
		{
			case 1: { _param_1_value = v_param_values[i]; break;	}
			case 2: { _param_2_value = v_param_values[i]; break;	}
			case 3: { _param_3_value = v_param_values[i]; break;	}
			default:{ break;}
		}
		
		
		inname = 	_param_1 + "_" + std::to_string(_param_1_value) + "_" + 
					_param_2 + "_" + std::to_string(_param_2_value) + "_" + 
					_param_3 + "_" + std::to_string(_param_3_value) +
					_input_prefix; 
				
		run_param.insert ( std::pair<std::string,int>(_param_1,_param_1_value) 	);
		run_param.insert ( std::pair<std::string,int>(_param_2,_param_2_value) 	);
		run_param.insert ( std::pair<std::string,int>(_param_3,_param_3_value) 	);
				
		//Open root TFile
		infile 	= new TFile( (TString)(_input_tree_file_path + "/" + _input_tree_file_name_core + "_" + inname + ".root"));
	
		if(infile->IsOpen() == false)
		{
			MSG(WARN, "[TR] Try to open: \n\t\t" << _input_tree_file_path + "/" + inname + ".root and procedure fail. If there was no parameters at all the program --> exit.");
			//exit(0);
		}
		else
		{	
			MSG(CNTR, "[TR] Succeeded to open file: \t" << _input_tree_file_path +"/" + inname + ".root");
			
			//If file is open, open tree
			tree 	= 	(TTree*) infile -> Get ( (TString) _input_tree_name );
			
			h2_integrated_frame_matrix 	= (TH2D*)	infile -> Get ( "h2_integrated_frame_matrix" );
			h2_integrated_frame_part	= (TH2D*)	infile -> Get ( "h2_integrated_frame_part" );
			h_fired_pixels_int_frame	= (TH1D*)	infile -> Get ( "h_fired_pixels_int_frame" );
			
			integrated_frame.h2_hit_map 	= h2_integrated_frame_matrix; // !!! part or full here?
			integrated_frame.nb_of_frames 	= _frames_in_run;
			integrated_frame.run_param 		= run_param;

			//Fill the vector
			v_MIM_int_frame.push_back(integrated_frame);
		}
			
		h2_hit_map 			= nullptr;
		infile 				= nullptr;
		tree 				= nullptr;
		run_param.clear();

	}	
	
	MSG(INFO, "[TR] MIMOSIS1_TreeReader built vector from: " + std::to_string(v_MIM_int_frame.size()) + " runs. (" + std::to_string(v_MIM_int_frame.size()) + " integrated frames loaded)  \n");

	delete h2_hit_map;
	delete infile;
	delete tree;

}

/*
 * Return vector of MIMOSIS0_Int_Frame structures
 */

std::vector<MIMOSIS1_Single_Run_Dataset> MIMOSIS1_TreeReader::get_integrated_frames()
{
	return v_MIM_int_frame;
}

/*
 * Return vector of values used in parameters scan
 */

std::vector<int> MIMOSIS1_TreeReader::get_scan_values()
{
	return v_param_values;
}


/*
 * Return vector of values used in parameters scan
 */
std::string MIMOSIS1_TreeReader::get_scanned_parameter_name()
{
	std::string _param {"WRONG PARAM NAME"};
	
	switch( _enable_scan ) 
		{
			case 1: { _param = _param_1; break;	}
			case 2: { _param = _param_2; break;	}
			case 3: { _param = _param_3; break;	}
			default:{ break;}
		}
		
	MSG(INFO, "[TR] Analysis is enabled for: " + _param + " scan.");
	return _param;
}
