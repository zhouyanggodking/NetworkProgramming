targets=socket_client socket_server
all: $(targets)
$(targets): % :%.cpp
	g++ $^ -o $@
.PHONY: clean
clean:
	rm -rf $(targets)
