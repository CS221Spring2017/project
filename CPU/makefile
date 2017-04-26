cpu1 : 
	gcc -Iinclude -o measure1.out measure1.c;
	mv measure1.out build;
	gcc -Iinclude -o measure2.out measure2.c;
	mv measure2.out build;

	gcc -Iinclude -o procedure.o procedure.c;
	mv procedure.o build;

	gcc -Iinclude -o system.o system.c;
	mv system.o build;

	gcc -Iinclude -o create_process.o create_process.c;
	mv create_process.o build;
	gcc -pthread -Iinclude -o create_thread.o create_thread.c;
	mv create_thread.o build;

	gcc -Iinclude -o context_switch_p.o context_switch_p.c;
	mv context_switch_p.o build;
	gcc -pthread -Iinclude -o context_switch_t.o context_switch_t.c;
	mv context_switch_t.o build;
