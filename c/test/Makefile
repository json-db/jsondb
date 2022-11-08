OBJS = dbBuildTest dbQueryTest

all: $(OBJS)

dbBuildTest:
	gcc -Wall -D_DEBUG ../docDB.c dbBuildTest.c -o dbBuildTest

dbQueryTest:
	gcc -Wall -D_DEBUG ../docDB.c dbQueryTest.c -o dbQueryTest

clean:
	rm $(OBJS)