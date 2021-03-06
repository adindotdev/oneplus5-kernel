/*
 * Copyright (c) 2016-2017 The Linux Foundation. All rights reserved.
 *
 * Previously licensed under the ISC license by Qualcomm Atheros, Inc.
 *
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * This file was originally distributed by Qualcomm Atheros, Inc.
 * under proprietary terms before Copyright ownership was assigned
 * to the Linux Foundation.
 */

#ifndef _MULTIBUS_H_
#define _MULTIBUS_H_

#include "osdep.h"
#include "qdf_status.h"
#include "hif_debug.h"

struct hif_softc;

struct hif_bus_ops {
	QDF_STATUS (*hif_bus_open)(struct hif_softc *hif_sc,
				   enum qdf_bus_type bus_type);
	void (*hif_bus_close)(struct hif_softc *hif_sc);
	void (*hif_bus_prevent_linkdown)(struct hif_softc *hif_sc, bool flag);
	void (*hif_reset_soc)(struct hif_softc *hif_sc);
	int (*hif_bus_early_suspend)(struct hif_softc *hif_ctx);
	int (*hif_bus_late_resume)(struct hif_softc *hif_ctx);
	int (*hif_bus_suspend)(struct hif_softc *hif_ctx);
	int (*hif_bus_resume)(struct hif_softc *hif_ctx);
	int (*hif_bus_suspend_noirq)(struct hif_softc *hif_ctx);
	int (*hif_bus_resume_noirq)(struct hif_softc *hif_ctx);
	int (*hif_target_sleep_state_adjust)(struct hif_softc *scn,
			bool sleep_ok, bool wait_for_it);
	void (*hif_disable_isr)(struct hif_softc *hif_sc);
	void (*hif_nointrs)(struct hif_softc *hif_sc);
	QDF_STATUS (*hif_enable_bus)(struct hif_softc *hif_sc,
			struct device *dev, void *bdev, const hif_bus_id *bid,
			enum hif_enable_type type);
	void (*hif_disable_bus)(struct hif_softc *hif_sc);
	int (*hif_bus_configure)(struct hif_softc *hif_sc);
	QDF_STATUS (*hif_get_config_item)(struct hif_softc *hif_sc,
		     int opcode, void *config, uint32_t config_len);
	void (*hif_set_mailbox_swap)(struct hif_softc *hif_sc);
	void (*hif_claim_device)(struct hif_softc *hif_sc);
	void (*hif_shutdown_device)(struct hif_softc *hif_sc);
	void (*hif_stop)(struct hif_softc *hif_sc);
	void (*hif_cancel_deferred_target_sleep)(struct hif_softc *hif_sc);
	void (*hif_irq_disable)(struct hif_softc *hif_sc, int ce_id);
	void (*hif_irq_enable)(struct hif_softc *hif_sc, int ce_id);
	int (*hif_dump_registers)(struct hif_softc *hif_sc);
	void (*hif_dump_target_memory)(struct hif_softc *hif_sc,
				       void *ramdump_base,
				       uint32_t address, uint32_t size);
	void (*hif_ipa_get_ce_resource)(struct hif_softc *hif_sc,
					qdf_dma_addr_t *sr_base_paddr,
					uint32_t *sr_ring_size,
					qdf_dma_addr_t *reg_paddr);
	void (*hif_mask_interrupt_call)(struct hif_softc *hif_sc);
	void (*hif_enable_power_management)(struct hif_softc *hif_ctx,
				 bool is_packet_log_enabled);
	void (*hif_disable_power_management)(struct hif_softc *hif_ctx);
	void (*hif_display_stats)(struct hif_softc *hif_ctx);
	void (*hif_clear_stats)(struct hif_softc *hif_ctx);
	void (*hif_set_bundle_mode) (struct hif_softc *hif_ctx, bool enabled,
					int rx_bundle_cnt);
	int (*hif_bus_reset_resume)(struct hif_softc *hif_ctx);
};

#ifdef HIF_SNOC
QDF_STATUS hif_initialize_snoc_ops(struct hif_bus_ops *hif_sc);
int hif_snoc_get_context_size(void);
#else
static inline QDF_STATUS hif_initialize_snoc_ops(struct hif_bus_ops *hif_sc)
{
	HIF_ERROR("%s: not supported", __func__);
	return QDF_STATUS_E_NOSUPPORT;
}
/**
 * hif_snoc_get_context_size() - dummy when snoc isn't supported
 *
 * Return: 0 as an invalid size to indicate no support
 */
static inline int hif_snoc_get_context_size(void)
{
	return 0;
}
#endif /* HIF_SNOC */

#ifdef HIF_PCI
QDF_STATUS hif_initialize_pci_ops(struct hif_softc *hif_sc);
int hif_pci_get_context_size(void);
#else
static inline QDF_STATUS hif_initialize_pci_ops(struct hif_softc *hif_sc)
{
	HIF_ERROR("%s: not supported", __func__);
	return QDF_STATUS_E_NOSUPPORT;
}
/**
 * hif_pci_get_context_size() - dummy when pci isn't supported
 *
 * Return: 0 as an invalid size to indicate no support
 */
static inline int hif_pci_get_context_size(void)
{
	return 0;
}
#endif /* HIF_PCI */

#ifdef HIF_AHB
QDF_STATUS hif_initialize_ahb_ops(struct hif_bus_ops *bus_ops);
int hif_ahb_get_context_size(void);
#else
/**
 * hif_initialize_ahb_ops() - dummy for when ahb not supported
 *
 * Return: QDF_STATUS_E_NOSUPPORT
 */
static inline QDF_STATUS hif_initialize_ahb_ops(struct hif_bus_ops *bus_ops)
{
	HIF_ERROR("%s: not supported", __func__);
	return QDF_STATUS_E_NOSUPPORT;
}

/**
 * hif_ahb_get_context_size() - dummy for when ahb not supported
 *
 * Return: 0 as an invalid size to indicate no support
 */
static inline int hif_ahb_get_context_size(void)
{
	return 0;
}
#endif

#ifdef HIF_SDIO
QDF_STATUS hif_initialize_sdio_ops(struct hif_softc *hif_sc);
int hif_sdio_get_context_size(void);
#else
/**
 * hif_initialize_sdio_ops() - dummy for when sdio not supported
 *
 * Return: QDF_STATUS_E_NOSUPPORT
 */

static inline QDF_STATUS hif_initialize_sdio_ops(struct hif_softc *hif_sc)
{
	HIF_ERROR("%s: not supported", __func__);
	return QDF_STATUS_E_NOSUPPORT;
}

/**
 * hif_sdio_get_context_size() - dummy when sdio isn't supported
 *
 * Return: 0 as an invalid size to indicate no support
 */
static inline int hif_sdio_get_context_size(void)
{
	return 0;
}
#endif /* HIF_SDIO */

#ifdef HIF_USB
QDF_STATUS hif_initialize_usb_ops(struct hif_bus_ops *bus_ops);
int hif_usb_get_context_size(void);
#else
static inline QDF_STATUS hif_initialize_usb_ops(struct hif_bus_ops *bus_ops)
{
	HIF_ERROR("%s: not supported", __func__);
	return QDF_STATUS_E_NOSUPPORT;
}
/**
 * hif_usb_get_context_size() - dummy when usb isn't supported
 *
 * Return: 0 as an invalid size to indicate no support
 */
static inline int hif_usb_get_context_size(void)
{
	return 0;
}
#endif /* HIF_USB */
#endif /* _MULTIBUS_H_ */
