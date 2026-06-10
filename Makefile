# Makefile for CryptoBook

.PHONY: all build run test clean lint format format-cpp lint-cpp help stream

# Variables
BUILD_DIR := build
BUILD_TYPE := Release
EXECUTABLE := $(BUILD_DIR)/src/binance_engine
PYTHON_FILES := main.py
CMAKE := cmake
CLANG_FORMAT := clang-format
PYTHON := uv run python

all: build

# Configure and build the C++ project
build:
	@echo "Configuring and building C++ project..."
	@$(CMAKE) -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang
	@$(CMAKE) --build $(BUILD_DIR) --config $(BUILD_TYPE)

# Run the main executable
run: build
	@echo "Running the executable..."
	@$(EXECUTABLE)

# Run Python market data streamer
stream:
	@echo "Starting market data stream..."
	@$(PYTHON) main.py

# Run the tests
test: build
	@echo "Running C++ tests..."
	@cd $(BUILD_DIR) && ctest --output-on-failure -C $(BUILD_TYPE) -V

# Lint C++ code
lint-cpp:
	@echo "Linting C++ code..."
	@find src -name "*.cpp" | xargs clang-tidy -p build

# Format C++ code
format-cpp:
	@echo "Formatting C++ code..."
	@find src include -name "*.cpp" -o -name "*.hpp" | xargs $(CLANG_FORMAT) -i

# Format all code (C++ and Python)
format: format-cpp
	@echo "Formatting Python code..."
	@uv run black --line-length 144 .

# Lint all code (C++ and Python)
lint: lint-cpp
	@echo "Linting Python code..."
	@uv run ruff check .

# Clean the build directory
clean:
	@echo "Cleaning the build directory..."
	@rm -rf $(BUILD_DIR)

help:
	@echo "Available commands:"
	@echo "  make all         - (Default) Configure and build the C++ project"
	@echo "  make build       - Configure and build the C++ project"
	@echo "  make run         - Build and run the main C++ executable"
	@echo "  make stream      - Run Python market data streamer"
	@echo "  make test        - Build and run C++ tests"
	@echo "  make lint        - Lint C++ and Python code"
	@echo "  make lint-cpp    - Lint C++ code only"
	@echo "  make format      - Format C++ and Python code"
	@echo "  make format-cpp  - Format C++ code only"
	@echo "  make clean       - Remove the build directory"
	@echo "  make help        - Show this help message"
