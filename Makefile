SHELL=cmd.exe
RM= @-del

TARGET = ogkg.exe

SRC_DIR = src
RES_DIR = res
OBJ_DIR = obj
INC_DIR = inc

vpath %.cpp SRC_DIR
vpath %.hpp INC_DIR
vpath %.rc RES_DIR
vpath %.o OBJ_DIR
vpath %.res OBJ_DIR

CFLAGS = -c -I $(INC_DIR)
LDFLAGS = -mwindows -lkernel32 -lmsimg32
RCFLAGS = -O coff

AS=\masm32\bin\ml64.exe
CC=c++
LD=c++
RC=windres

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
ASMS = $(wildcard $(SRC_DIR)/*.asm)
RESOURCES = $(wildcard $(RES_DIR)/*.rc)
OBJECTS = $(subst $(SRC_DIR)/,$(OBJ_DIR)/,$(SOURCES:.cpp=.o))
OBJECTS += $(subst $(RES_DIR)/,$(OBJ_DIR)/,$(RESOURCES:.rc=.res))
OBJECTS += $(subst $(SRC_DIR)/,$(OBJ_DIR)/,$(ASMS:.asm=.o))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ $<
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	$(AS) /c /Cp /Fo $@ $<
	
$(OBJ_DIR)/%.res: $(RES_DIR)/%.rc
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
	$(RM) $(OBJ_DIR)\*.o $(OBJ_DIR)\*.res
