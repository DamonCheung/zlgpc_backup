#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/timer.h>
#include <asm/atomic.h>
#include <linux/slab.h>
#include <linux/device.h>

#define CDEVDEMO_MAJOR 255  /*Ԥ��cdevdemo�����豸��*/

static int cdevdemo_major = CDEVDEMO_MAJOR;

/*�豸�ṹ��,�˽ṹ����Է�װ�豸��ص�һЩ��Ϣ��
  �ź�����Ҳ���Է�װ�ڴ˽ṹ�У��������豸ģ��һ�㶼
  Ӧ�÷�װһ�������Ľṹ�壬���˽ṹ���б������ĳЩ
  ��Ա�������ַ��豸��˵�����Ǳ������struct cdev cdev*/
struct cdevdemo_dev	
{
	struct cdev cdev;
};

struct cdevdemo_dev *cdevdemo_devp;	/*�豸�ṹ��ָ��*/

/*�ļ��򿪺������ϲ�Դ��豸����openʱ��ִ��*/
int cdevdemo_open(struct inode *inode, struct file *filp)	
{
	printk(KERN_NOTICE "======== cdevdemo_open ");
	return 0;
}

/*�ļ��ͷţ��ϲ�Դ��豸����closeʱ��ִ��*/
int cdevdemo_release(struct inode *inode, struct file *filp)	
{
	printk(KERN_NOTICE "======== cdevdemo_release ");	
	return 0;
}

/*�ļ��Ķ��������ϲ�Դ��豸����readʱ��ִ��*/
static ssize_t cdevdemo_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos)
{
	printk(KERN_NOTICE "======== cdevdemo_read ");	
}

/* �ļ������ṹ�壬�����Ѿ���������ṹ*/
static const struct file_operations cdevdemo_fops =
{
	.owner = THIS_MODULE,
	.open = cdevdemo_open,
	.release = cdevdemo_release,
	.read = cdevdemo_read,
};

/*��ʼ����ע��cdev*/
static void cdevdemo_setup_cdev(struct cdevdemo_dev *dev, int index)
{
	printk(KERN_NOTICE "======== cdevdemo_setup_cdev 1");	
	int err, devno = MKDEV(cdevdemo_major, index);
	printk(KERN_NOTICE "======== cdevdemo_setup_cdev 2");

	/*��ʼ��һ���ַ��豸���豸��֧�ֵĲ�����cdevdemo_fops��*/	
	cdev_init(&dev->cdev, &cdevdemo_fops);
	printk(KERN_NOTICE "======== cdevdemo_setup_cdev 3");	
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &cdevdemo_fops;
	printk(KERN_NOTICE "======== cdevdemo_setup_cdev 4");	
	err = cdev_add(&dev->cdev, devno, 1);
	printk(KERN_NOTICE "======== cdevdemo_setup_cdev 5");
	if(err)
	{
		printk(KERN_NOTICE "Error %d add cdevdemo %d", err, index);	
	}
}

int cdevdemo_init(void)
{
	printk(KERN_NOTICE "======== cdevdemo_init ");	
	int ret;
	dev_t devno = MKDEV(cdevdemo_major, 0);

	struct class *cdevdemo_class;
	/*�����豸�ţ��������ʧ�ܲ��ö�̬���뷽ʽ*/
	if(cdevdemo_major)
	{
		printk(KERN_NOTICE "======== cdevdemo_init 1");
		ret = register_chrdev_region(devno, 1, "cdevdemo");
	}else
	{
		printk(KERN_NOTICE "======== cdevdemo_init 2");
		ret = alloc_chrdev_region(&devno,0,1,"cdevdemo");
		cdevdemo_major = MAJOR(devno);
	}
	if(ret < 0)
	{
		printk(KERN_NOTICE "======== cdevdemo_init 3");
		return ret;
	}
	/*��̬�����豸�ṹ���ڴ�*/
	cdevdemo_devp = kmalloc(sizeof(struct cdevdemo_dev), GFP_KERNEL);
	if(!cdevdemo_devp)	/*����ʧ��*/
	{
		ret = -ENOMEM;
		printk(KERN_NOTICE "Error add cdevdemo");	
		goto fail_malloc;
	}

	memset(cdevdemo_devp,0,sizeof(struct cdevdemo_dev));
	printk(KERN_NOTICE "======== cdevdemo_init 3");
	cdevdemo_setup_cdev(cdevdemo_devp, 0);

	/*���������Ǵ�����һ���������ͣ�����/sys/class������cdevdemoĿ¼
	  ����Ļ���һ����Ҫ������ִ��device_create�����/dev/���Զ�����
	  cdevdemo�豸�ڵ㡣����������ô˺����������ͨ���豸�ڵ�����豸
	  ��Ҫ�ֶ�mknod�������豸�ڵ���ٷ��ʡ�*/
	cdevdemo_class = class_create(THIS_MODULE, "cdevdemo");
	device_create(cdevdemo_class, NULL, MKDEV(cdevdemo_major, 0), NULL, "cdevdemo");

	printk(KERN_NOTICE "======== cdevdemo_init 4");
	return 0;

	fail_malloc:
		unregister_chrdev_region(devno,1);
}

void cdevdemo_exit(void)	/*ģ��ж��*/
{
	printk(KERN_NOTICE "End cdevdemo");	
	cdev_del(&cdevdemo_devp->cdev);	/*ע��cdev*/
	kfree(cdevdemo_devp);		/*�ͷ��豸�ṹ���ڴ�*/
	unregister_chrdev_region(MKDEV(cdevdemo_major,0),1);	//�ͷ��豸��
}

MODULE_LICENSE("Dual BSD/GPL");
module_param(cdevdemo_major, int, S_IRUGO);
module_init(cdevdemo_init);
module_exit(cdevdemo_exit);
