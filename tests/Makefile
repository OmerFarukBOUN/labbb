main: build check_output clean

build: build_dir cudd_test lex_yacc_test

build_dir:
	mkdir -p build

cudd_test: cudd_test1 cudd_test2 cudd_test3

cudd_test1: src/cudd_test1.c
	gcc -o build/cudd_test1 src/cudd_test1.c -lcudd
	echo "cudd_test1 compiled successfully"

cudd_test2: src/cudd_test2.c
	gcc -o build/cudd_test2 src/cudd_test2.c -lcudd
	echo "cudd_test2 compiled successfully"

cudd_test3: src/cudd_test3.c
	gcc -o build/cudd_test3 src/cudd_test3.c -lcudd
	echo "cudd_test3 compiled successfully"

lex_yacc_test: src/lex_yacc_test.y src/lex_yacc_test.l
	bison -dy src/lex_yacc_test.y -o build/test.tab.c
	flex -o build/test.yy.c src/lex_yacc_test.l
	gcc -o build/lex_yacc_test build/test.tab.c build/test.yy.c -lfl -ly
	echo "lex_yacc_test compiled successfully"

check_output: check_cudd_test3 check_lex_yacc_test

check_cudd_test3: build/cudd_test3 outputs/cudd_test3.txt
	./build/cudd_test3 > build/cudd_test3.txt
	diff <(grep -v 'ID' build/cudd_test3.txt) <(grep -v 'ID' outputs/cudd_test3.txt)
	echo "cudd_test3 output is correct"

check_lex_yacc_test: build/lex_yacc_test outputs/lex_yacc_test.txt
	printf "4+5\n(4*-5)\n(4-(-5))\n" | ./build/lex_yacc_test  > build/lex_yacc_test.txt
	diff build/lex_yacc_test.txt outputs/lex_yacc_test.txt
	echo "lex_yacc_test output is correct"

clean:
	rm -rf build
