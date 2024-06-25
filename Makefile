
all:
	javac *.java
	gcc src/*.c -std='c99' -Wall -Wextra -pedantic -o programaTrab
clean:
	rm *.class
	rm programaTrab
run:
	java MainFrame
