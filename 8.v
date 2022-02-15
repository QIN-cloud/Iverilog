module main;

   wire   Y;
   reg 	  A, B, S;

   muxx1 MUX2 ( Y, S, A, B ) ;

endmodule

primitive muxx1(Q, S, A, B);
   output Q;
   input  S, A, B;

   table
   //   S  A  B    Q
        0  0  ?  : 0 ;
        0  1  ?  : 1 ;
        0  x  ?  : x ;  // problem line
        1  ?  0  : 0 ;
        1  ?  1  : 1 ;
        1  ?  x  : x ;  // problem line
        x  0  0  : 0 ;
        x  1  1  : 1 ;

   endtable
endprimitive

