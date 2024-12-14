
cc=gcc
name=ssed
flags=

build_dir=build/
test_dir=testd/

Coff='\033[0m'
Cred='\033[0;31m'
Cgreen='\033[0;32m'

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
	$(shell echo -n 123abc123456 > $(test_dir)file.txt)
	$(shell ./$(name) $(test_dir)file.txt 's/456/abc/')
	if [ $(shell cat $(test_dir)file.txt) = 123abc123abc ]; then echo $(Cgreen) "replace work"; else echo $(Cred) "replace error"; fi; echo -n $(Coff)
	$(shell ./$(name) $(test_dir)file.txt '/abc/d/')
	if [ $(shell cat $(test_dir)file.txt) = 123123 ]; then echo $(Cgreen) "delete work"; else echo $(Cred) "delete error"; fi; echo -n $(Coff)
	$(shell ./$(name) $(test_dir)file.txt 's/^/aaa/')
	if [ $(shell cat $(test_dir)file.txt) = aaa123123 ]; then echo $(Cgreen) "prefix work"; else echo $(Cred) "prefix error"; fi; echo -n $(Coff)
	$(shell ./$(name) $(test_dir)file.txt 's/$$/bbb/')
	if [ $(shell cat $(test_dir)file.txt) = aaa123123bbb ]; then echo $(Cgreen) "sufix work"; else echo $(Cred) "sufix error"; fi; echo -n $(Coff)

