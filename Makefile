# File     : Makefile
# Purpose  : top-level makefile

# Variables

BINDIR=${CURDIR}/bin
LIBDIR=${CURDIR}/lib
INCDIR=${CURDIR}/include

# Rules

all :
	@mkdir -p ${BINDIR} ${LIBDIR} ${INCDIR}
	@${MAKE} --no-print-directory BINDIR=${BINDIR} LIBDIR=${LIBDIR} INCDIR=${INCDIR} -C build install

clean almostclean :
	@${MAKE} -w -C build $@
	rm -f ${BINDIR}/* ${LIBDIR}/* ${INCDIR}/*

.PHONY: all clean
