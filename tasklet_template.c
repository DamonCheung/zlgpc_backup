#include <linux/interrupt.h>

/定义tasklet和底半部函数，并将它们关联/
void xxx_do_tasklet(unsigned long);
DECLARE_TASKLET(xxx_tasklet, xxx_do_tasklet, 0);

/*中断底半部分*/
void xxx_do_tasklet(unsigned long)
{
    //......
    
    //......
}

/*中断处理顶半部分*/
irqreturn_t xxx_interrupt(int irq, void *dev_id)
{
    //......
    tasklet_schedule(&xxx_tasklet);
    //......
}

/* 设备驱动模块加载函数 */
int __init xxx_init(void)
{
    //......
    /*申请中断*/
    result = request_irq(xxx_irq, xxx_interrupt, 0, "xxx", NULL);
    //......
    return IRQ_HANDLED;
}

/* 设备驱动模块写在函数 */
{
    //......
    /*释放中断*/
    free_irq(xxx_irq, xxx_interrupt;);
    //......
}



