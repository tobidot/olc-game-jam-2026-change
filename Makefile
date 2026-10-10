NAME ?= tce-change
KIND ?= element
VERSION := $(shell cat VERSION)
UPLOAD_URL ?= http://localhost/api/v1/tobidot-elements

-include upload.env

.PHONY: web tce clean-tce upload

linux: 
	cmake --build build

web:
	@if [ ! -f build-web/CMakeCache.txt ]; then \
		emcmake cmake -S . -B build-web; \
	fi
	cmake --build build-web

tce: web
	rm -rf build-tce
	mkdir -p build-tce/dist
	cp build-web/index.html build-web/index.js build-web/index.wasm build-web/index.data build-tce/dist/
	mkdir -p releases
	cd build-tce && zip -r "../releases/$(NAME)-v$(VERSION).zip" dist

clean-tce:
	rm -rf build-tce
	rm -f "releases/$(NAME)-v$(VERSION).zip"

upload: tce
	@if [ -z "$(UPLOAD_TOKEN)" ]; then \
		echo "UPLOAD_TOKEN is not set. Put it in a local, gitignored 'upload.env' file (UPLOAD_TOKEN=...) or pass UPLOAD_TOKEN=... on the command line."; \
		exit 1; \
	fi
	@http_code=$$(curl -sS -o /tmp/tce-upload-response.json -w "%{http_code}" -X POST "$(UPLOAD_URL)" \
		-H "Authorization: Bearer $(UPLOAD_TOKEN)" \
		-H "Accept: application/json" \
		-F "name=$(NAME)" \
		-F "version=$(VERSION)" \
		-F "kind=$(KIND)" \
		-F "zip=@releases/$(NAME)-v$(VERSION).zip;type=application/zip"); \
	echo; cat /tmp/tce-upload-response.json; echo; \
	if [ "$$http_code" -lt 200 ] || [ "$$http_code" -ge 300 ]; then \
		echo "Upload to $(UPLOAD_URL) failed with HTTP $$http_code"; \
		exit 1; \
	fi; \
	echo "Upload to $(UPLOAD_URL) succeeded with HTTP $$http_code"
