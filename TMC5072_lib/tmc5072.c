/*
 * FILENAME:	TMC5072.c
 * REV:				1.0
 *sd
 * Autor:sdd	Sylwester -	28.05.2018
* (C) Copyright 2018 InMotion Labs
 */

#include "tmc5072.h"

static int tmc5072_cb_init(int cnt, uint8_t *arg[]);
static int tmc5072_cb_set(int cnt, uint8_t *arg[]);
static int tmc5072_cb_config(int cnt, uint8_t *arg[]);
static int tmc5072_cb_get(int cnt, uint8_t *arg[]);
static int tmc5072_cb_help(int cnt, uint8_t *arg[]);

static tmc5072_set_reg_struct tmc5072_set_dict[] = {
	{"v1", 		TMC5072_REG_V1_1},
	{"v2", 		TMC5072_REG_V1_2},
	{"vmax1",	TMC5072_REG_VMAX_1},
	{"vmax2",	TMC5072_REG_VMAX_2},
	{"a1",		TMC5072_REG_A1_1},asd
	{"a2",		TMC5072_REG_A1_2},
	{"amax1",	TMC5072_REG_AMAX_1},
	{"amax2",	TMC5072_REG_AMAX_2},
	{"d1",		TMC5072_REG_D1_1},
	{"d2",		TMC5072_REG_D1_2},
	{"dmax1",	TMC5072_REG_DMAX_1},
	{"dmax2",	TMC5072_REG_DMAX_2},
	{"pos1",	TMC5072_REG_XTARGET_1},
	{"pos2",	TMC5072_REG_XTARGET_2},
	{"mode1",	TMC5072_REG_RAMPMODE_1},
	{"mode2",	TMC5072_REG_RAMPMODE_2}
};eew

static tmc5072_comm_struct tmc5072_dictionary[] = {
	{"init",		tmc5072_cb_init,	0},
	{"set",			tmc5072_cb_set,		2},
	{"config",	tmc5072_cb_config,2},
	{"get",			tmc5072_cb_get,		1},
	{" ",				tmc5072_cb_help,	0},
	{"?",				tmc5072_cb_help,	0},
	{"help",		tmc5072_cb_help,	0}
};

static int tmc5072_init(void) {
	/*
	@TODO implement GPIO & SPI initialization for tmc5072
	*/
}

static int tmc5072_deinit(void){
	/*
	@TODO implement GPIO & SPI deinitialization for tmc5072
	*/
}

static uint32_t tmc5072_send(uint8_t write_read, uint8_t address, int32_t data) {
	uint32_t response;
	uint8_t tmp[] = {(address | write_read), ((uint8_t) (data >> 24)), ((uint8_t) (data >> 16)), ((uint8_t) (data >> 8)), ((uint8_t) data)};

	printf("SPI Sending:\t %02x %08x\n", address, data);

	/*
	@TODO implement SPI  TX 5 bytes from tmp[] & RX 5 bytes to response
	*/

	return response;
}

static int tmc5072_motor1_config(void) {
	printf("CHOPCONF 1 = %08x\n", TMC5072_CONFIG_CHOPCONF_1);
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_CHOPCONF_1, TMC5072_CONFIG_CHOPCONF_1); // CHOPCONF: TOFF=5, HSTRT=4, HEND=1, TBL=2, CHM=0 (spreadCycle)

	printf("IHOLD_IRUN 1 = %08x\n", TMC5072_CONFIG_IHOLD_IRUN_1);
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_IHOLD_IRUN_1, TMC5072_CONFIG_IHOLD_IRUN_1); // IHOLD_IRUN: IHOLD=5, IRUN=31 (max. current), IHOLDDELAY=1

	printf("PWMCONF 1 = %08x\n", TMC5072_CONFIG_PWMCONF_1);
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_PWMCONF_1, 		TMC5072_CONFIG_PWMCONF_1); // PWM_CONF: AUTO=1, 2/1024 Fclk, Switch amplitude limit=200, Grad=1

	printf("VHIGH 1 = %08x\n", TMC5072_CONFIG_VHIGH_1);
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_VHIGH_1, 			TMC5072_CONFIG_VHIGH_1); // VHIGH=400 000: Set VHIGH to a high value to allow stealthChop

	printf("VCOOLTHRS 1 = %08x\n", TMC5072_CONFIG_VCOOLTHRS_1);
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_VCOOLTHRS_1, 	TMC5072_CONFIG_VCOOLTHRS_1); // VCOOLTHRS=30000: Set upper limit for stealthChop to about 30RPM

	printf("TZEROWAIT 1 = %08x\n", TMC5072_CONFIG_TZEROWAIT_1);
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_TZEROWAIT_1, 	TMC5072_CONFIG_TZEROWAIT_1);

	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_A1_1, 	TMC5072_CONFIG_A_1);
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_V1_1, 	TMC5072_CONFIG_V_1);
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_D1_1, 	TMC5072_CONFIG_D_1);
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_AMAX_1, TMC5072_CONFIG_MAX_A_1);
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_VMAX_1, TMC5072_CONFIG_MAX_V_1);
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_DMAX_1, TMC5072_CONFIG_MAX_D_1);
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_VSTOP_1,TMC5072_CONFIG_VSTOP_1);
}

static int tmc5072_motor2_config(void) {
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_CHOPCONF_2, 	0x00010044); // CHOPCONF: TOFF=5, HSTRT=4, HEND=1, TBL=2, CHM=0 (spreadCycle)
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_IHOLD_IRUN_2, 0x00011F05); // IHOLD_IRUN: IHOLD=5, IRUN=31 (max. current), IHOLDDELAY=1
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_PWMCONF_2, 		0x000501C8); // PWM_CONF: AUTO=1, 2/1024 Fclk, Switch amplitude limit=200, Grad=1
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_VHIGH_2, 			0x00061A80); // VHIGH=400 000: Set VHIGH to a high value to allow stealthChop
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_VCOOLTHRS_2, 	200000); // VCOOLTHRS=30000: Set upper limit for stealthChop to about 30RPM
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_TZEROWAIT_2, 	0x0000000F);


	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_A1_2,	TMC5072_CONFIG_A_2);
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_V1_2, 	TMC5072_CONFIG_V_2);
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_D1_2, 	TMC5072_CONFIG_D_2);
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_AMAX_2, TMC5072_CONFIG_MAX_A_2);
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_VMAX_2, TMC5072_CONFIG_MAX_V_2);
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_DMAX_2, TMC5072_CONFIG_MAX_D_2);
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_VSTOP_2,TMC5072_CONFIG_VSTOP_2);
}

static int tmc5072_cb_init(int cnt, uint8_t *arg[]) {
	printf("GCONF = %08x\n", TMC5072_CONFIG_GCONF);
	tmc5072_send(TMC5072_REG_WRITE, TMC5072_REG_GCONF, 0x00000008); // GCONF=8: Enable PP and INT outputs

  	tmc5072_motor1_config();
	tmc5072_motor2_config();
}

static int tmc5072_cb_set(int cnt, uint8_t *arg[]){
	if(arg[1] == NULL){
		printf("ERROR: set value is missing\n");
		return 1;
	}
	uint32_t val = atoi(arg[1]);
	uint32_t i;

	for(i = 0; i < 16;i++){
		if(!(strcmp((char*)arg[0], tmc5072_set_dict[i].argc))){
			tmc5072_send(TMC5072_REG_WRITE, tmc5072_set_dict[i].reg_addr, val);

			return 0;
		}
	}
	printf("ERROR: command not found\n");
	return 1;
}

static int tmc5072_cb_config(int cnt, uint8_t *arg[]){
	if((arg[0] == NULL) || (arg[1] == NULL)){
		printf("ERROR: argument is missing\n");
		return 1;
	}
	uint8_t addr = atoi(arg[0]);
	uint32_t val = atoi(arg[1]);
	tmc5072_send(TMC5072_REG_WRITE, addr, val);
}

static int tmc5072_cb_get(int cnt, uint8_t *arg[]){
	if(arg[0] == NULL){
		printf("ERROR: argument is missing\n");
		return 1;
	}
	uint8_t 	addr = atoi(arg[0]);
	uint32_t	res;

	tmc5072_send(TMC5072_REG_READ, addr, 0);
	res = tmc5072_send(TMC5072_REG_READ, addr, 0);
	printf("Get value: %d\n", res);
	return res;
}

static int tmc5072_cb_help(int cnt, uint8_t *arg[]) {
	uint32_t tab_len = (sizeof(tmc5072_dictionary)/sizeof(tmc5072_comm_struct));
  	uint32_t i;
  	printf("Avaible commands:\n");
  	for(i = 0; i < (tab_len - 3); i++){
  		printf("%s\n", tmc5072_dictionary[i].command);
  	}
}

int tmc5072_command_parse(int arg_num, uint8_t *argc[]){
	uint32_t tab_len = (sizeof(tmc5072_dictionary)/sizeof(tmc5072_comm_struct));
	int status = 0;
	uint32_t i, arg_ptr = 1;

	printf("arg: %s\n", argc[arg_ptr]);
	status |= tmc5072_init();

	while(arg_ptr != arg_num){
		for(i = 0; i < tab_len; i++){
	    		if(!(strcmp((char*)argc[arg_ptr], tmc5072_dictionary[i].command))){
				printf("callback funct: %s\n", tmc5072_dictionary[i].command);
				status |= tmc5072_dictionary[i].cb(arg_num, (argc + arg_ptr + 1));
				arg_ptr += (tmc5072_dictionary[i].arg_num+1);
				break;
			}
		}
	//if(i == tab_len){
	//	printf("ERROR: invalid argument\n");
	//	return 1;
	//	}
	}

	status |= tmc5072_deinit();
	return status;
}
