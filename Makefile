exe = cppmicro

src = src/main.cpp

cxx = g++

cflags = -std=c++17
lflags = -lPocoFoundation -lPocoUtil -lPocoJSON -lPocoNet

all: $(exe)

run: $(exe)
	./$(exe)

clean:
	rm -f $(exe) $(exe).d

$(exe): $(src)
	$(cxx) $< -MD -o $@ $(cflags) $(lflags)

-include $(exe).d
