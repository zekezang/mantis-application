#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/stat.h>
#include <linux/fs.h>
#include <asm/unistd.h>
#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/ioctl.h>

char c1,c2;
static struct file *file;

static int __init testmod_init(void) {

	mm_segment_t old_fs;

	ssize_t result;
	ssize_t ret;

	char* bao = "/tmp/zeke.data";

	c1 = '1';

	file = filp_open(bao, O_CREAT | O_RDWR, 0);

	if (IS_ERR(file))
		goto fail0;
	old_fs = get_fs();
	set_fs(get_ds());

	ret = file->f_op->write(file, &c1, sizeof(char), &file->f_pos);

	printk("ret-->%d\n", ret);
	set_fs(old_fs);
	filp_close(file, NULL);


	//------------------------华丽的分割线---------------------------------


	file = filp_open(bao, O_CREAT | O_RDWR, 0);

	if (IS_ERR(file))
		goto fail0;
	old_fs = get_fs();
	set_fs(get_ds());

	result = file->f_op->read(file, &c2, sizeof(char), &file->f_pos);

	char c3 = '1';

	int a = memcmp(&c2,&c3,sizeof(char));

	printk(KERN_INFO "----%d\n",a);

	if (result >= 0) {
		printk("buf2-->%c\n", c2);
	} else {
		printk("failed\n");
	}
	set_fs(old_fs);
	filp_close(file, NULL);
	printk("file loaded\n");
	return 0;
	fail0: {
		filp_close(file, NULL);
		printk("load failed\n");
	}
	return 1;
}
static void __exit testmod_cleanup(void) {

	printk("module exit......................................................\n");
}
module_init(testmod_init);
module_exit(testmod_cleanup);
