
<linux/workqueue.h>

/定义workqueue和底半部函数/
struct work_struct xxx_wq;
void xxx_do_work(struct work_struct *work);

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
    schedule_work(&xxx_wq);
    //......
    return IRQ_HANDLED;
}

/* 设备驱动模块加载函数 */
int __init xxx_init(void)
{
    //......
    /*申请中断*/
    result = request_irq(xxx_irq, xxx_interrupt, 0, "xxx", NULL);
    //......
    /*初始化工作队列*/
    INIT_WORK(&xxx_wq, xxx_do_work);
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



