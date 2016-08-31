# 
# 	Copyright 2008-2011 Michel Pollet <buserror@gmail.com>
#   Copyright 2016 Peter Dunshee <peter@petezah.com>
#
#	This file is part of simavr.
#
#	simavr is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	simavr is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with simavr.  If not, see <http://www.gnu.org/licenses/>.

target=	simkeytoy

IPATH = .

VPATH = .

CPPFLAGS += ${shell pkg-config --cflags simavr}

LDFLAGS += -lpthread
LDFLAGS += ${shell pkg-config --libs simavr}

include Makefile.opengl

all: obj ${target}

include Makefile.common

board = ${OBJ}/${target}.elf

${board} : ${OBJ}/st7735_virt.o
${board} : ${OBJ}/st7735_glut.o
${board} : ${OBJ}/uart_stdin.o
${board} : ${OBJ}/${target}.o

${target}: ${board}
	@echo $@ done

clean: clean-${OBJ}
	rm -rf *.a ${target} *.vcd .*.swo .*.swp .*.swm .*.swn
