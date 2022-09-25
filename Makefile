APP ?=
BUILD_DIR := ./bin
SRC_DIRS := ./src/$(APP)
CC := gcc

# Find all the C files we want to compile
SRCS := $(shell find $(SRC_DIRS) -name '*.c')




$(BUILD_DIR)/$(APP): check-app
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(SRCS) -o $@


run: $(BUILD_DIR)/$(APP)
	@$(BUILD_DIR)/$(APP)


clean:
	@rm -r $(BUILD_DIR)

check-app:
ifndef APP
	$(error APP is unspicified, \
	Please choose one of the following apps: femtoShell, ... \
	Example command: make APP=femtoShell )	 
endif

.PHONY: clean check-app run