ls cpp-src/test | grep -v "test-util.cpp" | sed 's/^/#include "/' | sed 's/$/"/' > test-main.cpp

read -d CPP_MAIN_SRC << EOF
int main(int argc, char** argv){
    TestCase::ExecuteTests(argc > 1);
    return 0;
}
EOF

echo $CPP_MAIN_SRC > test-main.cpp

cd cpp-src/test
make
cd ../..

gcc test-main.cpp cpp-src/test/test.lib -o test-main
./test-main $@
