echo "preliminary work"
mkdir -p tester_output/diffs tester_output/executed open_testcases tester_output/memcheck tester_output/callgrind tester_output/massif

echo "compiling for debug"
gcc $1 -Wall -Werror -O2 -g3 -fsanitize=address -o tester_output/debug_main

echo "compiling for valgrind"
gcc $1 -Wall -Werror -O2 -g3 -o tester_output/valgrind_main

cd open_testcases
for file in ./*
do
    echo "  running ${file}"
    echo "      diff"
    ../tester_output/debug_main < ${file} > ../tester_output/executed/${file}
    diff ../open_results/${file} ../tester_output/executed/${file} > ../tester_output/diffs/${file}
    echo "      memcheck"
    valgrind --tool=memcheck --log-file=../tester_output/memcheck/${file} ../tester_output/valgrind_main < ${file} > /dev/null
    echo "      callgrind"
    valgrind --quiet --tool=callgrind --callgrind-out-file=../tester_output/callgrind/${file} ../tester_output/valgrind_main < ${file} > /dev/null
    echo "      massif"
    valgrind --quiet --tool=massif --massif-out-file=../tester_output/massif/${file} ../tester_output/valgrind_main < ${file} > /dev/null
done
