% Author: Igor Tryhub, 275235

% Start of the program.
% Firstly, the input correctness is checked.
% Then, numbers are given proper coordinates.
% After that, the program tries to set elements in pairs to fill up all
% the panel.
% Lately, the factors for those pairs (treated as two-digit numbers) are
% being found.
% Afterwards, the possible solutions are being filtered.
% And finally, the right solutions are being chosen by the
% 'select-solution' predicate.
% The solutions are written out not by instantiating the third variable
% of the 'solve' predicate, but by using 'write_solution' predicate
% instead.

solve(N,ListaList,_) :-
	N>1,
	M is N-1,
	length(ListaList,M),
	set_coord(ListaList,N,CoordList),
	gen_pairs(CoordList,N,PairedPanel),
	factors(PairedPanel,N,FactorList),
	get_comb(FactorList,N,[],CombList),
	select_solution(CombList,PairedPanel,Sol),
	msort(PairedPanel,SortedPairedPanel),
	write_solution(Sol,SortedPairedPanel).

% Sets coordinates to the elements on the input ListaList.
set_coord(ListaList,N,Rez) :-
	flat(ListaList,N,Flat,[]),
	put_coord(Flat,N,Rez,[],1,1).

% Flattens the input list of lists 'ListaList' and returns Rez as a
% result.
flat([],_,X,X).
flat([H|T],N,Rez,Acc) :-
	length(H,N),
	append(Acc,H,X),
	flat(T,N,Rez,X).

% Appends values X and coordinates X,Y to the elements and puts triplets
% into lists.
put_coord([],_,X,X,_,_).
put_coord([H|T],N,Rez,Acc,X,Y) :-
	integer(H),H>=0,
	append(Acc,[[X,Y,H]],W),
	next_coord(N,X,Y,NextX,NextY),
	put_coord(T,N,Rez,W,NextX,NextY).

% Auxilary predicate. Calculates what coordinates should be for the next
% element.
next_coord(N,X,Y,NextX,NextY) :-
	Y < N,
	!,
	NextY is Y+1,
	NextX=X.
next_coord(_,X,_,NextX,NextY) :-
	NextY=1,
	NextX is X+1.

% Packaging predicate. Decides which version of gen_pairs to choose
% from.
gen_pairs(CoordList,N,PairedPanel) :-
	N =:= 2,
	!,
	gen_pairs_2(CoordList,PairedPanel).
gen_pairs(CoordList,_,PairedPanel) :-
	gen_pairs_3(CoordList,PairedPanel).


% Generates all possible sets of pairs for a given 2-element panel and
% accumulates the result in Rez.
gen_pairs_2([],[]).
gen_pairs_2([[Xh,Yh,Valh]|T],Rez) :-
	append([[Valn,Xh,Yh,Xx,Yx]],Acc,Rez),
	select([Xx,Yx,Valx],T,S),
	is_neighb([Xh,Yh,Valh],[Xx,Yx,Valx]),
	atom_concat(Valh,Valx,Vals),
	atom_number(Vals,Valn),
	Valn>9,
	gen_pairs_2(S,Acc).
gen_pairs_2([[Xh,Yh,Valh]|T],Rez) :-
	append([[Valn,Xx,Yx,Xh,Yh]],Acc,Rez),
	select([Xx,Yx,Valx],T,S),
	is_neighb([Xh,Yh,Valh],[Xx,Yx,Valx]),
	atom_concat(Valx,Valh,Vals),
	atom_number(Vals,Valn),
	Valn>9,
	gen_pairs_2(S,Acc).

% Generates all possible sets of pairs for a given >2-element panel and
% accumulates the result in Rez. Checks if the paired elements are
% between 10-99 and if they are not prime numbers.
gen_pairs_3([],[]).
gen_pairs_3([[Xh,Yh,Valh]|T],Rez) :-
	append([[Valn,Xh,Yh,Xx,Yx]],Acc,Rez),
	select([Xx,Yx,Valx],T,S),
	is_neighb([Xh,Yh,Valh],[Xx,Yx,Valx]),
	atom_concat(Valh,Valx,Vals),
	atom_number(Vals,Valn),
	Valn>9,
	\+ member(Valn,[11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97]),
	gen_pairs_3(S,Acc).
gen_pairs_3([[Xh,Yh,Valh]|T],Rez) :-
	append([[Valn,Xx,Yx,Xh,Yh]],Acc,Rez),
	select([Xx,Yx,Valx],T,S),
	is_neighb([Xh,Yh,Valh],[Xx,Yx,Valx]),
	atom_concat(Valx,Valh,Vals),
	atom_number(Vals,Valn),
	Valn>9,
	\+ member(Valn,[11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97]),
	gen_pairs_3(S,Acc).

% Auxilary predicate. Checks if two elements on a panel are
% neighbouring.
is_neighb([X,Y,_],[NeighbX,NeighbY,_]) :-
	NeighbX is X, NeighbY is Y+1;
	NeighbX is X+1, NeighbY is Y+1;
	NeighbX is X+1, NeighbY is Y;
	NeighbX is X+1, NeighbY is Y-1;
	NeighbX is X, NeighbY is Y-1;
	NeighbX is X-1, NeighbY is Y-1;
	NeighbX is X-1, NeighbY is Y;
	NeighbX is X-1, NeighbY is Y+1.

% Packaging predicate. Decides which Min/Max parameters to transmit to
% get_factors.
factors(PairedPanel,N,FactorList) :-
	N > 2,
	!,
	get_factors(PairedPanel,2,24,[],FactorList).
factors(PairedPanel,2,FactorList) :-
	get_factors(PairedPanel,1,99,[],FactorList).

% Generates all possible factors for each element in a given list
get_factors([[Num|_]|T],Min,Max,Acc,Rez) :-
	search_factor(Num,Min,Max,[],Factors),
	append([Min,Max],[Factors|Acc],List),
	flatten(List,FlatList),
	max_list(FlatList,NewMax),
	min_list(FlatList,NewMin),
	get_factors(T,NewMin,NewMax,[Factors|Acc],Rez).
get_factors([],_,_,X,X).

% Generates all pairs of factors for a given number.
% The searh continues until Min is less or equal then sqrt(Num).
% If given Min produces a correct factor, it appends to the Acc, which
% is at the end transmitted to Rez.
% Otherwise, Min is increased by one and search continues.
search_factor(Num,Min,_,X,X) :-
	Min > sqrt(Num),
	!.
search_factor(Num,Min,Max,Acc,Factors) :-
    is_factor(Num,Min,Max,Pair),
    !,
    append(Pair,Acc,NewAcc),
    NewMin is Min+1,
    search_factor(Num,NewMin,Max,NewAcc,Factors)
    ;
    NewAcc = Acc,
    NewMin is Min+1,
    search_factor(Num,NewMin,Max,NewAcc,Factors).

% Auxilary predicate. Returns a pair of satisfiable factors to a list.
is_factor(Num,D,Max,[[D,R]]) :-
	Num mod D =:= 0,
	!,
	R is Num div D,
	R =< Max.

% Generates possibles combinations of N elements from a factor list and
% checks their validit by valid_comb.
get_comb(FactorList,N,[],Rez) :-
	pairs_comb(FactorList,[],Comb),
	flatten(Comb,FlatComb),
	msort(FlatComb,SortedComb),
	M is N-1,
	valid_comb(SortedComb,M,Rez).

% Auxilary predicate. Generates combinations of factor pairs.
pairs_comb([H|T],Acc,Rez) :-
	select(Pair,H,_),
	pairs_comb(T,[Pair|Acc],Rez).
pairs_comb([],X,X).

% Packaging predicate. Instantiates GlobalM.
valid_comb(SortedComb,M,Rez) :-
	GlobalM=M,
	valid_comb(SortedComb,GlobalM,M,[],[],Rez).

% Validates the correctness of factor combinations.
% The main idea is to choose combinations one-by-one, flatten them,
% msort them, and calculate if each of them is M*C = (N-1)*C on this
% list. If so, it is potential candidate for a correct solution. Others
% combinations are abandoned.
valid_comb([H|T],GlobalM,M,Acc,RezAcc,Rez) :-
	M>0,
	!,
	NewM is M-1,
	valid_comb(T,GlobalM,NewM,[H|Acc],RezAcc,Rez).
valid_comb([],_,_,[],X,Y) :-
	!,
	msort(X,Y).
valid_comb(Tail,GlobalM,0,[H|T],RezAcc,Rez) :-
	check_equality([H|T]),
	valid_comb(Tail,GlobalM,GlobalM,[],[H|RezAcc],Rez).

% Auxillary predicate. Packaging predicate.
check_equality([H|T]) :-
	!,
	check_equality(T,H).
check_equality([]).

% Checks if all the elements on the input list are equal.
check_equality([H|T],Value) :-
	H =:= Value,
	check_equality(T,Value).
check_equality([],_).


% Looks for a solution for N-element factor combination H
select_solution(H,List,H) :-
	get_heads(List,Values),
	is_solution(H,Values).

% Writes down the solutions in demanded format.
write_solution(Sol,SortedPairedPanel) :-
	write('('),write(Sol),write(','),nl,write(' ['),
	write_coord(SortedPairedPanel,1).
% Auxillary predicate. Writes the coordinates in the correct order.
write_coord([[_,X1,Y1,X2,Y2]|T],First):-
	First = 1,
	!,
	write('(('),write(X1),write(','),write(Y1),write('),('),write(X2),write(','),write(Y2),write(')),'),nl,
	write_coord(T,0).
write_coord([[_,X1,Y1,X2,Y2]|T],0):-
	\+ T = [],
	!,
	write('  (('),write(X1),write(','),write(Y1),write('),('),write(X2),write(','),write(Y2),write(')),'),nl,
	write_coord(T,0).
write_coord([[_,X1,Y1,X2,Y2]],_):-
	write('  (('),write(X1),write(','),write(Y1),write('),('),write(X2),write(','),write(Y2),write('))'),write('])').

% Auxillary predicate. Returns all heads H of sublists.
get_heads([],[]).
get_heads([[H|_]|T],X) :-
	append([H],NewRez,X),
	get_heads(T,NewRez).

% Auxillary predicate. Checks if a given N-element factor combination
% satisfies the set of pairs on a panel.
is_solution(CombElem,PanelSet) :-
	gen_set(CombElem,FactSet),
	sets_equal(FactSet,PanelSet).

% Auxillary predicate. Generates the set of production ProdSet for given set of factors
% FactSet.
gen_set(FactSet,ProdSet) :-
	get_uniq_pairs(FactSet,Pairs),
	flat_set(Pairs,Flat,[]),
	findall(Number,(member([X1,X2],Flat),Number is X1*X2),ProdSet).

% Auxillary predicate. Generates set of sets Rez of unique pairs on a
% given List.
get_uniq_pairs(List,Rez) :-
	pairs_loop(List,Rez).

% Auxillary predicate. Flattens the input list of lists one level down.
flat_set([],X,X).
flat_set([H|T],Rez,Acc) :-
	append(Acc,H,X),
	flat_set(T,Rez,X).

% Auxillary predicate. Controls the loop for pair generation predicate
% get_uniq_pairs.
pairs_loop([_],[]).
pairs_loop([H|T],[X|Y]) :-
	pairs_loop(T,Y),
	findall(Pair,uniq_pairs([H|T],Pair),X).

% Auxillary predicate. Selects one pair from a given list.
uniq_pairs([H|T],[H,X]) :-
	select(X,T,_).

% Auxillary predicate. Checks if given sets of elements X,Y are equal.
sets_equal(X,Y) :-
	msort(X,Sorted),
	msort(Y,Sorted).




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% checker.pl
% VERSION: 0.3
% Write in swipl:
%    ?- [your_code, checker].
%    ?- check_solution.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

:- [tests_5]. % Use name of the file with downloaded tests

check_solution :-
  write('START TESTING'), nl,
  statistics(cputime, T0),
  performTests,
  statistics(cputime, T1),
  DT is T1 - T0,
  write('DONE IN '),
  write(DT),
  write('s'),
  nl.

performTests :-
  takeTest ,nl,
  fail.
performTests.

takeTest :-
  taskArity(N),  % Predicate defined in testy_N, number of input arguments
  length(Args, N),
  (Name = simple_test; Name = student_simple_test; Name = count_test; Name = student_count_test),
  append([Name|Args], [Correct], L),
  GetTest =.. L,
  GetTest,
  append([solve|Args], [Result], L2),
  Task =.. L2,
  (member(Name, [simple_test, student_simple_test]) ->
      trySimpleTest(Name, Task, Correct, Result);
      tryCountTest(Name, Task, Correct, Result)
  ).


trySimpleTest(Name, Test, Correct, Result) :-
  Test,
  testSimpleSolution(Correct, Result), !,  % Predicate defined in testy_N
  write(Name), write(' [ok]').

trySimpleTest(Name, _, _, _) :-
  write(Name), write(' [failed]').

tryCountTest(Name, Test, Correct, Result) :-
  findall( Result, Test, Found),
  length(Found, N),
  write(Name),
  (N == Correct -> write(' [ok]'); write( ' [failed]')).




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Tests for task 5, Mozaika iloczynow
% VERSION: 0.2
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

taskArity(2). % 2 is the number of input arguments, solve has arity 3

testSimpleSolution( Correct, Result) :-
  Correct == Result.

simple_test(2, [ [2,0] ], ( [4,5], [ ((1,1),(1,2)) ]) ).
simple_test(3, [[1,6,1],[1,6,6]], ( [4,4,4], [((1,1),(1,2)), ((2,1),(2,2)), ((1,3),(2,3))] )).
simple_test(4, [[5,8,4,0], [5,3,0,4], [6,3,4,2]], ([5,6,7,8], [((3,2),(2,3)), ((2,2),(1,1)), ((2,4),(1,4)), ((3,3),(3,4)), ((1,3),(1,2)), ((2,1),(3,1))])).

count_test(4, [[5,8,4,0], [5,3,0,4], [6,3,4,2]], 1).
count_test(4, [[0,8,4,0], [5,3,0,4], [6,3,4,2]], 0).
count_test(3, [[2,0,3], [2,4,0]], 2).

student_simple_test(3, [[2,3,0],[0,5,3]], ([5,5,6], [((1,1),(2,2)),((2,1),(1,2)),((2,3),(3,1)) ])).
student_simple_test(3, [[3,6,6],[6,3,3]],([6, 6, 6],[((1,1),(1,2)), ((2,2),(2,1)), ((2,3),(1,3))])).
student_simple_test(4,[[0,1,1,2],[0,3,1,2],[0,3,6,3]],([2, 5, 6, 6],[((1,2),(1,1)), ((1,3),(1,4)), ((2,3),(2,4)), ((2,2),(2,1)), ((3,2),(3,1)), ((3,4),(3,3))])).
student_simple_test(5,[[5,3,3,0,2],[0,2,2,5,5],[3,3,6,3,3],[0,0,3,0,0]],([5, 5, 5, 6, 6],[((1,5),(2,5)), ((2,2),(1,1)), ((1,2),(2,1)), ((1,3),(1,4)), ((2,3),(2,4)), ((3,1),(4,1)), ((3,2),(4,2)), ((3,4),(4,4)),((3,5),(4,5)),((4,3),(3,3))])).

student_count_test(2, [[1,2]], 5).
student_count_test(3,[[3,6,6],[6,3,3]],18).
