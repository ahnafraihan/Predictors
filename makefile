# Ahnaf Raihan
# predictors

all:	predictors

predictors:	main.o Predictor.o
	g++  main.o Predictor.o -o predictors

Predictor.o:	Predictor.cpp
	g++ -std=c++11 -c  Predictor.cpp 

main.o:	main.cpp
	g++ -std=c++11 -c main.cpp

clean:
	rm -f *.o predictors

