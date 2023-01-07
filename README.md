# Causal-Ordering-of-Messages-in-Distributed-Systems
Implemented n-node distributed system that provides a causally ordered multicasting service.

A makefile has been created that has the following commands:
1. compileAll: make compileAll
  a. make compile writing “make compileAll” in terminal will create the executable objects for Causal and Non-causal Multicast Ordering Programs Respectively.
2. P2_NoncausalRun: make p2_noncausalRun
  a. Writing “make p2_noncausalRun” in the terminal will run the non-causal multicast ordering program.
  b. Note: The order of the commands is as follows : PROGRAM ORDER MESSAGE. One can change the message and the order in the makefile every time before executing the code.
  c. For e.g., if order = 0, then the message will be represented as 1,0,0,0,Message.
  d. For e.g., if order = 1, then the message will be represented as 0,1,0,0,Message.
  e. Note : Please change the message and order in the makefile every time before executing any different process to see different vector clocks.
3. P2_CausalRun: make p2_causalRun
  a. Writing “make p2_causalRun” in the terminal will run the non-causal multicast ordering program.
  b. Note: The order of the commands is as follows : PROGRAM ORDER MESSAGE. One can change the message and the order in the makefile every time before executing the code.
  c. For e.g., if order = 0, then the message will be represented as 1,0,0,0,Message.
  d. For e.g., if order = 1, then the message will be represented as 0,1,0,0,Message.
  e. Note : Please change the message and order in the makefile every time before executing any different process to see different vector clocks.
