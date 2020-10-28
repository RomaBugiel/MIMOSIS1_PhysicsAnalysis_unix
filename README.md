# How to run 

## General desciption 
__MIMOSIS1_PhysicsAnalysis__

The software is dedicated for:
- reading the .root tree with MIMOSIS1 raw data in FIRED format (prepared by MIMOSIS1_DataConverter software)
- saving this data in univeral data format that can be passed to set of class performing the physical analysis of MIMOSIS1 data
- savig the output .root tree with results of physics analysis 

For that moment (10.09.2020) the software is working with __Integrated_Frame__ data structure that correspond to __FIRED__ data format from MIMOSIS1 DAQ. In the future it should be also done for RAW and DEC data format (when available). 

## GitHub
Repository can be cloned from:

__https://github.com/RomaBugiel/MIMOSIS1_PhysicsAnalysis_unix.git__

## Software
* C++14, ROOT 

### Needed packages 
The software was tested on:

* ROOT 6.22/00 
* Unix: Ubuntu 20 (there is also Windows version but without documentation. This one can be used since the naming and functionality is the same)
* Doxygen: 1.8.17
* g++: 9.3.0 

## Compilation and run

### Software
> __make__ <br/>
> ./mimosis1analyser

#### Configuration file

For configuration parameters list on HTLM documentation please go: Releated Pages --> config_file.cfg

#### Output
> In general the path for saving the root trees is provided via config_file.cfg.
> If not changed, one can find the outputs in: ./outputData

### Documentation

> * in main directory run: __make docs__ 
> 	* for Latex go to /doc/latex: __make__ and open __refman.pdf__
> 	* for HTLM: go to /doc/html and open __index.html__

## Scirpits

## Essentail steps for running the software
* after cloning the repository --> __make__
* check if outputData directory is created in software dir. 
* Edit config_file.cfg. Most of the parameters might stay unchanged. One should edit (check): _run, _frames_in_run, _val_min, _val_max, _step, 
_row_start, _row_end, _column_start, _column_end, _input_tree_file_path, _output_tree_file_path, _fit_norm_param_fix
* _fit_norm_param_fix should be the same as _frames_in_run 
* _input_tree_file_path should be in principle the same as output path of DataConverter, also _input_prefix should be the same as _out_prefix 
* If ones wants to limit the range of the histograms that are plotted for entire matrix, change: _nb_of_row, _nb_of_column
* To make analysis only for several pixels, put 1 on _active_selected and list this pixels in active_selected.txt
* To mask pixels, list these pixels in masked_pixels.txt
_input_file_masked_pixel:		masked_pixels.txt
_active_selected:				0
_input_file_active_selected:	active_selected.txt
