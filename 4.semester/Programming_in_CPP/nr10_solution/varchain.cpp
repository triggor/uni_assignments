
#include "varchain.h"
#include <algorithm>


std::ostream& operator << ( std::ostream& out, const powvar & p )
{
   if( p.n == 0 )
   {
      out << "1"; // Should not happen, but we still have to print something.
      return out;
   }

   out << p.v;
   if( p.n == 1 )
      return out;

   if( p.n > 0 )
      out << "^" << p.n;
   else
      out << "^{" << p.n << "}";
   return out;
}


std::ostream& operator << ( std::ostream& out, const varchain& c )
{
   if( c. isunit( ))
   {
      out << "1";
      return out;
   }

   for( auto p = c. repr. begin( ); p != c. repr. end( ); ++ p )
   {
      if( p != c. repr. begin( ))
         out << ".";
      out << *p;
   }

   return out;
}


int varchain::power( ) const 
{
   int p = 0;
   for( auto pv : repr )
      p += pv. n;
   return p;
}


void varchain::normalize( ){
  sort( repr.begin(), repr.end(), powvar({}));  // 1. Sort the representation by variable. 
  if( isunit() ) return;
  

  // .... i .... j ....
  /*for (size_t i = 0, j = 1; i < repr.size();) {
    for (; j < repr.size() && repr[i].v == repr[j].v; j++)
       repr[i].n += repr[j].n;
    if (j == repr.size()) break;
    if (repr[i].n != 0) i++;
    repr[i] = repr[j];
  }
  repr.resize(i, powvar({}));
  */


  std::vector<powvar> newrepr;
  auto prev = repr.begin();
  for( auto elem = repr.begin()+1; elem != repr.end(); elem++ ){
    if( elem->v == prev->v){ // 2. Merge powvars with identical variable.
      prev->n += elem->n;
      //repr.erase(elem);
    }
    else{
      if( prev->n != 0) newrepr.push_back(*prev); // 3. Remove powvars with n == 0.
      prev = elem;
    }
  }
  if (prev->n != 0) newrepr.push_back(*prev);
  
  repr = std::move(newrepr);
}

int varchain::compare( const varchain& c1, const varchain& c2 ){
  //if( c1.isunit() && c2.isunit() ) return 0;
  //if( c2.isunit() ) return 1;
  //if( c1.isunit() ) return -1;
  auto iter1 = c1.repr.begin();
  auto iter2 = c2.repr.begin();
  for(; iter1 != c1.repr.end() && iter2 != c2.repr.end(); iter1++, iter2++) {
    if (iter1->v != iter2->v) {
      if (iter1->v < iter2->v) return -1;
      return 1;
    }
    if (iter1->n != iter2->n) {
      return iter1->n - iter2->n;
    }    
  }
  
  if (iter1 == c1.repr.end() && iter2 == c2.repr.end()) return 0;
  if (iter1 == c1.repr.end()) return -1;
  return 1; //dla celow testowych, zeby widziec ze cos poszlo nie tak. oprocz tego, kompilator chyba moze krzyczac ze doszedl do konca non-void funkcji.
}

varchain operator * ( varchain c1, const varchain& c2 ){
  c1.repr.insert( c1.repr.end(), c2.repr.begin(), c2.repr.end() );
  c1.normalize();
  return c1;
}
