
#ifndef POLYNOMIAL_INCLUDED
#define POLYNOMIAL_INCLUDED 1

#include "varchain.h"
#include <map>


template< typename N >
struct polynomial
{

   //repr["x^2"] = 2 ==> 2x^2

   using maptype = std::map< varchain, N, varchain::cmp > ;

   maptype repr;

   typename maptype::iterator begin( ) { return repr. begin( ); }
   typename maptype::iterator end( ) { return repr. end( ); }

   typename maptype::const_iterator cbegin( ) const { return repr. cbegin( ); }
   typename maptype::const_iterator cend( ) const { return repr. cend( ); }

   typename maptype::const_iterator begin( ) const { return repr.cbegin( ); }
   typename maptype::const_iterator end( ) const { return cend( ); }


   const N& operator [] ( const varchain& c ) const
      { return repr[c]; }

   N& operator [] ( const varchain& c )
      { return repr[c]; }


   polynomial( )
      { }

   polynomial( const varchain& chain )
   {
      repr[ chain ] = 1;
   }

   template< typename D >
   polynomial( const D& d )
   {
      repr[ {} ] = d; //polynomial(3)
   }


   polynomial operator - ( )
   {
      auto res = *this;
      for( auto& p : res. repr )
         p. second = - p. second;
      return res;
   }// x^2 - 5x + 2   => -x^2 +5x -2

   polynomial& operator += ( const polynomial& pol )
   {
      for( const auto& p : pol. repr )
      {
         (*this)[ p. first ] += p. second;
      }
      return *this;
   }// (x^2 - 5x + 2)  + (3x^2 + 5 + y) = 4x^2 -5x +7 + y

   polynomial<N>& operator -= ( const polynomial& pol )
   {
      for( const auto& p : pol. repr )
         (*this) [ p. first ] -= p. second;
      return *this;
   }

   template<typename X > polynomial& operator *= ( X x )
   {
      for( auto& p : this->repr )
         p. second *= x;
      return *this;
   }

polynomial<N>& operator *= ( const polynomial<N> & pol1);



   template< typename X > polynomial<N> & operator /= ( X x )
   {
      for( auto& p : *this )
         p. second /= x;
      return *this;
   }

   polynomial<N> operator + ( const polynomial<N>& pol1)
   {
      auto res = *this;
      res += pol1;
      return res;
   }

   polynomial operator - ( const polynomial& pol1)
   {
      auto res = *this;
      res -= pol1;
      return res;
   }

};


template< typename M, typename N >
polynomial<M> operator * ( const polynomial<M> & pol, N n )
{
   auto res = pol;
   res *= n;
   return res;
}


template< typename M, typename N >
polynomial<M> operator / ( const polynomial<M> & pol, N n )
{
   auto res = pol;
   res /= n;
   return res;
}


template< typename M, typename N >
polynomial<N> operator * ( M m, const polynomial<N> & pol )
{
   auto res = pol;
   res *= m;
   return res;
}

// (x+2) * (5x+3) = (5x^2 + 13x + 6)
template< typename N >
polynomial<N> operator * ( const polynomial<N> & pol1, const polynomial<N> & pol2 ){
  polynomial<N> res;
  for( auto it = pol1.repr.begin(); it != pol1.repr.end(); it++ ){
    for( auto elem : pol2.repr ){
      // x * x  = x^2
      // x * 3  = 3*x
      // 2 * 5x  = 10*x
      // 2 * 3  = 6
      res.repr[it->first * elem.first] += it->second * elem.second;
    }
  }
  return res;
}


template<typename N>
polynomial<N>& polynomial<N>::operator *= ( const polynomial<N> & pol1) {
  polynomial<N> res = (*this) * pol1;
  return res;
}

template< typename N >
std::ostream& operator << ( std::ostream& out, const polynomial<N> & pol )
{
   bool somethingprinted = false;

   for( auto p = pol. begin( ); p != pol. end( ); ++ p )
   {
      if( p -> second != 0 )
      {
         if( somethingprinted )
            out << " + ";
         if( p -> second != 1 )
            out << ( p -> second ) << "*";
         out << ( p -> first );

         somethingprinted = true;
      }
   }

   if( !somethingprinted )
      out << N{0};

   return out;
}


#endif


