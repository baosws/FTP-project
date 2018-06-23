all: ftp args_parser
ftp: object/utility.o object/ftp_functions.o object/ftp_client.o
	g++ -o ftp object/utility.o object/ftp_functions.o object/ftp_client.o -std=c++11
object/ftp_client.o: source/ftp_client.cpp object/ftp_functions.o
	g++ -c source/ftp_client.cpp -o object/ftp_client.o -std=c++11
object/ftp_functions.o: object/utility.o source/ftp_functions.cpp header/ftp_functions.h
	g++ -c source/ftp_functions.cpp -o object/ftp_functions.o -std=c++11
object/utility.o: source/utility.cpp header/utility.h
	g++ -c source/utility.cpp -o object/utility.o -std=c++11
args_parser: object/args_parser.o
	g++ -o args_parser object/args_parser.o -std=c++11
object/args_parser.o: source/args_parser.cpp
	g++ -c source/args_parser.cpp -o object/args_parser.o -std=c++11
