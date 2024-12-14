
cc=gcc
name=ssed
flags=

build_dir=build/
test_dir=testd/

$(name): $(build_dir)main.o $(build_dir)files.o $(build_dir)ssed.o
	$(cc) $(build_dir)main.o $(build_dir)files.o $(build_dir)ssed.o $(flags) -o$(name)

$(build_dir)main.o: main.c
	$(cc) main.c $(flags) -c -o$(build_dir)main.o

$(build_dir)files.o: files.c
	$(cc) files.c $(flags) -c -o$(build_dir)files.o

$(build_dir)ssed.o: ssed.c
	$(cc) ssed.c $(flags) -c -o$(build_dir)ssed.o

clean:
	rm $(name)
	rm -r build/*

test: $(name)
	echo -n 123abc123456 > $(test_dir)file.txt
	./$(name) $(test_dir)file.txt 's/456/abc/'
	if [ $(file < $(test_dir)file.txt) = 123abc123abc ]; then echo "replace work"; else echo "replace error"; fi
	./$(name) $(test_dir)file.txt '/abc/d/'
	if [ $(file < $(test_dir)file.txt) = 123123 ]; then echo "delete work"; else echo "delete error"; fi
	./$(name) $(test_dir)file.txt 's/^/aaa/'
	if [ $(file < $(test_dir)file.txt) = aaa123123 ]; then echo "prefix work"; else echo "prefix error"; fi
	./$(name) $(test_dir)file.txt 's/$$/bbb/'
	if [ $(file < $(test_dir)file.txt) = aaa123123bbb ]; then echo "sufix work"; else echo "sufix error"; fi

