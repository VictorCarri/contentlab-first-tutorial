compiler=g++-10
optimOpts=-O3
dbgOpts=-ggdb3 -DDEBUG
files=$(addsuffix .cpp,main Request Reply)
dbgObjs=$(addsuffix .debug.o,main Request Reply)
prodObjs=$(addsuffix .o,main Request Reply)
commonOpts=$(addprefix -W,all error) -std=gnu++17 -save-temps
libDirs=-L/usr/local/lib/boost
boostLibs=json regex program_options
libs=$(addprefix -l,pthread $(addprefix boost_,$(addsuffix -gcc10-mt-x64-1_75,$(boostLibs))))
dbgLibs=$(addprefix -l,pthread $(addprefix boost_,$(addsuffix -gcc10-mt-d-x64-1_75,$(boostLibs))))

all: client dbgClient

rebuild: clean all

%.o: %.cpp
	$(compiler) -c -o $@ $^ $(optimOpts) $(commonOpts)

%.debug.o: %.cpp
	$(compiler) -c -o $@ $^ $(optimOpts) $(dbgOpts) $(commonOpts)

client: $(prodObjs)
	$(compiler) -o $@ $^ $(libDirs) $(libs) $(optimOpts) $(commonOpts)

dbgClient: $(dbgObjs)
	$(compiler) -o $@ $^ $(libDirs) $(dbgLibs) $(optimOpts) $(dbgOpts) $(commonOpts)

#client: $(files)
#	$(compiler) -o $@ $^ $(libs) $(optimOpts) $(commonOpts)

#dbgClient: $(files)
#	$(compiler) -o $@ $^ $(libs) $(optimOpts) $(dbgOpts) $(commonOpts)

clean:
	rm -f client dbgClient *.{o,ii,s}
