server:
	g++ socket_tcp.cpp -o a.out -lrt
	./a.out
	rm *.out
client1:
	g++ client.cpp -o b.out
	./b.out "127.0.0.1"
client2:
	g++ client.cpp -o c.out
	./c.out "127.0.0.1"
worker:
	g++ worker.cpp -o worker.out
	./worker.out "127.0.0.1" worker
worker1:
	g++ worker.cpp -o worker1.out
	./worker1.out "127.0.0.1" worker
delete:
	rm *.out
