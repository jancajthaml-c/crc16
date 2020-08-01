OUTDIR=dist
MODULE=crc

.PHONY: default clean

default: test clean

$(OUTDIR):
	@mkdir -p $(OUTDIR)

test: $(OUTDIR) $(OUTDIR)/$(MODULE).o $(OUTDIR)/$(MODULE)_o2.o $(OUTDIR)/$(MODULE)_o3.o $(OUTDIR)/main.o
	gcc $(OUTDIR)/$(MODULE).o $(OUTDIR)/main.o -o test_human
	gcc -o2 $(OUTDIR)/$(MODULE)_o2.o $(OUTDIR)/main.o -o test_o2
	gcc -o3 $(OUTDIR)/$(MODULE)_o3.o $(OUTDIR)/main.o -o test_o3
	@echo "\ntest [human optimised]"
	@./test_human || echo "failed"
	@echo "\ntest [automatic optimalisation level 2]"
	@./test_o2 || echo "failed"
	@echo "\ntest [automatic optimalisation level 3]"
	@./test_o3 || echo "failed"

$(OUTDIR)/$(MODULE).o: $(MODULE).c
	gcc -o $(OUTDIR)/$(MODULE).o -c $^

$(OUTDIR)/$(MODULE)_o2.o: $(MODULE).c
	gcc -o2 -o $(OUTDIR)/$(MODULE)_o2.o -c $^

$(OUTDIR)/$(MODULE)_o3.o: $(MODULE).c
	gcc -o3 -o $(OUTDIR)/$(MODULE)_o3.o -c $^

$(OUTDIR)/main.o: main.c
	gcc -o $(OUTDIR)/main.o -c $^

clean:
	@rm -f test_human
	@rm -f test_o2
	@rm -f test_o3
	@rm -rf $(OUTDIR)
