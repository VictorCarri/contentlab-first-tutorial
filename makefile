compiler=g++-10
libs=$(addprefix -l,pthread)
optimOpts=-O3
dbgOpts=-ggdb3 -DDEBUG
files=$(addsuffix .cpp,main Request Reply)
dbgObjs=$(addsuffix .debug.o,main Request Reply)
prodObjs=$(addsuffix .o,main Request Reply)
commonOpts=$(addprefix -W,all error) -std=gnu++17 -save-temps

all: client dbgClient

%.o: %.cpp
	$(compiler) -o $@ $^ $(libs) $(optimOpts) $(commonOpts)

%.debug.o: %.cpp
	$(compiler) -o $@ $^ $(libs) $(optimOpts) $(dbgOpts) $(commonOpts)

client: $(prodObjs)
	$(compiler) -o $@ $^ $(libs) $(optimOpts) $(commonOpts)

dbgClient: $(dbgObjs)
	$(compiler) -o $@ $^ $(libs) $(optimOpts) $(dbgOpts) $(commonOpts)

#client: $(files)
#	$(compiler) -o $@ $^ $(libs) $(optimOpts) $(commonOpts)

#dbgClient: $(files)
#	$(compiler) -o $@ $^ $(libs) $(optimOpts) $(dbgOpts) $(commonOpts)

clean:
	rm -f client dbgClient
