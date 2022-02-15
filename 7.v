// Template 1

module mod1 ;
endmodule

// Template 2
module mod2 ();
endmodule

// Template 3
module mod3 (a,b);
input a;
input b;
endmodule

//Template 4  - 
module mod4 (ident1,out1);
input  [31:0]  ident1;
output [31:0]  out1;

wire [31:0] out1 ;
assign out1 = ident1;
mod3 inst3 (ident1[0], ident1[1]);
endmodule

module main ();

wire [31:0] out1,out2;
reg  [31:0] val1,val2;
reg error;

mod4 inst1 (val1,out1);   		// Ordered port list
mod4 inst2 (val2,out2); // List by portname

endmodule // main
