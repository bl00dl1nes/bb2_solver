all: graph 


graph: bb2_solver.cpp field.cpp field.h node.h
	g++ -o bb2_solver bb2_solver.cpp field.cpp

debug: bb2_solver.cpp field.cpp field.h node.h
	g++ -o bb2_solver -g bb2_solver.cpp field.cpp

clean:
	rm bb2_solver bb2_solver.exe
