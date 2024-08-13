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
.build_dynacamic:
	g++ eten-cpu/eten.cpp -c -fPIC -o eten-cpu.o -fpermissive
	g++ eten-cpu/logic.cpp -c -fPIC -o logic-cpu.o
	g++ eten-cpu/base64.cpp -c -fPIC -o base64-cpu.o
	g++ eten-cpu/railfence.cpp -c -fPIC -o railfence-cpu.o 
	g++ eten-cpu/utils.cpp -c -fPIC -o utils-cpu.o -fpermissive
	g++ eten-cpu/sha.cpp -c -fPIC -o sha-cpu.o 
	g++ eten-cpu/md.cpp -c -fPIC -o md-cpu.o 
	g++ eten-cpu/RC4.cpp -c -fPIC -o RC4-cpu.o
	g++ eten-cpu/caesar.cpp -fPIC -c -o caesar-cpu.o
	g++ -shared -o libetenalgorithm.so eten-cpu.o logic-cpu.o base64-cpu.o railfence-cpu.o utils-cpu.o sha-cpu.o md-cpu.o RC4-cpu.o caesar-cpu.o