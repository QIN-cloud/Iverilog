module main;

reg  [3:0] a;

always@(a)
begin
	a[0] = 1'b1;
	a[1] = 1'b0;
	a[2] = 1'b1;
	a[3] = 1'b1;
	a[1] = 1'b1;
	a[3] = 1'b0;
end

endmodule
