# Compiler and flags
CXX       := g++
CXXFLAGS  := -O2 -std=c++23
LDFLAGS   := 
OUTPUT_DIR := ../bin

# Sources and objects
TARGET    := ARSTSetShareNumber
SRC       := ARSTSharedMemory.cpp ARSTSetTickNumber.cpp
OBJS      := $(SRC:.cpp=.o)

# Default target
all: $(OUTPUT_DIR)/$(TARGET)

# Build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link final binary
$(OUTPUT_DIR)/$(TARGET): $(OBJS)
	mkdir -p $(OUTPUT_DIR)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

# Clean
clean:
	rm -f $(OBJS) $(OUTPUT_DIR)/$(TARGET)

