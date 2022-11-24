target=a.out
src=main.o StarMap.o
$(target):$(src)
	g++ $(src) -o $(target)

StarMap.o:StarMap.cpp StarMap.h Star.h Helpers.h
	g++ -c -o $@ $< -g

main.o:main.cpp StarMap.h Helpers.h Star.h
	g++ -c -o $@ $< -g

clean:
	rm a.out main.o StarMap.o
