// $Id: scanner.h,v 1.1 2016-04-03 15:10:10-07 - - $

#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <iostream>
#include <utility>
using namespace std;

#include "debug.h"

enum class tsymbol {SCANEOF, NUMBER, OPERATOR};
struct token_t {
   tsymbol symbol;
   string lexinfo;
};

class scanner {
   private:
      bool seen_eof;
      char lookahead;
      void advance();
   public:
      scanner();
      token_t scan();
};

ostream& operator<< (ostream&, const tsymbol&);
ostream& operator<< (ostream&, const token_t&);

#endif

