CC=g++
CFLAGS=-std=c++17
DB_FLAGS=-lpqxx -lpq
EXEC_FILE=task_tracker

all: install

install: main.o database.o backend.o task_service.o
	$(CC) $(CFLAGS) $^ -o $(EXEC_FILE) $(DB_FLAGS)

main.o: src/main.cpp src/database/database.h
	$(CC) -c $(CFLAGS) src/main.cpp -o $@

backend.o: src/backend/backend.cpp src/backend/backend.h
	$(CC) -c $(CFLAGS) src/backend/backend.cpp -o $@

task_service.o: src/TaskService/task_service.cpp src/TaskService/task_service.h
	$(CC) -c $(CFLAGS) src/TaskService/task_service.cpp -o $@

database.o: src/database/database.cpp src/database/database.h
	$(CC) -c $(CFLAGS) src/database/database.cpp -o $@


clean:
	rm -rf *.o task_tracker