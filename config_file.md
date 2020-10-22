# config_file.cfg
## Configuration parameter list

This file contains all the needed variables for running the software, such as files paths, scan parameters value, etc.<br/>
Changes in __does not need__ the program to be __recompiled__. <br/>
For the documentation the markdown format file is created, but in the DataConverter class the config_file.cfg is loaded. .md and .cfg are not created automatically. If some adds any parameter to .cfg a proper entry in .md should be done. <br/>

The convention is that the names of variables in .cfg are the same as used in classes/structures/etc. <br/>


| Name                | Value (example)        | Info                                   | Description                                                                                       |
|---------------------|------------------------|----------------------------------------|---------------------------------------------------------------------------------------------------|
| _input_file_masked_pixel    | masked_pixels.txt       | __std::string__               | Name of first scan parameter. Used for creating a input file(s) name. Corresponds to back bias.   |
| _active_selected            | 0            			| __std::string__               | Name of first scan parameter. Used for creating a input file(s) name. Corresponds to pulse high.  | 
| _input_file_active_selected | active_selected.txt     | __std::string__               | Name of first scan parameter. Used for creating a input file(s) name. Corresponds to threshold .  |
| _run		          | 0                      | __int__, arbitrary                     | Number of a run, used in input file(s) name and in output data file.   							|
| _frames_in_run 	  | 1000	               | __int__, arbitrary               		| Number of frames aquired for the runs. Should be the same for all files, but this is not verified. If not given = -1.  |
| _val_min      	  | 0                      | __int__, arbitrary                     | Minimum analysis parameter value (step). (Scaned parameter, enabled via _enable_scan).                   |
| _val_max     		  | 0                      | __int__, arbitrary                     | Minimum analysis parameter value (step). (Scaned parameter, enabled via _enable_scan).               	|
| _step	     		  | 0                      | __int__, arbitrary                     | Step of ramping the parameter value from _val_min to _val_max. If =0, only _val_min will be analysed.	|
| _incl				  |	10 15 23			   | __std::string__						| Here one can add additional steps of parameter scan to the analysis, not covered by val_min - val_max - step |
| _excl				  |	50 120				   | __std::string__						| Here one can exclude any step of scan from the analysis. Highier priority than include list. |
| _nb_of_rows         | 504                    | __int__, arbitrary, > 0                | Number of rows in matrix. Currently unused.                  	                                    |
| _nb_of_columns      | 1024                   | __int__, arbitrary, > 0                | Number of columns in matrix. Currently unused.                                                    |
| _nb_of_frames       | 1000                   | __int__, arbitraty, > 0                | Number of frames aquired                                                                        	|
| _row_start          | 0                      | __int__, >=0                           | Pixel in X (row) for which analysis starts. Pixels numbered from 0.      		                    |
| _row_end            | 503                    | __int__, >= 0 && <= (_nb_of_rows-1)    | Pixel in X (row) for which analysis ends.                               	                        |
| _column_start       | 0                      | __int__, >=0                           | Pixel in Y (column) for which analysis starts. Pixels numbered from 0.               				|
| _column_end         | 1023                   | __int__, >= 0 && <= (_nb_of_columns-1) | Pixel in Y (column) for which analysis ends.                                                 		|
| _param_1            | VBB                    | __std::string__                        | Name of first scan parameter. Used for creating a input file(s) name. Corresponds to back bias.   |
| _param_2            | VPH                    | __std::string__                        | Name of first scan parameter. Used for creating a input file(s) name. Corresponds to pulse high.  | 
| _param_3            | VTH                    | __std::string__                        | Name of first scan parameter. Used for creating a input file(s) name. Corresponds to threshold .  |
| _param_1_value      | 0                      | __int__                                | Value of the _param_1. Used for creating a input file(s) name.                                    |
| _param_2_value      | 0                      | __int__                                | Value of the _param_2. Used for creating a input file(s) name.                                    |
| _param_3_value      | 0                      | __int__                                | Value of the _param_3. Used for creating a input file(s) name.    								|
| _enable_scan		  | 2	                   | __int__, options: 1,2,3                | Enables which parameter was scanned (back bias - 1, pulse high - 2, threshold -3. Only this param value is changed in input file names. |
| _dac_shift		  | 90	                   | __int__, arbitrary               		| Shift on the x (step, analysis parameter value) axis. This value is added to each parameter value (pulse high, threshold, back bias).   |
| _input_tree_file_path         | (...)/MIMOSIS1_DataConverter_unix/outputData          | __string__,    | Path to the input files. Most probably it will be the output of MIMOSIS1_DataConverter |
| _input_tree_file_name_core    | mimosis1     			| __string__    | The core of the input name (verify with the _out_tree_file_name in MIMOSIS1_DataConverter config_file.cfg             |                     	                        
| _input_tree_name       		| mimosis1_raw_tree		| __string__    | The input tree name (verify with the _out_tree_name in MIMOSIS1_DataConverter config_file.cfg  						|	  		          
| _input_prefix       			| mimosis1_raw_tree		| __string__    | Postfix to the input files name. Might be left empty.																	|	  		          
| _output_tree_file_path		| ./outputData      	| __string__    | Path to the output files directory. 																					|
| _output_tree_file_core		| mimosis1_phys     	| __string__    | The core of the output name. The full name is  _output_tree_file_core+_param_x+param_x_value (x: 1,2,3)	            |                     	                        
| _output_tree_file_part		| VBB_0_VTH_0     		| __string__    | Tego sie pozbyc																							            |                     	                        
| _output_tree_name				| mimosis1_phys_tree	| __string__    | The output tree name 																			 						|	  		          
| _fit_norm_param_fix		| 1000	| __int__    	| Fitting procedure: fixed value of N param0 (normalization) of Err function. Should be equal to _frames_in_run and changed manually in config. 	|  																				|
| _fit_noise_param_set		| 10	| __double__    | Fitting procedure: Initial value of sigma in Err function 	           																			|                     	                        
| _fit_noise_min_lim		| 0		| __double__    | Fitting procedure: Minimum value of fitting range for sigma of Err.																			    |                     	                        
| _fit_noise_max_lim		| 50	| __double__    | Fitting procedure: Maximum value of fitting range for sigma of Err.																			 	|	  		          
| _min_accepted_noise		| 1		| __double__    | Fitting procedure: If noise (Erf sigma, fit parameter) is below that value, fit is flaged as  failed.												|
| _max_accepted_noise		| 100	| __double__    | Fitting procedure: If noise (Erf sigma, fit parameter) is above that value, fit is flaged as  failed.			        |                     	                        
| _max_refit				| 2		| __int__    	| Fitting procedure: If fit is flaged as failed, there is _max_refit number of refits.						            |                     	                        
| _fit_tolerance 			| 1e-3	| __double__    | Fitting procedure: SetDefaultTolerance value in ROOT::Math::MinimizerOptions											|	  		          
| _fit_max_it				| 1000	| __int__    	| Fitting procedure: SetDefaultMaxIterations value in ROOT::Math::MinimizerOptions										|
| _fit_fun_cal				| 1000	| __int__    	| Fitting procedure: SetDefaultMaxFunctionCalls value in ROOT::Math::MinimizerOptions	            					|                     	                        
| _saturation_lvl			| 0.5	| __double__    | Fitting procedure: If pixel did not achieve _saturation_lvl*_frames_in_run hits in any analysed run, it is ignored from analysis		            |                     	                        
