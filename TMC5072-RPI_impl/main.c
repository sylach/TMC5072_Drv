#include "tmc5072.h"

int main(int argc, char *argv[]){

	uint32_t stat =	tmc5072_command_parse(argc, (uint8_t **) argv);

	if(stat){
		printf("exit_err\n");
	}else{
		printf("exit\n");
	}
	return 0;
}

