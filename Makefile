# Replace this with the path you get from `brew info sfml`
SFML_PATH = /opt/homebrew/Cellar/sfml@2/2.6.2_1
SFML_PATH = /mingw64
# Replace "src" with the name of the folder where all your cpp code is
cppFileNames := $(shell find . -type f -maxdepth 1 -name "*.cpp")

all: compile

compile:	
	mkdir -p bin
	g++ *.cpp -I $(SFML_PATH)/include/SFML -o main -L $(SFML_PATH)/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network
