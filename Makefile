ftp: utility.o ftp_functions.o ftp_client.o
	g++ -o ftp utility.o ftp_functions.o ftp_client.o -std=c++11
ftp_client.o: ftp_client.cpp ftp_functions.o
	g++ -c ftp_client.cpp -std=c++11
ftp_functions.o: utility.o ftp_functions.cpp ftp_functions.h
	g++ -c ftp_functions.cpp -std=c++11
utility.o: utility.cpp utility.h
	g++ -c utility.cpp -std=c++11
