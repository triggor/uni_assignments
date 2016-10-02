{-**********************************************************
Author:	Igor Tryhub
Student ID: 275235
Program description: Solution for the Puzzle #5 in Haskell
Tested with: GHCi-7.6.3 interpreter 
Due date: June 3rd, 2015
************************************************************
The overall algorithm of the program is the following:

	1) provide the coordinates for the input elements;
	2) consider which pairs can be formed using the firstmost element on the list, if any;
	3) construct such pairs;
	4) consider possible factorizations for every pair, if any;
	5) repeat starting from Step 2 reccursively until the end of the element list, 
	   passing on the obtained in previous steps data
	6) process the obtained results and output as a list. 
************************************************************
{- Caution!!! 
The program doesn't comply with the Test #2, because it was written according to the specification. In particular, it considers only the solutions which are substantially different, without caring about possible permutations in the order of products with the same value. -}
**********************************************************-}

import Puzzle
import Checker
import Data.List --importing the definition of the "sort" function

main :: IO()
main = checkerMain solve tests

solve :: Solver
solve n panel = concat[reccurOne n ([],[],x)|x<-(determFirsts n (setCoord panel))]

-----TYPE DEFINITIONS-----------------------------------------//

type Size = Int
type InputPanel = [[Int]]
type CoordTriple = (Int,Int,Int)
type CoordPanel = [(Int,Int,Int)]
type Factors = [Int]
type FactorList = [[Int]]
type SelectedPair = (Int,Int,Int,Int,Int)
type SelectedPairs = [(Int,Int,Int,Int,Int)]

-----HANDLING THE RECCURENCE----------------------------------//

{- 	this function is the "heart" of the program; it serves as a packaging for its overall structure;
	works reccursively until all the possible results for a given input data are considered;
	as an input takes a list of sequences in which the head of the element list can be set in pairs;
	then, the first two elements of the element list are set into pairs and factorized;
	all the possible combinations of set pairs and their factors are passed on to the shorter tails, 
	so that it can be decided later on if such a combination is possible or not;
	the function has many conditions to check; it cuts unappropriate "branches" on its way all the time:
	when elements are not neighbours, when they compose a pair which value is <10 or a prime number (for n>2),
	when the bigger of the factors is >24 (for n>2), or when the factor list is consisted of too many 
	different factors; the pathes which withstand all these obstacles are processed be auxilary functions 
	before they get into the Result list, so that they comply with the specification; if no solutions 
	are found, the function returns an empty list -}
reccurOne :: Size -> (Factors,SelectedPairs,CoordPanel) -> [Result] -- Size = Int; Factors = [Int]; SelectedPairs = [(Int,Int,Int,Int,Int)]; CoordPanel = [(Int,Int,Int)]
reccurOne n (factors,selpairs,notselected) = 
	if null notselected 
	then 	if correctFacts n (sort factors) selpairs && correspond -- if the solution is correct
		then [(finfacts,finpairs)]
		else []
	else concat indStep -- futher reccursion
		where	indStep = [(reccurOne n ([x,y]++factors,newpair:selpairs,newnotselect))|[x,y]<-(factorize n (getNum newpair)),suitable(x,y),newnotselect<-possibleSel]
			newpair = pairNeighb (head notselected) (head $ tail notselected)
			possibleSel = 	if null (drop 2 notselected) -- if in this step two last lements were set in a pair
					then [[]] 
					else calcRest n notselected
			suitable(x,y) = isSuitable n (x,y) factors 
			correspond = compare (sort [getNum num|num<-(sort selpairs)]) (sort (multiplyComb finfacts)) == EQ-- if the selected pairs correspond to the factors calculated for them; for most cases the program can do without this test, but for some cases in the past it was a solutions for excessive results
			finfacts = getFacts n (sort factors)
			finpairs = map getCoords (multOrder n (multiplyComb finfacts) selpairs []) 

-----CONSTRUCTION OF A COORDINATED PANEL----------------------//

{-	transforms input panel into panel with coordinates (value,x,y);
	firstly, the y-coordinate is introduced to each row;
	then x-coordinate is provided for each pair (y-coord,value);
	at the end, x-coordinate is pushed into each pair while simultaniously 
	changing the order in tuples -}
setCoord :: InputPanel -> CoordPanel -- InputPanel = [[Int]]; CoordPanel = [(Int,Int,Int)]
setCoord panel = concatMap (insertX) $ zip [1..] $ map (zip [1..]) panel

	{- auxilary function; introduces x-coordinate to each pair (y-coord,value) -}
insertX :: (Int,[(Int,Int)]) -> CoordPanel -- CoordPanel = [(Int,Int,Int)]
insertX (x,((y,v):tail)) = map (makeTriple x) ((y,v):tail)

	{- auxilary function; pushes x-coordinate inside tuples and 
	puts elements of a coordinate triple into correct order: (value,x-coord,y-coord) -}
makeTriple :: Int -> (Int,Int) -> CoordTriple -- CoordTriple = (Int,Int,Int)
makeTriple x (y,v) = (v,x,y)

-----PRODUCING COMBINATIONS WITH FIRST ELEMENT BINDED---------//

{-	produces all the ways the first element of a given panel can be linked with its neighbours;
 	checks if two given elements (values with corresponding coordinates) can make 
	a pair on a panel of Size;
	the function takes into consideration two cases: x>2 and x==2; 
	if x>2, it ignores numbers if they are prime because their gcd with other numbers is 1
	the output list consists out of two main parts:
	in the first one there are all the combinations where the first element is on the 1st position;
	in another one there are all the combinations with others where the first element is on the 2nd position;
	terminates when the input list is empty -}
determFirsts :: Size -> CoordPanel -> [CoordPanel] -- Size = Int; CoordPanel = [(Int,Int,Int)]
determFirsts n [] = []
determFirsts n ((v1,x1,y1):xs) = 
	[((v1,x1,y1):(v2,x2,y2):ys)|((v2,x2,y2):ys)<-frontrest,isneighb(x1,y1,x2,y2),num(v1,v2)>=10,notprime(v1,v2)]++
	[((v2,x2,y2):(v1,x1,y1):ys)|((v2,x2,y2):ys)<-frontrest,isneighb(x1,y1,x2,y2),num(v2,v1)>=10,notprime(v2,v1)]
		where	frontrest = each2front xs
			isneighb(x1,y1,x2,y2) = abs (x1-x2) <= 1 && abs (y1-y2) <= 1
			num(e1,e2) = 10*e1+e2
			notprime(e1,e2) = n==2 || (n>2 && notElem (num(e1,e2)) primes)
			primes = [11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97]

{- 	auxilary function; brings every element of the tail of the list to the front position;
	terminates when the imput list is empty -}
each2front :: CoordPanel -> [CoordPanel] -- CoordPanel = [(Int,Int,Int)]
each2front [] = []
each2front (x:xs) = (x:xs):[(fromxs:x:newxs)|(fromxs:newxs)<-each2front xs] 

{- 	auxilary function for defining the non-selected list of elements for the recursive step;
	produces all the ways the first element of a given panel without first two elements 
	(which are already set into a pair) can be linked with all others;
	the output list consists out of lists of possible selection of the following pair;
	the case when the input list is empty is ommited due to a check in the calling function -}
calcRest :: Size -> CoordPanel -> [CoordPanel] -- Size = Int; CoordPanel = [(Int,Int,Int)]
calcRest n notselected = determFirsts n (drop 2 notselected)

-----PAIRING UP THE ELEMENTS FROM THE PANEL-------------------//

{- 	pairs given elments together into a 5-elem tuple:
	(Number,x-,y-coords of the 1st elem,x-,y-coords of the 2nd elem);
	we can be sure that the given elements form an appropriate pair, because necessary conditions 
	were checked in the function determFirsts -}
pairNeighb ::  CoordTriple -> CoordTriple -> SelectedPair -- CoordTriple = (Int,Int,Int); SelectedPair = (Int,Int,Int,Int,Int)
pairNeighb (v1,x1,y1) (v2,x2,y2) = (num,x1,y1,x2,y2)
	where num = 10*v1+v2


{- 	pulls out a value number from a given selected pair -}
getNum :: SelectedPair -> Int -- SelectedPair = (Int,Int,Int,Int,Int)
getNum (num,x1,y1,x2,y2) = num

{- 	pulls out coordinates from a given selected pair and produces pairs of coordinates -}
getCoords :: SelectedPair -> ((Int,Int),(Int,Int)) -- SelectedPair = (Int,Int,Int,Int,Int)
getCoords (num,x1,y1,x2,y2) = ((x1,y1),(x2,y2))

-----DECOMPOSING A NUMBER ON FACTORS AND THEIR PROCESSING-----//

{- 	for the given number in a SelectedPair this function produces a list of two-element lists 
	of possible factor decomposition;
	the first of the factors is always <= the sqrt of the number;
	the predifined function sqrt works on Floating input, so num has to be casted from Int;
	if the bigger of two factors (fact2) is >=25 for Size>2, such a factor decomposition is ignored -}
factorize :: Size -> Int -> FactorList -- Size = Int; FactorList = [[Int]]
factorize n num = [[fact1,fact2]|fact1<-[1..f],fact2<-[1..num],not2big(fact2),(fact1*fact2)==num] 
	where 	f = floor $ sqrt (fromIntegral num)  
		not2big(fact2) = n==2 || fact2<25

{- 	checks if the given pair of factors suits to the ones accumulated in previous steps;
	returns True either if the factors list is empty or each of its elements multipled by
	the elements of the pair results in a number between 10 and 99;
	additionaly, it assures that from some length of the factors list on at least one 
	of the pair elements should be shared with the accumulated factors list;
	when new factors are being added to the factors list they are checked if they form any
	products greater than 99 or less then 10 -}
isSuitable :: Size -> (Int,Int) -> Factors -> Bool -- Size = Int; Facors = [Int]
isSuitable n (x,y) factors = null factors || iselem
	where	iselem = (n<4 || (length factors) <= 2*factLength(n-2) || elem x factors || elem y factors) && inlimits
		inlimits = length uniqfacts <= n && and (map (>=10) products) && and (map (<100) products)
		uniqfacts = map head $ group $ sort factors
		products = [new*old|new<-[x,y], old<-uniqfacts,new/=old]

{- 	checks if Factors produced by some hypothetically correct grouping of the panel in pairs
	are appropriate for the Size of the puzzle; the function flows recursively; 
	returns True if the Factors are correct for such a Size, and False otherwise  -}
correctFacts :: Size -> Factors -> SelectedPairs -> Bool -- Size = Int; Factors = [Int]; SelectedPairs = [(Int,Int,Int,Int,Int)]
correctFacts n facts pairs = 
	if firsteq`mod`(n-1) == 0
	then 	if rest == []
		then True 
		else correctFacts n rest pairs
	else False
		where 	firsteq = length (takeWhile (==(head facts)) facts)
			rest = drop firsteq facts

{-	calculates the number of possible grouping of two-element pairs in n-element set;
	the function works recursively -}
factLength :: Size -> Int -- Size = Int
factLength 2 = 1
factLength x = (factLength (x-1))+x-1

{- 	auxilary function; pulls out single factors out of factor list with their multiple occurence;
	it returns a list composed of every (n-1)st element out of the input sorted list of facts -}
getFacts :: Size -> Factors -> Factors -- Size = Int; Factors = [Int]
getFacts n facts = [facts !!((n-1)*k)|k<-[0..(n-1)]]

{- 	multiplies the final list of factors with each other and produces their products -}
multiplyComb :: Factors -> [Int] -- Factors = [Int]
multiplyComb [] = []
multiplyComb (x:xs) = (map (x*) xs) ++ multiplyComb xs

-----SORTING THE OUTPUT FOR PAIRS-----------------------------//

{- 	sorts the input SelectedPairs in the same order as they are produced from sorted factors multiplication;
	works reccursively using accumulator for collected SelectedPairs;
	when the list of factors is empty, the accumulated pairs should be reversed -}
multOrder :: Size -> Factors -> SelectedPairs -> SelectedPairs -> SelectedPairs -- Size = Int; Factors = [Int]; SelectedPairs = [(Int,Int,Int,Int,Int)]
multOrder n multcomb selpairs acc = 
	if null multcomb then reverse acc 
	else multOrder n (tail multcomb) (delete newpair selpairs) newacc
		where 	newacc = newpair:acc
			newpair = ordPairs multcomb selpairs

{- 	auxilary function for pulling out one SelectedPair in a single step of the multOrder function;
	 works reccursively untill the correspondent element is found-}
ordPairs :: Factors -> SelectedPairs -> SelectedPair -- Factors = [Int]; SelectedPair = (Int,Int,Int,Int,Int); SelectedPairs = [(Int,Int,Int,Int,Int)]
ordPairs multcomb selpairs =
	if (getNum (head selpairs))==(head multcomb) 
		then head selpairs
	else ordPairs multcomb (tail selpairs)

-----TESTS----------------------------------------------------//

tests :: [Test]
tests = [	SimpleTest 
		(Puzzle 4 [[1,1,2,8],[2,2,3,4],[4,8,6,8]])
		([2,6,6,14],[((1,2),(2,2)),((1,1),(2,1)),((1,3),(1,4)),((2,3),(3,3)),((3,2),(3,1)),((3,4),(2,4))]),

--First solution in 0.06 secs.
--Number of solutions: 10.
-----------------------------------------------------------------
		SimpleTest 
		(Puzzle 5 [[4,6,9,8,4],[1,8,2,2,3],[8,7,1,6,4],[4,2,6,5,2]])
		([3,6,7,8,12],[((2,1),(3,1)),((2,4),(3,3)),((4,2),(4,1)),((2,5),(3,4)),((3,5),(4,5)),((1,1),(2,2)),((3,2),(2,3)),((4,4),(4,3)),((1,4),(1,5)),((1,3),(1,2))]),

--First solution in 0.68 secs.
--Number of solutions: 4.
-----------------------------------------------------------------
		SimpleTest
		(Puzzle 5 [[3,0,2,4,3],[4,6,3,4,5],[5,8,4,0,5],[6,5,7,2,4]])
		([5,6,7,8,9],[((1,1),(1,2)),((1,5),(2,5)),((2,4),(3,4)),((2,1),(3,1)),((1,4),(1,3)),((3,3),(3,2)),((3,5),(4,5)),((4,2),(4,1)),((2,2),(2,3)),((4,3),(4,4))]),

--First solution in 1.25 secs.
--Number of solutions: 16.
-----------------------------------------------------------------
		SimpleTest
		(Puzzle 5 [[5,3,5,4,2],[0,4,3,8,4],[5,4,0,4,7],[5,6,3,6,2]])
		([5,6,7,8,9],[((1,2),(2,1)),((2,3),(1,3)),((3,4),(3,3)),((3,2),(4,1)),((1,4),(1,5)),((2,5),(2,4)),((1,1),(2,2)),((3,1),(4,2)),((4,4),(4,3)),((3,5),(4,5))]),

--First solution in 0.94 secs.
--Number of solutions: 14.
-----------------------------------------------------------------
		SimpleTest
		(Puzzle 5 [[2,5,0,2,3],[3,3,2,5,0],[0,5,2,5,3],[2,5,2,5,0]])
		([5,5,5,5,6],[((4,3),(4,4)),((4,1),(4,2)),((3,3),(3,4)),((3,5),(4,5)),((2,3),(3,2)),((1,4),(2,4)),((2,1),(3,1)),((1,1),(1,2)),((1,5),(2,5)),((2,2),(1,3))]),

--First solution in 3.34 secs.
--Number of solutions: 6.
-----------------------------------------------------------------
		SimpleTest
		(Puzzle 6 [[2,1,4,2,5,3],[5,3,4,6,8,1],[1,0,0,2,3,5],[4,6,4,8,1,9],[3,8,2,7,4,2]])
		([3,5,6,7,7,12],[((3,1),(2,1)),((2,6),(2,5)),((5,6),(4,5)),((1,1),(1,2)),((5,1),(4,2)),((2,2),(3,2)),((3,5),(3,6)),((1,6),(1,5)),((2,4),(3,3)),((2,3),(3,4)),((1,3),(1,4)),((5,4),(5,3)),((5,5),(4,6)),((4,4),(4,3)),((5,2),(4,1))]),

--First solution in 6.26 secs.
--Number of solutions: 1.
-----------------------------------------------------------------
 		SimpleTest
		(Puzzle 6 [[5,4,3,6,3,6],[3,5,0,7,0,0],[0,7,0,9,5,6],[8,2,8,5,2,4],[4,0,4,4,0,5]])
		([5,6,7,8,9,10],[((2,1),(3,1)),((1,3),(2,2)),((5,3),(5,2)),((4,6),(5,6)),((4,4),(5,5)),((5,4),(4,5)),((5,1),(4,1)),((1,1),(1,2)),((1,6),(2,6)),((3,5),(3,6)),((1,4),(1,5)),((2,4),(2,3)),((3,2),(4,2)),((4,3),(3,3)),((3,4),(2,5))]),

--First solution in 39.92 secs.
--Number of solutions: 80. 
-----------------------------------------------------------------
		SimpleTest
		(Puzzle 6 [[2,4,5,6,3,7],[5,3,5,6,4,2],[8,4,3,6,0,4],[4,3,5,5,6,5],[4,2,9,4,3,0]])
		([5,6,7,7,8,9],[((5,5),(5,6)),((4,2),(4,3)),((2,2),(2,3)),((2,5),(3,5)),((3,6),(4,6)),((5,1),(5,2)),((1,2),(1,1)),((4,1),(3,1)),((2,1),(3,2)),((5,4),(5,3)),((4,4),(4,5)),((3,4),(3,3)),((1,3),(1,4)),((2,4),(1,5)),((1,6),(2,6))]),

--First solution in 216.73 secs.
--Number of solutions: 14. 
-----------------------------------------------------------------
		SimpleTest
		(Puzzle 7 [[1,2,3,4,3,5,1],[3,7,6,8,0,8,2],[5,2,1,4,8,9,4],[3,6,5,2,5,0,6],[5,0,3,4,3,1,5],[1,2,0,5,3,4,2]])
		([3,5,5,6,7,7,12],[((5,6),(5,7)),((3,3),(4,3)),((1,7),(2,6)),((6,2),(6,1)),((1,2),(1,1)),((1,3),(2,3)),((2,7),(1,6)),((5,3),(6,3)),((6,5),(6,4)),((5,5),(4,5)),((4,7),(4,6)),((1,5),(2,5)),((4,1),(5,1)),((2,1),(3,1)),((4,2),(5,2)),((6,6),(6,7)),((5,4),(4,4)),((2,2),(3,2)),((3,7),(3,6)),((3,5),(3,4)),((2,4),(1,4))]),

--First solution in 22 min. 
--Number of solutions: 1.
-----------------------------------------------------------------
		CountTest 
		(Puzzle 4 [[1,1,2,8],[2,2,3,4],[4,8,6,8]])
		10,	
--End of program: 0.86 secs.
-----------------------------------------------------------------
		CountTest 
		(Puzzle 5 [[4,6,9,8,4],[1,8,2,2,3],[8,7,1,6,4],[4,2,6,5,2]])
		4,
--End of program in 2.00 secs.
-----------------------------------------------------------------
		CountTest
		(Puzzle 5 [[3,0,2,4,3],[4,6,3,4,5],[5,8,4,0,5],[6,5,7,2,4]])
		16,
--End of program: 5.28 secs.
-----------------------------------------------------------------
		CountTest
		(Puzzle 5 [[5,3,5,4,2],[0,4,3,8,4],[5,4,0,4,7],[5,6,3,6,2]])
		14,
--End of program: 6.27 secs.
-----------------------------------------------------------------
		CountTest
		(Puzzle 5 [[2,5,0,2,3],[3,3,2,5,0],[0,5,2,5,3],[2,5,2,5,0]])
		6,
--End of program: 14.32 secs.
-----------------------------------------------------------------
		CountTest
		(Puzzle 6 [[2,1,4,2,5,3],[5,3,4,6,8,1],[1,0,0,2,3,5],[4,6,4,8,1,9],[3,8,2,7,4,2]])
		1,
--End of program in 127.98 secs.
-----------------------------------------------------------------
 		CountTest
		(Puzzle 6 [[5,4,3,6,3,6],[3,5,0,7,0,0],[0,7,0,9,5,6],[8,2,8,5,2,4],[4,0,4,4,0,5]])
		80,
--End of program: 329.76 secs.
-----------------------------------------------------------------
		CountTest
		(Puzzle 6 [[2,4,5,6,3,7],[5,3,5,6,4,2],[8,4,3,6,0,4],[4,3,5,5,6,5],[4,2,9,4,3,0]])
		14,
--End of program: 441.91 secs.
-----------------------------------------------------------------
		CountTest
		(Puzzle 7 [[1,2,3,4,3,5,1],[3,7,6,8,0,8,2],[5,2,1,4,8,9,4],[3,6,5,2,5,0,6],[5,0,3,4,3,1,5],[1,2,0,5,3,4,2]])
		1]
--End of program in 55 min. 
------END------------------------------------------------------//

