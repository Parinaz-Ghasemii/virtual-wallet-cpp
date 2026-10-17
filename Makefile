CC       = gcc
CXX      = g++
CFLAGS   = -O2 -Wall -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable
CXXFLAGS = -std=c++11 -O2 -Wall -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wno-vla-extension
BUILD    = build

C_BINS   = $(patsubst phase1_c/%.c,$(BUILD)/%,$(wildcard phase1_c/*.c))
CXX_BINS = $(patsubst phase2_cpp/%.cpp,$(BUILD)/%,$(wildcard phase2_cpp/*.cpp))

all: $(C_BINS) $(CXX_BINS)

$(BUILD)/%: phase1_c/%.c | $(BUILD)
	$(CC) $(CFLAGS) -o $@ $< -lm

$(BUILD)/%: phase2_cpp/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -o $@ $< -lm

$(BUILD):
	mkdir -p $(BUILD)

test: all
	bash tests/run_tests.sh

clean:
	rm -rf $(BUILD)

.PHONY: all test clean
