CC = g++
CFLAGS = -g -I include -Wcast-qual -Wctor-dtor-privacy					 \
		 -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op \
         -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual     \
		 -Wpointer-arith -Wsign-promo -Wstrict-aliasing           \
		 -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -fexceptions -pipe     \

LDFLAGS =

SOURCES_DIR   	= src
OBJECTS_DIR   	= bin

SOURCE_FILES    = $(wildcard $(SOURCES_DIR)/*.cpp)
OBJECT_FILES    = $(subst $(SOURCES_DIR), $(OBJECTS_DIR), $(SOURCE_FILES:.cpp=.o))

all: $(OBJECT_FILES)

$(OBJECTS_DIR):
	mkdir -p $(OBJECTS_DIR)

$(EXECUTABLE_PATH): $(OBJECT_FILES) $(BUILD_DIR)
	$(CC) $(OBJECT_FILES) -o $@ $(LDFLAGS)

$(OBJECTS_DIR)/%.o: $(SOURCES_DIR)/%.cpp $(OBJECTS_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -fr $(OBJECTS_DIR)
