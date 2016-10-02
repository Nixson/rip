//---------------------------------------------------------------------------

#ifndef CommandUnitH
#define CommandUnitH
//#define _WINSOCKAPI_

#define XTE_MAC_ADDR_SIZE		6
#define XTE_IP_ADDR_SIZE		4

#define XTE_ETH_HDR_SIZE		14
#define XTE_IP_HDR_SIZE		   20
#define XTE_UDP_HDR_SIZE		8
#define XTE_CMD_HDR_SIZE		10
#define XTE_IP_UDP_HDR_SIZE	(XTE_IP_HDR_SIZE + XTE_UDP_HDR_SIZE)
#define XTE_ETH_IP_UDP_HDR_SIZE	(XTE_ETH_HDR_SIZE + XTE_IP_UDP_HDR_SIZE)

//---------------------------------------------------------------------------
void CopyMAC_SRC();
void ARPBufPrepare(unsigned char *Buffer, unsigned int Type);
bool CheckForARPRequest(unsigned char *FramePtr);
bool CheckForARPAnswer(unsigned char *FramePtr);
bool CheckForAck(unsigned char *Buffer, unsigned int &PacketNum);
bool CheckForAnswer(unsigned char *Buffer, unsigned short &CmdNum, unsigned int &PacketNum);
void AckFrameInit(unsigned char *Buffer, unsigned int PacketNum);

void EthFrameSetCmdHeader(unsigned char *Buffer, unsigned short CmdNum, unsigned int &PacketNum);

void SendARPRequest();
void SendARPAnswer();

void SendBulkPacket(unsigned int &PacketNum);
void SendCmdPacket(unsigned short BufferSize, unsigned char *Buffer, unsigned short CmdNum, unsigned int &PacketNum);
#endif
