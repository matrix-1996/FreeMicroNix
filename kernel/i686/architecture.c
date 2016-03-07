#include <stdint.h>
#include <x86.h>

setParam(uint32_t ret, uint32_t ret1, uint32_t ret2, uint32_t ret3,uint32_t ret4)
{
	ret_reg[0]=ret;
	ret_reg[1]=ret1;
	ret_reg[2]=ret2;
	ret_reg[3]=ret3;
	ret_reg[4]=ret4;
}
