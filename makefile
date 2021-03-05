compiler=g++-10
libs=$(addprefix -l,pthread)
optimOpts=-O3
dbgOpts=-ggdb3

prog: main.cpp Client.cpp
	$(compiler) -o $@ $^ $(libs) $(optimOpts) $(dbgOpts)
