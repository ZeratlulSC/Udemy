#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include "log.h"
#include "filter.h"

MODULE_LICENSE("GPL");

static struct dentry *hooker_filter_root = NULL;

enum {
	PID,
	GID,
	UID,
	PROC_NAME,
	FLT_CNT
} filter_types;

struct filter {
	struct dentry *entry;
	struct list_head head;
	size_t list_size;
	char *name;
	struct dentry *d;
	const struct file_operations *fops;
	struct mutex lock;
};
extern struct filter *filters;

struct filter_item {
	struct list_head link;
	void *data;
};

struct filter_name_item {
	struct list_head link;
	char proc_name[TASK_COMM_LEN];
};

static void filter_cleanup(struct filter *filter);

static int filter_open(struct inode *inode, struct file *file)
{
	struct filter *filter = inode->i_private;

	file->private_data = filter;

	return 0;
};

static ssize_t filter_val_write(struct file *file, const char __user *ubuf,
			size_t len, loff_t *ppos)
{
	struct filter *filter = file->private_data;
	unsigned long val;
	struct filter_item *item;
	int ret;

	ret = kstrtoul_from_user(ubuf, len, 10, &val);
	if (ret) {
	log_debug(filter, "failed added to filter %s, ret %d\n",
			filter->name, ret);
		return ret;
	}

	// if (!ppos);
		// Drop filter list;

	item = kmalloc(sizeof(struct filter_item), GFP_KERNEL);
	if (!item) {
		return -ENOMEM;
	}
	INIT_LIST_HEAD(&item->link);

	mutex_lock(&filter->lock);
	item->data = (void *)val;
	list_add_tail(&item->link, &filter->head);
	filter->list_size++;
	log_debug(filter, "added to filter %s val %ld, list_size %ld pos %llu\n",
			filter->name, val, filter->list_size);
	mutex_unlock(&filter->lock);

	return len;
}

static ssize_t filter_val_read(struct file *file, char __user *user_buf, size_t len, loff_t *ppos)
{
	struct filter *filter = file->private_data;
	struct filter_item *item;
	char get_buf[24];	/* enough to store a u64 and "\n\0" */
	char *kbuf;
	int kbuf_offset = 0;
	ssize_t buf_len;
	ssize_t bytes;

	if (*ppos)
		return 0;

	mutex_lock(&filter->lock);
	buf_len = sizeof(get_buf) * filter->list_size;

	kbuf = kmalloc(buf_len, GFP_KERNEL);
	if (!kbuf) {
		mutex_unlock(&filter->lock);
		return -ENOMEM;
	}

	log_debug(filter, "read from filter %s list_size %ld\n",
			filter->name, filter->list_size);
	list_for_each_entry(item, &filter->head, link) {
		log_debug(filter, "\titem %p val %d\n",
			item, (int)item->data);
		kbuf_offset += sprintf(kbuf + kbuf_offset, "%d\n", (int)item->data);
	}

	mutex_unlock(&filter->lock);

	bytes = simple_read_from_buffer(user_buf, len, ppos, kbuf, buf_len);
	kfree(kbuf);
	log_debug(filter, "read %ld bytes from filter %s\n",
			bytes, filter->name);

	return bytes;
}

static ssize_t filter_name_write(struct file *file, const char __user *ubuf,
			size_t len, loff_t *ppos)
{
	struct filter *filter = file->private_data;
	struct filter_name_item *item;
	int ret;

	if (len > TASK_COMM_LEN) {
		log_debug(filter, "failed added to filter %s, too long name\n",
			filter->name);
		return -EINVAL;
	}

	// if (!ppos);
		// Drop filter list;
	item = kmalloc(sizeof(struct filter_item), GFP_KERNEL);
	if (!item) {
		return -ENOMEM;
	}

	INIT_LIST_HEAD(&item->link);
	mutex_lock(&filter->lock);

	ret = copy_from_user(item->proc_name, ubuf, len);
	if (ret) {
		mutex_unlock(&filter->lock);
		log_debug(filter, "filter %s failed to copy_from_user data len %ld\n",
				filter->name, len);
		kfree(item);
		return ret;
	}

	item->proc_name[strcspn(item->proc_name, "\r\n")] = '\0';
	list_add_tail(&item->link, &filter->head);
	filter->list_size++;
	log_debug(filter, "added to filter %s func %s, list_size %ld pos %llu\n",
			filter->name, item->proc_name, filter->list_size, *ppos);
	mutex_unlock(&filter->lock);

	return len;
}

static ssize_t filter_name_read(struct file *file, char __user *user_buf, size_t len, loff_t *ppos)
{
	struct filter *filter = file->private_data;
	struct filter_name_item *item;
	char get_buf[TASK_COMM_LEN];
	char *kbuf;
	int kbuf_offset = 0;
	ssize_t buf_len;
	ssize_t bytes;

	if (*ppos)
		return 0;

	mutex_lock(&filter->lock);
	buf_len = sizeof(get_buf) * filter->list_size;

	kbuf = kmalloc(buf_len, GFP_KERNEL);
	if (!kbuf) {
		mutex_unlock(&filter->lock);
		return -ENOMEM;
	}

	log_debug(filter, "read from filter %s list_size %ld\n",
			filter->name, filter->list_size);
	list_for_each_entry(item, &filter->head, link) {
		log_debug(filter, "\titem %p func %s\n",
		item, item->proc_name);
		kbuf_offset += snprintf(kbuf + kbuf_offset, TASK_COMM_LEN, "%s\n", item->proc_name);
	}

	mutex_unlock(&filter->lock);

	bytes = simple_read_from_buffer(user_buf, len, ppos, kbuf, buf_len);
	kfree(kbuf);
	log_debug(filter, "read %ld bytes from filter %s\n",
			bytes, filter->name);

	return bytes;
}

#if 0
static loff_t char_device_lseek(struct file *file, loff_t offset,
		int orig)
{
	struct filter *filter = file->private_data;
	loff_t new_pos=0;

	log_debug(filter, "@@@ lseek: %d offset %lld bytes filter %s\n",
			orig, offset, filter->name);

	switch (orig) {
	case 0: /* SEEK_SET: */
		// Drop filter list
		new_pos = offset;
		break;
	case 1: /* SEEK_CUR: */
		new_pos = file->f_pos + offset;
		// Do nothing just return counter list
		break;
	case 2: /* SEEK_END: */
		new_pos = TASK_COMM_LEN - offset;
		break;
	}
	if( new_pos > TASK_COMM_LEN ) new_pos = TASK_COMM_LEN;
	if( new_pos < 0 ) new_pos = 0;

	file->f_pos = new_pos;

	return new_pos;
}
#endif

long device_ioctl(struct file *file,
		unsigned int ioctl_num,
		unsigned long ioctl_param)
{
	struct filter_ioctl_data filter_ioctl_tmp;
	struct filter_ioctl_data *user_param = (struct filter_ioctl_data *)ioctl_param;
	struct filter *filter;
	int ret;
	loff_t ppos = 0;

	log_debug(filter, "ioctl cmd:%u \n", ioctl_num);
	ret = copy_from_user(&filter_ioctl_tmp, user_param, sizeof(struct filter_ioctl_data));
	if (ret) {
		log_debug(filter, "ioctl failed to copy_from_user data len %zd %lx ret %d\n",
				sizeof(struct filter_ioctl_data), ioctl_param, ret);
		return ret;
	}

	if (filter_ioctl_tmp.filter_type > FLT_CNT) {
		log_info(filter, "ioctl failed: incorrect filter type %d; should be less than %d\n",
				filter_ioctl_tmp.filter_type, FLT_CNT);
		return -EINVAL;
	}

	filter = &filters[filter_ioctl_tmp.filter_type];
	file->private_data = filter;
	log_debug(filter, "ioctl selected filter %s %p\n", filter->name, &filters[filter_ioctl_tmp.filter_type]);

	switch (ioctl_num) {
	case IOCTL_ADD_FILTER:
		ret = filter->fops->write(file, filter_ioctl_tmp.data, filter_ioctl_tmp.len, &ppos);
		if (ret >= 0)
			ret = 0;

		break;
	case IOCTL_READ_FILTERS:
		ret = filter->fops->read(file, filter_ioctl_tmp.data, filter_ioctl_tmp.len, &ppos);
		if (ret >= 0)
			ret = 0;
		break;
	case IOCTL_CLEANUP_FILTERS:
		filter_cleanup(filter);
		break;


	case IOCTL_SET_FILTER:
	case IOCTL_GET_FILTER:
	case IOCTL_REMOVE_FILTER:
		break;
	}

	return ret;
}


static const struct file_operations filter_name_fops = {
	.owner = THIS_MODULE,
	.open = filter_open,
	.read = filter_name_read,
	.write = filter_name_write,
	.llseek = default_llseek,
};

static const struct file_operations filter_val_fops = {
	.owner = THIS_MODULE,
	.open = filter_open,
	.read = filter_val_read,
	.write = filter_val_write,
	// .llseek = char_device_lseek,
	.llseek = default_llseek,
};

static const struct file_operations filter_ioctl_fops = {
// #if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
	// .ioctl = device_ioctl,
// #else
	.unlocked_ioctl = device_ioctl,
// #endif
};

static struct filter _filters[] = {
	[PID] = {
		.name = "pid",
		.fops = &filter_val_fops,
		.list_size = 0,
	},
	[GID] = {
		.name = "gid",
		.fops = &filter_val_fops,
		.list_size = 0,
	},
	[UID] = {
		.name = "uid",
		.fops = &filter_val_fops,
		.list_size = 0,
	},
	[PROC_NAME] = {
		.name = "proc_name",
		.fops = &filter_name_fops,
		.list_size = 0,
	},
};

struct filter *filters = _filters;

// TODO: define a generic macros apply_##flt_name_filter()
void apply_pid_filter(struct filter *filter, pid_t pid, int *is_pass)
{
	struct filter_item *item;

	mutex_lock(&filter->lock);
	if (list_empty(&filter->head))
		goto unlock;
	list_for_each_entry(item, &filter->head, link) {
		if ((pid_t)item->data == pid) {
			goto unlock;
		}
	}
	mutex_unlock(&filter->lock);

	*is_pass = 0;
	return;

unlock:
	mutex_unlock(&filter->lock);
	return;
}
void apply_gid_filter(struct filter *filter, gid_t gid, int *is_pass)
{
	struct filter_item *item;

	mutex_lock(&filter->lock);
	if (list_empty(&filter->head))
		goto unlock;
	list_for_each_entry(item, &filter->head, link) {
		if ((gid_t)item->data == gid) {
			goto unlock;
		}
	}
	mutex_unlock(&filter->lock);

	*is_pass = 0;
	return;

unlock:
	mutex_unlock(&filter->lock);
	return;
}
void apply_uid_filter(struct filter *filter, uid_t uid, int *is_pass)
{
	struct filter_item *item;

	mutex_lock(&filter->lock);
	if (list_empty(&filter->head))
		goto unlock;
	list_for_each_entry(item, &filter->head, link) {
		if ((uid_t)item->data == uid) {
			goto unlock;
		}
	}
	mutex_unlock(&filter->lock);

	*is_pass = 0;
	return;

unlock:
	mutex_unlock(&filter->lock);
	return;
}
void apply_proc_name_filter(struct filter *filter, char *proc_name, int *is_pass)
{
	struct filter_name_item *item;

	mutex_lock(&filter->lock);
	if (list_empty(&filter->head))
		goto unlock;
	list_for_each_entry(item, &filter->head, link) {
		if (strncmp(proc_name, item->proc_name, TASK_COMM_LEN) == 0) {
			goto unlock;
		} else {
			log_debug(filter, "@@@@ flt %s len %zd; proc %s len %zd\n",
					item->proc_name, strlen(item->proc_name), proc_name, strlen(proc_name));
		}
	}
	mutex_unlock(&filter->lock);

	*is_pass = 0;
	return;

unlock:
	mutex_unlock(&filter->lock);
	return;
}

void hooker_filter(struct filter_data *data)
{
	pid_t parent_pid = data->parent_pid;
	pid_t child_pid = data->child_pid;
	gid_t gid = data->gid;
	uid_t uid = data->uid;
	char *name = data->proc_name;
	unsigned int filter_num = 0;
	struct filter *filter;
	int is_pass = 1;

	log_debug(filter, "task %d: uid %d gid %d name %s created proc %d\n",
			parent_pid, uid, gid, name, child_pid);

	BUILD_BUG_ON(ARRAY_SIZE(_filters) != FLT_CNT);
	for (filter_num = 0; filter_num < ARRAY_SIZE(_filters); filter_num++) {
		filter = &filters[filter_num];
		switch (filter_num) {
		case PID:
			apply_pid_filter(filter, parent_pid, &is_pass);
			break;
		case GID:
			apply_gid_filter(filter, gid, &is_pass);
			break;
		case UID:
			apply_uid_filter(filter, uid, &is_pass);
			break;
		case PROC_NAME:
			apply_proc_name_filter(filter, name, &is_pass);
			break;
		default:
			BUG();
		}
		if (!is_pass) {
			log_debug(filter, "failed on filter %s\n",
				filter->name);
			return;
		}
	}
	log_info(filter, "task %d: uid %d gid %d name %s created proc %d\n",
			parent_pid, uid, gid, name, child_pid);
	return;
}

#if 0
static unsigned int cfake_major = 0;
int filter_register_dev()
{
	int err;
	dev_t dev = 0;

	err = alloc_chrdev_region(&dev, 0, 1, DEVICE_FILE_NAME);
	if (err < 0) {
		// printk(KERN_WARNING "[target] alloc_chrdev_region() failed\n");
		return err;
	}
	cfake_major = MAJOR(dev);

	//$ls /sys/class
	if ((cl = class_create(THIS_MODULE, DEVICE_FILE_NAME)) == NULL) {
		unregister_chrdev_region(first, 1);
		return -1;
	}
	//$ls /dev/
	if (device_create(cl, NULL, first, NULL, DEVICE_FILE_NAME) == NULL) {
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return -1;
	}

	cdev_init(&c_dev, &fops);

	if (cdev_add(&c_dev, first, 1) == -1) {
		device_destroy(cl, first);
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return -1;
	}

	return 0;
}
#endif

int filter_init(void)
{
	unsigned int filter_num = 0;
	struct filter *filter;
	int ret_val;

	ret_val = register_chrdev(MAJOR_NUM, DEVICE_FILE_NAME, &filter_ioctl_fops);
	if (ret_val < 0) {
		log_error(filter, "register dev/%s failed with %d\n", DEVICE_FILE_NAME, ret_val);
		return ret_val;
	}
	log_info(filter, "mknod %s c %d 0\n", DEVICE_FILE_NAME, MAJOR_NUM);

	hooker_filter_root = debugfs_create_dir(DEVICE_FILE_NAME, NULL);
	if (!hooker_filter_root) {
		log_error(filter, "register debugfs %s failed with %d\n", DEVICE_FILE_NAME, ret_val);
		return -ENOMEM;
	}

	for (filter_num = 0; filter_num < ARRAY_SIZE(_filters); filter_num++) {
		filter = &filters[filter_num];

		INIT_LIST_HEAD(&filter->head);
		mutex_init(&filter->lock);
		filter->entry = debugfs_create_file(filter->name, 0666, hooker_filter_root, filter,
						filter->fops);
		if (!filter->entry) {
			debugfs_remove_recursive(hooker_filter_root);
			hooker_filter_root = NULL;
			unregister_chrdev(MAJOR_NUM, DEVICE_FILE_NAME);
			return -ENOMEM;
		}
	}

	return 0;
}

static void filter_cleanup(struct filter *filter)
{
	struct filter_item *item, *item_tmp;

	log_debug(filter, "clean filter %s size %ld\n",
			filter->name, filter->list_size);

	if (!list_empty(&filter->head)) {
		list_for_each_entry_safe(item, item_tmp, &filter->head, link) {
			list_del_init(&item->link);
			kfree(item);
		}
	}
	filter->list_size = 0;
}

void filter_exit(void)
{
	unsigned int filter_num = 0;
	struct filter *filter;

	if (hooker_filter_root) {
		for (filter_num = 0; filter_num < ARRAY_SIZE(_filters); filter_num++) {
			filter = &filters[filter_num];
			filter_cleanup(filter);
		}
		debugfs_remove_recursive(hooker_filter_root);
	}
	unregister_chrdev(MAJOR_NUM, DEVICE_FILE_NAME);
	return;
}

