/*
 * Contact: Eugene Krasnikov <k.eugene.e@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#ifndef _DXE_H_
#define _DXE_H_

#include <linux/dma-mapping.h>
/*
TX_LOW	= DMA0
TX_HIGH	= DMA4
RX_LOW	= DMA1
RX_HIGH	= DMA3
*/

// DXE registers
#define WCN36XX_DXE_MEM_BASE            	0x03000000
#define WCN36XX_DXE_MEM_REG    			0x202000

#define WCN36XX_DXE_CCU_INT			0xA0011
#define WCN36XX_DXE_REG_CCU_INT			0x200b10

// TODO This must calculated properly but not hardcoded
#define WCN36XX_DXE_CTRL_TX_L			0x328a44
#define WCN36XX_DXE_CTRL_TX_H			0x32ce44
#define WCN36XX_DXE_CTRL_RX_L			0x12ad2f
#define WCN36XX_DXE_CTRL_RX_H			0x12d12f
#define WCN36XX_DXE_CTRL_TX_H_BD		0x32ce45
#define WCN36XX_DXE_CTRL_TX_H_SKB		0x32ce4d

// TODO This must calculated properly but not hardcoded
#define WCN36XX_DXE_WQ_TX_L			0x17
#define WCN36XX_DXE_WQ_TX_H			0x17
#define WCN36XX_DXE_WQ_RX_L			0xB
#define WCN36XX_DXE_WQ_RX_H			0x4

// TODO This must calculated properly but not hardcoded
// DXE default control register values
#define WCN36XX_DXE_CH_DEFAULT_CTL_RX_L		0x847EAD2F
#define WCN36XX_DXE_CH_DEFAULT_CTL_RX_H		0x84FED12F
#define WCN36XX_DXE_CH_DEFAULT_CTL_TX_H		0x853ECF4D
// Common DXE registers
#define WCN36XX_DXE_MEM_CSR          		WCN36XX_DXE_MEM_REG + 0x00
#define WCN36XX_DXE_REG_CH_EN            	WCN36XX_DXE_MEM_REG + 0x08
#define WCN36XX_DXE_INT_MASK_REG            	WCN36XX_DXE_MEM_REG + 0x18
#define WCN36XX_DXE_INT_SRC_RAW_REG            	WCN36XX_DXE_MEM_REG + 0x20

#define WCN36XX_DXE_REG_CSR_RESET            	WCN36XX_DXE_MEM_REG+0x00

#define WCN36XX_DXE_REG_RESET			0x5c89

// Temporary BMU Workqueue 4
#define WCN36XX_DXE_BMU_WQ_RX_LOW		0xB
#define WCN36XX_DXE_BMU_WQ_RX_HIGH		0x4
// DMA channel offset
#define WCN36XX_DXE_TX_LOW_OFFSET		0x400
#define WCN36XX_DXE_TX_HIGH_OFFSET		0x500
#define WCN36XX_DXE_RX_LOW_OFFSET		0x440
#define WCN36XX_DXE_RX_HIGH_OFFSET		0x4C0
// DXE Descriptor address where address of the next descriptor must be written
#define WCN36XX_DXE_CH_NEXT_DESC_ADDR		0x001C
#define WCN36XX_DXE_CH_NEXT_DESC_ADDR_TX_L	WCN36XX_DXE_MEM_REG + WCN36XX_DXE_TX_LOW_OFFSET + WCN36XX_DXE_CH_NEXT_DESC_ADDR	
#define WCN36XX_DXE_CH_NEXT_DESC_ADDR_TX_H	WCN36XX_DXE_MEM_REG + WCN36XX_DXE_TX_HIGH_OFFSET + WCN36XX_DXE_CH_NEXT_DESC_ADDR
#define WCN36XX_DXE_CH_NEXT_DESC_ADDR_RX_L	WCN36XX_DXE_MEM_REG + WCN36XX_DXE_RX_LOW_OFFSET + WCN36XX_DXE_CH_NEXT_DESC_ADDR
#define WCN36XX_DXE_CH_NEXT_DESC_ADDR_RX_H	WCN36XX_DXE_MEM_REG + WCN36XX_DXE_RX_HIGH_OFFSET + WCN36XX_DXE_CH_NEXT_DESC_ADDR

// DXE Descriptor source address
#define WCN36XX_DXE_CH_SRC_ADDR			0x000C
#define WCN36XX_DXE_CH_SRC_ADDR_RX_L		WCN36XX_DXE_MEM_REG + WCN36XX_DXE_RX_LOW_OFFSET + WCN36XX_DXE_CH_SRC_ADDR
#define WCN36XX_DXE_CH_SRC_ADDR_RX_H		WCN36XX_DXE_MEM_REG + WCN36XX_DXE_RX_HIGH_OFFSET + WCN36XX_DXE_CH_SRC_ADDR

// DXE Descriptor address where destination address must be written
#define WCN36XX_DXE_CH_DEST_ADDR		0x0014
#define WCN36XX_DXE_CH_DEST_ADDR_TX_L		WCN36XX_DXE_MEM_REG + WCN36XX_DXE_TX_LOW_OFFSET + WCN36XX_DXE_CH_DEST_ADDR
#define WCN36XX_DXE_CH_DEST_ADDR_TX_H		WCN36XX_DXE_MEM_REG + WCN36XX_DXE_TX_HIGH_OFFSET + WCN36XX_DXE_CH_DEST_ADDR
#define WCN36XX_DXE_CH_DEST_ADDR_RX_L		WCN36XX_DXE_MEM_REG + WCN36XX_DXE_RX_LOW_OFFSET + WCN36XX_DXE_CH_DEST_ADDR
#define WCN36XX_DXE_CH_DEST_ADDR_RX_H		WCN36XX_DXE_MEM_REG + WCN36XX_DXE_RX_HIGH_OFFSET + WCN36XX_DXE_CH_DEST_ADDR

// DXE default control register
#define WCN36XX_DXE_REG_CTL_RX_L		WCN36XX_DXE_MEM_REG + WCN36XX_DXE_RX_LOW_OFFSET
#define WCN36XX_DXE_REG_CTL_RX_H		WCN36XX_DXE_MEM_REG + WCN36XX_DXE_RX_HIGH_OFFSET
#define WCN36XX_DXE_REG_CTL_TX_H		WCN36XX_DXE_MEM_REG + WCN36XX_DXE_TX_HIGH_OFFSET

#define WCN36XX_SMSM_WLAN_TX_ENABLE 		0x00000400
#define WCN36XX_SMSM_WLAN_TX_RINGS_EMPTY	0x00000200


/* Interrupt control channel mask */
#define WCN36XX_INT_MASK_CHAN_TX_L		0x00000001
#define WCN36XX_INT_MASK_CHAN_RX_L		0x00000002
#define WCN36XX_INT_MASK_CHAN_RX_H		0x00000008
#define WCN36XX_INT_MASK_CHAN_TX_H		0x00000010

#define WCN36XX_BD_CHUNK_SIZE 			128

#define WCN36XX_PKT_SIZE			0xF20
enum wcn36xx_dxe_ch_type {
	WCN36XX_DXE_CH_TX_L,
	WCN36XX_DXE_CH_TX_H,
	WCN36XX_DXE_CH_RX_L,
	WCN36XX_DXE_CH_RX_H
};

// amount of descriptors per channel
enum wcn36xx_dxe_ch_desc_num {
	WCN36XX_DXE_CH_DESC_NUMB_TX_L		= 128,
	WCN36XX_DXE_CH_DESC_NUMB_TX_H		= 10,
	WCN36XX_DXE_CH_DESC_NUMB_RX_L		= 512,
	WCN36XX_DXE_CH_DESC_NUMB_RX_H		= 40
};


// Packets
struct wcn36xx_pkt {
	void 	*bd;
	void 	*bd_phy;
	void 	*os_struct;
	void 	*pkt_info;
	int	pkt_type;
	u16	len;
	void	*int_data;

};
// DXE descriptor data type
struct wcn36xx_dxe_desc_data
{
	int                      src_addr_l;
	int                      dst_addr_l;
	int                      phy_next_l;
	int                      src_addr_h;
	int                      dst_addr_h;
	int                      phy_next_h;
};


struct wcn36xx_dxe_desc
{
   union
   {
      int                   ctrl;
      int                   valid		:1;     //0 = DMA stop, 1 = DMA continue with this descriptor
      int                   transfer_type	:2;     //0 = Host to Host space
      int                   eop			:1;     //End of Packet
      int                   bd_handling		:1;          //if transferType = Host to BMU, then 0 means first 128 bytes contain BD, and 1 means create new empty BD
      int                   siq			:1;     // SIQ
      int                   diq			:1;     // DIQ
      int                   pdu_rel		:1;     //0 = don't release BD and PDUs when done, 1 = release them
      int                   bthld_sel		:4;     //BMU Threshold Select
      int                   prio		:3;     //Specifies the priority level to use for the transfer
      int                   stop_channel	:1;     //1 = DMA stops processing further, channel requires re-enabling after this
      int                   intr		:1;     //Interrupt on Descriptor Done
      int                   rsvd		:1;     //reserved
      int                   size		:14;    //14 bits used - ignored for BMU transfers, only used for host to host transfers?
   } desc_ctl;
   int                      fr_len;
   struct wcn36xx_dxe_desc_data desc;
};

// DXE Control block
struct wcn36xx_dxe_ctl {
   void                            	*next;
   struct wcn36xx_pkt                   *frame;
   struct wcn36xx_dxe_desc		*desc;
   unsigned int				desc_phy_addr;
   int                       		ctl_blk_order;
   struct sk_buff 			*skb;
};

struct wcn36xx_dxe_mem_info {
	unsigned long len;
	//The offset from beginning of the buffer where it is allocated
	unsigned long offset;
	unsigned long phy_addr;
};


struct wcn36xx_dxe_ch {
	enum wcn36xx_dxe_ch_type  	ch_type;
	void 				*descs_cpu_addr;
	dma_addr_t 			descs_dma_addr;
	enum wcn36xx_dxe_ch_desc_num	desc_num;
	// DXE control block ring
	struct wcn36xx_dxe_ctl		*head_blk_ctl;
	struct wcn36xx_dxe_ctl		*tail_blk_ctl;
	uint				num_free_desc;
};

// Memory Pool for BD headers
struct wcn36xx_dxe_mem_pool {
	int 	chunk_size;		// size of every chunk
	void	*virt_addr;		// virtual address that is visible to CPU
	void	*phy_addr;		// physical address that
	void	*bitmap;		// bitmap array for all headers
};
struct wcn36xx;
int wcn36xx_dxe_allocate_mem_pools(struct wcn36xx *wcn);
void wcn36xx_rx_ready_work(struct work_struct *work);
int wcn36xx_dxe_alloc_ctl_blks(struct wcn36xx *wcn);
int wcn36xx_dxe_init(struct wcn36xx *wcn);
int wcn36xx_dxe_init_channels(struct wcn36xx *wcn);
int wcn36xx_dxe_request_irqs(struct wcn36xx *wcn);
int wcn36xx_dxe_tx(struct wcn36xx *wcn, struct sk_buff *skb, u8 broadcast);
#endif	/* _DXE_H_ */
