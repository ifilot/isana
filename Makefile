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
TEST = $(EXEC)-test
# use the GNU C++ compiler
CXX = g++
# use some optimization, report all warnings and enable debugging
OPTS = -O0 -Wall -Wno-write-strings -g
# add compile flags
CFLAGS = $(OPTS) -std=c++0x
CFLAGS += `pkg-config --cflags freetype2 libpng`
# specify link flags here
LDFLAGS = `pkg-config --libs --static glfw3 glew`
LDFLAGS += `pkg-config --libs freetype2 libpng`

# set a list of directories
OBJDIR  = ./obj
BINDIR  = ./bin
SRCDIR  = ./src
TESTDIR = ./test

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	CFLAGS += -I/opt/local/include
	LDFLAGS += -L/opt/local/lib -lboost_regex-mt -lboost_unit_test_framework-mt -lboost_chrono-mt -lboost_system-mt
else
	CFLAGS +=
	LDFLAGS += -lboost_regex -lboost_unit_test_framework -lboost_chrono -lboost_system
endif

# set the include folder where the .h files reside
CFLAGS += -I$(SRCDIR)

# add here the source files for the compilation
_SOURCES = isana.cpp \
accessoires/perlin_noise.cpp \
core/armature.cpp \
core/camera.cpp \
core/display.cpp \
core/font_writer.cpp \
core/mesh.cpp \
core/object.cpp \
core/post_processor.cpp \
core/screen.cpp \
core/shader.cpp \
core/texture_manager.cpp \
core/visualizer.cpp \
environment/sky.cpp \
environment/terrain.cpp \
objects/objects_engine.cpp \
objects/buildings/hq.cpp \
objects/buildings/turbine.cpp \
ui/console.cpp
SOURCES = $(patsubst %,$(SRCDIR)/%,$(_SOURCES))

# add paths in sources to the VPATH
VPATH := $(dir $(SOURCES))

# every source file has its own header file
INCS = $(SOURCES:.cpp=.h)

# add object file for the sources
OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(notdir $(SOURCES)))

all: $(BINDIR)/$(EXEC)

$(BINDIR)/$(EXEC): $(OBJS) $(INCS)
	@echo creating $@ ...
	$(CXX) -o $(BINDIR)/$(EXEC) $(OBJS) $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp %.h
	$(CXX) -c -o $@ $< $(CFLAGS)

clean:
	rm -vf $(BINDIR)/$(EXEC) $(OBJS)
