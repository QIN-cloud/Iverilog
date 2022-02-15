module main(a, b, c, d);
parameter width = 2;
input [width - 1:0] a, b;
output [width - 1:0] c, d;
reg [width - 1:0] c, d;

always@(a or b)
begin//:begin1
  //c = a;
  d = b;//2
  if(a)//3
  begin
  	c = a & b;//4
  	d = a | b;//5
  end
  else if(b)
   	d = a & b;  //7
end
endmodule