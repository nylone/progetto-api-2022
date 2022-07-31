echo "preliminary work"
mkdir -p tester_output/diffs tester_output/executed open_testcases tester_output/memcheck tester_output/callgrind tester_output/massif

echo "compiling for debug"
gcc $1 -Wall -Werror -O2 -g3 -fsanitize=address -o tester_output/debug_main

echo "compiling for valgrind"
gcc $1 -Wall -Werror -O2 -g3 -o tester_output/valgrind_main

echo "  running insert.txt"
./tester_output/debug_main < open_testcases/insert.txt > tester_output/executed/insert.txt
diff open_testcases/insert.output.txt tester_output/executed/insert.txt > tester_output/diffs/insert.txt
echo "  running slide.txt"
./tester_output/debug_main < open_testcases/slide.txt > tester_output/executed/slide.txt
diff open_testcases/slide.output.txt tester_output/executed/slide.txt > tester_output/diffs/slide.txt
echo "  running test1.txt"
./tester_output/debug_main < open_testcases/test1.txt > tester_output/executed/test1.txt
diff open_testcases/test1.output.txt tester_output/executed/test1.txt > tester_output/diffs/test1.txt
echo "  running test2.txt"
./tester_output/debug_main < open_testcases/test2.txt > tester_output/executed/test2.txt
diff open_testcases/test2.output.txt tester_output/executed/test2.txt > tester_output/diffs/test2.txt
echo "  running test3.txt"
./tester_output/debug_main < open_testcases/test3.txt > tester_output/executed/test3.txt
diff open_testcases/test3.output.txt tester_output/executed/test3.txt > tester_output/diffs/test3.txt

echo "  memcheck:"
echo "    running insert.txt"
valgrind --tool=memcheck --log-file=tester_output/memcheck/insert.out ./tester_output/valgrind_main < open_testcases/insert.txt > /dev/null
echo "    running slide.txt"
valgrind --tool=memcheck --log-file=tester_output/memcheck/slide.out ./tester_output/valgrind_main < open_testcases/slide.txt > /dev/null
echo "    running test1.txt"
valgrind --tool=memcheck --log-file=tester_output/memcheck/test1.out ./tester_output/valgrind_main < open_testcases/test1.txt > /dev/null
echo "    running test1.txt"
valgrind --tool=memcheck --log-file=tester_output/memcheck/test2.out ./tester_output/valgrind_main < open_testcases/test2.txt > /dev/null
echo "    running test1.txt"
valgrind --tool=memcheck --log-file=tester_output/memcheck/test3.out ./tester_output/valgrind_main < open_testcases/test3.txt > /dev/null

echo "  callgrind:"
echo "    running insert.txt"
valgrind --quiet --tool=callgrind --callgrind-out-file=tester_output/callgrind/callgrind.out.insert ./tester_output/valgrind_main < open_testcases/insert.txt > /dev/null
echo "    running slide.txt"
valgrind --quiet --tool=callgrind --callgrind-out-file=tester_output/callgrind/callgrind.out.slide ./tester_output/valgrind_main < open_testcases/slide.txt > /dev/null
echo "    running test1.txt"
valgrind --quiet --tool=callgrind --callgrind-out-file=tester_output/callgrind/callgrind.out.test1 ./tester_output/valgrind_main < open_testcases/test1.txt > /dev/null
echo "    running test2.txt"
valgrind --quiet --tool=callgrind --callgrind-out-file=tester_output/callgrind/callgrind.out.test2 ./tester_output/valgrind_main < open_testcases/test2.txt > /dev/null
echo "    running test3.txt"
valgrind --quiet --tool=callgrind --callgrind-out-file=tester_output/callgrind/callgrind.out.test3 ./tester_output/valgrind_main < open_testcases/test3.txt > /dev/null

echo "  massif:"
echo "    running insert.txt"
valgrind --quiet --tool=massif --massif-out-file=tester_output/massif/massif.out.insert ./tester_output/valgrind_main < open_testcases/insert.txt > /dev/null
echo "    running slide.txt"
valgrind --quiet --tool=massif --massif-out-file=tester_output/massif/massif.out.slide ./tester_output/valgrind_main < open_testcases/slide.txt > /dev/null
echo "    running test1.txt"
valgrind --quiet --tool=massif --massif-out-file=tester_output/massif/massif.out.test1 ./tester_output/valgrind_main < open_testcases/test1.txt > /dev/null
echo "    running test2.txt"
valgrind --quiet --tool=massif --massif-out-file=tester_output/massif/massif.out.test2 ./tester_output/valgrind_main < open_testcases/test2.txt > /dev/null
echo "    running test3.txt"
valgrind --quiet --tool=massif --massif-out-file=tester_output/massif/massif.out.test3 ./tester_output/valgrind_main < open_testcases/test3.txt > /dev/null