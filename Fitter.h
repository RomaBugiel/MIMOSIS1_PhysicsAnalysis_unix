/*! @file Fitter.h
 *  @brief Header file for Fitter class.
 */


#ifndef _FITTER_H_      
#define _FITTER_H_   


#include "globals.h"
#include "cout_msg.h"
#include "data_structure.h"


/*! \class 		Fitter
 *  \brief 		Makes a S-curves fitting procedure, returns noise and mean of the pixel. 
 *  \author    	Roma Bugiel
 *  \date      	September 2020
 *  \pre       	ROOT installation
 *  \warning   	Exeception handling not added
 *	\details	This class is detector-independent. It fits the ERF fuction (S-curve) to the data
 * using user defined TFunction.
 */


class Fitter { 
    
	public:

		Fitter()	{}; //!< Default constructor
		
		~Fitter() {}; //!< Destructor
		
		friend std::ostream &operator<<(std::ostream &os, const Fitter &fitter);

		// Variables	
		std::vector<int> 		_v_global_x;	//!< Vector containing variables for x-coordinate of the S-curves 
		std::pair<double,double> fit_params;	//!< Fit parameters result: first is a mu of Erf function and seconf is a noise (sigma) of Erf function
		TGraph *s_curve {nullptr};				//!< Graph with raw data 

		/*! \brief Number of interation needed to get a good fit
		 * 	\details Good is defined by the user variables given in config file, see this class variables
		 */
		int _refitted {0};	
		
		// Methods

		void					 init(const char * config_file);	//!< Initializes variables mainly 
		void					 set_fitting_options(); 			//!< Sets ROOT::Math::Minimizer options
		
		/*! \brief Fits ERF function 
		 * 	\details  This methods fits the Erf function defined as:
		 *  \f{eqnarray*}{ f(x) = \frac{1}{2} \cdot N \cdot TMath::Erf(\frac{x - \mu}{\sqrt{2}\cdot \sigma}) \f}
		 * 	where noise is (\f$ \sigma \f$) and mean (\f$ \mu \f$)
		 *  Fitting is done for the data  passed to method as parameter			 
		 * 	\param[in] v_x vector with x-variables (internal TGraph parameter) of data for fitting
		 *  \param[in] v_y vector with y-variables (internal TGraph parameter) of data for fitting
		 * 	\return 0 if the fitting was failed (false returned by is_good_fit())
		 *  \return 1 if the fitting was good (true returned by is_good_fit())
		 * 	Number of fit iteration if fitting was failed is defined by #_max_refit
		 *  Fitting is started with:
		 *  - N fixed at #_fit_norm_param_fix
		 *  - Mean with initial value of #_min_param_val and limited to the range from #_min_param_val to #_max_param_val 
		 *  - Sigma with initial value of #_fit_noise_param_set and limited to the range from #_fit_noise_min_lim to #_fit_noise_max_lim
		 */
		int						 fit_error_function(std::vector<int> &v_x, std::vector<int> &v_y );
		
		/*! \brief Find minimum and maximum values in vector #v
		 * 	\details  \param[in] input data vector
		 * \param[out] min minimum value in the #v vector
		 * \param[out] max maximum value in the #v vector
		 * This function is used by fit_error_function() method to find minimum and maximum y-coordinate variables of the input data. 
		 * This is neede=d for limiting the N parameter in Erf function.
		 */
		void 					 find_min_max(std::vector<int> &v, int &min, int &max);
		TGraph* 				 get_S_curve();					 //!< \return TGraph with the Erf Fit
		std::pair<double,double> get_S_curve_fit_params();		 //!< \return std::pair with the first equal to fit 1 parameter (mean) and second to 2 parameter (noise, sigma)
		void					 make_fit(TGraph* gr, TF1* f);	 //!< Calls ROOT::Fit method, gets the fit parameters and saves in #fit_params
		
		/*! \brief Checks if fit is good
		 * 	\details It takes the values stored in #fit_params that are filled by make_fit method. Basing on values stored inthere, verifies if fit is good or not.
		 * 	It is good if:
		 * 
		 * 	#_min_accepted_noise > noise (sigma of Erf) > #_max_accepted_noise 
		 *  AND
		 * 	#_min_param_val > mean > #_max_param_val;
		 *  \return true if above condition is fulfilled
		 *  \return false if above condition is not fulfilled
		 */
		bool 					 is_good_fit();
		void 					 clear(); //!< clears the fit (nulls the #s_curve pointer) and fit parameters stored in #fit_param
		
		/*! \brief Minimum value of the scanned analysis parameter (min of x-coordinate variables). 
		*	\details Taken from config_file.cfg. If mean (Erf mean, fit parameter) is below that value, fit is flaged as  failed
		*/
		int 	_min_param_val; 
		/*! \brief Maximum value of the scanned analysis parameter (min of x-coordinate variables). 
		*	\details Taken from config_file.cfg.  If mean (Erf mean, fit parameter) is above that value, fit is flaged as  failed
		*/
		int 	_max_param_val; 

	private:
		TEnv config;
		
		int 	_fit_norm_param_fix; 	//!< From config_file. Fixed value of fit 0 param (N in Erf, norm constant). 
		double 	_fit_noise_param_set;	//!< From config_file. Initial value of noise (Erf sigma).
		double 	_fit_noise_min_lim;		//!< From config_file. Minimum value of fitting range for sigma and the down condition for accepting fit as a good (noise above this value)
		double 	_fit_noise_max_lim;		//!< From config_file. Maximum value of fitting range for sigma and the up condition for accepting fit as a good (noise below this value)
		double 	_min_accepted_noise;	//!< From config_file. If noise (Erf sigma, fit parameter) is below that value, fit is flaged as  failed
		double 	_max_accepted_noise;	//!< From config_file. If noise (Erf sigma, fit parameter) is above that value, fit is flaged as  failed
		int 	_max_refit;				//!< From config_file. Number of fit iteration (fit repetition) to get a good fit (see is_good_fit())
		double 	_fit_tolerance;			//!< SetDefaultTolerance value in ROOT::Math::MinimizerOptions	
		int 	_fit_max_it;			//!< SetDefaultMaxIterations value in ROOT::Math::MinimizerOptions
		int 	_fit_fun_cal;			//!< SetDefaultMaxFunctionCalls value in ROOT::Math::MinimizerOptions
		
};

#endif
