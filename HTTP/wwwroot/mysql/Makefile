.PHONY:all
all:insert select
insert:insert.c
	gcc $^ -o $@ -L /usr/lib64/mysql -lmysqlclient
select:select.c
	gcc $^ -o $@ -L /usr/lib64/mysql -lmysqlclient

.PHONY:clean
clean:
	rm select insert
