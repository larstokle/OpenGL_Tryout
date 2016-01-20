#SOURCE FILES AND TARGET----------------------------
#Source directory
SRCDIR = ./source/
#all Source files in SRCDIR
SRCS = $(wildcard $(SRCDIR)*.cpp)
#Target directory
TARGETDIR = ./binary/
#Target file name (The final executable)
TARGETNAME = openGLTest
#Put DIR and NAME together
TARGET = $(addprefix $(TARGETDIR), $(TARGETNAME))
#Directory to put objects
OBJDIR = ./obj/
#Object list generated by source names
OBJS = $(patsubst $(SRCDIR)%.cpp,$(OBJDIR)%.o,$(SRCS))

#COMPILER, FLAGS AND LIBRARIES---------------------
#Compiler to use
CC = g++
#Flags to give the compiler
CFLAGS = -std=c++0x -g -Wall
#Library locations
LFLAGS = -Wl,-rpath=/usr/lib64,-L/lib/Simple_OpenGL_Image_Library/lib
#Libraries
LIBS = -lGLEW -lglfw3 -lGL -lX11 -lpthread -lXxf86vm -lXrandr -lXi -lm -lXinerama -lXcursor -lSOIL
#Headerfile locations
INCLUDES = -I/lib/Simple_OpenGL_Image_Library/src -I/lib/glm
#Where to search for prerequisites
vpath %.cpp ./source/

#MAKE COMMANDS--------------------------------------
.PHONY: depend clean write

all: $(TARGETDIR) $(TARGET)  
	@echo $(TARGET) has been built

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(INCLUDES) $(LFLAGS) $(LIBS)

$(OBJDIR)%.o: $(SRCDIR)%.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) -o $@ -c $^ $(INCLUDES) $(LFLAGS) $(LIBS)

#MAKE DIRECTORIES--------------------------------------
$(TARGETDIR):
	mkdir $(TARGETDIR)
$(OBJDIR):
	mkdir $(OBJDIR)

#CLEAN COMMANDS---------------------------------------
clean:
	rm -rv $(OBJDIR)

#TESTING----------------------------------------------
#.cpp.o:
#	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

write:
	@echo -e '\n'\
	TARGET'\t\t'	= 	'\t'	$(TARGET) '\n'\
	OBJS'\t\t'		= 	'\t'	$(OBJS) '\n'\
	SRCS'\t\t'		= 	'\t'	$(SRCS) '\n'\


#terminal compile: g++ -std=c++0x -o openGLTest ./source/main.cpp -lGLEW -lglfw3 -lGL -lX11 -lpthread -lXxf86vm -lXrandr -lXi -lm -lXinerama -lXcursor
#LIBS = -lGLEW -lglfw3 -lGL -lX11 -lpthread -lXxf86vm -lXrandr -lXi -lm -lXinerama -lXcursor
#terminal run: LD_LIBRARY_PATH=/usr/lib64  ./openGLTest

