# File     : Makefile
# Purpose  : makefile for build subdir

# Default settings

# Enable debugging (with a performance penalty)
DEBUG ?= no

# Enable MPI parallelization
MPI ?= no

# Enable FPE checks
FPE ?= yes

# Targets

LIB_TARGETS = libforum libyarg
EXEC_TARGETS = yarg_wind yarg_rt

# Object lists & related variables
#
# libxxx_OBJS = object files required to compile libxxx
# lbbxxx_DEPS = dependency that must be added to .libxxx.d file  
#
# prog_OBJS = object files required to compile prog
# prog_LIBS = library files required to link prog
# prog_DEPS = dependencies that must be added to .prog.d file

libforum_OBJS = forum_m.o cinter_m.o const_m.o hdf5io_m.o kinds_m.o memory_m.o \
                order_m.o system_m.o

libyarg_OBJS = yarg_m.o cubint_m.o clump_m.o limb_m.o matrix_m.o photon_m.o polbuff_m.o \
               random_m.o mpi_m.o tau_m.o vector_m.o volume_m.o voxel_m.o
libyarg_LIBS = libforum
libyarg_DEPS = forum_m
ifeq (${MPI},yes)
  libyarg_OBJS := ${libyarg_OBJS} mpi_full_m.o
else
  libyarg_OBJS := ${libyarg_OBJS} mpi_stub_m.o
endif

yarg_wind_OBJS = yarg_wind.o
yarg_wind_LIBS = libforum libyarg
yarg_wind_DEPS = forum_m yarg_m

yarg_rt_OBJS = yarg_rt.o
yarg_rt_LIBS = libforum libyarg
yarg_rt_DEPS = forum_m yarg_m

# Search subdirs

GITMODS = forum
SUBDIRS = $(filter-out ${GITMODS},$(wildcard ../src/*)) $(wildcard $(patsubst %,../src/%/src,${GITMODS}))

# Include other makefiles

include Make.inc

# Dump variables (for debugging)

$(info SUBDIRS is [${SUBDIRS}])
$(info LIB_TARGETS_ is [${LIB_TARGETS_}])
$(info EXEC_TARGETS is [${EXEC_TARGETS}])
