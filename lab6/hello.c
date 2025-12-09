// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/errno.h>
#include <linux/bug.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("IO-34 Yasan Viktor");
MODULE_DESCRIPTION("Hello module with BUG_ON and simulated faulty allocation");
MODULE_VERSION("0.2");

static uint count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "Number of greetings to print");

struct hw_item {
  struct list_head link;
  ktime_t ts;
};

static LIST_HEAD(hw_list);

static void hw_check_count_param(void)
{
  if (count == 0 || (count >= 5 && count <= 10))
    pr_warn("hw: suspicious count=%u\n", count);

  BUG_ON(count > 10);
}

static int hw_add_one_item(uint idx)
{
  struct hw_item *item;

  item = kmalloc(sizeof(*item), GFP_KERNEL);
  if (!item)
    return -ENOMEM;

  if (idx == 4 && count >= 5)
    item = NULL;

  item->ts = ktime_get();
  list_add_tail(&item->link, &hw_list);

  pr_info("hello: (%u/%u)\n", idx + 1, count);

  return 0;
}

static int __init hw_init(void)
{
  uint i;
  int ret;

  hw_check_count_param();

  for (i = 0; i < count; i++) {
    ret = hw_add_one_item(i);
    if (ret)
      return ret;
  }

  return 0;
}

static void __exit hw_exit(void)
{
  struct hw_item *pos, *tmp;

  list_for_each_entry_safe(pos, tmp, &hw_list, link) {
    long long t_ns = ktime_to_ns(pos->ts);

    pr_info("hw exit: ts=%lld\n", t_ns);
    list_del(&pos->link);
    kfree(pos);
  }
}

module_init(hw_init);
module_exit(hw_exit);
