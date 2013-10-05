all::

TARGET = boggle

CC = gcc
RM = rm -rf
CFLAGS = -g -O2 -D__LINUX__


QUIET_CC = @echo '    ' CC $@;
QUIET_LD = @echo '    ' LD $@;
QUIET_RM = @echo '    ' RM;

OBJS =
HEADERS =

OBJS += BoggleGUI.o
OBJS += BoggleGraph.o
OBJS += MyBogglePlayer.o
OBJS += MyBoggleUtil.o
OBJS += Geometry.o
OBJS += Texture.o
OBJS += main.o

HEADERS += BoggleGUI.h
HEADERS += BoggleGraph.h
HEADERS += BogglePlayer.h
HEADERS += MyBogglePlayer.h
HEADERS += MyBoggleUtil.h
HEADERS += Geometry.h
HEADERS += Texture.h
HEADERS += vec2.h
HEADERS += vec3.h
HEADERS += vec4.h

$(OBJS) : $(HEADERS)

%.o: %.cpp
	$(QUIET_CC)$(CC) $(CFLAGS) -c $<

$(TARGET): $(OBJS)
	$(QUIET_LD)$(CC) $(OBJS) -lglut -lGL -lGLU -o $(TARGET)

all:: $(TARGET)

clean: 
	$(QUIET_RM)$(RM) $(OBJS) $(TARGET)


purify:
	purify g++ SimpleBoggleClient.cpp MyBogglePlayer.cpp MyBoggleUtil.cpp -o SimpleBoggleClient


