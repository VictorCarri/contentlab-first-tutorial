compiler=g++-10
libs=$(addprefix -l,pthread)
optimOpts=-O3
dbgOpts=-ggdb3

all: client dbgClient

client: main.cpp
	$(compiler) -o $@ $^ $(libs) $(optimOpts)

dbgClient: main.cpp
	$(compiler) -o $@ $^ $(libs) $(optimOpts) $(dbgOpts)
