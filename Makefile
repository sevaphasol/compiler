SUBDIRS = commons front-end back-end

all: build

build:
	@for dir in $(SUBDIRS); do \
		(cd $$dir && make) || exit 1; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		(cd $$dir && make clean) || exit 1; \
	done
