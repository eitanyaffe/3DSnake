# Makefile for 3D Snake game

# Compiler settings
CXX = g++
CXXFLAGS = -Wall -O2 -std=c++11 -D__APPLE__ -DGL_SILENCE_DEPRECATION

# OpenGL and GLUT libraries for macOS
INCLUDES = -I/System/Library/Frameworks/OpenGL.framework/Headers \
           -I/System/Library/Frameworks/GLUT.framework/Headers \
           -I/opt/X11/include
LDFLAGS = -framework OpenGL -framework GLUT -framework CoreFoundation -framework AudioToolbox -L/opt/X11/lib -lX11

# Source files
SOURCES = 3dsnake.cpp Scene.cpp StaticElement.cpp apple.cpp Snake.cpp Textures.cpp Sound.cpp
HEADERS = Scene.h Snake.h apple.h StaticElement.h resource.h Textures.h Sound.h

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Executable name
TARGET = 3dsnake

# Rules
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(OBJECTS) $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean 