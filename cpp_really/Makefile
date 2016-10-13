CC=g++
INC=-I/usr/local/Cellar/boost/1.62.0/include/boost/ -I/usr/local/Cellar/avro-cpp/1.8.1/include/

main: main.cpp
	$(CC) -g -o main main.cpp $(INC) -l avrocpp 
	
clean:
	rm main
	rm -rf main.dSYM/
