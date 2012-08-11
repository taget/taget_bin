APP_NAME=CLIENT

OBJS=strutil.o client.o
SRC=.

$(APP_NAME) : $(OBJS)
	g++ -g -Wno-deprecated -fpermissive  $(OBJS) -o $(APP_NAME) -lpthread
$(SRC)/%.o : $(SRC)/%.cpp
	g++ -g -Wno-deprecated -fpermissive -c $^

clean:
	rm -rf $(APP_NAME)
	rm -rf ./*.o
