#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x75999ebb, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x61ab74f, __VMLINUX_SYMBOL_STR(blk_init_queue) },
	{ 0xc67c2e62, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x38223167, __VMLINUX_SYMBOL_STR(alloc_disk) },
	{ 0x95c1c433, __VMLINUX_SYMBOL_STR(blk_cleanup_queue) },
	{ 0x46434934, __VMLINUX_SYMBOL_STR(disable_kprobe) },
	{ 0x6bf1c17f, __VMLINUX_SYMBOL_STR(pv_lock_ops) },
	{ 0x754d539c, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x1b9aca3f, __VMLINUX_SYMBOL_STR(jprobe_return) },
	{ 0xb1bedeb7, __VMLINUX_SYMBOL_STR(register_jprobe) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x1ebfc662, __VMLINUX_SYMBOL_STR(del_gendisk) },
	{ 0xe265d1fd, __VMLINUX_SYMBOL_STR(netlink_kernel_release) },
	{ 0x71a50dbc, __VMLINUX_SYMBOL_STR(register_blkdev) },
	{ 0xb390eadc, __VMLINUX_SYMBOL_STR(netlink_unicast) },
	{ 0xe007de41, __VMLINUX_SYMBOL_STR(kallsyms_lookup_name) },
	{ 0xd5c9ed, __VMLINUX_SYMBOL_STR(init_net) },
	{ 0x61651be, __VMLINUX_SYMBOL_STR(strcat) },
	{ 0xb5a459dc, __VMLINUX_SYMBOL_STR(unregister_blkdev) },
	{ 0xe16b0d30, __VMLINUX_SYMBOL_STR(unregister_jprobe) },
	{ 0x67370f16, __VMLINUX_SYMBOL_STR(__alloc_skb) },
	{ 0xbba45bba, __VMLINUX_SYMBOL_STR(bdevname) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x5c4e46a, __VMLINUX_SYMBOL_STR(kfree_skb) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0xeaf89948, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xe259ae9e, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0xe4826bf2, __VMLINUX_SYMBOL_STR(__netlink_kernel_create) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x69acdf38, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xc447dd93, __VMLINUX_SYMBOL_STR(enable_kprobe) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0x5fd04157, __VMLINUX_SYMBOL_STR(device_add_disk) },
	{ 0xf0cf566f, __VMLINUX_SYMBOL_STR(__nlmsg_put) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "5551F1403294D72D905B4F1");
