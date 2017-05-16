all:
	g++ -o main  `xml2-config --libs --cflags` -lpqxx -lpq main.cpp
