all:
	@echo "\033[34m[INFO]\tBuilding Project\033[0m"
	@gcc main.c -o main
	@echo "\033[34m[INFO]\tStarting app\033[0m"
	@./main

clean:
	rm -rf main