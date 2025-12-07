/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/errno.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("IO-34 Yasan Viktor");
MODULE_DESCRIPTION("Hello, world kernel module.");
MODULE_VERSION("0.1");

static unsigned int count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "Times to print 'Hello, world!' (uint)");

struct hello_item {
	struct list_head list;
	ktime_t ts;
};

static LIST_HEAD(hello_head);

static int __init hello_init(void)
{
	unsigned int i;

	if (count == 0 || (count >= 5 && count <= 10))
		pr_warn("hello: count=%u (warning range)\n", count);

	if (count > 10)
		return -EINVAL;

	for (i = 0; i < count; i++) {
		struct hello_item *it = kmalloc(sizeof(*it), GFP_KERNEL);

		if (!it)
			return -ENOMEM;

		it->ts = ktime_get();
		list_add_tail(&it->list, &hello_head);
		pr_info("Hello, world! (%u/%u)\n", i + 1, count);
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_item *it, *tmp;

	list_for_each_entry_safe(it, tmp, &hello_head, list) {
		s64 ns = ktime_to_ns(it->ts);

		pr_info("goodbye: ts=%lld ns\n", (long long)ns);
		list_del(&it->list);
		kfree(it);
	}
}

module_init(hello_init);
module_exit(hello_exit);
