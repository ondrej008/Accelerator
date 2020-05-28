CFLAGS = -std=c++17 -Wall -Wpedantic -Wextra -O0 -g -mwindows -static
LFLAGS = -lglfw3dll -lopengl32 -lglew32 \
	-I"C:\MinGW\vcpkg\installed\x86-windows\include" \
	-L"C:\MinGW\vcpkg\installed\x86-windows\lib"

SOURCES = $(wildcard *.cpp) $(wildcard obstacles/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
DEPENDS = $(SOURCES:.cpp=.d)

all: app.exe
	rm -f *.o *.d obstacles/*.o obstacles/*.d
	./app.exe

app.exe: $(OBJECTS)
	g++ $(OBJECTS) -o app.exe $(CFLAGS) $(LFLAGS)

%.o: %.cpp
	g++ -c -MMD -MP $(CFLAGS) $(LFLAGS) -o $@ $<

-include $(DEPENDS)

#all:
#	g++ $(SOURCES) -o app.exe $(LFLAGS) $(CFLAGS) 