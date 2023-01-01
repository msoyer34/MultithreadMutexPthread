In this project mutexes have been used for all threads which will eliminates the race condition risks during initialization of the same processes. This way the implementation is more better. 

Interface classes have been created for running situtations. For each thread, there exists an execute function which is a command base pattern and every thread executes their own execute function. For each thread there are mutexes; which locks during process and unlocks afterwards.

Input file has been splitted, and main function waits till every single customer has made their payments, then every vending machine execution stops.

Output is stored in every vending machine and has been written afterwards.

There are multiple static data sources to be used in various threads.
