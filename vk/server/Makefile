MAIN_SERVER_DIR = main_server
MAIN_SERVER_NAME = main_server
HANDLER_DIR = handler
HANDLER_NAME = handler

.PHONY: all clean start

all:
	cd $(HANDLER_DIR) && make && make clean_obj
	cd $(MAIN_SERVER_DIR) && make && make clean_obj
clean:
	cd $(HANDLER_DIR) && make clean
	cd $(MAIN_SERVER_DIR) && make clean

start:
	./$(MAIN_SERVER_DIR)/$(MAIN_SERVER_NAME)
