CC = g++
CFLAGS = -Wall -O2 -g #-pg #gprof
LDFLAGS = -lSDL -lSDL_image
INCLUDES = -I include -I include/eigen

SRC_PATH = src
BIN_PATH = bin
EXEC = trifocal
SRC_FILES = $(shell find $(SRC_PATH) -type f -name '*.cpp')
OBJ_FILES = $(patsubst $(SRC_PATH)/%.cpp, $(SRC_PATH)/%.o, $(SRC_FILES))
MAT_FILES = debug

RM       = rm -f
DIRNAME  = $(shell basename $$PWD)
BACKUP   = $(shell date +`basename $$PWD`-%m.%d.%H.%M.tgz)

all: $(BIN_PATH)/$(EXEC)

$(BIN_PATH)/$(EXEC): $(OBJ_FILES)
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo "--------------------------------------------------------------"
	@echo "                 to execute type: ./$(BIN_PATH)/$(EXEC) &"
	@echo "--------------------------------------------------------------"

$(SRC_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(CC) -c -o $@ $(CFLAGS) $(INCLUDES) $^ 

clean :	
	@echo "**************************"
	@echo "CLEAN"
	@echo "**************************"
	$(RM) *~ $(OBJ_FILES) $(BIN_PATH)/$(EXEC)
	$(RM) $(MAT_FILES)/*.mat

cleanall :
	@echo "**************************"
	@echo "CLEAN ALL"
	@echo "**************************"
	find . -name '*~' -exec rm -fv {} \;
	$(RM) *~ $(OBJ_FILES) $(BIN_PATH)/$(EXEC) output/*
	$(RM) $(MAT_FILES)/*.mat

tar : clean 
	@echo "**************************"
	@echo "TAR"
	@echo "**************************"
	cd .. && tar cvfz $(BACKUP) $(DIRNAME)
