// megafunction wizard: %FIR II v18.1%
// GENERATION: XML
// rx_ciccomp.v

// Generated using ACDS version 18.1 625

`timescale 1 ps / 1 ps
module rx_ciccomp (
		input  wire        clk,              //                     clk.clk
		input  wire        reset_n,          //                     rst.reset_n
		input  wire [31:0] ast_sink_data,    //   avalon_streaming_sink.data
		input  wire        ast_sink_valid,   //                        .valid
		input  wire [1:0]  ast_sink_error,   //                        .error
		output wire [63:0] ast_source_data,  // avalon_streaming_source.data
		output wire        ast_source_valid, //                        .valid
		output wire [1:0]  ast_source_error  //                        .error
	);

	rx_ciccomp_0002 rx_ciccomp_inst (
		.clk              (clk),              //                     clk.clk
		.reset_n          (reset_n),          //                     rst.reset_n
		.ast_sink_data    (ast_sink_data),    //   avalon_streaming_sink.data
		.ast_sink_valid   (ast_sink_valid),   //                        .valid
		.ast_sink_error   (ast_sink_error),   //                        .error
		.ast_source_data  (ast_source_data),  // avalon_streaming_source.data
		.ast_source_valid (ast_source_valid), //                        .valid
		.ast_source_error (ast_source_error)  //                        .error
	);

endmodule
// Retrieval info: <?xml version="1.0"?>
//<!--
//	Generated by Altera MegaWizard Launcher Utility version 1.0
//	************************************************************
//	THIS IS A WIZARD-GENERATED FILE. DO NOT EDIT THIS FILE!
//	************************************************************
//	Copyright (C) 1991-2020 Altera Corporation
//	Any megafunction design, and related net list (encrypted or decrypted),
//	support information, device programming or simulation file, and any other
//	associated documentation or information provided by Altera or a partner
//	under Altera's Megafunction Partnership Program may be used only to
//	program PLD devices (but not masked PLD devices) from Altera.  Any other
//	use of such megafunction design, net list, support information, device
//	programming or simulation file, or any other related documentation or
//	information is prohibited for any other purpose, including, but not
//	limited to modification, reverse engineering, de-compiling, or use with
//	any other silicon devices, unless such use is explicitly licensed under
//	a separate agreement with Altera or a megafunction partner.  Title to
//	the intellectual property, including patents, copyrights, trademarks,
//	trade secrets, or maskworks, embodied in any such megafunction design,
//	net list, support information, device programming or simulation file, or
//	any other related documentation or information provided by Altera or a
//	megafunction partner, remains with Altera, the megafunction partner, or
//	their respective licensors.  No other licenses, including any licenses
//	needed under any third party's intellectual property, are provided herein.
//-->
// Retrieval info: <instance entity-name="altera_fir_compiler_ii" version="18.1" >
// Retrieval info: 	<generic name="filterType" value="single" />
// Retrieval info: 	<generic name="interpFactor" value="1" />
// Retrieval info: 	<generic name="decimFactor" value="1" />
// Retrieval info: 	<generic name="symmetryMode" value="nsym" />
// Retrieval info: 	<generic name="L_bandsFilter" value="1" />
// Retrieval info: 	<generic name="inputChannelNum" value="1" />
// Retrieval info: 	<generic name="clockRate" value="122.880" />
// Retrieval info: 	<generic name="clockSlack" value="0" />
// Retrieval info: 	<generic name="inputRate" value="0.048" />
// Retrieval info: 	<generic name="coeffReload" value="false" />
// Retrieval info: 	<generic name="baseAddress" value="0" />
// Retrieval info: 	<generic name="readWriteMode" value="read_write" />
// Retrieval info: 	<generic name="backPressure" value="false" />
// Retrieval info: 	<generic name="deviceFamily" value="Cyclone IV E" />
// Retrieval info: 	<generic name="speedGrade" value="medium" />
// Retrieval info: 	<generic name="delayRAMBlockThreshold" value="20" />
// Retrieval info: 	<generic name="dualMemDistRAMThreshold" value="1280" />
// Retrieval info: 	<generic name="mRAMThreshold" value="1000000" />
// Retrieval info: 	<generic name="hardMultiplierThreshold" value="10" />
// Retrieval info: 	<generic name="reconfigurable" value="false" />
// Retrieval info: 	<generic name="num_modes" value="2" />
// Retrieval info: 	<generic name="reconfigurable_list" value="0" />
// Retrieval info: 	<generic name="MODE_STRING" value="None Set" />
// Retrieval info: 	<generic name="channelModes" value="0,1,2,3" />
// Retrieval info: 	<generic name="inputType" value="int" />
// Retrieval info: 	<generic name="inputBitWidth" value="32" />
// Retrieval info: 	<generic name="inputFracBitWidth" value="0" />
// Retrieval info: 	<generic name="coeffSetRealValue" value="6489.0,-13289.0,22160.0,-32364.0,40621.0,-38727.0,10008.0,75052.0,-264681.0,631855.0,-1282270.0,2360646.0,-4060242.0,6627016.0,-1.0370763E7,1.5664226E7,-2.2958472E7,3.2774559E7,-4.5731538E7,6.2530539E7,-8.4015755E7,1.11156709E8,-1.45188301E8,1.87614385E8,-2.40536763E8,3.06754243E8,-3.9054487E8,4.97979876E8,-6.38268954E8,8.20993735E8,-1.036162315E9,1.0737418E9,1.0737418E9,-1.036162315E9,8.20993735E8,-6.38268954E8,4.97979876E8,-3.9054487E8,3.06754243E8,-2.40536763E8,1.87614385E8,-1.45188301E8,1.11156709E8,-8.4015755E7,6.2530539E7,-4.5731538E7,3.2774559E7,-2.2958472E7,1.5664226E7,-1.0370763E7,6627016.0,-4060242.0,2360646.0,-1282270.0,631855.0,-264681.0,75052.0,10008.0,-38727.0,40621.0,-32364.0,22160.0,-13289.0,6489.0" />
// Retrieval info: 	<generic name="coeffSetRealValueImag" value="0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.0530093, -0.04498, 0.0, 0.0749693, 0.159034, 0.224907, 0.249809, 0.224907, 0.159034, 0.0749693, 0.0, -0.04498, -0.0530093, -0.0321283, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0" />
// Retrieval info: 	<generic name="coeffScaling" value="auto" />
// Retrieval info: 	<generic name="coeffType" value="int" />
// Retrieval info: 	<generic name="coeffBitWidth" value="26" />
// Retrieval info: 	<generic name="coeffFracBitWidth" value="31" />
// Retrieval info: 	<generic name="coeffComplex" value="false" />
// Retrieval info: 	<generic name="karatsuba" value="false" />
// Retrieval info: 	<generic name="outType" value="int" />
// Retrieval info: 	<generic name="outMSBRound" value="trunc" />
// Retrieval info: 	<generic name="outMsbBitRem" value="0" />
// Retrieval info: 	<generic name="outLSBRound" value="round" />
// Retrieval info: 	<generic name="outLsbBitRem" value="0" />
// Retrieval info: 	<generic name="bankCount" value="1" />
// Retrieval info: 	<generic name="bankDisplay" value="0" />
// Retrieval info: </instance>
// IPFS_FILES : rx_ciccomp.vo
// RELATED_FILES: rx_ciccomp.v, dspba_library_package.vhd, dspba_library.vhd, auk_dspip_math_pkg_hpfir.vhd, auk_dspip_lib_pkg_hpfir.vhd, auk_dspip_avalon_streaming_controller_hpfir.vhd, auk_dspip_avalon_streaming_sink_hpfir.vhd, auk_dspip_avalon_streaming_source_hpfir.vhd, auk_dspip_roundsat_hpfir.vhd, altera_avalon_sc_fifo.v, rx_ciccomp_0002_rtl_core.vhd, rx_ciccomp_0002_ast.vhd, rx_ciccomp_0002.vhd
