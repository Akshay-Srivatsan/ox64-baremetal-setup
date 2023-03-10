#pragma once
#include <openssl/sha.h>
#include <string.h>
#include <zlib.h>

#include "boot-header.h"
#include "segment-header.h"

static struct boot_header default_boot_header(void) {
  return (struct boot_header){
      .magic_code = 0x504e4642,
      .revision = 0x1,
      .flashcfg_magic_code = 0x0,
      .io_mode = 0x0,
      .cont_read_support = 0x0,
      .sfctrl_clk_delay = 0x0,
      .sfctrl_clk_invert = 0x0,
      .reset_en_cmd = 0x0,
      .reset_cmd = 0x0,
      .exit_contread_cmd = 0x0,
      .exit_contread_cmd_size = 0x0,
      .jedecid_cmd = 0x0,
      .jedecid_cmd_dmy_clk = 0x0,
      .enter_32bits_addr_cmd = 0x0,
      .exit_32bits_addr_clk = 0x0,
      .sector_size = 0x0,
      .mfg_id = 0x0,
      .page_size = 0x0,
      .chip_erase_cmd = 0x0,
      .sector_erase_cmd = 0x0,
      .blk32k_erase_cmd = 0x0,
      .blk64k_erase_cmd = 0x0,
      .write_enable_cmd = 0x0,
      .page_prog_cmd = 0x0,
      .qpage_prog_cmd = 0x0,
      .qual_page_prog_addr_mode = 0x0,
      .fast_read_cmd = 0x0,
      .fast_read_dmy_clk = 0x0,
      .qpi_fast_read_cmd = 0x0,
      .qpi_fast_read_dmy_clk = 0x0,
      .fast_read_do_cmd = 0x0,
      .fast_read_do_dmy_clk = 0x0,
      .fast_read_dio_cmd = 0x0,
      .fast_read_dio_dmy_clk = 0x0,
      .fast_read_qo_cmd = 0x0,
      .fast_read_qo_dmy_clk = 0x0,
      .fast_read_qio_cmd = 0x0,
      .fast_read_qio_dmy_clk = 0x0,
      .qpi_fast_read_qio_cmd = 0x0,
      .qpi_fast_read_qio_dmy_clk = 0x0,
      .qpi_page_prog_cmd = 0x0,
      .write_vreg_enable_cmd = 0x0,
      .wel_reg_index = 0x0,
      .qe_reg_index = 0x0,
      .busy_reg_index = 0x0,
      .wel_bit_pos = 0x0,
      .qe_bit_pos = 0x0,
      .busy_bit_pos = 0x0,
      .wel_reg_write_len = 0x0,
      .wel_reg_read_len = 0x0,
      .qe_reg_write_len = 0x0,
      .qe_reg_read_len = 0x0,
      .release_power_down = 0x0,
      .busy_reg_read_len = 0x0,
      .reg_read_cmd0 = 0x0,
      .reg_read_cmd1 = 0x0,
      .reg_write_cmd0 = 0x0,
      .reg_write_cmd1 = 0x0,
      .enter_qpi_cmd = 0x0,
      .exit_qpi_cmd = 0x0,
      .cont_read_code = 0x0,
      .cont_read_exit_code = 0x0,
      .burst_wrap_cmd = 0x0,
      .burst_wrap_dmy_clk = 0x0,
      .burst_wrap_data_mode = 0x0,
      .burst_wrap_code = 0x0,
      .de_burst_wrap_cmd = 0x0,
      .de_burst_wrap_cmd_dmy_clk = 0x0,
      .de_burst_wrap_code_mode = 0x0,
      .de_burst_wrap_code = 0x0,
      .sector_erase_time = 0x0,
      .blk32k_erase_time = 0x0,
      .blk64k_erase_time = 0x0,
      .page_prog_time = 0x0,
      .chip_erase_time = 0x0,
      .power_down_delay = 0x0,
      .qe_data = 0x0,
      .flashcfg_crc32 = 0x74ccea76,
      .clkcfg_magic_code = 0x0,
      .xtal_type = 0x0,
      .mcu_clk = 0x0,
      .mcu_clk_div = 0x0,
      .mcu_bclk_div = 0x0,
      .mcu_pbclk_div = 0x0,
      .lp_div = 0x0,
      .dsp_clk = 0x0,
      .dsp_clk_div = 0x0,
      .dsp_bclk_div = 0x0,
      .dsp_pbclk = 0x0,
      .dsp_pbclk_div = 0x0,
      .emi_clk = 0x0,
      .emi_clk_div = 0x0,
      .flash_clk_type = 0x0,
      .flash_clk_div = 0x0,
      .wifipll_pu = 0x0,
      .aupll_pu = 0x0,
      .cpupll_pu = 0x0,
      .mipipll_pu = 0x0,
      .uhspll_pu = 0x0,
      .clkcfg_crc32 = 0xfd59b8d,
      .sign = 0x0,
      .encrypt_type = 0x0,
      .key_sel = 0x0,
      .xts_mode = 0x0,
      .aes_region_lock = 0x0,
      .no_segment = 0x1,
      .boot2_enable = 0x0,
      .boot2_rollback = 0x0,
      .cpu_master_id = 0x0,
      .notload_in_bootrom = 0x0,
      .crc_ignore = 0x0,
      .hash_ignore = 0x0,
      .power_on_mm = 0x1,
      .em_sel = 0x1,
      .cmds_en = 0x1,
      .cmds_wrap_mode = 0x2,
      .cmds_wrap_len = 0x2,
      .icache_invalid = 0x1,
      .dcache_invalid = 0x1,
      .fpga_halt_release = 0x0,
      .group_image_offset = 0x0,
      .aes_region_len = 0x0,
      .img_len_cnt = 0x1,
      .hash_0 = 0,
      .hash_1 = 0,
      .hash_2 = 0,
      .hash_3 = 0,
      .hash_4 = 0,
      .hash_5 = 0,
      .hash_6 = 0,
      .hash_7 = 0,
      .m0_config_enable = 0x1,
      .m0_halt_cpu = 0x0,
      .m0_cache_enable = 0x0,
      .m0_cache_wa = 0x0,
      .m0_cache_wb = 0x0,
      .m0_cache_wt = 0x0,
      .m0_cache_way_dis = 0x0,
      .m0_reserved = 0x0,
      .m0_cache_range_h = 0x0,
      .m0_cache_range_l = 0x0,
      .m0_image_address_offset = 0x0,
      .m0_boot_entry = 0x22020000,
      .m0_msp_val = 0x0,
      .d0_config_enable = 0x0,
      .d0_halt_cpu = 0x0,
      .d0_cache_enable = 0x0,
      .d0_cache_wa = 0x0,
      .d0_cache_wb = 0x0,
      .d0_cache_wt = 0x0,
      .d0_cache_way_dis = 0x0,
      .d0_reserved = 0x0,
      .d0_cache_range_h = 0x0,
      .d0_cache_range_l = 0x0,
      .d0_image_address_offset = 0x0,
      .d0_boot_entry = 0x0,
      .d0_msp_val = 0x0,
      .lp_config_enable = 0x0,
      .lp_halt_cpu = 0x0,
      .lp_cache_enable = 0x0,
      .lp_cache_wa = 0x0,
      .lp_cache_wb = 0x0,
      .lp_cache_wt = 0x0,
      .lp_cache_way_dis = 0x0,
      .lp_reserved = 0x0,
      .lp_cache_range_h = 0x0,
      .lp_cache_range_l = 0x0,
      .lp_image_address_offset = 0x0,
      .lp_boot_entry = 0x0,
      .lp_msp_val = 0x0,
      .boot2_pt_table_0 = 0x0,
      .boot2_pt_table_1 = 0x0,
      .flashCfgTableAddr = 0x0,
      .flashCfgTableLen = 0x0,
      .patch_read_addr0 = 0x0,
      .patch_read_value0 = 0x0,
      .patch_read_addr1 = 0x0,
      .patch_read_value1 = 0x0,
      .patch_read_addr2 = 0x0,
      .patch_read_value2 = 0x0,
      .patch_read_addr3 = 0x0,
      .patch_read_value3 = 0x0,
      .patch_jump_addr0 = 0x0,
      .patch_jump_value0 = 0x0,
      .patch_jump_addr1 = 0x0,
      .patch_jump_value1 = 0x0,
      .patch_jump_addr2 = 0x0,
      .patch_jump_value2 = 0x0,
      .patch_jump_addr3 = 0x0,
      .patch_jump_value3 = 0x0,
      .reserved1 = 0x0,
      .reserved2 = 0x0,
      .reserved3 = 0x0,
      .reserved4 = 0x0,
      .reserved5 = 0x0,
      .crc32 = 0,
  };
}

static void update_header_crcs(struct boot_header *header) {
  void *flash_start = &header->flashcfg_magic_code + 1;
  void *flash_end = &header->flashcfg_crc32;
  header->flashcfg_crc32 =
      crc32(0, flash_start, (size_t)flash_end - (size_t)flash_start);
  void *clk_start = &header->clkcfg_magic_code + 1;
  void *clk_end = &header->clkcfg_crc32;
  header->clkcfg_crc32 =
      crc32(0, clk_start, (size_t)clk_end - (size_t)clk_start);
  void *start = header;
  void *end = &header->crc32;
  header->crc32 = crc32(0, start, (size_t)end - (size_t)start);
}

static void update_header_hash(struct boot_header *header,
                               struct segment_header *segment_header,
                               const unsigned char *data, size_t n_bytes) {
  size_t size = n_bytes + sizeof(struct segment_header);
  unsigned char *buf = malloc(size);
  memcpy(buf, segment_header, sizeof(struct segment_header));
  memcpy(buf + sizeof(struct segment_header), data, n_bytes);
  SHA256(buf, size, (unsigned char *)&header->hash_0);
  free(buf);
}

struct segment_header make_segment_header(uint32_t addr, uint32_t len) {
  struct segment_header segment_header = {
      .destaddr = addr,
      .len = len,
      .rsvd = 0,
      .crc32 = 0,
  };
  void *start = &segment_header;
  void *end = &segment_header.crc32;
  segment_header.crc32 = crc32(0, start, (size_t)end - (size_t)start);
  return segment_header;
}
