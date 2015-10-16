CC=g++
CFLAGS= -std=c++11
objects = kpi_stat_analysis.o test.o
object	= kpi_primitive_view-odb.o
#cxx	= kpi_primitive_view-odb.cxx
#other	= kpi_primitive_view-odb.hxx kpi_primitive_view-odb.cxx
all:$(objects) $(object)
	g++ -std=c++11 -o kpi_stat_test $(objects) $(object) -lodb-mysql -lodb
$(objects): %.o: %.cpp
	$(CC) -c -g $(CFLAGS) $< -o $@
$(object): %.o: %.cxx
	$(CC) -c -g $(CFLAGS) $< -o $@	
clean:
	rm -f *.o kpi_stat_test
other: 
	odb -d mysql --generate-query kpi_primitive_view.h 
