APP_NAME=CLIENT
GCC=g++


OBJS=strutil.o lin_socket.o lin_thread.o lin_download.o protocolfactory.o protocol.o http.o 
MAIN=main.o
TEST=client.o  
SRC=.

$(APP_NAME) : $(OBJS) $(MAIN)
	$(GCC) -g -Wno-deprecated -fpermissive  $(OBJS) $(MAIN) -o $(APP_NAME) -lpthread
$(SRC)/%.o : $(SRC)/%.cpp
	$(GCC) -g -Wno-deprecated -fpermissive -c $^

test: $(OBJS) $(TEST)
	$(GCC) -g -Wno-deprecated -fpermissive  $(OBJS) $(TEST) -o test -lpthread

clean:
	rm -rf $(APP_NAME)
	rm -rf test
	rm -rf ./*.o
