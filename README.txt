prof2jason.c is developed for heat equation workflow. Following are the steps to 


1) compile the heat equation with TAU wrapper.
2) run the script ./run_workflow.sh 20. This will generate TAU profile.* files in different directories specified by the user in the swift script.
3) run with "time ./rin_workflow.sh 20 >> out_1.txt" to get the workflow execution time  
3) use "pprof -a >> out_2.txt" to generate the summary of the profile information for each component.
4) compile prof2jason.c "gcc prof2jason.c -o run"
5) produce the performance information in JSON format:
	./run <out.txt> <out1.txt> <out2.txt> <jasonfile>

	where
		out.txt  = file containing workflow execution time
		out1.txt = file containing summary of profile for component 1 ( pprof -a >> out1.txt)
		out2.txt = file containing summary of profile information for component 2 ( pprof -a >> out2.txt)
		jasonfile= file containing information in jasonformat.




