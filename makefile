all:sim
sim:
	g++ -std=c++11 *.cpp -o pipeline
clean:
	rm -f *.o
clean_bin:
	rm -f *.bin
run:
	./pipeline
mv:
	rm -f *.rpt
	g++ -std=c++11 *.cpp -o pipeline
	mv pipeline ~/Archi2017_Project2/archiTA/testcase
