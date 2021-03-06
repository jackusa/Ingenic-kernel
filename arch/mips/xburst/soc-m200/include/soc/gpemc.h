/*
 *  Copyright (C) 2013 Fighter Sun <wanmyqawdr@126.com>
 *  GPEMC(NEMC) support functions
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under  the terms of the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef __SOC_GPEMC_H__
#define __SOC_GPEMC_H__

typedef enum {
	BANK_TYPE_SRAM = 0,
	BANK_TYPE_NAND,
	BANK_TYPE_TOGGLE,

	CNT_BANK_TYPES
} bank_type_t;

typedef enum {
	BUS_WIDTH_8 = 8
} bus_width_t;

typedef enum  {
	BURST_LENGTH_4 = 4,
	BURST_LENGTH_8 = 8,
	BURST_LENGTH_16 = 16,
	BURST_LENGTH_32 = 32
} burst_length_t;

typedef enum {
	SRAM_TYPE_NORMAL = 0,
	SRAM_TYPE_BURST,

	CNT_SRAM_TYPES
} sram_type_t;

typedef struct {
	u64 clk_T_real_ps;
	bus_width_t BW;

	struct {

		/*
		 * got from datasheet of SoC-jz4780,
		 * there are obvious errors on timing diagram
		 *
		 * CS/ADDR/DATA(write)
		 * ______<-------    Tah+Tas+Taw<+2>    ------> ________
		 *       |_____________________________________|
		 *       +                                     +
		 *       |         +             +             |
		 *       |<--Tas-->|             |             |
		 *       +         |             |<--- Tah --->|
		 * WE/RD           |<--- Taw --->|             +
		 * ________________+             +______________________
		 *                 |_____________|
		 * DATA(read)
		 * _________________________         ___________________
		 *                          |_______|
		 *
		 */

		/* every timing parameter count in nanoseconds */
		u32 Tstrv;
		u32 Taw;
		u32 Tbp;
		u32 Tah;
		u32 Tas;

		/* access attributes */
		burst_length_t BL;
		sram_type_t sram_type;
	} sram_timing;

	/*
	 * TODO
	 */
	struct {
		u32 Trv;
		u32 Trw;
		u32 Tww;
		u32 Tah;
		u32 Tas;
		u32 Tdpht;
		u32 Tdqsre;
		u32 Tfda;
		u32 Tclr;
		u32 Tdphtd;
		u32 Tcdqss;
		u32 Tcwaw;
	} toggle_timing;
} gpemc_bank_timing_t;

typedef struct {
	/*
	 * CLE Setup Time
	 */
	u32 Tcls;

	/*
	 * CLE Hold Time
	 */
	u32 Tclh;

	/*
	 * ALE Setup Time
	 */
	u32 Tals;

	/*
	 * ALE Hold Time
	 */
	u32 Talh;

	/*
	 * #CE Hold Time
	 */
	u32 Tch;

	/*
	 * Data Setup Time
	 */
	u32 Tds;

	/*
	 * Data Hold Time
	 */
	u32 Tdh;

	/*
	 * #WE Pulse Width
	 */
	u32 Twp;

	/*
	 * #WE High Hold Time
	 */
	u32 Twh;

	/*
	 * Write Cycle Time
	 */
	u32 Twc;

	/*
	 * Read Cycle Time
	 */
	u32 Trc;

	/*
	 * #RE High to #WE Low
	 */
	u32 Trhw;

	/*
	 * #RE Pulse Width
	 */
	u32 Trp;

	struct {
		/*
		 * #CE Setup Time
		 */
		u32 Tcs;

		/*
		 * #WE High to #RE Low
		 */
		u32 Twhr;

		/*
		 * address to data loading delay for sequence in
		 */
		u32 Tadl;

		/*
		 * Change column setup time to data in/out or next command
		 * often used in Micron NAND chips
		 */
		u32 Tccs;

		/*
		 * Ready to #RE low
		 */
		u32 Trr;

		/*
		 * Command Write cycle to Address Write
		 * cycle Time for Random data input
		 */
		u32 Tcwaw;

		/*
		 * #WE high to Busy
		 */
		u32 Twb;

		/*
		 * #WP High/Low to #WE Low
		 */
		u32 Tww;

		/*
		 * Device Resetting Time(Read/Program/Erase)
		 */
		u32 Trst;

		/*
		 * Busy time for Set Feature and Get Feature
		 */
		u32 Tfeat;

		/*
		 * Cache Busy in Read Cache (following 31h and 3Fh)
		 */
		u32 Tdcbsyr;

		/*
		 * Dummy Busy Time for Intelligent Copy-Back Read
		 */
		u32 Tdcbsyr2;

		/*
		 * #WE High to #RE Low for Random data out
		 */
		u32 Twhr2;
	} busy_wait_timing;

	bus_width_t BW;

	int32_t all_timings_plus;
} common_nand_timing_t;

typedef struct {
	/*
	 * TODO
	 */
} toggle_nand_timing_t;

typedef struct {
	/*
	 * TODO
	 */
} sram_timing_t;

typedef struct {
	int cs;
	bank_type_t bank_type;
	gpemc_bank_timing_t bank_timing;

	struct device* dev;

	unsigned int cnt_addr_pins;
	void __iomem *io_base;

	void __iomem *io_nand_dat;
	void __iomem *io_nand_addr;
	void __iomem *io_nand_cmd;
} gpemc_bank_t;

extern int gpemc_request_cs(struct device *dev, gpemc_bank_t *bank, int cs);
extern void gpemc_release_cs(gpemc_bank_t *bank);

extern void gpemc_relax_bank_timing(gpemc_bank_t *bank);
extern int gpemc_config_bank_timing(gpemc_bank_t *bank);
extern int gpemc_config_toggle_bank_timing(gpemc_bank_t *bank);

extern void gpemc_set_bank_as_common_nand(gpemc_bank_t *bank);
extern void gpemc_set_bank_as_toggle_nand(gpemc_bank_t *bank);

extern void gpemc_enable_nand_flash(gpemc_bank_t *bank, bool enable);

extern void gpemc_fill_timing_from_nand(gpemc_bank_t *bank, common_nand_timing_t *timing);
extern void gpemc_fill_timing_from_toggle(gpemc_bank_t *bank, toggle_nand_timing_t *timing);
extern void gpemc_fill_timing_from_sram(gpemc_bank_t *bank, sram_timing_t *timing);

#endif
