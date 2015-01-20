targets=socket_client socket_server socket_option socket_signal
all: $(targets)
$(targets): % :%.cpp common.hpp
	g++ $^ -o $@
.PHONY: clean
clean:
	rm -rf $(targets)
