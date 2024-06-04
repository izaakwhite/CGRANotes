module EQ8 (input [7:0] I0, input [7:0] I1, output  O);
wire  LUT6_2_inst0_O5;
wire  LUT6_2_inst0_O6;
wire  MUXCY_inst0_O;
wire  LUT6_2_inst1_O5;
wire  LUT6_2_inst1_O6;
wire  MUXCY_inst1_O;
wire  LUT6_2_inst2_O5;
wire  LUT6_2_inst2_O6;
wire  MUXCY_inst2_O;
wire  LUT6_2_inst3_O5;
wire  LUT6_2_inst3_O6;
wire  MUXCY_inst3_O;
LUT6_2 #(.INIT(64'h9009900900000000)) LUT6_2_inst0 (.I0(I0[0]), .I1(I1[0]), .I2(I0[1]), .I3(I1[1]), .I4(1'b0), .I5(1'b1), .O5(LUT6_2_inst0_O5), .O6(LUT6_2_inst0_O6));
MUXCY MUXCY_inst0 (.DI(LUT6_2_inst0_O5), .CI(1'b1), .S(LUT6_2_inst0_O6), .O(MUXCY_inst0_O));
LUT6_2 #(.INIT(64'h9009900900000000)) LUT6_2_inst1 (.I0(I0[2]), .I1(I1[2]), .I2(I0[3]), .I3(I1[3]), .I4(1'b0), .I5(1'b1), .O5(LUT6_2_inst1_O5), .O6(LUT6_2_inst1_O6));
MUXCY MUXCY_inst1 (.DI(LUT6_2_inst1_O5), .CI(MUXCY_inst0_O), .S(LUT6_2_inst1_O6), .O(MUXCY_inst1_O));
LUT6_2 #(.INIT(64'h9009900900000000)) LUT6_2_inst2 (.I0(I0[4]), .I1(I1[4]), .I2(I0[5]), .I3(I1[5]), .I4(1'b0), .I5(1'b1), .O5(LUT6_2_inst2_O5), .O6(LUT6_2_inst2_O6));
MUXCY MUXCY_inst2 (.DI(LUT6_2_inst2_O5), .CI(MUXCY_inst1_O), .S(LUT6_2_inst2_O6), .O(MUXCY_inst2_O));
LUT6_2 #(.INIT(64'h9009900900000000)) LUT6_2_inst3 (.I0(I0[6]), .I1(I1[6]), .I2(I0[7]), .I3(I1[7]), .I4(1'b0), .I5(1'b1), .O5(LUT6_2_inst3_O5), .O6(LUT6_2_inst3_O6));
MUXCY MUXCY_inst3 (.DI(LUT6_2_inst3_O5), .CI(MUXCY_inst2_O), .S(LUT6_2_inst3_O6), .O(MUXCY_inst3_O));
assign O = MUXCY_inst3_O;
endmodule

