all: graph nograph


graph: bb2_solver.cpp node.h
	g++ -DGRAPH -o bb2_graph bb2_solver.cpp field.cpp

nograph: bb2_solver.cpp node.h
	g++ -o bb2_nograph bb2_solver.cpp field.cpp

clean:
	rm bb2_graph bb2_nograph bb2_graph.exe bb2_nograph.exe
