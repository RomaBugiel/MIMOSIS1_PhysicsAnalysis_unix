/*! @file RunControl.cpp
 *  @brief main() 
 * 	\todo Configurator class should be written with all parameters that are in config. MIMOSIS1_TreeReader and LabTest should implement Configurator object
 * and use the values stored by it. Not load param_1 ... each seperetly. 
 */


#include "globals.h"
#include "cout_msg.h"

#include "Matrix.h"
#include "HistoPlotter.h"
#include "BadPixelFinder.h"
#include "MIMOSIS1_TreeReader.h"


int main() 
{
	
	const char *config_file {"config_file.cfg"};
	
	MSG(INFO, "\n ------- MIMOSIS-1 ANALYSIS ------- \n " );
		
	// Initialize matrix and Find Bad Pixels for this matrix
	Matrix 			*MIMOSIS1_matrix 	= new Matrix(1024,504);
	BadPixelFinder	*bpf 				= new BadPixelFinder(MIMOSIS1_matrix);
	
	bpf				->	init(config_file);
	std::cout 		<< 	*bpf 			  << std::endl;
	std::cout 		<<	*MIMOSIS1_matrix  << std::endl;	
	bpf				->	execute();

	// Initilializie input data reader and convert input data to internal data structure
	MIMOSIS1_TreeReader *tree_reader = new MIMOSIS1_TreeReader();
	
	tree_reader		->		init(config_file);
	tree_reader		->		load_intput_files();
	std::cout 		<< 		*tree_reader << std::endl;

	//Initializie HistoPlotter providing Matrix, input data and list of run parameters
	HistoPlotter	*test_vph = new HistoPlotter 
								( 
									MIMOSIS1_matrix,
									tree_reader -> get_integrated_frames(), 
									tree_reader -> get_scan_values() 
								);						
	test_vph ->	_scaned_param = tree_reader -> get_scanned_parameter_name() ;
	
	test_vph		->		init(config_file);
	std::cout 		<< 		*test_vph << std::endl;
	test_vph		->		open_output_tree();
	test_vph		->		init_histo();
	test_vph		->		copy_histos();
	test_vph		->		plot_S_curves();
	test_vph		->		save_png();
	test_vph		->		close_output_tree();
	
	return 0;

}
