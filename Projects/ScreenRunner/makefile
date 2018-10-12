CC = g++
CFLAGS = -std=c++17 -Wall
LDLIBS = -lGL -lSDL2_image
LDFLAGS = $(shell sdl2-config --cflags --libs)

SRC_DIR:= ../../SharedCode Source
BUILD_DIR:= $(addprefix build/,$(SRC_DIR))

SRC:= $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ:= $(patsubst %.cpp,build/%.o,$(SRC))
INCS:= $(addprefix -I,$(SRC_DIR))

vpath %.cpp $(SRC_DIR)

define make-target
$1/%.o: %.cpp
	$(CC) $(INCS) -c $$< -o $$@ $(CFLAGS) $(LDLIBS) $(LDFLAGS)
endef

.PHONY: all checkdirs clean

all: checkdirs ScreenRunner

ScreenRunner: $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS) $(LDLIBS) $(LDFLAGS)
	echo $(OBJ)

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR)

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-target,$(bdir))))
