INIT_PROGRAM_NAME = process_handler
INIT_PROGRAM_DIR = process_handler
DATABASE_NAME = database
DATABASE_DIR = database

.PHONY: all clean start

all:
	cd $(DATABASE_DIR) && make && make clean_obj
	cd $(INIT_PROGRAM_DIR) && make && make clean_obj
clean:
	cd $(DATABASE_DIR) && make clean
	cd $(INIT_PROGRAM_DIR) && make clean

start:
	./$(INIT_PROGRAM_DIR)/$(INIT_PROGRAM_NAME)
