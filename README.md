# stress_tools

g++ -pthread stresscache.cpp -o stresscache
./stresscache 2

g++ -pthread stresscpu.cpp -o stresscpu
./stresscpu 2

g++ -pthread stresscpuratio.cpp -o stresscpuratio 
./stresscpu 2 30
