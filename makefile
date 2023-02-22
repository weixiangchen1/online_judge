.PHONY:all
all:
	@cd compile_server;\
	make;\
	cd -;\
	cd oj_server;\
	make;\
	cd -;

.PHONY:clean
clean:
	@cd compile_server;\
	rm -rf compile_server;\
	cd -;\
	cd oj_server;\
	rm -rf oj_server;\
	cd -;\
	rm -rf output;

.PHONY:output
output:
	@mkdir -p output/compile_server;\
	mkdir -p output/oj_server;\
	cp -rf compile_server/compile_server output/compile_server;\
	cp -rf compile_server/temp output/compile_server;\
	cp -rf oj_server/conf output/oj_server;\
	cp -rf oj_server/ctemplate_html output/oj_server;\
	cp -rf oj_server/include output/oj_server;\
	cp -rf oj_server/lib output/oj_server;\
	cp -rf oj_server/questions output/oj_server;\
	cp -rf oj_server/wwwroot output/oj_server;\
	cp -rf oj_server/oj_server output/oj_server;
