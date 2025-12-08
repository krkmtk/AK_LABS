/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

#include "hello1.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("IO-34 Yasan Viktor");
MODULE_DESCRIPTION("Hello1 module: time measurements and list");
MODULE_VERSION("0.2");

struct hello_item {
  struct list_head list;
  ktime_t start;
  ktime_t end;
};

static LIST_HEAD(hello_head);
static unsigned int call_id;

void print_hello(void)
{
  struct hello_item *it;

  it = kmalloc(sizeof(*it), GFP_KERNEL);
  if (!it) {
    pr_err("hello1: failed to allocate memory for hello_item\n");
    return;
  }

  it->start = ktime_get();
  call_id++;
  pr_info("hello1: Hello, world! call #%u\n", call_id);
  it->end = ktime_get();

  list_add_tail(&it->list, &hello_head);
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
  pr_info("hello1: module loaded\n");
  return 0;
}

static void __exit hello1_exit(void)
{
  struct hello_item *it, *tmp;
  unsigned int i = 0;

  list_for_each_entry_safe(it, tmp, &hello_head, list) {
    s64 delta_ns = ktime_to_ns(ktime_sub(it->end, it->start));

    i++;
    pr_info("hello1: call #%u took %lld ns\n",
      i, (long long)delta_ns);
    list_del(&it->list);
    kfree(it);
  }

  pr_info("hello1: module unloaded, printed %u messages\n", call_id);
}

module_init(hello1_init);
module_exit(hello1_exit);
