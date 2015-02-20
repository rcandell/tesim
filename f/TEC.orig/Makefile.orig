executables=$(addsuffix .exe,temain temain_mod temain_nc temain_out)

all: $(executables)

run: temain_out.exe cleandat
		temain_out.exe

%.exe: %.f tecommon.inc teprob.o
	gfortran $< teprob.o -o $@

teprob.o: teprob.f
	gfortran -c $<

clean: cleandat
	-del $(executables) teprob.o

cleandat:
	-del singlefileoutput\*.dat

.PHONY: clean run
