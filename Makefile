.build:
	g++ eten-cpu/eten.cpp -c -o ../../build/eten-cpu.o -fpermissive
	g++ eten-cpu/logic.cpp -c -o ../../build/logic-cpu.o
	g++ eten-cpu/base64.cpp -c -o ../../build/base64-cpu.o
	g++ eten-cpu/railfence.cpp -c -o ../../build/railfence-cpu.o 
	g++ eten-cpu/utils.cpp -c -o ../../build/utils-cpu.o -fpermissive
	g++ eten-cpu/sha.cpp -c -o ../../build/sha-cpu.o 
	g++ eten-cpu/md.cpp -c -o ../../build/md-cpu.o 
	g++ eten-cpu/RC4.cpp -c -o ../../build/RC4-cpu.o
	g++ eten-cpu/caesar.cpp -c -o ../../build/caesar-cpu.o
	g++ eten-cpu/test.cpp ../../build/base64-cpu.o ../../build/caesar-cpu.o  -o ../test-cpu
	
	g++ eten-opencl/eten.cpp -c -o ../../build/eten-opencl.o
	g++ eten-opencl/base64.cpp -c -o ../../build/base-opencl.o
	g++ eten-opencl/caesar.cpp -c -o ../../build/caesar-opencl.o
	g++ eten-cpu/test.cpp ../../build/base64-opencl.o ../../build/caesar-opencl.o -o -lOpenCL ../test-opencl 