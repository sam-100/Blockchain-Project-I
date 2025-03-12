
simulation: 
	g++ src/* -I include -o simulation

clean:
	rm log/* log/events/* log/blockchain/* log/mempool/* simulation

