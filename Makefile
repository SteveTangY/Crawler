#
#
object = main.o GetUrlResource.o
options = 
CC = g++ -g
exLib = -lpthread -lssl -lcrypto
#
mythreadDir = ./mythread/mythread.cpp -lpthread
mysocketDir = ./mysocket/mysocket.cpp -lssl -lcrypto
html_analysisDir = ./html_analysis/html_analysis.cpp -lpthread
exObj = mythread.o mysocket.o html_analysis.o
#
reptile : $(object) $(exObj)
	$(CC) $(object) $(exObj) -o reptile $(options) $(exLib)
#
main.o : main.cpp GetUrlResource.h $(exObj)
	$(CC) -c main.cpp $(options)
#
GetUrlResource.o : main.cpp GetUrlResource.cpp GetUrlResource.h $(exObj)
	$(CC) -c GetUrlResource.cpp 
#
mythread.o : $(mythreadDir)
	$(CC) -c $(mythreadDir)
#
mysocket.o : $(mysocketDir)
	$(CC) -c $(mysocketDir) 
#
html_analysis.o : $(html_analysisDir)
	$(CC) -c $(html_analysisDir)
#
.PHONY : clean
	rm -f $(object) $(exObj)
