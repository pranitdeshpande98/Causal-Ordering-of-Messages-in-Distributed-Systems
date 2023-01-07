CFLAGS=-o
compileAll: p2_noncausal p2_causal

p2_noncausal:p2_noncausal.cpp
	g++ -o p2_noncausal p2_noncausal.cpp -lpthread
	

p2_causal:p2_causal.cpp
	g++ -o p2_causal p2_causal.cpp -lpthread

p2_noncausalRun:
	./p2_noncausal 0 Message

p2_causalRun:
	./p2_causal 0 Message
		
clean:
	rm -rf *.o p2_noncausal p2_causal
