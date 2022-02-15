module foo(a);
   output a;
   wire a = 1'b1 ;
endmodule

module main;
  wire b;

  foo foo2 (.a(b));
 
endmodule // main
