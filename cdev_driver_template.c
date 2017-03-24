/*
 *  设备结构体
 */
struct xxx_dev_t {
    struct cdev cdev;
    //......
}xxx_dev;


struct file_opertions xxx_fops = {
    .owner = THIS_MODULE,
    .read  = xxx_read,
    .write = xxx_write,
    .ioctl = xxx_ioctl,
    //......
};

/*读设备 */
ssize_t xxx_read (struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    //......
    copy_to_user(buf, src, len);
    //......
}

/* 写设备 */
ssize_t xxx_write (struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    //......
    copy_from_user(dest, buf, len);
    //......

}


/* icotl函数 */
long xxx_ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
    //......
    switch (cmd) {
    case XXX_CMD0:
        //......

        break;
    case XXX_CMD1:
        //......

        break;
    case XXX_CMD2:
        //......

        break;

    default:
        /* 不支持的命令 */
        return -ENOTTY;
    }
    //......
    return 0;
}


/* 设备驱动模块加载函数 */
static int __init xxx_init (void)
{
    //......
    cdev_init(&xxx_dev.cdev, &xxx_fops);
    /* alloc cdev major */
    if (xxx_major) {
        register_chrdev_region(xxx_dev_no, 1, DEV_NAME);
    } else {
        alloc_chrdev_region(&xxx_dev_no, 0, 1, DEV_NAME);
    }
    
    /* register cdev  */
    ret = cdev_add(&xxx_dev.cdev, xxxdev_no, 1);
    //......
}

/* 设备驱动卸载函数 */
static void __exit xxx_exit (void)
{
    unregister_chrdev_region(xxx_dev_no, 1);
    cdev_del(&xxx_dev.cdev);
    //......
}
