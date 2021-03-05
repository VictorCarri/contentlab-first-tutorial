compiler=g++-10
libs=$(addprefix -l,pthread)

prog: main.cpp Client.cpp
	$(compiler) -o $@ $^ $(libs)
