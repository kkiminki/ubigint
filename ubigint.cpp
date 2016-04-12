// $Id: ubigint.cpp,v 1.1 2016-04-03 15:10:10-07 - - $

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "ubigint.h"
#include "debug.h"

/*ubigint::ubigint (unsigned long that): ubig_value (that) {
   DEBUGF ('~', this << " -> " << ubig_value)
}*/

ubigint::ubigint (const string& that): ubig_value(0) {
   int i;
   //int place=0;
   udigit_t digit;
   for (i = that.size()-1;i>=0;i--){
      //place++;
      digit = that[i];
      ubig_value.push_back( digit - '0');
   }
   
}

ubigint ubigint::operator+ (const ubigint& that) const {
   //return ubigint (uvalue + that.uvalue);
   bool carry = false;
   ubigint result;
   udigit_t digit, this_digit, that_digit;
   unsigned int loop_size = ubig_value.size() > that.ubig_value.size()? ubig_value.size() : that.ubig_value.size();
   for (unsigned int i=0; i<loop_size;i++){
      if (ubig_value.size() < i+1) {
         this_digit=0;
      }else{
         this_digit = ubig_value.at(i);
      }if(that.ubig_value.size() < i+1){
         that_digit = 0;
      }else{
         that_digit = that.ubig_value.at(i);
      }
      digit = this_digit + that_digit;
      if(carry){
         digit = digit + 1;
         carry = false;
      }
      if((digit)/(10) > 0) carry = true;
      digit = digit%10;
      result.ubig_value.push_back(digit);
   }
   if(carry)result.ubig_value.push_back(1);
   while(result.ubig_value.size()>0 and result.ubig_value.back()==0) result.ubig_value.pop_back();
   return result;
}

ubigint ubigint::operator- (const ubigint& that) const {
   if (*this < that) throw domain_error ("ubigint::operator-(a<b)");
   //return ubigint (uvalue - that.uvalue);
   udigit_t this_digit, that_digit;
   int digit;
   ubigint result;
   bool carry=false;
   for(unsigned int i =0; i < ubig_value.size();i++){
      if(that.ubig_value.size()-1<i){
         that_digit=0;
      }else{
         that_digit=that.ubig_value.at(i);
      }
      this_digit=ubig_value.at(i);
      digit=this_digit-that_digit;
      if(carry){
         digit = digit-1;
         carry=false;
      }
      if(digit<0){
         digit = digit+10;
         carry = true;
      }
      //digit = digit%10;
      result.ubig_value.push_back(digit);
   }
   while(result.ubig_value.size()>0 and result.ubig_value.back()==0) result.ubig_value.pop_back();
   return result;
}

ubigint ubigint::operator* (const ubigint& that) const {
   //return ubigint (uvalue * that.uvalue);
   ubigint result;
   std::vector<unsigned char> p(ubig_value.size() + that.ubig_value.size(), 0);
   udigit_t c, digit;
   unsigned int i, j;
   for(i=0;i<ubig_value.size();i++){
      c = 0;
      for(j=0;j<that.ubig_value.size();j++){
         digit = p[i+j] + ubig_value.at(i)*that.ubig_value.at(j) + c;
         p[i+j] = digit%10;
         c = digit/10;
      }
      p[i+that.ubig_value.size()] = c;
   }
   result.ubig_value = p;
   while(result.ubig_value.size()>0 and result.ubig_value.back()==0) result.ubig_value.pop_back();
   return result;
}

void ubigint::multiply_by_2() {
   *this = *this + *this;
}

void ubigint::divide_by_2() {
   for(unsigned int i=0; i<ubig_value.size();i++){
      if(i==ubig_value.size()-1 or ubig_value.at(i+1)%2==0){
         ubig_value[i] = ubig_value[i]/2;
      }else{
         ubig_value[i] = (ubig_value[i]/2) + 5;
      }
   }
}


struct quo_rem { ubigint quotient; ubigint remainder; };
quo_rem udivide (const ubigint& dividend, ubigint divisor) {
   // Note: divisor is modified so pass by value (copy).
   ubigint zero {"0"};
   if (divisor == zero) throw domain_error ("udivide by zero");
   ubigint power_of_2 {"1"};
   ubigint quotient {"0"};
   ubigint remainder {dividend}; // left operand, dividend
   while (divisor < remainder) {
      divisor.multiply_by_2();
      power_of_2.multiply_by_2();
   }
   while (power_of_2 > zero) {
      if (divisor < remainder or divisor==remainder) {
         remainder = remainder - divisor;
         quotient = quotient + power_of_2;
      }
      divisor.divide_by_2();
      power_of_2.divide_by_2();
   }
   return {.quotient = quotient, .remainder = remainder};
}

ubigint ubigint::operator/ (const ubigint& that) const {
   return udivide (*this, that).quotient;
}

ubigint ubigint::operator% (const ubigint& that) const {
   return udivide (*this, that).remainder;
}

bool ubigint::operator== (const ubigint& that) const {
   if(ubig_value.size() != that.ubig_value.size()) return false;
   for(unsigned int i=0; i<ubig_value.size();i++){
      if(ubig_value.at(i)!=that.ubig_value.at(i)) return false;
   }
   return true;
}

bool ubigint::operator< (const ubigint& that) const {
   if(ubig_value.size()>that.ubig_value.size()){return false;}
   for(int i=ubig_value.size()-1;i >=0; i--){
      if(ubig_value.at(i) < that.ubig_value.at(i))return true;
   }
   return false;
}

bool ubigint::operator> (const ubigint& that) const{
   return not ((*this<that) or (*this==that));
}

ostream& operator<< (ostream& out, const ubigint& that) {
   string output = "ubigint("; 
   for(int i=that.ubig_value.size()-1; i>=0;i--){
      output +=that.ubig_value.at(i)+'0';
   }
   output+=")";
   return out << output;
}
