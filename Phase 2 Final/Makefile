#------------------------
# Do not modify this file
#------------------------

all: trader

# For part1: marked with a blank trader
market: main.cpp market.cpp worker.cpp blank_trader.cpp
#market: main.cpp market.cpp worker.cpp blank_trader.cpp trade_heap.cpp stock_rbt.cpp client_rbt.cpp simple_rbt.cpp stock_rbt_prime.cpp median_heap.cpp
	g++ -std=c++20 main.cpp market.cpp worker.cpp blank_trader.cpp -lpthread -o market
#	g++ -std=c++20 main.cpp market.cpp worker.cpp blank_trader.cpp trade_heap.cpp stock_rbt.cpp client_rbt.cpp simple_rbt.cpp stock_rbt_prime.cpp median_heap.cpp -lpthread -o market
	./market 2 > output.txt

# For part2
trader: main.cpp market.cpp worker.cpp trader.cpp
	g++ -std=c++20 main.cpp market.cpp worker.cpp trader.cpp -lpthread -o trader
#trader: main.cpp market.cpp worker.cpp trader.cpp trade_heap.cpp stock_rbt.cpp client_rbt.cpp simple_rbt.cpp stock_rbt_prime.cpp
#	g++ -std=c++20 main.cpp market.cpp worker.cpp trader.cpp trade_heap.cpp stock_rbt.cpp client_rbt.cpp simple_rbt.cpp stock_rbt_prime.cpp -lpthread -o trader
	./trader 2 > output.txt

clean:
	rm -rf market trader output.txt
	rm -rf *~
