if [ "$1" == "pmpid" ]; then
	gcc pmpi_d.c \
		pmpi_recv_source.c \
		pmpi_recv_msg_direct.c \
		pmpi_compile.c \
		pmpi_run.c \
		-o pmpid
elif [ "$1" == "pmpiexec" ]; then
	gcc pmpi_exec.c \
		pmpi_distribute_source.c \
		pmpi_send_source.c \
		pmpi_run.c \
		-o pmpiexec
elif [ "$1" == "all" ]; then
	gcc pmpi_d.c \
		pmpi_recv_source.c \
		pmpi_recv_msg_direct.c \
		pmpi_compile.c \
		pmpi_run.c \
		-o pmpid

	gcc pmpi_exec.c \
		pmpi_distribute_source.c \
		pmpi_send_source.c \
		pmpi_run.c \
		-o pmpiexec
else 
	echo "ERROR"
fi
