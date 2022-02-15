module fifo (clk, fifo_clr_n, fifo_reset_n,
put, // put strobe, active high
get, // get strobe, active high
data_in, data_out
);
  // FIFO parameters
  parameter fifo_width = 16;
  parameter fifo_depth = 8;
  parameter fifo_pntr_w = 3;
  parameter fifo_cntr_w = 4;
  
  input clk, fifo_clr_n, fifo_reset_n, put, get;
  input [fifo_width-1:0] data_in;
  output [fifo_width-1:0] data_out;
  wire [fifo_width-1:0] data_out;
  // FIFO itself
  reg [fifo_width-1:0] fifo[fifo_depth-1:0];
  // FIFO pointers
  reg [fifo_pntr_w-1:0] top; // top
  reg [fifo_pntr_w-1:0] btm; // bottom
  reg [fifo_cntr_w-1:0] cnt; // count
  integer i;
  
  always @(posedge clk or negedge fifo_clr_n)
  begin
    if (fifo_clr_n == 1'b0) begin
      top <= {fifo_pntr_w {1'b0}};
      btm <= {fifo_pntr_w {1'b0}};
      for (i=0; i<fifo_depth; i=i+1)
        fifo[i] <= {fifo_width{1'b0}};
      cnt <= {fifo_cntr_w {1'b0}};
    end
    else if (fifo_reset_n == 1'b0) begin
      top <= {fifo_pntr_w {1'b0}};
      btm <= {fifo_pntr_w {1'b0}};
      cnt <= { fifo_cntr_w {1'b0}};
    end
    else begin
      case ({put, get})
        2'b10 : // WRITE
          if (cnt<fifo_depth) begin
            fifo[top] <= data_in;
            top <= top + 1;
            cnt <= cnt + 1;
          end
        2'b01 : //READ
          if(cnt>0) begin
            fifo[btm] <= 0;
            btm <= btm + 1;
            cnt <= cnt - 1;
          end
        2'b11 : // WRITE & READ
          begin
            fifo[btm] <= 0;
            fifo[top] <= data_in;
            btm <= btm + 1;
            top <= top + 1;
          end
      endcase
    end
  end // always

  assign data_out = fifo[btm];
  // Assert that the FIFO cannot overflow
  //assert never ({put,get}==2'b10 && cnt==fifo_depth-1) @(posedge clk);
  // Assert that the FIFO cannot underflow
  //assert never (get && cnt==0) @(posedge clk);

endmodule