all: clean-coverage
	@make -C build

setup:
	@rm -rf build
	@cmake -B build

setup-tests:
	@rm -rf build
	@cmake -B build -DENABLE_COVERAGE=ON

run:
	@./build/src/game-engine

clean-coverage:
	@find build -name '*.gcda' -delete

test: clean-coverage
	@ctest --test-dir build --output-on-failure

coverage: test
	@lcov --capture --directory build \
		--output-file build/coverage.info \
		--ignore-errors inconsistent,gcov,unsupported,mismatch,unused \
		--exclude '*/_deps/*' \
		--exclude '*/vendor/*' \
		--exclude '*/tests/*' \
		--exclude '/usr/*' \
		--exclude '/Applications/*' \
		--exclude '*/include/*'
	@genhtml build/coverage.info \
		--output-directory build/coverage_report \
		--ignore-errors inconsistent,source,category

show-coverage: coverage
	@open build/coverage_report/index.html

convert:
	@echo ffmpeg -i ${file}.mp3 -acodec pcm_s16le -ar 44100 ${file}.wav

compile-wasm:
	@asc assets/scripts/wasm/${file}.ts -o assets/scripts/wasm/${file}.wasm --exportRuntime --optimize

objdump-wasm:
	@wasm-objdump -x assets/scripts/wasm/${file}.wasm | grep -A 20 "Import"

debug:
	@lldb build/src/game-engine