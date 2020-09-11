/*! @file RunControl.cpp
 *  @brief main() 
 * 	\todo Configurator class should be written with all parameters that are in config. MIMOSIS1_TreeReader and LabTest should implement Configurator object
 * and use the values stored by it. Not load param_1 ... each seperetly. 
 */


#include "globals.h"
#include "cout_msg.h"

#include "MIMOSIS1_TreeReader.h"
#include "LabTest.h"



int main() 
{
	
	MSG(INFO, "\n ------- MIMOSIS-1 ANALYSIS ------- \n " );
	std::unique_ptr<MIMOSIS1_TreeReader> 	tree_reader;
	std::unique_ptr<LabTest> 				test_vph;

	tree_reader 	= 		std::make_unique<MIMOSIS1_TreeReader>();
	tree_reader		->		init("config_file.cfg");
	std::cout 		<< 		*tree_reader << std::endl;
	tree_reader		->		load_intput_files();
	
	
	test_vph 		= 		std::make_unique<LabTest>( 
									tree_reader -> get_integrated_frames(), 
									tree_reader -> get_scan_values() );
	test_vph->_scaned_param = 		tree_reader -> get_scanned_parameter_name() ;
	test_vph		->		init("config_file.cfg");
	std::cout 		<< 		*test_vph << std::endl;
	test_vph		->		open_output_tree();
	test_vph		->		init_histo();
	test_vph		->		copy_integrated_frames_histos_to_output();
	test_vph		->		plot_S_curves();
	test_vph		->		close_output_tree();
	return 0;

}
