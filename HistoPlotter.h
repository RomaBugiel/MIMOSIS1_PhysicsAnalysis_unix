/*! @file HistoPlotter.h
 *  @brief Header file for HistoPlotter class.
 */


#ifndef _HISTOPLOTTER_H_      
#define _HISTOPLOTTER_H_   


#include "globals.h"
#include "cout_msg.h"
#include "data_structure.h"
#include "Fitter.h"
#include "Matrix.h"


/*! \class 		HistoPlotter
 *  \brief 		Loades the vector<MIMOSIS1_Integrated_Frame> and performs physical analysis of the input data (S-curves, noise, threshold scans)
 *  \author    	Roma Bugiel
 *  \date      	August 2020
 *  \pre       	ROOT installation
 *  \warning   	Exeception handling not added
 *	\details	This class is detector-independent. It needs a proper input format defined in data_structure.h to make a physics analysis of the data:
 * 	- S-Curves fitting
 *  - noise extraction
 *  - bad pixels finding
 *  - saving the output data into .root tree
 * \todo 		Saves histo as a .png 
 */


class HistoPlotter { 
    

	public:

		HistoPlotter()	{}; //!< Default constructor
		/*! \brief Overloaded constructor 
		 * 	\details Takes parameters by reference. Initializes #_v_MIM_int_frame and #_v_param_values class members
		 *  \param[in] *MIMOSIS1 pointer to Matrix objest, defining the matrix for which histograms are plotted 
		 * 	\param[in] &v_MIM_int_frame reference to STL vector containing MIMOSIS1_Integrated_Frame  
		 *  \param[in] &v_param_values reference to STL vector containing scaned parameter values  */
		HistoPlotter(Matrix* MIMOSIS1, std::vector<MIMOSIS1_Single_Run_Dataset> &v_MIM_int_frame, std::vector<int> &&v_param_values) :
			_MIMOSIS1(MIMOSIS1), _v_MIM_int_frame(v_MIM_int_frame), _v_param_values(v_param_values)
		{};
		
		/*! \brief Overloaded constructor 
		 * 	\details Takes parameters by R-value reference. Initializes #_v_MIM_int_frame and #_v_param_values class members
		 *  \param[in] *MIMOSIS1 pointer to Matrix objest, defining the matrix for which histograms are plotted 
		 *	\param[in] &v_MIM_int_frame R-value reference to STL vector containing MIMOSIS1_Integrated_Frame  
		 *  \param[in] &v_param_values  R-value reference to STL vector containing scaned parameter values  */
		HistoPlotter(Matrix* MIMOSIS1, std::vector<MIMOSIS1_Single_Run_Dataset> &&v_MIM_int_frame, std::vector<int> &&v_param_values) :
			_MIMOSIS1(MIMOSIS1), _v_MIM_int_frame(v_MIM_int_frame), _v_param_values(v_param_values)
		{};

		friend std::ostream &operator<<(std::ostream &os, const HistoPlotter &plotter);

		//Members
		std::vector<MIMOSIS1_Single_Run_Dataset> 	_v_MIM_int_frame; //!< Contains the vector of MIMOSIS1_Integrated_Frame structurea
		std::vector<int> 							_v_param_values;  //!< Default the vector of parameter (VPH, VBB, VTH) scan values
		Matrix*										_MIMOSIS1;		  //!< Plots histograms for this particular detector 
		
		//Methods
		/*! \brief Mainly initialized class members
		 * 	\details Takes values from config_file.cfg to intialize class variables. The name of variables in config file and
		 * class members are the same for simpilicity.
		 * 
		 * Also finds the minimum and maximum scan value. */
		void 	init(const char * config_file);
		
		/*! \brief Opens ROOT tree with output results.
		 * 	\details The path to the output tree is given in config_file.cfg. 
		 *  The name of the output tree is constructed as follows:
		 * 
		 * _output_tree_file_core + _ + param_X + param_X_value + _ + param_Y + param_Y_value + _ + param_Z + _ + #_min_param_val + _ + #_max_param_val + .root
		 * 
		 * where X, Y are the parameters that were fixed (not pointed by scan_enable variable) and Z is the scaned parameter. #_min_param_val and #_max_param_val are resectively minimum and maximum scan values, found in #init method.
		 * 
		 *  \return 1 if succesfull, 0 if failed to open ROOT tree
		 * */
		int	 	open_output_tree();
		
		/*! \brief Initializes the pointers to output histograms
		 * 	\details The list of output histos: 
		 *  - TH2D* #h2_noise_sigma;
		 * 	- TH2D* #h2_mu;
		 * 	- TH2D* #h2_empty_pix;
		 * 	- TH2D* #h2_badnoise_pix;
		 * 	- TH2D* #h2_badmean_pix;
		 * 	- TH1D* #h_noise_sigma;
		 * 	- TH1D* #h_mu;
		 * 	- TH1D* #h_scan_values;	
		 *  - TH1D* #h_fake_rate;	
		 * 	- TH2D* #test2, *test;
		 * 	- TMultiGraph *#mg_scurves;
		 * 	- TMultiGraph *#mg_sc_badnoise;
		 * 	- TMultiGraph *#mg_sc_badmean;
		 * */
		void 	init_histo();
				
		/*! \brief Clones histograms from other sources to save it in the output tree 
		 *  \details It simply copies the integrated frame MIMOSIS1_Single_Run_Dataset::h2_hit_map (FIRED MIMOSIS1 format, integrated hit map), #Matrix::h2_masked_pixels, #Matrix::h2_activated_pixels into the output root tree. It copies the maps to each loaded .root tree (so in the output maps there are integrated frames for each scanned parameter).
		 * */
		void 	copy_histos();
		
		/*! \brief Creates TGraph for each pixel, fits ERF function, extracts parameters
		 * 	\details For each pixel the TGraph of number of entries vs scanned parameter value is created.
		 * For each TGraph the error function:
		 * 
		 * \f{eqnarray*}{ f(x) = \frac{1}{2} \cdot N \cdot TMath::Erf(\frac{x - \mu}{\sqrt{2}\cdot \sigma}) \f}
		 * 				 
		 * is fitted and noise  (\f$ \sigma \f$)  and mean (\f$ \mu \f$) are passed to proper output histograms.
		 * 
		 * There are two conditions that fit is not proper: 
		 * 0.1 < noise || noise > 10
		 * 0 < mean || mean > #_max_param_val 
		 * 
		 * If fit parameters do not match this condition, the fit procedure is repeated and conditions are verified once more. Is still fails, the pixel is added to "bad" noise or "bad" mean control plots. 
		 * 
		 * \todo maybe bottom condition should be changed !!
		 * \todo better validation of bad fit pixels should be added
		 * */
		void 	fit_S_curves();
		
		/*! \brief plots_S_curves and uses SCurvesFitter for fitting 
		 * 	\details Calls the SCurvesFitter object that fits Error Function to the given data.
		 * 	\todo This should replace fit_S_curves();
		 * */
		void 	plot_S_curves();
		
		
		/*! \brief Write and closes output .root tree.
		 * */
		int 	close_output_tree();
		
		/*! \brief Saves the histograms as images */
		void 	save_png();

		/*! \brief Generate fake rate plot */
		void 	fake_rate();
		
		//Histos
		//std::vector<TH2D*> vm_hit_map_run;
		TH2D* h2_noise_sigma;   //!< 2-D map of each pixel sigma (noise, sigma of ERF fit)
		TH2D* h2_mu;			//!< 2-D map of each pixel mean (mean of ERF fit)
		TH2D* h2_empty_pix;		//!< 2-D map of pixel without data
		TH2D* h2_badnoise_pix;	//!< 2-D map of "bad" noise pixels (0.1 < noise > 10)
		TH2D* h2_badmean_pix;	//!< 2-D map of "bad" mean pixels (0 < noise > _max_param_val)
		TH2D* h2_not_saturated_pix; //!< 2-D map of pixel that did not reach saturation
		TH2D* h2_failed_fit; 		//!< 2-D map of pixel for which fit failed
		TH2D* h2_masked_pixels;		//!< 2-D map produced in #BadPixelFinder showing pixels taken as bad (noisy, etc...)
		TH1D* h_noise_sigma;	//!< 1-D histo of pixel noise (from selected part of matrix)
		TH1D* h_mu;				//!< 1-D histo of pixel mean (from selected part of matrix)
		TH1D* h_scan_values;	//!< 1-D histo scanned parameter values
		//TH1D* h_fake_rate;		//!< 1-D histo produced in #BadPixelFinder 
		TH1D* h_chi2;			//!< 1-D histo of fits chi2
		TH1D* h_fake_rate;		//!< 1-D fake rate for noise scans \todo should be in BadPixelFinder not here. At least somewheras else.
		TH1D* h_noisy_pixels;	//!< 1-D the histo of how many pixels responded for particular scan value \todo should be in BadPixelFinder not here. At least somewheras else.
		TH1D* h_very_noisy_pixels;
		TH1D* h_hit_rate;
		
		TMultiGraph *mg_scurves;		//!< Multigraph of all S-curves with fits
		TMultiGraph *mg_sc_badnoise;	//!< Multigraph of "bad" noise pixels S-curves with fits
		TMultiGraph *mg_sc_badmean;		//!< Multigraph of "bad" mean pixels S-curves with fits
		TMultiGraph *mg_failed_fit;		//!< Multigraph of pixels with failed fits. \todo Should replace mg_sc_badnoise mg_sc_badmean
		
		std::string _scaned_param{""};	//!< Name of the scaned parameters.
		TString _output_full_name;

		~HistoPlotter()	{};		//!< Destructor 

	private:
	
		int 	_min_param_val; //!< Minimum value of the scanned analysis parameter. 
		int 	_max_param_val; //!< Maximum value of the scanned analysis parameter. 
		double  _saturation_lvl; //!< [0 - 1] determines the level of pixel saturation that has to be achieved, to start the fitting procedure
	
		double _noise_cut_low ;
		double _noise_cut_high;
		double _threshold_cut_low;
		double _threshold_cut_high;
	
	#ifndef DOXYGEN_SHOULD_SKIP_THIS
		
		TEnv 	config;
		TFile 	*_output_data_file;
		TTree 	*_mimosis0_out_tree;
	
		//Variables
		int	_run;
		
		std::string _param_1;	
		std::string _param_2;	
		std::string _param_3;	 
		
		int _param_1_value;	
		int _param_2_value; 
		int _param_3_value; 
 
		int _nb_of_row;	
		int _nb_of_column;
		int _row_start;			
		int _row_end;			
		int _column_start;		
		int _column_end;	
		int _frames_in_run; //!< \todo should be taken from other class

		int _dac_shift;
		
		std::string _output_tree_file_path 	{""};
		std::string _output_tree_file_core  {""};
		std::string _output_tree_file_part  {""};
		std::string _output_tree_name 		{""};
		std::string _output_prefix 			{""};	
	#endif /* DOXYGEN_SHOULD_SKIP_THIS */
};

#endif
