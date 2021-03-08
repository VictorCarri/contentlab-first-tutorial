compiler=g++-10
libs=$(addprefix -l,pthread)
optimOpts=-O3
dbgOpts=-ggdb3 -DDEBUG
files=$(addsuffix .cpp,main Request)
commonOpts=$(addprefix -W,all error)

all: client dbgClient

client: $(files)
	$(compiler) -o $@ $^ $(libs) $(optimOpts) $(commonOpts)

dbgClient: $(files)
	$(compiler) -o $@ $^ $(libs) $(optimOpts) $(dbgOpts) $(commonOpts)

clean:
	rm -f client dbgClient
