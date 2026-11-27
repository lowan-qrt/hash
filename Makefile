# Hash: Markdown editor (macOS and Linux)
#
#   make deps    : installs IUP into deps/ (only once)
#   make build   : compiles the application ("make" alone does the same)
#   make release : make deps + make build + removes the dependencies
#   make run     : runs the application, without recompiling
#   make clean   : removes the compiled application (keeps deps/)
#   make purge   : make clean + removes the dependencies
#
# Warning: the project path must not contain spaces.

# ---------- Application ----------
CC        = gcc
CFLAGS    = -Wall -Wextra -g
BUILD_DIR = build
# Add future .c files here
SRC     = src/main.c src/file.c src/ui.c
HEADERS = src/colors.h src/file.h src/theme.h src/ui.h
TARGET  = $(BUILD_DIR)/hash

# ---------- IUP ----------
IUP_VERSION = 4.0.0
IUP_URL     = https://github.com/gen2brain/iup-go/releases/download/iup%2Fv0.40.0/iup-4.0.0.tar.gz
IUP_SHA256  = 471668f1a29e0f019afb8914cc9a1d8cc068da4724d20703800864c60cf96968

DEPS        = $(CURDIR)/deps
IUP_ARCHIVE = $(DEPS)/iup-$(IUP_VERSION).tar.gz
IUP_SRC     = $(DEPS)/iup-$(IUP_VERSION)
DEPS_LOG    = $(DEPS)/deps.log

# Overridable: make build IUP_DIR=/other/path, make deps CMAKE=/path/to/cmake
IUP_DIR ?= $(DEPS)/iup-install
CMAKE   ?= cmake

# ---------- Compiler options ----------
PKG     = PKG_CONFIG_PATH=$(IUP_DIR)/lib/pkgconfig pkg-config
CFLAGS += $(shell $(PKG) --cflags iup)
LIBS    = $(shell $(PKG) --static --libs iup)

# ---------- Commands ----------
.PHONY: build deps release run clean purge check-deps check-build

build: $(TARGET)

# "| check-build": checks first, without forcing a rebuild
$(TARGET): $(SRC) $(HEADERS) | check-build
	@echo "Building $(TARGET)..."
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)
	@echo "Build done."

deps: check-deps
	@mkdir -p $(DEPS)
	@echo "[1/4] Downloading IUP $(IUP_VERSION)..."
	@curl -fsSL -o $(IUP_ARCHIVE) $(IUP_URL)
	@echo "$(IUP_SHA256)  $(IUP_ARCHIVE)" | shasum -a 256 -c - >/dev/null
	@echo "[2/4] Extracting..."
	@tar -xzf $(IUP_ARCHIVE) -C $(DEPS)
	@echo "[3/4] Building (may take a while)..."
	@{ $(CMAKE) -S $(IUP_SRC) -B $(IUP_SRC)/build && \
	   $(CMAKE) --build $(IUP_SRC)/build; } > $(DEPS_LOG) 2>&1 || \
	   { echo "ERROR: build failed, see $(DEPS_LOG)"; exit 1; }
	@echo "[4/4] Installing..."
	@$(CMAKE) --install $(IUP_SRC)/build --prefix $(IUP_DIR) >> $(DEPS_LOG) 2>&1 || \
	   { echo "ERROR: install failed, see $(DEPS_LOG)"; exit 1; }
	@rm -rf $(IUP_SRC) $(IUP_ARCHIVE)
	@echo "IUP is installed. Then: make build"

release: deps build
	@rm -rf $(DEPS)
	@echo "Dependencies removed."

run:
	@test -x $(TARGET) || { echo "ERROR: Program missing, run: make build"; exit 1; }
	@./$(TARGET)

clean:
	@rm -rf $(BUILD_DIR)
	@echo "Application cleaned."

purge: clean
	@rm -rf $(DEPS)
	@echo "Dependencies removed."

# ---------- Tool checks ----------
# "@": does not print the command. "$$": a literal "$" for the shell.
check-deps:
	@for t in $(CC) curl tar shasum pkg-config $(CMAKE); do \
		command -v $$t >/dev/null 2>&1 || { \
			echo "ERROR: Missing tool: $$t (see README requirements)"; \
			exit 1; }; \
	done
	@if [ "$$(uname)" = "Linux" ]; then \
		pkg-config --exists gtk+-3.0 || { \
			echo "ERROR: GTK3 missing, run: sudo apt install libgtk-3-dev"; \
			exit 1; }; \
	fi

check-build:
	@command -v pkg-config >/dev/null 2>&1 || { \
		echo "ERROR: Missing tool: pkg-config (see README requirements)"; \
		exit 1; }
	@test -f $(IUP_DIR)/lib/pkgconfig/iup.pc || { \
		echo "ERROR: IUP missing, run: make deps"; \
		exit 1; }
