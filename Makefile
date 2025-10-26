clean:
	rm -f *.o simulate

test:
	g++ -std=c++17 simulate.cpp -o simulate
