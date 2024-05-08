MAIN := bin/xxd
SRC_C := main.c

$(MAIN): $(SRC_C)
	gcc -Wall -Werror $^ -o $@

clean:
	rm -f $(MAIN)
	rm -rf __pycache__ .pytest_cache
	rm tests/*.out tests/*.exp

tests: $(MAIN)
	pytest tests.py
