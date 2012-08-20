APP_NAME=CLIENT

OBJS=strutil.o lin_socket.o protocolfactory.o protocol.o http.o 
MAIN=main.o
TEST=client.o  
SRC=.

$(APP_NAME) : $(OBJS) $(MAIN)
	g++ -g -Wno-deprecated -fpermissive  $(OBJS) $(MAIN) -o $(APP_NAME) -lpthread
$(SRC)/%.o : $(SRC)/%.cpp
	g++ -g -Wno-deprecated -fpermissive -c $^

test: $(OBJS) $(TEST)
	g++ -g -Wno-deprecated -fpermissive  $(OBJS) $(TEST) -o test -lpthread

clean:
	rm -rf $(APP_NAME)
	rm -rf test
	rm -rf ./*.o
