/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>

#include "hello1.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("IO-34 Yasan Viktor");
MODULE_DESCRIPTION("Hello2 module: calls print_hello() from hello1");
MODULE_VERSION("0.2");

static unsigned int count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count,
     "How many times to call print_hello() (uint)");

static int __init hello2_init(void)
{
  unsigned int i;

  if (count == 0 || (count >= 5 && count <= 10))
    pr_warn("hello2: count=%u (warning range)\n", count);

  if (count > 10) {
    pr_err("hello2: invalid count=%u (>10), aborting\n", count);
    return -EINVAL;
  }

  for (i = 0; i < count; i++)
    print_hello();

  pr_info("hello2: successfully called print_hello() %u times\n",
    count);

  return 0;
}

static void __exit hello2_exit(void)
{
  pr_info("hello2: module unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
