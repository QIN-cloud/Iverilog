q(OUTPUTS):-
	get_fd_labeling(Lab),
	statistics(runtime, _),
	
	findall(INPUTS, temp(Lab,INPUTS,OUTPUTS) , INLIST),
	temp(Lab,INPUTS,OUTPUTS),
	statistics(runtime, [_,Y]),
	/*write(OUTPUTS), nl,
	write(INPUTS), nl,*/
	output_results(INLIST),
	write('time: '), write(Y), nl.

	output_results([]).
	output_results([IN | REST]) :-
	write('input: '), write(IN), nl,
	output_results(REST).

/*----------- Model definition -----------*/

temp(Lab, INPUTS, OUTPUTS):-


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
