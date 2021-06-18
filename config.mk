# ======= BUILD CONFIG ======= #

# --- COMPILER --- #

CC = gcc

# --- LINKER --- #

LD = gcc

# --- DEPENDENCY GENERATOR --- #

MM = gcc

# --- COMPILER FLAGS --- #

CCFLAGS += -std=c17
CCFLAGS += -Wall
CCFLAGS += -Wpedantic

# --- LINKER FLAGS --- #

LDFLAGS += -std=c17
LDFLAGS += -Wall
LDFLAGS += -Wpedantic

# --- LINKER LIBRARIES --- #

LDLIBS +=

# --- ARTIFACTS FOLDER --- #

ART = bin

# --- BINARY EXECUTABLE NAME --- #

EXE = demo

# --- SOURCE FILE DIRECTORIES --- #

DIR +=

# ============================ #
