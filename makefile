compiler=g++-10
libs=$(addprefix -l,pthread)
optimOpts=-O3
dbgOpts=-ggdb3 -DDEBUG
files=$(addsuffix .cpp,main Request)

all: client dbgClient

client: $(files)
	$(compiler) -o $@ $^ $(libs) $(optimOpts)

dbgClient: $(files)
	$(compiler) -o $@ $^ $(libs) $(optimOpts) $(dbgOpts)

clean:
	rm -f client dbgClient
