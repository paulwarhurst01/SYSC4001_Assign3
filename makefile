all: User Text-Manager

User: User.c
	gcc -o User User.c

Text-Manager: Text-Manager.c
	gcc -o Text-Manager Text-Manager.c