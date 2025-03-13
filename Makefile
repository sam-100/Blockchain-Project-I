
simulation: log
	g++ src/* -I include -o simulation

log:
	mkdir log log/events log/blockchain log/mempool

clean:
	rm log/* log/events/* log/blockchain/* log/mempool/* simulation simulation-gdb simulation-debug

gdb: log
	g++ src/* -I include -o simulation-gdb -g

debug: log
	g++ src/* -I include -o simulation-debug -D=DEBUG

clean-log:
	rm log/* log/events/* log/blockchain/* log/mempool/*

