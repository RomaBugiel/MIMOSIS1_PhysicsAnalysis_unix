#!/bin/bash


run=44390
i=0

for vthreshold in {98..98..3}
do 

run=`expr $i + $run`

chip=44
matrix='B'

VCASN=${vthreshold}
backbias=1000

val_min=0
val_max=245
step=5

row_start=1
row_end=503

vplvph_shift=255

#column_start=0
#column_end=127

case ${matrix} in
A)
column_start=0
column_end=127
echo "--> matrix A analysed from ${column_start} to ${column_end}"
;;
B)
column_start=128
column_end=511
echo "--> matrix B analysed from ${column_start} to ${column_end}"
;;
C)
column_start=512
column_end=895
echo "--> matrix C analysed from ${column_start} to ${column_end}"
;;
D)
column_start=896
column_end=1023
echo "--> matrix D analysed from ${column_start} to ${column_end}"
;;
*)
column_start=0
column_end=127
echo "--> WRONG MATRIX INDEX PROVIDED, specify A, B, C or D. Default taken for column ranges: ${column_start} to ${column_end}"
;;
esac


mkdir -p ./outputData/run_${run}

rm 	config_file.cfg
touch 	config_file.cfg


#echo "_input_file_masked_pixel:	./masked/masked_pixels_no44_C_165_3000.txt" >> config_file.cfg	
echo "_input_file_masked_pixel:	./masked/empty.txt" >> config_file.cfg	

echo "_active_selected:		0" >> config_file.cfg		
echo "_input_file_active_selected:	active_selected.txt" >> config_file.cfg	

echo "_run:			${run}" >> config_file.cfg	
echo "_frames_in_run:		500" >> config_file.cfg	

echo "_val_min:	${val_min}" >> config_file.cfg	
echo "_val_max:	${val_max}" >> config_file.cfg	
echo "_step:		${step}" >> config_file.cfg	
echo "_incl:		" >> config_file.cfg		
echo "_excl:		" >> config_file.cfg	

echo "_nb_of_row:		504" >> config_file.cfg	
echo "_nb_of_column:		1024" >> config_file.cfg	
echo "_row_start:		${row_start}" >> config_file.cfg	
echo "_row_end:		${row_end}" >> config_file.cfg	
echo "_column_start:		${column_start}" >> config_file.cfg	
echo "_column_end:		${column_end}" >> config_file.cfg	

echo "_param_1:	VBB" >> config_file.cfg	
echo "_param_2:	VPH_fine" >> config_file.cfg	
echo "_param_3:	VTH" >> config_file.cfg	

echo "_param_1_value: 	${backbias}" >> config_file.cfg	
echo "_param_2_value:	0" >> config_file.cfg	
echo "_param_3_value:	${VCASN}" >> config_file.cfg	

echo "_enable_scan:		2" >> config_file.cfg	

echo "_dac_shift:		${vplvph_shift}" >> config_file.cfg	

echo "_input_tree_file_path:		../MIMOSIS1_DataConverter_unix/outputData/run_${run}" >> config_file.cfg	
echo "_input_tree_file_name_core:	mimosis1" >> config_file.cfg	
echo "_input_tree_name: 		mimosis1_raw_tree" >> config_file.cfg	
echo "_input_prefix: 			_noPLL" >> config_file.cfg	

echo "_output_tree_file_path:		./outputData/run_${run}" >> config_file.cfg	
echo "_output_tree_file_core:		mimosis1_phys" >> config_file.cfg	
echo "_output_tree_file_part:		VBB_${backbias}_VCASN_${VCASN}" >> config_file.cfg	
echo "_output_tree_name:		mimosis1_phys_tree" >> config_file.cfg	

echo "_fit_norm_param_fix:		500" >> config_file.cfg	
echo "_fit_noise_param_set:		10" >> config_file.cfg	
echo "_fit_noise_min_lim:		0" >> config_file.cfg	
echo "_fit_noise_max_lim:		50" >> config_file.cfg	
echo "_min_accepted_noise:		1" >> config_file.cfg		
echo "_max_accepted_noise:		100" >> config_file.cfg		
echo "_max_refit:				5" >> config_file.cfg	
echo "_fit_tolerance: 			1e-3" >> config_file.cfg	
echo "_fit_max_it:			1000" >> config_file.cfg	
echo "_fit_fun_cal:			1000" >> config_file.cfg	

echo "_saturation_lvl:			0.5" >> config_file.cfg	
echo "_noise_cut_low:			0" >> config_file.cfg	
echo "_noise_cut_high:			10000" >> config_file.cfg	
echo "_threshold_cut_low:		0" >> config_file.cfg	
echo "_threshold_cut_high:		10000000" >> config_file.cfg	


./mimosis1analyser

echo "" 
echo "ANALYSIS DONE -- CHECK RESULTS"

i=1

done









