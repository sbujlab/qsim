OBJECT += plot
OBJECT += z
OBJECT += layer
OBJECT += convert_layer

.PHONY: all
all: $(OBJECT)

$(OBJECT): % : %.cc
	g++ -o $@ $^ -std=c++0x `root-config --cflags --libs`
