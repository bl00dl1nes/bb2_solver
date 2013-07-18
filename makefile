all: graph 


graph: bb2_solver.cpp node.h
	g++ -o bb2_graph bb2_solver.cpp field.cpp

clean:
	rm bb2_solver bb2_solver.exe
