CFLAGS = -Wno-implicit-function-declaration

rerun: main.o sqlite3.o database.o utils.o
	@echo Linking and producing the final application
	@gcc $(CFLAGS) main.o database.o sqlite3.o utils.o -o app

main.o: main.c interface.c
	@echo Compling main file
	@gcc $(CFLAGS) -c main.c

database.o: database.c
	@echo Compiling the database file
	@gcc $(CFLAGS) -c database.c

sqlote3.o: sqlite3.c
	@echo Compiling the sqlite3 file
	@gcc $(CFLAGS) -c sqlite3.c

utils.o: utils.c
	@echo Compiling the utils file
	@gcc $(CFLAGS) -c utils.c

clean:
	@echo Removing everything but the source and sqlite3.o files
	@rm main.o database.o utils.o
	
