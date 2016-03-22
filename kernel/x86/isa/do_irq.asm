; Code to call a specified irq number
;

%macro do_irq 1
	int (%1+32)
	ret
%endmacro

global do_irq_0
global do_irq_1
global do_irq_2
global do_irq_3
global do_irq_4
global do_irq_5
global do_irq_6
global do_irq_7
global do_irq_8
global do_irq_9
global do_irq_10
global do_irq_11
global do_irq_12
global do_irq_13
global do_irq_14
global do_irq_15

do_irq_0: do_irq 0
do_irq_1: do_irq 1
do_irq_2: do_irq 2
do_irq_3: do_irq 3
do_irq_4: do_irq 4
do_irq_5: do_irq 5
do_irq_7: do_irq 6
do_irq_8: do_irq 8
do_irq_9: do_irq 9
do_irq_10: do_irq 10
do_irq_11: do_irq 11
do_irq_12: do_irq 12
do_irq_13: do_irq 13
do_irq_14: do_irq 14
do_irq_15: do_irq 15
