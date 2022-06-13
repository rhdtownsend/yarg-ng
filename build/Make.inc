# File     : Make.inc
# Purpose  : standard makefile rules, targets

# Check YARG_DIR is set

ifneq ($(MAKECMDGOALS),clean)
  ifeq (${YARG_DIR},) 
    $(error Please set the YARG_DIR environment variable to point to the top-level Yarg directory)
  endif
endif

# Check SDK version

MINIMUM_VERSION = 22.6.1

ifneq ($(MAKECMDGOALS),clean)

  VERSION_CHECK = $(shell ./check_sdk_version $(MINIMUM_VERSION))

  ifneq ($(VERSION_CHECK),passed)
    ifeq ($(VERSION_CHECK),SDK too old)
      $(error Your MESA SDK installation is too old; please update to version $(MINIMUM_VERSION) or newer)
    else ifeq ($(VERSION_CHECK),SDK not installed)
      $(error MESA SDK is either not installed or not properly initialized)
    else
      $(error Could not determine MESA SDK availability)
    endif
  endif

endif

# Preprocessor flags

OPTS = DEBUG FPE MPI

FYPPFLAGS:=${FYPPFLAGS} $(foreach subdir,$(subst :, ,${SUBDIRS}),-I${subdir}) $(foreach opt,${OPTS},$(if $(filter yes,${${opt}}),-D${opt}=1))

# Compiler settings

# Fortran

export F9XC

ifeq (${MPI},yes)
   F9XC = mpif90
else
   F9XC = gfortran
endif

FFLAGS := ${FFLAGS} -finit-real=snan -fbacktrace -fmax-errors=25 -fPIC

ifeq (${FPE},yes)
  FFLAGS := ${FFLAGS} -ffpe-trap=invalid,overflow,zero
endif

ifeq (${DEBUG},yes)
  FFLAGS := ${FFLAGS} -fcheck=all -Wall -Wno-unused-dummy-argument -Wno-maybe-uninitialized -finline-limit=0 -ggdb
else
  FFLAGS := ${FFLAGS} -O2 -march=native
endif

F9XFLAGS := ${F9XFLAGS} $(addprefix -I,${MODPATH}) ${FFLAGS} -std=f2018
F77FLAGS := ${F77FLAGS} ${FFLAGS} -ffixed-form

# Linking

LDFLAGS = `hdf5_link` `lapack95_link`

ifeq ($(shell uname -s),Darwin)
  LIB_SUFFIX = .dylib
  LIB_LDFLAGS = -shared -install_name ${YARG_DIR}/lib/$@
  EXEC_LDFLAGS = 
else			   
  LIB_SUFFIX = .so
  LIB_LDFLAGS = -shared -Wl,-rpath -Wl,${YARG_DIR}/lib
  EXEC_LDFLAGS = -Wl,-rpath -Wl,${YARG_DIR}/lib
endif

# Targets-related variables

LIB_TARGETS_ = $(addsuffix ${LIB_SUFFIX},${LIB_TARGETS})

DEP_TARGETS = $(patsubst %,.%.d,${LIB_TARGETS} ${EXEC_TARGETS})

MOD_TARGETS = $(patsubst lib%,%_m.mod,${LIB_TARGETS})

# Paths

vpath %.fypp ${SUBDIRS}
vpath %.inc ${SUBDIRS}

# Explicit rules

.PHONY: all install lib-install exec-install headers-install clean almostclean

all : ${LIB_TARGETS_} ${EXEC_TARGETS}

install : lib-install exec-install headers-install

lib-install : ${LIB_TARGETS_}
	@for t in $^; do \
            echo CP $${t} ${LIBDIR}; \
            cp $${t} ${LIBDIR}; \
        done

exec-install : ${EXEC_TARGETS}
	@for t in $^; do \
            echo CP $${t} ${BINDIR}; \
            cp $${t} ${BINDIR}; \
        done

headers-install : ${MOD_TARGETS}
	@for t in $^; do \
            echo CP $${t} ${INCDIR}; \
            cp $${t} ${INCDIR}; \
        done

clean : almostclean
	rm -f .*.d .*.dpp

almostclean :
	rm -f ${LIB_TARGETS_} ${EXEC_TARGETS} *.o *.mod *.smod *.f90 *.c

${LIB_TARGETS_} :
	@echo LD_LIB $@
	@${F9XC} ${F9XFLAGS} -o $@ $^ ${LDFLAGS} ${LIB_LDFLAGS}

${EXEC_TARGETS} :
	@echo LD $@
	@${F9XC} ${F9XFLAGS} -o $@ $^ ${EXEC_LDFLAGS}

${DEP_TARGETS} : 
	@echo DEPEND $(patsubst .%.d,%,$@) "->" $@
	@makedepf90 $(addprefix -d ,${EXTRA_DEPS}) -m %m.mod $^ > $@
	@echo "include \$$(patsubst %.f90,.%.dpp,$^)" >> $@

# Implicit rules

%.o : %.mod
% : %.mod
% : %.o

%.f90 .%.dpp : %.fypp
	@./fypp_deps ${FYPPFLAGS} $< $*.f90 > .$*.dpp

%.o %.mod : %.f90
	@echo FC $<
	@${F9XC} ${F9XFLAGS} -c $<
	@if [[ -e $*.mod ]]; then touch $*.mod; fi

%.d : EXTRA_DEPS = $(addsuffix .mod,${$*_DEPS})

.PRECIOUS : %.f90

# Macros

strip_d = $(patsubst %.d,%,$(1))

# Dependency handling

ifneq ($(MAKECMDGOALS),clean)
-include ${DEP_TARGETS}
endif

.SECONDEXPANSION:

${LIB_TARGETS_} : %${LIB_SUFFIX} : $${$$*_OBJS} $$(addsuffix ${LIB_SUFFIX},$${$$*_LIBS})

${EXEC_TARGETS} : $$($$@_OBJS) $$(addsuffix ${LIB_SUFFIX},$${$$@_LIBS})

${DEP_TARGETS} : .%.d : $$(subst .o,.f90,$${$$*_OBJS})
