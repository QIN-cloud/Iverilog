`timescale 1ns/1ps

module top;
  realtime rtime;
  time itime;

  initial begin
    repeat (5) begin
      rtime = $realtime;
      itime = $time;
      $display("%4t %.2f %2d, %4t %0.2f %2d, %4t %.2f, %4t %0.2f, %4t %.2f,",
               $time, $time, $time, itime, itime, itime,
               $realtime, $realtime, rtime, rtime, rtm($realtime),
               rtm($realtime),, $time,, $realtime);
      #0.6;
    end
  end

  function real rtm;
    input real rin;
    rtm = rin;
  endfunction
endmodule
