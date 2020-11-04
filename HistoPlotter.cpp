#include "HistoPlotter.h"

/*
 * Provides the list of all the parameters stored by the class
 */
 
std::ostream &operator<<(std::ostream &os, const HistoPlotter &plotter) 
{
	os << std::setw(30) << std::endl;
	os << std::setw(30) << " HistoPlotter is run with: " 	<< std::endl;
	os << std::setw(30) << "_run : " 					<< plotter._run						<< std::endl;
	os << std::setw(30) << "_scaned_param : " 			<< plotter._scaned_param			<< std::endl;
	os << std::setw(30) << "_output_tree_file_path : "	<< plotter._output_tree_file_path	<< std::endl;
	os << std::setw(30) << "_output_tree_file_core : "  << plotter._output_tree_file_core	<< std::endl;
	os << std::setw(30) << "_output_tree_file_part : "  << plotter._output_tree_file_part	<< std::endl;
	os << std::setw(30) << "_output_tree_name : "    	<< plotter._output_tree_name     	<< std::endl;
	os << std::setw(30) << "_output_prefix : "   	 	<< plotter._output_prefix     		<< std::endl;
	os << std::setw(30) << "_nb_of_row : " 				<< plotter._nb_of_row				<< std::endl;
	os << std::setw(30) << "_nb_of_column : "			<< plotter._nb_of_column			<< std::endl;
	os << std::setw(30) << "_row_start : " 				<< plotter._row_start				<< std::endl;
	os << std::setw(30) << "_row_end : "  				<< plotter._row_end					<< std::endl;
	os << std::setw(30) << "_column_start : "    		<< plotter._column_start     		<< std::endl;
	os << std::setw(30) << "_column_end : "    			<< plotter._column_end     			<< std::endl;
	os << std::setw(30) << "_param_1 : " 				<< plotter._param_1 				<< std::endl;
	os << std::setw(30) << "_param_2 : " 				<< plotter._param_2 				<< std::endl;
	os << std::setw(30) << "_param_3 : " 				<< plotter._param_3 				<< std::endl;
	os << std::setw(30) << "_param_1_value : " 			<< plotter._param_1_value 			<< std::endl;
	os << std::setw(30) << "_param_2_value : " 			<< plotter._param_2_value 			<< std::endl;
	os << std::setw(30) << "_param_3_value : " 			<< plotter._param_3_value 			<< std::endl;
	os << std::setw(30) << "_dac_shift : " 				<< plotter._dac_shift 			<< std::endl;
	os << std::setw(30) << "_saturation_lvl : " 		<< plotter._saturation_lvl 			<< std::endl;								  									
	return os;                          
}

/*
 * Initializes class members taking the values from the file
 */

void HistoPlotter::init(const char * config_file)
{
	
	if(config.ReadFile(config_file, kEnvUser) < 0)
	{ 
		MSG(ERR, "[HP] Config file not loaded --> exit." );
		exit(0);
	}
	else
	{   
		_run					=	config.GetValue("_run", 999999);
		
		_frames_in_run			=	config.GetValue("_frames_in_run", 0);
		_nb_of_row				=	config.GetValue("_nb_of_row", 0);
		_nb_of_column			=	config.GetValue("_nb_of_column", 0);
		_row_start				=	config.GetValue("_row_start", 0);
		_row_end				=	config.GetValue("_row_end", 0);
		_column_start			=	config.GetValue("_column_start", 0);
		_column_end				=	config.GetValue("_column_end", 0);
				
		_output_tree_file_path	=	config.GetValue("_output_tree_file_path", "");		
		_output_tree_file_core 	=   config.GetValue("_output_tree_file_core", "");	
		_output_tree_file_part	=   config.GetValue("_output_tree_file_part", "");			
		_output_tree_name		=	config.GetValue("_output_tree_name", "");
		
		_output_prefix			=   config.GetValue("_input_prefix", "");
		
		_param_1 				=	config.GetValue("_param_1", "");
		_param_2 				=   config.GetValue("_param_2", "");
		_param_3 				=   config.GetValue("_param_3", "");
									
		_param_1_value			=   config.GetValue("_param_1_value", 0);
		_param_2_value			=   config.GetValue("_param_2_value", 0);
		_param_3_value			=   config.GetValue("_param_3_value", 0);
	 	
	 	_dac_shift				=   config.GetValue("_dac_shift", 0);

		_saturation_lvl			=   config.GetValue("_saturation_lvl", 0.5);	                       
	}
	
	
	if(!_v_param_values.size())
	{
		MSG(ERR, "[HP] There is no parameter value given for analysis --> exit." );
		exit(0);
	}
	else
	{	
		for(int i = 0; i < (int)(_v_param_values.size()); i++) 
		{
			_v_param_values[i] = _v_param_values[i]+_dac_shift;	
		}
		
		_min_param_val 	= *std::min_element(_v_param_values.begin(),_v_param_values.end());
		_max_param_val	= *std::max_element(_v_param_values.begin(),_v_param_values.end());
	
		MSG(INFO, "[HP] HistoPlotter is running analysis for: " + _scaned_param + " from " + _min_param_val + " to " + _max_param_val);
		MSG(INFO, "[HP] HistoPlotter is running analysis in range: [" << _column_start << " - " <<  _column_end << "] " << _row_start << " - " << _row_end << "] ");

	}
	
	_output_full_name =	_output_tree_file_path 			+ "/" +	
						_output_tree_file_core 			+ "_" + 
						"run" + std::to_string(_run) 	+ "_" + 
						_output_tree_file_part 			+ "_" +
						_scaned_param + "_" + std::to_string(_min_param_val) + "_"  +std::to_string(_max_param_val) +
						_output_prefix  ;
}

/*
 * Opens output tree that will store the histos
 */

int HistoPlotter::open_output_tree()
{
	
							
	_output_data_file	=	TFile::Open(_output_full_name+ ".root" , "recreate");
		
	if (!_output_data_file) 
	{ 	
		MSG(ERR, "[HP] Failed to open output tree --> exit." );
		exit(0);
	}
	else
	{	
		_output_data_file	->	cd();
		_mimosis0_out_tree 	= new TTree((TString)_output_tree_name, (TString)_output_tree_name);
		MSG(INFO, "[HP] Output file open: " + _output_full_name + " with TTree: " + _output_tree_name);
		
		return 1;
	}
	
}

/*
 * Initializes histograms that are saved in output tree
 */



void HistoPlotter::init_histo()
{
	long unsigned int nb_of_values  {_v_param_values.size()};
	const int nb_of_bins_x  {(_column_end-_column_start)+1};
	const int nb_of_bins_y  {(_row_end-_row_start)+1};

	//vm_hit_map_run.resize(nb_of_values);
	
	//TH1D
	h_scan_values	= new TH1D("h_scan_values", "["+(TString)(std::to_string(_run))+"] "+(TString)(_scaned_param) + " scan values (" + (TString)(_output_tree_file_part) + ")", nb_of_values+1, _min_param_val, _max_param_val );
	h_noise_sigma	= new TH1D("h_noise_sigma", "["+(TString)(std::to_string(_run))+"] Pixels noise histo for " + (TString)(_scaned_param) + " scan (" + (TString)(_output_tree_file_part) + ") ; pixel noise [e] ; entries", 1000, 0, 100 );
	h_mu			= new TH1D("h_mu", "["+(TString)(std::to_string(_run))+"] Pixels threshold histo for " + (TString)(_scaned_param) + " scan (" + (TString)(_output_tree_file_part) + ") ; pixel threshold [LSB]; entries", ((_max_param_val-_min_param_val))+1, 0.8*_min_param_val, _max_param_val*1.2 );
 	h_chi2			= new TH1D("h_chi2", "["+(TString)(std::to_string(_run))+"] Chi2 of fits for " + (TString)(_scaned_param) + " scan (" + (TString)(_output_tree_file_part) + ") ; chi2; entries", 1000,0,200 );

	//TH2D
	h2_noise_sigma 		= new TH2D("h2_noise_sigma", "["+(TString)(std::to_string(_run))+"] Pixels noise map for " + (TString)(_scaned_param) + " scan (" + (TString)(_output_tree_file_part) +  ") ; column; row", nb_of_bins_x, _column_start, _column_end+1, nb_of_bins_y, _row_start, _row_end+1);
	h2_mu				= new TH2D("h2_mu", "["+(TString)(std::to_string(_run))+"] Pixels threshold map for " + (TString)(_scaned_param) + " scan (" + (TString)(_output_tree_file_part) +  ") ; column; row", nb_of_bins_x, _column_start, _column_end+1, nb_of_bins_y, _row_start, _row_end+1);
	//h2_activated_pixels = new TH2D("h2_activated_pixels", "Map of selected pixels activated for analysis", nb_of_bins_x, _column_start, _column_end+1, nb_of_bins_y, _row_start, _row_end+1); <
	//h2_masked_pixels 	= new TH2D("h2_masked_pixels", "Map of masked pixels", nb_of_bins_x, _column_start, _column_end+1, nb_of_bins_y, _row_start, _row_end+1); <
	h2_empty_pix 		= new TH2D("h2_empty_pix", "["+(TString)(std::to_string(_run))+"] Empty pixels (no data from converter)", nb_of_bins_x, _column_start, _column_end+1, nb_of_bins_y, _row_start, _row_end+1); 
	h2_not_saturated_pix = new TH2D("h2_not_saturated_pix", "["+(TString)(std::to_string(_run))+"] Pixel that did not reached saturation level (= nb_of_frames)", nb_of_bins_x, _column_start, _column_end+1, nb_of_bins_y, _row_start, _row_end+1); 
	h2_failed_fit		= new TH2D("h2_failed_fit", "["+(TString)(std::to_string(_run))+"] Pixels for which the ERF function fit failed", nb_of_bins_x, _column_start, _column_end+1, nb_of_bins_y, _row_start, _row_end+1); 

	//h2_badnoise_pix	= new TH2D("h2_badnoise_pix", "Pixels with noise qualified as not proper", nb_of_bins_x, _column_start, _column_end+1, nb_of_bins_y, _row_start, _row_end+1); 
	//h2_badmean_pix	= new TH2D("h2_badmean_pix", "Pixels with mean qualified as not proper", nb_of_bins_x, _column_start, _column_end+1, nb_of_bins_y, _row_start, _row_end+1); 
	mg_scurves 		= 	new TMultiGraph("mg_scurves", "mg_scurves");
	mg_scurves		->	SetTitle("["+(TString)(std::to_string(_run))+"] S curves for " + (TString) _scaned_param + " scan ; " + (TString) _scaned_param + " [LSB] ; #");

	mg_sc_badnoise 	= 	new TMultiGraph("mg_sc_badnoise", "mg_sc_badnoise");
	mg_sc_badnoise	->	SetTitle("["+(TString)(std::to_string(_run))+"] S curves with noise higher than 10 for " + (TString) _scaned_param + " scan ; " + (TString) _scaned_param + " ; #");

	mg_sc_badmean 	= 	new TMultiGraph("mg_sc_badmean", "mg_sc_badmean");
	mg_sc_badmean	->	SetTitle("["+(TString)(std::to_string(_run))+"] S curves with mean higher than min max param " + (TString) _scaned_param + " scan ; " + (TString) _scaned_param + " [LSB] ; #");

	mg_failed_fit	= 	new TMultiGraph("mg_failed_fit", "mg_failed_fit");
	mg_failed_fit	->	SetTitle("["+(TString)(std::to_string(_run))+"] Failed ERF fits for s" + (TString) _scaned_param + "  scan ; " + (TString) _scaned_param + " [LSB] ; #");

}   

/*
 * Plots the hit map - the 2D histograms with the number of entries in each pixel (how many
 * each pixel fired in N frames)
 */

void HistoPlotter::copy_histos()
{
	TH2D * histo; 

	for(long unsigned int i = 0; i < _v_MIM_int_frame.size(); i++) 
	{	
		histo = (TH2D*) ((_v_MIM_int_frame[i].h2_hit_map)->Clone());  
	}

	histo	=  (TH2D*) ((_MIMOSIS1 -> _h2_masked_pixels) 	-> Clone());
	histo	=  (TH2D*) ((_MIMOSIS1 -> _h2_activated_pixels) -> Clone());
						 
}


/*
 * Plots the S-curves
 */
 
void HistoPlotter::plot_S_curves()
{
	TBenchmark *benchmark = new TBenchmark();
	benchmark->Start("SearchTime");
	
	Fitter fitter;	
	int row_bin {0};
	int col_bin {0};	
	int how_many_hits {0};
	std::vector<int> v_pixel_fired;
	int is_saturated {0};
	int good_fit{0};
	std::pair<double,double> mean_noise;
	TGraph *s_curve;
	
	fitter.init("config_file.cfg");
	std::cout << fitter << std::endl;
	
	for(int row_it = _row_start; row_it <= _row_end ; row_it++) 
	{			
		for(int col_it = _column_start; col_it <= _column_end ; col_it++) 
		{	
			col_bin 	= _MIMOSIS1->_h2_masked_pixels->GetXaxis()->FindBin(col_it);
			row_bin		= _MIMOSIS1->_h2_masked_pixels->GetYaxis()->FindBin(row_it);	
			
			if(_MIMOSIS1->_actived_selected > 0)
			{
				if(_MIMOSIS1->_h2_activated_pixels 	-> GetBinContent(col_bin, row_bin) == 0 ) continue;
			}
			
			if(_MIMOSIS1->_h2_masked_pixels 	-> GetBinContent(col_bin, row_bin) == 1 ) continue;

			for(int val = 0 ; val < (int) _v_param_values.size() ; val++) 
			{	
				h_scan_values->Fill(val);				
				//Calculated two times because _v_MIM_int_frame and h2_masked might have different bining
				//It might be that it should be solved another way because would be not efficient.
				//GetBinContent and finging bin is done only for determining if there was saturation or not -> should be done in more effective way
				col_bin 		= _v_MIM_int_frame[val].h2_hit_map->GetXaxis()->FindBin(col_it);
				row_bin			= _v_MIM_int_frame[val].h2_hit_map->GetYaxis()->FindBin(row_it);	
					
					
				how_many_hits 	= (_v_MIM_int_frame[val].h2_hit_map)->GetBinContent(col_bin, row_bin);
				v_pixel_fired.push_back( how_many_hits );
				
				//Count how many times pixel fired in each frame. If this not happened, S curve will be not fitted,
				if( how_many_hits >= _saturation_lvl*_v_MIM_int_frame[val].nb_of_frames ) is_saturated ++;
			}
			
						
			if(is_saturated == 0) 
			{	
				h2_not_saturated_pix -> Fill( col_it, row_it);
			}
			else
			{
				good_fit 	= fitter.fit_error_function(_v_param_values, v_pixel_fired);
				s_curve		= fitter.get_S_curve();
				mean_noise 	= fitter.get_S_curve_fit_params();
				h_chi2		-> Fill( fitter.get_S_curve_fit_chi2() );
				
				if(good_fit)
				{
					h2_noise_sigma		->	Fill(col_it, row_it, mean_noise.second);
					h2_mu				->	Fill(col_it, row_it, mean_noise.first);
					h_noise_sigma		->	Fill(mean_noise.second);
					h_mu				->	Fill(mean_noise.first);
					mg_scurves			->	Add(s_curve,"L");
					//std::cout << col_it << "\t" << row_it << "\t" << mean_noise.second << std::endl;
					
				}
				else
				{	
					mg_failed_fit	->	Add(s_curve,"L");
					h2_failed_fit	->	Fill(col_it, row_it);					
					//MSG(CNTR, "[HP] Pixel with failed fit: [" +  std::to_string( col_it ) + ", " + std::to_string( row_it )  "]. ---- mean: " +  std::to_string( mean_noise.first ) + ", sigma: " + std::to_string( mean_noise.second ) );

				}
			
			}				
			
			fitter.clear();			
			v_pixel_fired.clear();
			s_curve 		= nullptr;
			is_saturated 	= 0;
		} 
	}


	delete s_curve;
	MSG(WARN, "[HP] Refitted functions: 	 " +  	std::to_string( fitter._refitted ));
	MSG(WARN, "[HP] Pixels not saturated: " + 	std::to_string( h2_not_saturated_pix -> GetEntries() ));
	MSG(WARN, "[HP] Pixels failed fit: 	 " +  	std::to_string( h2_failed_fit -> GetEntries() ));

	std::cout << "generate_S_curves() time: "; benchmark->Show("SearchTime");	
	
}


/*
 * Saves histos in the output tree and closes it
 */		
int HistoPlotter::close_output_tree()
{	
	_output_data_file	->	cd();
	mg_scurves 		-> 	Write();
	//mg_sc_badnoise -> Write();
	//mg_sc_badmean	 -> Write();
	mg_failed_fit	-> 	Write();

	_output_data_file  ->	Write();
	_output_data_file  ->	Close();
	
	return 1;
}

/*
 * Saves histos in the output tree and closes it
 */		
void HistoPlotter::save_png()
{	
	//gStyle->SetPalette(kCool);
	
	TCanvas *c = new TCanvas(); 
	c->cd(); c->SetGrid();
	h_noise_sigma->GetXaxis()->SetRangeUser(0, h_noise_sigma->GetMean()+10*h_noise_sigma->GetRMS());
	h_noise_sigma->SetLineWidth(2);
	h_noise_sigma->SetFillColor(kBlue);
	h_noise_sigma->SetFillStyle(3244);
	h_noise_sigma->Draw();
	c->Print(_output_full_name+"_h_noise_sigma.pdf");
	

	c->Clear(); c->cd();
	h_mu->SetLineWidth(2);
	h_mu->SetFillColor(kBlue);
	h_mu->SetFillStyle(3244);	
	h_mu->Draw();
	c->Print(_output_full_name+"_h_mu.pdf");

	c->Clear(); c->cd();
	h_scan_values->Draw();
	c->Print(_output_full_name+"_h_scan_values.pdf");
	
	c->Clear(); c->cd();
	h_chi2->Draw();
	c->Print(_output_full_name+"h_chi2.pdf");
	
	//c->Clear(); c->cd();
	//h_fake_rate->Draw();
	//c->Print(_output_full_name+"_h_fake_rate.pdf");
	
	c->Clear(); c->cd();
	mg_scurves->SetMinimum(0);
	mg_scurves->SetMaximum(_frames_in_run*1.1);
	//mg_scurves->GetXaxis()->SetRangeUser(0,270);
	mg_scurves->Draw("alp*");
	c->Print(_output_full_name+"_mg_scurves.png");

	c->Clear(); c->cd();
	mg_failed_fit->SetMinimum(0);
	mg_failed_fit->SetMaximum(_frames_in_run*1.1);
	mg_failed_fit->Draw("alp*");
	c->Print(_output_full_name+"_mg_failed_fit.pdf");
	/*
	
	c->Clear(); c->cd();
	mg_sc_badnoise->Draw();
	c->Print(_output_full_name+"mg_sc_badnoise.pdf");
	
	c->Clear(); c->cd();
	mg_sc_badmean->Dc1->SetGrid();raw();
	c->Print(_output_full_name+"mg_sc_badmean.pdf");*/
	
	gStyle->SetOptStat(0);
	c->Update();
	
	c->Clear(); c->cd();
	h2_mu->SetMinimum(0);
	h2_mu->Draw("COLZ");
	c->Print(_output_full_name+"_h2_mu.pdf");

	c->Clear(); c->cd();
	h2_noise_sigma->SetMinimum(0);
	h2_noise_sigma->Draw("COLZ");
	c->Print(_output_full_name+"_h2_noise_sigma.pdf");

	c->Clear(); c->cd();
	h2_empty_pix->Draw("COLZ");
	c->Print(_output_full_name+"_h2_empty_pix.pdf");

	//c->Clear(); c->cd();
	//h2_badnoise_pix->Draw("COLZ");
	//c->Print(_output_full_name+"_h2_badnoise_pix.pdf");
	
	//c->Clear(); c->cd();
	//h2_badmean_pix->Draw("COLZ");
	//c->Print(_output_full_name+"_h2_badmean_pix.pdf");
	
	c->Clear(); c->cd();
	h2_not_saturated_pix->Draw("COLZ");
	c->Print(_output_full_name+"_h2_not_saturated_pix.pdf");
	
	c->Clear(); c->cd();
	h2_failed_fit->Draw("COLZ");
	c->Print(_output_full_name+"_h2_failed_fit.pdf");	
	
	/*c->Clear(); c->cd();
	h2_masked_pixels->Draw("COLZ");
	c->Print(_output_full_name+"_h2_masked_pixels.pdf");	*/
	
	/*double p1 = _v_MIM_int_frame[0].nb_of_frames;
	double p2 = h_mu->GetMean();
	double p3 = h_noise_sigma->GetMean();

	TF1 *ferf  = new TF1("ferf","0.5*[0]*(1+TMath::Erf((x-[1])/TMath::Sqrt2()*[2]))",0,150);
	ferf->SetParameters(p1,p2,p3);
	ferf->SetTitle("Erf function with N = 1000, mean = " + (TString)std::to_string(p2) + " and noise = " +  (TString)std::to_string(p3) + "; vpulse; val" );
	c->Clear(); c->cd();
	ferf->Draw("");
	c->Print(_output_full_name+"_S_curve_sim.png");	*/

}

		
/*
 * Fit the S-curves
 */
 /*
void HistoPlotter::fit_S_curves()
{

	TBenchmark *benchmark = new TBenchmark();
	benchmark->Start("SearchTime");
		
	int curve_color {800};
	double norm {1.};
	double lower_norm_limit {0.9};
	double upper_norm_limit {1.1};
    const int param_nb = _v_param_values.size();
	std::vector<int> entries;
	double noise{0};
	double mean{0};
	TGraph *s_curve;
	TF1 *ferf;
	c1->SetGrid();
	int badsigma {0};
	int badmean {0};
	int is_saturated {0};
	int bad_pixel {0};
	int refited {0};
	int row_bin {0};
	int col_bin {0};
	int how_many_hits {0};
	int nb_of_frames {0};
	
	//std::cout << " Number of entries: " << (_MIMOSIS1->_h2_masked_pixels)->GetEntries() << std::endl;

	//Hand removing files from the analysis 
	


	for(int row_it = _row_start; row_it <= _row_end ; row_it++) 
	//for(int row_it = 0; row_it < row_bin ; row_it++) 
	{	
		MSG(DEB, " Row: " + std::to_string(row_it) );
		
		for(int col_it = _column_start; col_it <= _column_end ; col_it++) 
		//for(int col_it = 0; col_it < 7 ; col_it++) 
		{	
			MSG(DEB, "\t Col: " + std::to_string(col_it) );
			
			col_bin 		= (_MIMOSIS1->_h2_masked_pixels)->GetXaxis()->FindBin(col_it);
			row_bin			= (_MIMOSIS1->_h2_masked_pixels)->GetYaxis()->FindBin(row_it);	
			
			MSG(DEB, "\t\t col_bin: " + std::to_string(col_bin) + " -- row_bin: " + std::to_string(row_bin) );

			
			if(_MIMOSIS1->_h2_masked_pixels -> GetBinContent(col_bin, row_bin) == 1 ) continue;

			for(int val = 0 ; val < param_nb ; val++) 
			{	
				MSG(DEB, "\t\t\t val: " + std::to_string(val) );
				
				col_bin 		= _v_MIM_int_frame[val].h2_hit_map->GetXaxis()->FindBin(col_it);
				row_bin			= _v_MIM_int_frame[val].h2_hit_map->GetYaxis()->FindBin(row_it);
				
				how_many_hits 	= (_v_MIM_int_frame[val].h2_hit_map)->GetBinContent(col_bin, row_bin);
				nb_of_frames 	= _v_MIM_int_frame[val].nb_of_frames;
				
				MSG(DEB, "\t\t\t\t how_many_hits: " + std::to_string(how_many_hits) );
				MSG(DEB, "\t\t\t\t nb_of_frames: "  + std::to_string(nb_of_frames) );

				
				entries.push_back( how_many_hits );
				
				if( how_many_hits > 0.5*nb_of_frames ) is_saturated ++;
				
				//std::cout << "Number of frames: " <<  _v_MIM_int_frame[val].nb_of_frames << std::endl;
			}
			
						
			if(is_saturated == 0) 
			{	//To do: add log class, pixel class -> and log should store .txt with not saturated pixels
				//MSG(CNTR, "Not saturated pixel: " + std::to_string(col_it) + "\t" + std::to_string(row_it) );
				h2_not_saturated_pix -> Fill( col_it, row_it);
			}
			else
			{	
				MSG(CNTR, "Fitting started.");

				for(auto &i : _v_param_values) std::cout << i << std::endl;

				s_curve	= 	new TGraph(param_nb, &_v_param_values[0], &entries[0]);
				s_curve	->	SetLineWidth(2);
				s_curve	->	SetLineColor(kBlue);
					
				ferf 	= 	new TF1("ferf","0.5*[0]*(1+TMath::Erf((x-[1])/TMath::Sqrt2()*[2]))");
				//std::cout << "number of frames: " << nb_of_frames << std::endl;
				//std::cout << "mean:" << (_max_param_val-_min_param_val)/2+_min_param_val << std::endl;
				//ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
				
				
				//ROOT::Math::MinimizerOptions::SetDefaultPrecision(0.000001);

				ROOT::Math::MinimizerOptions::SetDefaultTolerance(1e-3); 
				ROOT::Math::MinimizerOptions::SetDefaultMaxIterations(1000);
				ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(1000);
	
				ferf 	->	FixParameter(0,	nb_of_frames); 
				ferf	->	SetParLimits(1, _min_param_val,_max_param_val );				
				ferf 	->	SetParLimits(2,	0,50); 
				ferf 	->	SetParameters(nb_of_frames, _min_param_val, 0.1); 
				ferf	->	SetParError(1, 0.1);
				ferf	->	SetParError(2, 0.01);
					
				s_curve	->	Fit(ferf,"WQ");
				
				noise 	= 	ferf->GetParameter(2);
				mean 	= 	ferf->GetParameter(1);	

				if(noise < 0.01 || noise > 10) 
				{ 
					//Redo fit
					ferf	->	SetParameter(2,0.1);
					s_curve	->	Fit(ferf,"WQ");
					noise 	= 	ferf->GetParameter(2);
					mean 	= 	ferf->GetParameter(1);	
					refited++;
					
					if(noise < 0.01 || noise > 10) 
					{
						badsigma++; 
						bad_pixel = 1;					
						mg_sc_badnoise	->	Add(s_curve,"L"); 
						h2_badnoise_pix	->	Fill(col_it, row_it, noise);
					
					}
					else 
					{
						bad_pixel = 0;
					}
				}
				
				if(mean < 0 || mean > _max_param_val) 
				{ 	
					//Redo fit
					s_curve	->	Fit(ferf,"WQ");
					noise 	= 	ferf->GetParameter(2);
					mean 	= 	ferf->GetParameter(1);
					refited++;
						
					if(mean < 0 || mean > _max_param_val) 
					{
						badmean++;  
						mg_sc_badmean	->	Add(s_curve,"L");
						h2_badmean_pix	->	Fill(col_it, row_it, mean);
						bad_pixel = 1;
					}
					else 
					{
						bad_pixel = 0;
					}

				}

				if(!bad_pixel) 
				{	std::cout << "Check: final noise: " << noise << " and mean: " << mean << std::endl;
					h2_noise_sigma		->	Fill(col_it, row_it, noise);
					h2_mu				->	Fill(col_it, row_it, mean);
					mg_scurves			->	Add(s_curve,"L");
				}
				
				
				h_noise_sigma		->	Fill(noise);
				h_mu				->	Fill(mean);
				
			}				
			
			bad_pixel = 0;
			s_curve = nullptr;
			ferf = nullptr;;
			entries.clear();
			is_saturated = 0;
		} 
	}



	delete s_curve;
	delete ferf;
	MSG(WARN, "Refitted functions: " +  std::to_string( refited ));
	MSG(WARN, "Pixels not saturated: " +  std::to_string( h2_not_saturated_pix -> GetEntries() ));
	MSG(WARN, "Pixels with noise larger than 10: " +  std::to_string(badsigma));
	MSG(WARN, "Pixels with mean larger than " + std::to_string(_max_param_val) + ": "  +  std::to_string( badmean));

	std::cout << "generate_S_curves() time: "; benchmark->Show("SearchTime");	
}*/
