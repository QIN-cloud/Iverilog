#
#    This source code is free software; you can redistribute it
#    and/or modify it in source code form under the terms of the GNU
#    Library General Public License as published by the Free Software
#    Foundation; either version 2 of the License, or (at your option)
#    any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Library General Public License for more details.
#
#    You should have received a copy of the GNU Library General Public
#    License along with this program; if not, write to the Free
#    Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
#    Boston, MA 02110-1301, USA.
#
SHELL = /bin/sh

suffix = 

prefix = /usr/local/iverilog
exec_prefix = ${prefix}
srcdir = .
datarootdir = ${prefix}/share



suffix = 

bindir = $(exec_prefix)/bin
libdir = $(exec_prefix)/lib
includedir = $(prefix)/include
mandir = ${datarootdir}/man

dllib=-ldl

CC = gcc
HOSTCC := gcc
WINDRES = false
INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644

ifeq (.,.)
INCLUDE_PATH = -I. -I..
else
INCLUDE_PATH = -I. -I.. -I$(srcdir) -I$(srcdir)/..
endif

CPPFLAGS = $(INCLUDE_PATH)  -DHAVE_CONFIG_H
CFLAGS = -Wall -Wextra -Wshadow -Wstrict-prototypes  -g -O2
CXXFLAGS = -Wall -Wextra -Wshadow   -g -O2
LDFLAGS =  

O = main.o res.o

all: iverilog-vpi

check: all

clean:
	rm -f *.o config.h iverilog-vpi res.rc

distclean: clean
	rm -f Makefile config.log

cppcheck: main.c
	cppcheck --enable=all --std=c99 --std=c++03 -f $(INCLUDE_PATH) $^

Makefile: $(srcdir)/Makefile.in ../config.status
	cd ..; ./config.status --file=driver-vpi/$@

iverilog-vpi: $O
	$(CC) $(LDFLAGS) $O -o iverilog-vpi 

main.o: $(srcdir)/main.c config.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(srcdir)/main.c


config.h: $(srcdir)/config.h.in Makefile
	sed -e 's;@IVLCC@;gcc;' -e 's;@IVLCXX@;g++;' \
	    -e 's;@SUFFIX@;$(suffix);g' \
	    -e 's;@IVLCFLAGS@;$(CFLAGS);' \
	    -e 's;@IVLCXXFLAGS@;$(CXXFLAGS);' \
	    -e 's;@SHARED@;-shared;' $< > $@

# Windows specific...
res.rc: $(srcdir)/res.rc.in ../version.exe
	sed -e 's;@PRODUCTVERSION@;'`../version.exe '%M,%n,0,0'`';' \
	$(srcdir)/res.rc.in > $@

res.o: res.rc
	$(WINDRES) -i res.rc -o res.o
#

install: all installdirs installfiles

F = ./iverilog-vpi

installfiles: $(F) | installdirs
	$(INSTALL_PROGRAM) ./iverilog-vpi "$(DESTDIR)$(bindir)/iverilog-vpi$(suffix)"
ifeq (no,yes)
ifneq ($(HOSTCC),$(CC))
	$(INSTALL_PROGRAM) $(shell $(HOSTCC) --print-file-name=libwinpthread-1.dll) "$(DESTDIR)$(bindir)"
	$(INSTALL_PROGRAM) $(shell $(HOSTCC) --print-file-name=libgcc_s_sjlj-1.dll) "$(DESTDIR)$(bindir)"
	$(INSTALL_PROGRAM) $(shell $(HOSTCC) --print-file-name=libstdc++-6.dll) "$(DESTDIR)$(bindir)"
endif
endif

installdirs: $(srcdir)/../mkinstalldirs
	$(srcdir)/../mkinstalldirs "$(DESTDIR)$(bindir)"

uninstall:
	rm -f $(DESTDIR)$(bindir)/iverilog-vpi$(suffix)
