#**************************************************************************
#   Makefile                                                              #
#                                                                         #
#   ISANA                                                                 #
#                                                                         #
#   This program is free software; you can redistribute it and/or modify  #
#   it under the terms of the GNU General Public License as published by  #
#   the Free Software Foundation, version 3                               #
#                                                                         #
#   This program is distributed in the hope that it will be useful, but   #
#   WITHOUT ANY WARRANTY; without even the implied warranty of            #
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     #
#   General Public License for more details.                              #
#                                                                         #
#   You should have received a copy of the GNU General Public License     #
#   along with this program; if not, write to the Free Software           #
#   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA             #
#   02110-1301, USA.                                                      #
#                                                                         #
# **************************************************************************/

# set compiler and compile options
EXEC = isana
# use the GNU C++ compiler
CXX = g++
# use some optimization, report all warnings and enable debugging
OPTS = -O0 -Wall -Wno-write-strings
# add compile flags
CFLAGS = $(OPTS) -std=c++0x
# specify link flags here
LDFLAGS = `pkg-config --libs --static glfw3 glew` -lpng

# set a list of directories
INCDIR  = ./include
OBJDIR  = ./obj
BINDIR  = ./bin
SRCDIR  = ./src

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	CFLAGS += -I/opt/local/include
	LDFLAGS += -L/opt/local/lib -lboost_regex-mt -lboost_unit_test_framework-mt
else
	CFLAGS +=
	LDFLAGS += -lboost_regex -lboost_unit_test_framework
endif

# set the include folder where the .h files reside
CFLAGS += -I$(INCDIR) -I$(SRCDIR)

# add here the source files for the compilation
SOURCES = isana.cpp camera.cpp display.cpp visualizer.cpp object.cpp \
shader.cpp texture_manager.cpp mesh.cpp terrain.cpp perlin_noise.cpp

_INCS = $(SOURCES:.cpp=.h)
INCS = $(patsubst %,./include/%,$(_INCS))

# create the obj variable by substituting the extension of the sources
# and adding a path
_OBJ = $(SOURCES:.cpp=.o)
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

all: $(BINDIR)/$(EXEC)

$(BINDIR)/$(EXEC): $(OBJ) $(INCS)
	$(CXX) -o $(BINDIR)/$(EXEC) $(OBJ) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCDIR)/%.h
	$(CXX) -c -o $@ $< $(CFLAGS)

clean:
	rm -vf $(BINDIR)/$(EXEC) $(OBJ) $(TESTS_EXEC)
