default:
	gcc *.c -o sandbox -std=c99
	@echo -e "\n###\nYou need to 'suid' the sandbox program in order to use it"

clean:
	rm -f sandbox
