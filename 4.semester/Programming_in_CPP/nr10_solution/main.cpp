//Exercise 10
//Author: Igor Tryhub, 275235

#include "varchain.h"
#include "polynomial.h"
#include "bigint.h"
#include "rational.h"


template< typename N >
polynomial< N > exptaylor( unsigned int n )
{
   varchain v;

   N fact = 1;

   polynomial< N > result;
   for( unsigned int i = 0; i < n; ++ i )
   {
      result[v] += fact;
      v = v * powvar( "x" );
      fact = fact / (i+1); //sprytnie...
   }

   return result;
}

int main( int argc, char* argv [] )
{
   std::cout << "hello\n";

   polynomial< rational > pol;

   int N = 50;

   pol[ { } ] = 1; // dodajemy wyraz wolny == 1
   pol[ { "x" } ] = rational( 1, N ); // += (1/50)*x

   //pol = 0;
   //pol = polynomial<rational>();

   polynomial< rational > res = 1;

   for( int i = 0; i < N; ++ i )
      res = res * pol;

   std::cout << "result = " << res << "\n";

   std::cout << " taylor expansion = " << exptaylor<rational>(20) << "\n";

   std::cout << "difference = " ;
   std::cout << res - exptaylor<rational> ( 40 ) << "\n";

   polynomial< int > pol1;
   pol1 [ { } ] = 1;
   pol1 [ { "x" } ] = 1;
   for( int i = 0; i < 4; ++ i )
      pol1 = pol1 * pol1;
   std::cout << "(1+x)^5 = " << pol1 << "\n";

   polynomial< int > pol2;
   pol2 [ { } ] = 3;
   pol2 [ { "xy" } ] = 1;
   for( int i = 0; i < 5; ++ i )
      pol2 = pol2 * pol2;
   std::cout << "(3+xy)^6 = " << pol2 << "\n";
   return 0;
}

