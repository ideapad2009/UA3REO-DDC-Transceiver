// DEBUG_I_TX.v

// Generated using ACDS version 17.1 593

`timescale 1 ps / 1 ps
module DEBUG_I_TX (
		input  wire [15:0] probe  // probes.probe
	);

	altsource_probe_top #(
		.sld_auto_instance_index ("YES"),
		.sld_instance_index      (0),
		.instance_id             ("I_TX"),
		.probe_width             (16),
		.source_width            (0),
		.enable_metastability    ("NO")
	) in_system_sources_probes_0 (
		.probe (probe)  // probes.probe
	);

endmodule
