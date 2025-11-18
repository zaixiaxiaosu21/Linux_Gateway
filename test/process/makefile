fork: 01_fork.c
	-$(CC) $^ -o $@
	-./$@
	-rm $@
other: 02_other.c
	-gcc $^ -o $@

execve: 02_execve.c other
	-gcc $< -o $@
	-./$@
	-rm $@ other

signal: 04_signal.c
	-gcc $^ -o $@
	-./$@
	-rm $@
wait_pid: 03_wait_pid.c
	-gcc $^ -o $@
	-./$@
	-rm $@