/*! @file BadPixelFinder.cpp
 *  @brief Source file for BadPixelAnalyser class.
 */
#include "BadPixelFinder.h"

std::ostream &operator<<(std::ostream &os, const BadPixelFinder &bpf) 
{
	os << std::setw(30) << std::endl;
	os << std::setw(30) << " BadPixelFinder is running with: " 	<< std::endl;
	os << std::setw(30) << " _input_file_masked_pixel:	" 	<< bpf._input_file_masked_pixel	<< std::endl;
	os << std::setw(30) << " _active_selected:	"			<< bpf._active_selected		<< std::endl;
	os << std::setw(30) << " _input_file_active_selected:	" 	<< bpf._input_file_active_selected	<< std::endl;
	os << std::setw(30) << " _nb_of_row:	" 				<< bpf._nb_of_row			<< std::endl;
	os << std::setw(30) << " _nb_of_column:	" 				<< bpf._nb_of_column		<< std::endl;
	os << std::setw(30) << " _row_start:	" 				<< bpf._row_start			<< std::endl;
	os << std::setw(30) << " _row_end:	" 					<< bpf._row_end			<< std::endl;
	os << std::setw(30) << " _column_start:	" 				<< bpf._column_start		<< std::endl;
	os << std::setw(30) << " _column_end:	" 				<< bpf._column_end		<< std::endl;
	os << std::setw(30) << "_frames_in_run:	" 				<< bpf._frames_in_run		<< std::endl;
									
	return os;                          
}



/*
 * ---------- init() ---------
 */

void BadPixelFinder::init(const char * config_file)
{
	
	if(config.ReadFile(config_file, kEnvUser) < 0)
	{ 
		MSG(ERR, "Config file not loaded for BadPixelFinder --> exit." );
		exit(0);
	}
           
	_input_file_masked_pixel 		=	config.GetValue("_input_file_masked_pixel", "");  
	_input_file_active_selected 	=	config.GetValue("_input_file_active_selected", "");  
	_nb_of_row						=	config.GetValue("_nb_of_row", 0);
	_nb_of_column					=	config.GetValue("_nb_of_column", 0);
	_row_start						=	config.GetValue("_row_start", 0);
	_row_end						=	config.GetValue("_row_end", 0);
	_column_start					=	config.GetValue("_column_start", 0);
	_column_end						=	config.GetValue("_column_end", 0);
	_frames_in_run					=	config.GetValue("_frames_in_run", -1);
	_active_selected				=	config.GetValue("_active_selected", 0);

	if(_active_selected) MSG(WARN, "[BPF] Only selected pixels are taken to the analysis");

	//Init histograms
	init_histo();
	

	

}

/*
 * ---------- init_histo ---------
 */
void BadPixelFinder::init_histo()
{
	_h_fake_rate = new TH1D("_h_fake_rate", "Number of hits collected by pixel in frames "+ TString::Itoa(_frames_in_run,10) +" for below-threshold run = fake rate; hits; #", _frames_in_run, 0, _frames_in_run); 
}

/*
 * ---------- execute ---------
 */

void BadPixelFinder::execute()
{
	//Load masked pixel list
	load_masked_pixel_file();
	_MIMOSIS1	->	list_mask_pixels();

	//Activate only selected pixels
	if(_active_selected) 
	{
		 load_activate_selected_file();
		_MIMOSIS1	->	list_active_pixels();
	}
}

/*
 * ---------- load_activate_selected_file ---------
 */

void BadPixelFinder::load_activate_selected_file()
{
	std::string line;
	std::fstream in_activated(_input_file_active_selected, std::fstream::in | std::fstream::out);
	Pixel active_pixel;

	if (in_activated.is_open())
	{
		MSG(WARN, "[BPF] File " + _input_file_active_selected + " opened sucessfully." );
		
		while ( in_activated >> (active_pixel._pixel_address).first >> (active_pixel._pixel_address).second  )
		{
			(_MIMOSIS1 -> _actived_selected)++;
			(_MIMOSIS1 -> _v_activated_pixels).push_back(active_pixel);
			(_MIMOSIS1 -> _h2_activated_pixels) -> Fill (active_pixel._pixel_address.first, active_pixel._pixel_address.second);

			//std::cout << (active_pixel->_pixel_address).first  << "\t" << (active_pixel->_pixel_address).second << std::endl;
		}
		
		in_activated.close();
	}
	else
	{
		MSG(WARN, "[BPF] No file with activated pixels found");
	}

}

/*
 * ---------- load_masked_pixel_file ---------
 */

void BadPixelFinder::load_masked_pixel_file()
{	
	std::string line;
	std::fstream in_masked(_input_file_masked_pixel, std::fstream::in | std::fstream::out );
	Pixel masked_pixel;
	
	
	if (in_masked.is_open())
	{
		MSG(WARN, "[BPF] File " + _input_file_masked_pixel + " with listed of masked pixels open sucessfully.");
		
		while ( in_masked >> (masked_pixel._pixel_address).first >> (masked_pixel._pixel_address).second  )
		{
			(_MIMOSIS1 -> _bad_pixels)++;
			(_MIMOSIS1 -> _v_masked_pixels).push_back(masked_pixel);
			(_MIMOSIS1 -> _h2_masked_pixels) -> Fill (masked_pixel._pixel_address.first, masked_pixel._pixel_address.second);

			//std::cout << (masked_pixel->_pixel_address).first  << "\t" << (masked_pixel->_pixel_address).second << std::endl;
		}
		
		in_masked.close();
	}
	else
	{
		MSG(WARN, "[BPF] No file with masked pisels found");
	}
	
}

/*
 * ---------- get_fake_rate_histo ---------
 */

TH1D* BadPixelFinder::get_fake_rate_histo() 
{
	return _h_fake_rate;
}


