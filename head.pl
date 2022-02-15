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