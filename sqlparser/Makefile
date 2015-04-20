

test: FORCE
	@echo "Compiling..."
	@make clean -C src/ >/dev/null || exit 1
	#@make tests -C src/ >/dev/null || exit 1
	@make my_test -C src/ >/dev/null || exit 1
	#@make grammar_test -C src/ >/dev/null || exit 1
	#@echo "Running tests:"
	#@./bin/grammar_test -f "src/tests/valid_queries.sql"
	#@./bin/tests
	@./bin/my_test

my_test: FORCE
	@make my_test -C src/ >/dev/null || exit 1
	@./bin/my_test

docs: FORCE
	doxygen docs/doxy.conf


FORCE:
