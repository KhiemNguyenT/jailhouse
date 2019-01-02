/*
 * Jailhouse, a Linux-based partitioning hypervisor
 *
 * Configuration for Ebisu board (with R-Car E3)
 *
 * Copyright (c) 2019 Renesas Electronics Corporation.
 *
 * Authors:
 *  Khiem Nguyen <khiem.nguyen.xt@renesas.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 *
 */

#include <jailhouse/types.h>
#include <jailhouse/cell-config.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

struct {
	struct jailhouse_system header;
	__u64 cpus[1];
	struct jailhouse_memory mem_regions[5];
	struct jailhouse_irqchip irqchips[1];
	struct jailhouse_pci_device pci_devices[1];
} __attribute__((packed)) config = {
	.header = {
		.signature = JAILHOUSE_SYSTEM_SIGNATURE,
		.revision = JAILHOUSE_CONFIG_REVISION,
		.flags = JAILHOUSE_SYS_VIRTUAL_DEBUG_CONSOLE,
		.hypervisor_memory = {
			.phys_start = 0x6c000000,
			.size =       0x000400000,
		},
		.debug_console = {
			.address = 0xe6e88000,
			.size = 0x64,
			.type = JAILHOUSE_CON_TYPE_SCIFA,
			.flags = JAILHOUSE_CON_ACCESS_MMIO |
				JAILHOUSE_CON_REGDIST_4,
		},
		.platform_info = {
			.pci_mmconfig_base = 0xfc000000,
			.pci_mmconfig_end_bus = 0,
			.pci_is_virtual = 1,
			.pci_domain = 1,
			.arm = {
				.gic_version = 2,
				.gicd_base = 0xf0210000,
				.gicc_base = 0xf022f000,
				.gich_base = 0xf0240000,
				.gicv_base = 0xf026f000,
				.maintenance_irq = 25,
			},
		},
		.root_cell = {
			.name = "r8a77980-ebisu-4d",

			.cpu_set_size = sizeof(config.cpus),
			.num_memory_regions = ARRAY_SIZE(config.mem_regions),
			.num_irqchips = ARRAY_SIZE(config.irqchips),
			.num_pci_devices = ARRAY_SIZE(config.pci_devices),

			.vpci_irq_base = 108-32,
		},
	},

	.cpus = {
		0x3,
	},

	.mem_regions = {
		/* RAM (Linux part 1 - prior to Lossy area) */ {
			.phys_start = 0x48000000,
			.virt_start = 0x48000000,
			.size = 0xC000000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_EXECUTE,
		},
		/* RAM (Linux part 2 - after Lossy area) */ {
			.phys_start = 0x57000000,
			.virt_start = 0x57000000,
			.size = 0x15000000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_EXECUTE,
		},
		/* RAM (inmate) */ {
			.phys_start = 0x6c400000,
			.virt_start = 0x6c400000,
			.size = 0x400000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_EXECUTE,
		},
		/* IVSHMEM shared memory region for 00:00.0 */ {
			.phys_start = 0x6c800000,
			.virt_start = 0x6c800000,
			.size = 0x100000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE,
		},
	},

	.irqchips = {
		/* GIC */ {
			.address = 0xf1010000,
			.pin_base = 32,
			.pin_bitmap = {
				0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
			},
		},
		/* GIC */ {
			.address = 0xf1010000,
			.pin_base = 160,
			.pin_bitmap = {
				0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
			},
		}
		/* GIC */ {
			.address = 0xf1010000,
			.pin_base = 288,
			.pin_bitmap = {
				0xffffffff
			},
		},
	},

	.pci_devices = {
		/* 0001:00:00.0 */ {
			.type = JAILHOUSE_PCI_TYPE_IVSHMEM,
			.domain = 1,
			.bdf = 0 << 3,
			.bar_mask = {
				0xffffff00, 0xffffffff, 0x00000000,
				0x00000000, 0x00000000, 0x00000000,
			},
			.shmem_region = 4,
			.shmem_protocol = JAILHOUSE_SHMEM_PROTO_VETH,
		},
	},
};
