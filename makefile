#copilador
CC = gcc

#Opções de copilação
CFLAGS = -Wall -Wextra -O3 -I lib/linux

#pastas

OBJ_DIR = obj
LINUX_LIBS = lib/linux
WINDOWS_LIBS = lib/windows
EXE_DIR = out
SRC_DIR = src

#arquivos

TARGET = out/exec

LIB_FILES = $(wildcard $(LINUX_LIBS)/*.c)
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

OBJ_FILES = $(patsubst $(LINUX_LIBS)/%.c, $(OBJ_DIR)/%.o, $(wildcard $(LINUX_LIBS)/*.c)) \
			$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

#Copilando

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	@mkdir -p $(EXE_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(LINUX_LIBS)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(EXE_DIR)

