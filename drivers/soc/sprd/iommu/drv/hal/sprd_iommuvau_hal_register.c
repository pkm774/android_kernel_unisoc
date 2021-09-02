/*
 * Copyright (C) 2019 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "sprd_iommuvau_hal_register.h"

void mmu_vau_enable(ulong ctrl_base_addr, u32 iommu_id, u32 mmu_enable)
{
	ulong reg_addr = ctrl_base_addr;

	putbit(reg_addr, mmu_enable, 0);
}

/*
 * sharkl3 dpu register is shadowed to internal ram, so we have to set
 * vaorbypass, clkgate and enable in a single function.
 */
void mmu_vau_vaorbypass_clkgate_enable_combined(ulong ctrl_base_addr,
	u32 iommu_id)
{
	ulong reg_addr = ctrl_base_addr + MMU_EN;
	u32  reg_value = 0;

	if (iommu_id == IOMMU_VAU_DCAM || iommu_id == IOMMU_VAU_ISP ||
	    iommu_id == IOMMU_VAU_VSP || iommu_id == IOMMU_VAU_VSP1 ||
	    iommu_id == IOMMU_VAU_VSP2 || iommu_id == IOMMU_VAU_JPG ||
	    iommu_id == IOMMU_VAU_CPP) {
		reg_value = reg_read_dword(reg_addr);
		reg_write_dword(reg_addr, reg_value | 0x3);
	} else {
		reg_value = reg_read_dword(reg_addr);
		reg_write_dword(reg_addr, reg_value | 0x13);
	}

}

void mmu_vau_vaout_bypass_enable(ulong ctrl_base_addr, u32 iommu_id,
				 bool vaor_bp_en)
{
	ulong reg_addr = ctrl_base_addr + MMU_EN;

	if (vaor_bp_en)
		putbit(reg_addr, 1, 4);
	else
		putbit(reg_addr, 0, 4);
}

/*just isp_iommu need,be similar to update*/
void mmuvau_tlb_enable(ulong ctrl_base_addr, u32 r_enable, u32 w_enable)
{
	ulong reg_addr = ctrl_base_addr + 0x18;

	if (w_enable)
		reg_write_dword(reg_addr, 0xffffffff);
	else
		reg_write_dword(reg_addr, 0);

	reg_addr += 0x4;
	if (r_enable)
		reg_write_dword(reg_addr, 0xffffffff);
	else
		reg_write_dword(reg_addr, 0);
}

void mmu_vau_update(ulong ctrl_base_addr, u32 iommu_id)
{
	ulong reg_addr = ctrl_base_addr + UPDATE_OFFSET;

	reg_write_dword(reg_addr, 0xffffffff);
}

void mmu_vau_first_vpn(ulong ctrl_base_addr, u32 iommu_id, u32 vp_addr)
{
	ulong reg_addr = ctrl_base_addr;
	reg_addr += FIRST_VPN_OFFSET;

	reg_write_dword(reg_addr, (vp_addr >> MMU_MAPING_PAGESIZE_SHIFFT));
}

void mmu_vau_vpn_range(ulong ctrl_base_addr, u32 iommu_id, u32 vp_range)
{
	ulong reg_addr = ctrl_base_addr + VPN_RANGE_OFFSET;

	reg_write_dword(reg_addr, vp_range);
}

void mmu_vau_first_ppn(ulong ctrl_base_addr, u32 iommu_id, ulong pp_addr)
{
	ulong reg_addr = ctrl_base_addr;

	reg_addr += FIRST_PPN_OFFSET;
	reg_write_dword(reg_addr,
		(pp_addr >> MMU_MAPING_PAGESIZE_SHIFFT));
}

/*just isp_iommu need,max is 256K*/
void mmuvau_pagetable_size(ulong ctrl_base_addr, ulong pt_size)
{
	ulong reg_addr = ctrl_base_addr + 0x30;

	reg_write_dword(reg_addr, pt_size & 0x3ffff);
}

void mmu_vau_default_ppn(ulong ctrl_base_addr, u32 iommu_id, ulong pp_addr)
{
	ulong reg_addr = ctrl_base_addr;
	reg_addr += DEFAULT_PPN_OFFSET;

	reg_write_dword(reg_addr, (pp_addr >> MMU_MAPING_PAGESIZE_SHIFFT));
}

void mmu_vau_pt_update_arqos(ulong ctrl_base_addr, u32 arqos)
{
	ulong reg_addr = ctrl_base_addr + PT_UPDATE_QOS_OFFSET;

	reg_write_dword(reg_addr, (arqos & 0xf));
}

/*1M align*/
void mmu_vau_mini_ppn1(ulong ctrl_base_addr, u32 iommu_id, ulong ppn1)
{
	ulong reg_addr = 0;

	if (iommu_id == IOMMU_VAU_JPG)
		reg_addr = ctrl_base_addr + JPG_PPN1_OFFSET;
	else
		reg_addr = ctrl_base_addr + MINI_PPN1_OFFSET;

	reg_write_dword(reg_addr, (ppn1 >> 20));
}

void mmu_vau_ppn1_range(ulong ctrl_base_addr, u32 iommu_id, ulong ppn1_range)
{
	ulong reg_addr = 0;

	if (iommu_id == IOMMU_VAU_JPG)
		reg_addr = ctrl_base_addr + JPG_PPN1_RANGE_OFFSET;
	else
		reg_addr = ctrl_base_addr + PPN1_RANGE_OFFSET;

	reg_write_dword(reg_addr, (ppn1_range >> 20));
}

void mmu_vau_mini_ppn2(ulong ctrl_base_addr, u32 iommu_id, ulong ppn2)
{
	ulong reg_addr = 0;

	if (iommu_id == IOMMU_VAU_JPG)
		reg_addr = ctrl_base_addr + JPG_PPN2_OFFSET;
	else
		reg_addr = ctrl_base_addr + MINI_PPN2_OFFSET;

	reg_write_dword(reg_addr, (ppn2 >> 20));
}

void mmu_vau_ppn2_range(ulong ctrl_base_addr, u32 iommu_id, ulong ppn2_range)
{
	ulong reg_addr = 0;

	if (iommu_id == IOMMU_VAU_JPG)
		reg_addr = ctrl_base_addr + JPG_PPN2_RANGE_OFFSET;
	else
		reg_addr = ctrl_base_addr + PPN2_RANGE_OFFSET;

	reg_write_dword(reg_addr, (ppn2_range >> 20));
}

u32 mmu_vau_read_page_entry(ulong page_table_addr, u32 entry_index)
{
	ulong reg_addr = page_table_addr + entry_index * 4;
	u32 phy_addr = 0;

	phy_addr = reg_read_dword(reg_addr);
	return phy_addr;
}

void mmu_vau_int_enable(ulong ctrl_base_addr, u32 iommu_id)
{
	ulong reg_addr = ctrl_base_addr + MMU_INT_EN;

	reg_write_dword(reg_addr, 0xff);
}

