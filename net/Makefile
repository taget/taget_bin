APP_NAME=CLIENT
GCC=g++
ARGS=-g -Wno-deprecated -fpermissive
LIB=-lpthread

OBJS=strutil.o lin_socket.o lin_thread.o lin_download.o protocolfactory.o protocol.o http.o 
MAIN=main.o
TEST=client.o  
SRC=.

$(APP_NAME) : $(OBJS) $(MAIN)
	$(GCC) $(ARGS)  $(OBJS) $(MAIN) -o $(APP_NAME) $(LIB)
$(SRC)/%.o : $(SRC)/%.cpp
	$(GCC) $(ARGS) -c $^

test: $(OBJS) $(TEST)
	$(GCC) $(ARGS) $(OBJS) $(TEST) -o test $(LIB)

clean:
	rm -rf $(APP_NAME)
	rm -rf test
	rm -rf ./*.o
