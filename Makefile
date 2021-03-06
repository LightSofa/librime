RIME_ROOT = $(CURDIR)
THIRDPARTY_PATHS = -DCMAKE_INCLUDE_PATH=$(RIME_ROOT)/thirdparty/include -DCMAKE_LIBRARY_PATH=$(RIME_ROOT)/thirdparty/lib

sharedir = $(DESTDIR)/usr/share
bindir = $(DESTDIR)/usr/bin

.PHONY: all install uninstall thirdparty clean librime-static librime debug

all: librime
	@echo ':)'

install: install-librime
	@echo ':)'

uninstall: uninstall-librime
	@echo ':)'

thirdparty:
	make -f Makefile.thirdparty

clean:
	rm -Rf build build-static debug-build

librime-static:
	mkdir -p build-static
	(cd build-static; cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DBUILD_STATIC=ON -DBUILD_SHARED_LIBS=OFF $(THIRDPARTY_PATHS) ..)
	make -C build-static

librime:
	mkdir -p build
	(cd build; cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release ..)
	make -C build

install-librime:
	make -C build install

uninstall-librime:
	make -C build uninstall

debug:
	mkdir -p debug-build
	(cd debug-build; cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug ..)
	make -C debug-build

install-debug:
	make -C debug-build install

uninstall-debug:
	make -C debug-build uninstall
