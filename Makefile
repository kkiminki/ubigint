# $Id: Makefile,v 1.1 2016-04-03 15:10:10-07 - - $

MKFILE      = Makefile
DEPFILE     = ${MKFILE}.dep
NOINCL      = ci clean spotless
NEEDINCL    = ${filter ${NOINCL}, ${MAKECMDGOALS}}
GMAKE       = ${MAKE} --no-print-directory
COMPILECPP  = g++ -std=gnu++14 -g -O0 -Wall -Wextra
MAKEDEPCPP  = g++ -std=gnu++14 -MM

MODULES     = bigint ubigint libfns scanner debug general
CPPHEADER   = ${MODULES:=.h} iterstack.h relops.h
CPPSOURCE   = ${MODULES:=.cpp} main.cpp
EXECBIN     = ydc
OBJECTS     = ${CPPSOURCE:.cpp=.o}
MODULESRC   = ${foreach MOD, ${MODULES}, ${MOD}.h ${MOD}.cpp}
OTHERSRC    = ${filter-out ${MODULESRC}, ${CPPHEADER} ${CPPSOURCE}}
ALLSOURCES  = ${MODULESRC} ${OTHERSRC} ${MKFILE}
LISTING     = Listing.ps

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${COMPILECPP} -o $@ ${OBJECTS}

%.o : %.cpp
	${COMPILECPP} -c $<

ci : ${ALLSOURCES}
	- checksource ${ALLSOURCES}
	- cpplint.py.perl ${CPPSOURCE}
	cid + ${ALLSOURCES}

lis : ${ALLSOURCES}
	mkpspdf ${LISTING} ${ALLSOURCES} ${DEPFILE}

clean :
	- rm ${OBJECTS} ${DEPFILE} core ${EXECBIN}.errs

spotless : clean
	- rm ${EXECBIN} ${LISTING} ${LISTING:.ps=.pdf}

dep : ${CPPSOURCE} ${CPPHEADER}
	@ echo "# ${DEPFILE} created `LC_TIME=C date`" >${DEPFILE}
	${MAKEDEPCPP} ${CPPSOURCE} >>${DEPFILE}

${DEPFILE} :
	@ touch ${DEPFILE}
	${GMAKE} dep

again :
	${GMAKE} spotless dep ci all lis

submit :
	submit cmps109-wm.s16 asg1 ${ALLSOURCES} README PARTNER

testsub : 
	cp testsub ${ALLSOURCES} README PARTNER

cleansub:
	cd testsub
	rm *
	cd ..
	rmdir testsub

ifeq (${NEEDINCL}, )
include ${DEPFILE}
endif

