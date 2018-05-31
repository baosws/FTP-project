ftp: utility.o ftp_functions.o ftp_client.o
	g++ -o ftp utility.o ftp_functions.o ftp_client.o
ftp_client.o: ftp_client.cpp ftp_functions.o
	g++ -c ftp_client.cpp
ftp_functions.o: utility.o ftp_functions.cpp ftp_functions.h
	g++ -c ftp_functions.cpp
utility.o: utility.cpp utility.h
	g++ -c utility.cpp
