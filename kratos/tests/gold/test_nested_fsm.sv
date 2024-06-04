module mod (
  input logic clk,
  input logic [1:0] in,
  input logic rst,
  output logic [1:0] out
);

typedef enum logic[1:0] {
  Color_Blue = 2'h0,
  Color_Red = 2'h1,
  HSV_idle = 2'h2
} Color_state;
Color_state Color_current_state;
Color_state Color_next_state;

always_ff @(posedge clk, posedge rst) begin
  if (rst) begin
    Color_current_state <= Color_Red;
  end
  else Color_current_state <= Color_next_state;
end
always_comb begin
  Color_next_state = Color_current_state;
  unique case (Color_current_state)
    Color_Blue: begin
        if (in == 2'h1) begin
          Color_next_state = Color_Red;
        end
      end
    Color_Red: begin
        if (in == 2'h0) begin
          Color_next_state = Color_Red;
        end
        else if (in == 2'h1) begin
          Color_next_state = Color_Blue;
        end
        else if (in == 2'h2) begin
          Color_next_state = HSV_idle;
        end
      end
    HSV_idle: begin
        if (in == 2'h0) begin
          Color_next_state = Color_Red;
        end
      end
    default: Color_next_state = Color_current_state;
  endcase
end
always_comb begin
  unique case (Color_current_state)
    Color_Blue: begin :Color_Color_Blue_Output
        out = 2'h1;
      end :Color_Color_Blue_Output
    Color_Red: begin :Color_Color_Red_Output
        out = 2'h2;
      end :Color_Color_Red_Output
    HSV_idle: begin :Color_HSV_idle_Output
        out = 2'h2;
      end :Color_HSV_idle_Output
    default: begin :Color_default_Output
        out = 2'h2;
      end :Color_default_Output
  endcase
end
endmodule   // mod

