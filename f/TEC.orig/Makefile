executables=$(addsuffix .exe,temain_rc)

all: $(executables)

run: temain_rc.exe cleandat
		./temain_rc.exe

%.exe: %.f tecommon.inc teprob.o
	gfortran $< teprob.o -o $@

teprob.o: teprob.f
	gfortran -c $<

clean: cleandat
	rm $(executables) teprob.o

cleandat:
	rm singlefileoutput/*.dat

.PHONY: clean run
