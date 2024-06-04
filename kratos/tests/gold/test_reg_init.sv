module test (
  input logic clk,
  input logic in,
  input logic rst,
  output logic out
);

logic a;
logic b;

always_ff @(posedge clk, posedge rst) begin
  if (rst) begin
    a <= 1'h0;
    b <= 1'h0;
  end
  else begin
    a <= in;
    b <= a;
  end
end
assign out = b;
endmodule   // test

