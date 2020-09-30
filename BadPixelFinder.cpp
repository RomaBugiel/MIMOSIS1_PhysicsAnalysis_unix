/*! @file BadPixelFinder.cpp
 *  @brief Source file for BadPixelAnalyser class.
 */
#include "BadPixelFinder.h"

std::ostream &operator<<(std::ostream &os, const BadPixelFinder &bpf) 
{
	os << std::setw(40) << " BadPixelFinder is run with: " 			<< std::endl;
	os << std::setw(40) << " _input_file_masked_pixel_list: " << bpf._input_file_masked_pixel_list	<< std::endl;
	os << std::setw(40) << " _nb_of_row:	" << bpf._nb_of_row			<< std::endl;
	os << std::setw(40) << " _nb_of_column:	" << bpf._nb_of_column		<< std::endl;
	os << std::setw(40) << " _row_start:	" << bpf._row_start			<< std::endl;
	os << std::setw(40) << " _row_end:		" << bpf._row_end			<< std::endl;
	os << std::setw(40) << " _column_start:	" << bpf._column_start		<< std::endl;
	os << std::setw(40) << " _column_end:	" << bpf._column_end		<< std::endl;
	os << std::setw(40) << "_frames_in_run:	" << bpf._frames_in_run		<< std::endl;
	/*os << std::setw(30) << "" << bpf.	<< std::endl;
	os << std::setw(30) << "" << bpf.	<< std::endl;
	os << std::setw(30) << "" << bpf.	<< std::endl;
	os << std::setw(30) << "" << bpf.	<< std::endl;
	os << std::setw(30) << "" << bpf.	<< std::endl;
	os << std::setw(30) << "" << bpf.	<< std::endl;
	os << std::setw(30) << "" << bpf.	<< std::endl;*/
	
										
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
           
	_input_file_masked_pixel_list 	=	config.GetValue("_input_file_masked_pixel_list", "");                       
	_nb_of_row						=	config.GetValue("_nb_of_row", 0);
	_nb_of_column					=	config.GetValue("_nb_of_column", 0);
	_row_start						=	config.GetValue("_row_start", 0);
	_row_end						=	config.GetValue("_row_end", 0);
	_column_start					=	config.GetValue("_column_start", 0);
	_column_end						=	config.GetValue("_column_end", 0);
	_frames_in_run					=	config.GetValue("_frames_in_run", -1);
	
	//Init histograms
	init_histo();
	
	//Load masked pixel list
	load_masked_pixel_file();
	

}

void BadPixelFinder::init_histo()
{
	_h_fake_rate = new TH1D("_h_fake_rate", "Number of hits collected by pixel in frames "+ TString::Itoa(_frames_in_run,10) +" for below-threshold run = fake rate; hits; #", _frames_in_run, 0, _frames_in_run); 

}

/*
 * ---------- load_masked_pixel_file ---------
 */

void BadPixelFinder::load_masked_pixel_file()
{	
	std::string line;
	std::fstream in(_input_file_masked_pixel_list);
	Pixel masked_pixel;
	
	if (in.is_open())
	{
		while ( in >> (masked_pixel._pixel_address).first >> (masked_pixel._pixel_address).second  )
		{
			(_MIMOSIS1 -> _bad_pixels)++;
			(_MIMOSIS1 -> _v_masked_pixels).push_back(masked_pixel);
			(_MIMOSIS1 -> _h2_masked_pixels) -> Fill (masked_pixel._pixel_address.first, masked_pixel._pixel_address.second);

			//std::cout << (masked_pixel->_pixel_address).first  << "\t" << (masked_pixel->_pixel_address).second << std::endl;
		}
		
		in.close();
	}
	else
	{
		MSG(WARN, "No file with masking pixels added");
	}
	
}

/*
 * ---------- get_fake_rate_histo ---------
 */

TH1D* BadPixelFinder::get_fake_rate_histo() 
{
	return _h_fake_rate;
}


