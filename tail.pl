	lab(Lab, INPUTS).

/*----------- End of model definition -----------*/

lab(normal,L):-
        fd_labeling(L).

lab(ff,L):-
        fd_labelingff(L).

get_fd_labeling(Lab):-
        argument_counter(C),
        get_labeling1(C,Lab).

get_labeling1(1,normal).

get_labeling1(2,Lab):-
        argument_value(1,Lab).