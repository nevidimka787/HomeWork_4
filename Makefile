CC = gcc
OUT = Task3.elf
MAKE = Task2

CODE = code
CLASS = Classes
SUBCLASS = SubClasses
NOT_MY = NotMy
BIN = bin
CLEAN_COMMAND = rm -rf $(BIN)/*.o *.o

LIBS = -ldl -lm -lGL -lglfw -lepoxy -lstdc++

BIN_FILES = $(BIN)/main.o $(BIN)/glad.o $(BIN)/stb_image.o $(BIN)/Buffer.o $(BIN)/Color.o $(BIN)/Camera.o $(BIN)/Line.o $(BIN)/Mat.o $(BIN)/Shader.o $(BIN)/Texture.o $(BIN)/GraphRealisation.o $(BIN)/OpenGLRealisation.o

all: $(MAKE)

rebuild: clean all

$(MAKE): $(BIN_FILES)
	$(CC) $(BIN_FILES) $(LIBS) -o "$(OUT)"
	
$(BIN)/main.o:
	$(CC) -c "$(CODE)/main.cpp" -o "$(BIN)/main.o"
	
	
$(BIN)/glad.o:
	$(CC) -c "$(CODE)/$(NOT_MY)/glad.c" -o "$(BIN)/glad.o"
	
$(BIN)/stb_image.o:
	$(CC) -c "$(CODE)/$(NOT_MY)/stb_image.cpp" -o "$(BIN)/stb_image.o"
	
	
$(BIN)/Buffer.o:
	$(CC) -c "$(CODE)/$(CLASS)/Buffer.cpp" -o "$(BIN)/Buffer.o"
	
$(BIN)/Camera.o:
	$(CC) -c "$(CODE)/$(CLASS)/Camera.cpp" -o "$(BIN)/Camera.o"
	
$(BIN)/Color.o:
	$(CC) -c "$(CODE)/$(CLASS)/Color.cpp" -o "$(BIN)/Color.o"
	
$(BIN)/Line.o:
	$(CC) -c "$(CODE)/$(CLASS)/Line.cpp" -o "$(BIN)/Line.o"
	
$(BIN)/Mat.o:
	$(CC) -c "$(CODE)/$(CLASS)/Mat.cpp" -o "$(BIN)/Mat.o"
	
$(BIN)/Shader.o:
	$(CC) -c "$(CODE)/$(CLASS)/Shader.cpp" -o "$(BIN)/Shader.o"
	
$(BIN)/Texture.o:
	$(CC) -c "$(CODE)/$(CLASS)/Texture.cpp" -o "$(BIN)/Texture.o"
	
	
$(BIN)/GraphRealisation.o:
	$(CC) -c "$(CODE)/$(SUBCLASS)/GraphRealisation.cpp" -o "$(BIN)/GraphRealisation.o"
	
$(BIN)/OpenGLRealisation.o:
	$(CC) -c "$(CODE)/$(SUBCLASS)/OpenGLRealisation.cpp" -o "$(BIN)/OpenGLRealisation.o"
	
	
	
main.o:
	rm -rf "$(BIN)/main.o"
	$(CC) -c "$(CODE)/main.cpp" -o "$(BIN)/main.o"
	$(CC) $(BIN_FILES) $(LIBS) -o "$(OUT)"
	
	
glad.o:
	rm -rf "$(BIN)/gald.o"
	$(CC) -c "$(CODE)/$(NOT_MY)/glad.c" -o "$(BIN)/glad.o"
	$(CC) $(BIN_FILES) $(LIBS) -o "$(OUT)"
	
stb_image.o:
	rm -rf "$(BIN)/std_image.o"
	$(CC) -c "$(CODE)/$(NOT_MY)/stb_image.cpp" -o "$(BIN)/stb_image.o"
	$(CC) $(BIN_FILES) $(LIBS) -o "$(OUT)"
	
	
Buffer.o:
	rm -rf "$(BIN)/Buffer.o"
	$(CC) -c "$(CODE)/$(CLASS)/Buffer.cpp" -o "$(BIN)/Buffer.o"
	$(CC) $(BIN_FILES) $(LIBS) -o "$(OUT)"
	
Camera.o:
	rm -rf "$(BIN)/Camera.o"
	$(CC) -c "$(CODE)/$(CLASS)/Camera.cpp" -o "$(BIN)/Camera.o"
	$(CC) $(BIN_FILES) $(LIBS) -o "$(OUT)"
	
Color.o:
	rm -rf "$(BIN)/Color.o"
	$(CC) -c "$(CODE)/$(CLASS)/Color.cpp" -o "$(BIN)/Color.o"
	$(CC) $(BIN_FILES) $(LIBS) -o "$(OUT)"
	
Line.o:
	rm -rf "$(BIN)/Line.o"
	$(CC) -c "$(CODE)/$(CLASS)/Line.cpp" -o "$(BIN)/Line.o"
	$(CC) $(BIN_FILES) $(LIBS) -o "$(OUT)"
	
Mat.o:
	rm -rf "$(BIN)/Mat.o"
	$(CC) -c "$(CODE)/$(CLASS)/Mat.cpp" -o "$(BIN)/Mat.o"
	$(CC) $(BIN_FILES) $(LIBS) -o "$(OUT)"
	
Shader.o:
	rm -rf "$(BIN)/Shader.o"
	$(CC) -c "$(CODE)/$(CLASS)/Shader.cpp" -o "$(BIN)/Shader.o"
	$(CC) $(BIN_FILES) $(LIBS) -o "$(OUT)"
	
Texture.o:
	rm -rf "$(BIN)/Texture.o"
	$(CC) -c "$(CODE)/$(CLASS)/Texture.cpp" -o "$(BIN)/Texture.o"
	$(CC) $(BIN_FILES) $(LIBS) -o "$(OUT)"
	
	
GraphRealisation.o:
	rm -rf "$(BIN)/GraphRealisation.o"
	$(CC) -c "$(CODE)/$(SUBCLASS)/GraphRealisation.cpp" -o "$(BIN)/GraphRealisation.o"
	$(CC) $(BIN_FILES) $(LIBS) -o "$(OUT)"
	
OpenGLRealisation.o:
	rm -rf "$(BIN)/OpenGLRealisation.o"
	$(CC) -c "$(CODE)/$(SUBCLASS)/OpenGLRealisation.cpp" -o "$(BIN)/OpenGLRealisation.o"
	$(CC) $(BIN_FILES) $(LIBS) -o "$(OUT)"
	
	
clean:
	$(CLEAN_COMMAND)







