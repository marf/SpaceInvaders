OBJ = main.o Engine.o GameManager.o Player.o Enemy.o
LDFLAGS = -L /usr/local/lib -lallegro -lallegro_font -lallegro_primitives -lallegro_ttf -lallegro_image
CXXFLAGS = -Wall -DDEBUG 

SpaceInvaders: $(OBJ)
	g++ -o SpaceInvaders $(OBJ) $(LDFLAGS)
	./SpaceInvaders

-include dependencies

.PHONY: depend clean cleanall record

depend:
	g++ -MM *.cc > dependencies
clean:
	rm -f *.o
cleanall:
	rm -f SpaceInvaders *.o *~
record:
	rm -f data/record.txt
run:
	./SpaceInvaders	

