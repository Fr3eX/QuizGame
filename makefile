.PHONY: clean

.SUFFIXES:

SERVER:Server
CLIENT:Client



all: $(SERVER) $(CLIENT)

$(SERVER):
	make -C $(SERVER) all

$(CLIENT):
	make -C $(CLIENT) all


clean:
	make -C $(SERVER) clean
	make -C $(CLIENT) clean


