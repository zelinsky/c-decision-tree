CIVL=java -jar -Xmx3g /home/zelinsky/Documents/Summer2018/CIVL-1.16_4871/lib/civl-1.16_4871.jar
VERIFY=$(CIVL) verify

all: verify_tree verify_entropy clean

verify_tree: tree-driver.cvl tree.cvl input.c
	$(VERIFY) tree-driver.cvl tree.cvl input.c

verify_entropy: entropy-driver.cvl entropy.cvl input.c
	$(VERIFY) entropy-driver.cvl entropy.cvl input.c

clean:
	rm -rf CIVLREP *~
