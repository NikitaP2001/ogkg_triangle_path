ENV = $(shell uname)
ifneq (,$(findstring MSYS_NT,$(ENV)))

RM= rm
else
SHELL=cmd.exe
RM= @-del

endif
TARGET = ogkg.exe

CFLAGS = -c
LDFLAGS = -mwindows -lkernel32
RCFLAGS = -O coff

CC=c++
LD=c++
RC=windres

SOURCES = $(wildcard *.cpp)
RESOURCES = $(wildcard *.rc)
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS += $(RESOURCES:.rc=.res)


%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ $<
	
%.res: %.rc
	$(RC) $< $(RCFLAGS) $@

all: CFLAGS += -DDEBUG
all: $(TARGET) 

release: CFLAGS += -s -fdata-sections -ffunction-sections
release: CFLAGS += -O3
release: LDFLAGS += -Wl,--gc-sections,-s
release: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(LDLIBS) $^

clean:
	$(RM) *.o *.res
