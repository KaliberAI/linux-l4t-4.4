/*
 * Copyright (c) 2018 NVIDIA CORPORATION.  All rights reserved.
 *
 * based on clk-mux.c

 * Copyright (C) 2011 Sascha Hauer, Pengutronix <s.hauer@pengutronix.de>
 * Copyright (C) 2011 Richard Zhao, Linaro <richard.zhao@linaro.org>
 * Copyright (C) 2011-2012 Mike Turquette, Linaro Ltd <mturquette@linaro.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/clk-provider.h>
#include <linux/err.h>
#include <linux/types.h>
#include <soc/tegra/tegra-dvfs.h>

#include "clk.h"

#define DIV_MASK GENMASK(7, 0)
#define MUX_SHIFT 29
#define MUX_MASK GENMASK(MUX_SHIFT + 2, MUX_SHIFT)

#define get_max_div(d) DIV_MASK
#define get_div_field(val) ((val) & DIV_MASK)
#define get_mux_field(val) (((val) & MUX_MASK) >> MUX_SHIFT)

static const char *mux_sdmmc_parents[] = { "pll_p", "pll_c4_out2",
					   "pll_c4_out0", "pll_c4_out1",
					   "clk_m" };
static u8 mux_lj_idx[] = { [0] = 0, [1] = 1, [2] = 2, [3] = 5, [4] = 6 };
static u8 mux_non_lj_idx[] = { [0] = 0, [1] = 3, [2] = 7, [3] = 4, [4] = 6 };

static u8 clk_sdmmc_mux_get_parent(struct clk_hw *hw)
{
	struct tegra_sdmmc_mux *sdmmc_mux = to_clk_sdmmc_mux(hw);
	int num_parents, i;
	u32 src, val;
	u8 *mux_idx;

	num_parents = clk_hw_get_num_parents(hw);

	val = readl_relaxed(sdmmc_mux->reg);
	src = get_mux_field(val);
	if (get_div_field(val))
		mux_idx = mux_non_lj_idx;
	else
		mux_idx = mux_lj_idx;

	for (i = 0; i < num_parents; i++) {
		if (mux_idx[i] == src)
			return i;
	}

	WARN(1, "Unknown parent selector %d\n", src);

	return 0;
}

static int clk_sdmmc_mux_set_parent(struct clk_hw *hw, u8 index)
{
	struct tegra_sdmmc_mux *sdmmc_mux = to_clk_sdmmc_mux(hw);
	u32 val;


	val = readl_relaxed(sdmmc_mux->reg);
	if (get_div_field(val))
		index = mux_non_lj_idx[index];
	else
		index = mux_lj_idx[index];

	val &= ~MUX_MASK;
	val |= index << MUX_SHIFT;

	writel(val, sdmmc_mux->reg);

	return 0;
}

static unsigned long clk_sdmmc_mux_recalc_rate(struct clk_hw *hw,
					       unsigned long parent_rate)
{
	struct tegra_sdmmc_mux *sdmmc_mux = to_clk_sdmmc_mux(hw);
	u32 val;
	int div, mul;
	u64 rate = parent_rate;

	val = readl_relaxed(sdmmc_mux->reg);
	div = get_div_field(val);

	mul = 2;
	div += mul;

	rate *= mul;
	rate += div - 1;
	do_div(rate, div);

	return rate;
}

static int clk_sdmmc_mux_determine_rate(struct clk_hw *hw,
					struct clk_rate_request *req)
{
	struct tegra_sdmmc_mux *sdmmc_mux = to_clk_sdmmc_mux(hw);
	int div, mul;
	unsigned long output_rate = req->best_parent_rate;

	req->rate = max(req->rate, req->min_rate);
	req->rate = min(req->rate, req->max_rate);

	if (!req->rate)
		return output_rate;

	div = div71_get(req->rate, output_rate, 8, 1, sdmmc_mux->div_flags);
	if (div < 0)
		div = 0;

	mul = 2;
	if (sdmmc_mux->div_flags & TEGRA_DIVIDER_ROUND_UP)
		req->rate =  DIV_ROUND_UP(output_rate * mul, div + mul);
	else
		req->rate =  output_rate * mul / (div + mul);

	return 0;
}

static int clk_sdmmc_mux_set_rate(struct clk_hw *hw, unsigned long rate,
				  unsigned long parent_rate)
{
	struct tegra_sdmmc_mux *sdmmc_mux = to_clk_sdmmc_mux(hw);
	int div;
	unsigned long flags = 0;
	u32 val;
	u8 src;

	div = div71_get(rate, parent_rate, 8, 1, sdmmc_mux->div_flags);
	if (div < 0)
		return div;

	if (sdmmc_mux->lock)
		spin_lock_irqsave(sdmmc_mux->lock, flags);

	src = clk_sdmmc_mux_get_parent(hw);
	if (div)
		src = mux_non_lj_idx[src];
	else
		src = mux_lj_idx[src];

	val = src << MUX_SHIFT;
	val |= div;
	writel(val, sdmmc_mux->reg);
	fence_udelay(2, sdmmc_mux->reg);

	if (sdmmc_mux->lock)
		spin_unlock_irqrestore(sdmmc_mux->lock, flags);

	return 0;
}

static int clk_sdmmc_mux_is_enabled(struct clk_hw *hw)
{
	struct tegra_sdmmc_mux *sdmmc_mux = to_clk_sdmmc_mux(hw);
	const struct clk_ops *gate_ops = sdmmc_mux->gate_ops;
	struct clk_hw *gate_hw = &sdmmc_mux->gate.hw;

	__clk_hw_set_clk(gate_hw, hw);

	return gate_ops->is_enabled(gate_hw);
}

static int clk_sdmmc_mux_enable(struct clk_hw *hw)
{
	struct tegra_sdmmc_mux *sdmmc_mux = to_clk_sdmmc_mux(hw);
	const struct clk_ops *gate_ops = sdmmc_mux->gate_ops;
	struct clk_hw *gate_hw = &sdmmc_mux->gate.hw;

	__clk_hw_set_clk(gate_hw, hw);

	return  gate_ops->enable(gate_hw);
}

static void clk_sdmmc_mux_disable(struct clk_hw *hw)
{
	struct tegra_sdmmc_mux *sdmmc_mux = to_clk_sdmmc_mux(hw);
	const struct clk_ops *gate_ops = sdmmc_mux->gate_ops;
	struct clk_hw *gate_hw = &sdmmc_mux->gate.hw;

	gate_ops->disable(gate_hw);
}

static int clk_sdmmc_mux_prepare(struct clk_hw *hw)
{
	return tegra_dvfs_set_rate(hw->clk, clk_hw_get_rate(hw));
}

static void clk_sdmmc_mux_unprepare(struct clk_hw *hw)
{
	tegra_dvfs_set_rate(hw->clk, 0);
}

const struct clk_ops tegra_clk_sdmmc_mux_ops = {
	.get_parent = clk_sdmmc_mux_get_parent,
	.set_parent = clk_sdmmc_mux_set_parent,
	.determine_rate = clk_sdmmc_mux_determine_rate,
	.recalc_rate = clk_sdmmc_mux_recalc_rate,
	.set_rate = clk_sdmmc_mux_set_rate,
	.is_enabled = clk_sdmmc_mux_is_enabled,
	.enable = clk_sdmmc_mux_enable,
	.disable = clk_sdmmc_mux_disable,
	.prepare = clk_sdmmc_mux_prepare,
	.unprepare = clk_sdmmc_mux_unprepare,
};

struct clk *tegra_clk_register_sdmmc_mux_div(const char *name,
	void __iomem *clk_base, u32 offset, u32 clk_num, u8 div_flags,
	unsigned long flags, void *lock)
{
	struct clk *clk;
	struct clk_init_data init;
	const struct tegra_clk_periph_regs *bank;
	struct tegra_sdmmc_mux *sdmmc_mux;

	init.ops = &tegra_clk_sdmmc_mux_ops;
	init.name = name;
	init.flags = flags;
	init.parent_names = mux_sdmmc_parents;
	init.num_parents = ARRAY_SIZE(mux_sdmmc_parents);

	bank = get_reg_bank(clk_num);
	if (!bank)
		return ERR_PTR(-EINVAL);

	sdmmc_mux = kzalloc(sizeof(*sdmmc_mux), GFP_KERNEL);
	if (!sdmmc_mux)
		return ERR_PTR(-ENOMEM);

	/* Data in .init is copied by clk_register(), so stack variable OK */
	sdmmc_mux->hw.init = &init;
	sdmmc_mux->reg = clk_base + offset;
	sdmmc_mux->lock = lock;
	sdmmc_mux->gate.clk_base = clk_base;
	sdmmc_mux->gate.regs = bank;
	sdmmc_mux->gate.enable_refcnt = periph_clk_enb_refcnt;
	sdmmc_mux->gate.clk_num = clk_num;
	sdmmc_mux->gate.flags = TEGRA_PERIPH_ON_APB;
	sdmmc_mux->div_flags = div_flags;
	sdmmc_mux->gate_ops = &tegra_clk_periph_gate_ops;

	clk = clk_register(NULL, &sdmmc_mux->hw);
	if (IS_ERR(clk))
		return clk;

	sdmmc_mux->gate.hw.clk = clk;

	return clk;
}

