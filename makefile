all:	SharedPtr.hpp 
	g++ test.cpp -Wall -Wextra -pedantic -ldl -std=c++11 -o out
	./out
	rm out
debug:	
	g++ -g test.cpp -Wall -Wextra -pedantic -ldl -std=c++11 -o out
	gdb ./out

valgrind:
	g++ test.cpp -Wall -Wextra -pedantic -ldl -std=c++11 -o out
	valgrind ./out	

valgrind_detail:
	g++ test.cpp -Wall -Wextra -pedantic -ldl -std=c++11 -o out
	valgrind --leak-check=full ./out

test:
	g++ ptr_test.cpp -Wall -Wextra -pedantic -ldl -std=c++11 -o out
	./out
