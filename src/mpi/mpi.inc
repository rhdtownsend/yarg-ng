! Incfile : mpi
! Purpose : macros for mpi_m
!
! Copyright 2010-2022 Rich Townsend
!
! This file is part of Yarg. Yarg is free software: you can redistribute
! it and/or modify it under the terms of the GNU General Public
! License as published by the Free Software Foundation, version 3.
!
! Yarg is distributed in the hope that it will be useful, but WITHOUT
! ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
! or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
! License for more details.
!
! You should have received a copy of the GNU General Public License
! along with this program.  If not, see <http://www.gnu.org/licenses/>.

#!!
#! Map from suffixes to MPI type
#!!

#:def MPI_TYPE(suffix)
   #:if suffix == 'is'
      MPI_INTEGER4
   #:elif suffix == 'id'
      MPI_INTEGER8
   #:elif suffix == 'rs'
      MPI_REAL
   #:elif suffix == 'rd'
      MPI_DOUBLE_PRECISION
   #:elif suffix == 'cs'
      MPI_COMPLEX
   #:elif suffix == 'cd'
      MPI_DOUBLE_COMPLEX
   #:endif
#:enddef
