all:	SharedPtr.hpp 
	g++ test.cpp -Wall -Wextra -pedantic -ldl -std=c++11 -o out
	./out
debug:	
	g++ -g ptr_test.cpp -Wall -Wextra -pedantic -ldl -std=c++11 -o out
	gdb ./out

valgrind:
	g++ ptr_test.cpp -Wall -Wextra -pedantic -ldl -std=c++11 -o out
	valgrind ./out	

valgrind_detail:
	g++ test.cpp -Wall -Wextra -pedantic -ldl -std=c++11 -o out
	valgrind --leak-check=full ./out

test:
	g++ ptr_test.cpp -Wall -Wextra -pedantic -ldl -std=c++11 -o out
	./out
clang:	SharedPtr.hpp 
	clang++ ptr_test.cpp -Wall -Wextra -pedantic -ldl -std=c++11 -o out
	./out

