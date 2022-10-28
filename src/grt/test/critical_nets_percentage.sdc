create_clock -name core_clock -period 1.78 [get_ports clk]  
set clk [lindex [all_clocks] 0]
set period [get_property $clk period]
set delay [expr $period * 0.2]
set_input_delay $delay -clock $clk [delete_from_list [all_inputs] [all_clocks]]
set_output_delay $delay -clock $clk [delete_from_list [all_outputs] [all_clocks]]