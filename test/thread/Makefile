create:01_create.c
	-gcc $^ -o $@ 
	-./$@
	-rm $@
sum_sqr:02_sum_sqr.c
	-gcc -o $@ $^
	-./$@
	-rm $@

sum_sqr_thread:02_sum_sqr_thread.c
	-gcc -o $@ $^
	-./$@
	-rm $@

cancel: 03_cancel.c
	-$(CC) $^ -o $@
	-./$@
	-rm $@

race_condition: 04_race_condition.c
	-$(CC) $^ -o $@
	-./$@
	-rm $@

mutex_lock: 04_mutex_lock.c
	-$(CC) $^ -o $@
	-./$@
	-rm $@

mqueue: 05_mqueue.c
	-$(CC) $^ -o $@
	-./$@
	-rm $@