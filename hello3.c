
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Khusainov Dmytro Dmytro");
MODULE_DESCRIPTION("Lab5 Module hello.c");
MODULE_LICENSE("Dual BSD/GPL");

static uint cycle_param = 1;

// 0444 = S_IRUGO - флаг дозволу на читання параметра
module_param(cycle_param, uint, 0444);
MODULE_PARM_DESC(cycle_param, "My description");

struct my_struct {
	struct list_head list;
	ktime_t my_time;
};

// статична зміна голови списку
static LIST_HEAD(myList);
int i;


#define freeMemory(message) do {\
	struct my_struct *ptr, *next; \
	list_for_each_entry_safe(ptr, next, &myList, list) {\
		if (message == 1) {\
			pr_emerg("Time in nanoseconds: %lld\n", ktime_to_ns(ptr->my_time));\
		}\
		list_del(&ptr->list);\
		kfree(ptr);\
	}\
} while (0)


static int __init hello_init(void)
{
	BUG_ON(cycle_param > 10);

	if (cycle_param == 0 || (cycle_param <= 10 && cycle_param >= 5)) {
		pr_emerg("Warning: %u\n", cycle_param);
	}

	i = 0;
	while (i < cycle_param) {
		struct my_struct *ptr = kmalloc(sizeof(*ptr), GFP_KERNEL);
		// Примусово встановлення ptr на 0 для 9-го елементу
		if (i == 7) {
			ptr = NULL;		
		}
		if (!ptr) {
			pr_err("kmalloc() returned 0\n");
			freeMemory(0);
			BUG();
		}
		ptr->my_time = ktime_get();
		list_add_tail(&ptr->list, &myList);
		pr_emerg("Hello world!\n");
		i++;
	}

	return 0;
}

static void __exit hello_exit(void)
{
	freeMemory(1);
}

module_init(hello_init);
module_exit(hello_exit);
