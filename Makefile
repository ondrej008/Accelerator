CFLAGS = -std=c++17 -Wall -Wpedantic -Wextra -O0 -g -mwindows -static
LFLAGS = -lglfw3dll -lopengl32 -lglew32 \
	-I"C:\MinGW\vcpkg\installed\x86-windows\include" \
	-L"C:\MinGW\vcpkg\installed\x86-windows\lib"

all:
	g++ main.cpp accelerator.cpp utils.cpp player.cpp shader.cpp -o app.exe $(LFLAGS) $(CFLAGS) 