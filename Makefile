SUBDIRS = commons front-end back-end middle-end

all: build

build:
	mkdir -p logs
	mkdir -p logs/dot_files
	mkdir -p logs/images
	@for dir in $(SUBDIRS); do \
		(cd $$dir && make) || exit 1; \
	done

clean:
	rm -rf logs
	@for dir in $(SUBDIRS); do \
		(cd $$dir && make clean) || exit 1; \
	done
