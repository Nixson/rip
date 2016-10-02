//---------------------------------------------------------------------------

//#include "params.h"
#include "MainUnit.h"
#include "CommandUnit.h"
//#include "Hardware.h"
//#include <IniFiles.hpp>

unsigned char MAC_SRC[6]   = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char MAC_DEST[6]  = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char IP_SRC[4]    = {192, 168, 114, 100}; // IP-адрес наш
unsigned char IP_DEST[4]   = {192, 168, 114, 80}; // IP-адрес платы
unsigned char CMDPort_SRC[2]     = {0x33, 0x33};
unsigned char DATAPort_SRC[2]    = {0x77, 0x77};
unsigned char CMDPort_DEST[2]    = {0x33, 0x33};
unsigned char DATAPort_DEST[2]   = {0x77, 0x77};

//unsigned int EthPayloadSize;
//bool ARP_ready = 0;

/*************************************************************************/
/************************* FUNCTIONS PROTOTYPES **************************/
/*************************************************************************/
void SendAck(unsigned int PacketNum);
/*************************************************************************/
/********************************* ETHERNET *******************************/
/*************************************************************************/
void CopyMAC_SRC()// подгружаем из Hardware наш адрес
{
   //for(char i=0;i<XTE_MAC_ADDR_SIZE;i++) MAC_SRC[i] = ((char *)(&(Hardware->Address)))[i];
}

unsigned short checksum(unsigned short *buffer, unsigned char size)
{
    unsigned int cksum=0;
    while (size > 1)
    {
        cksum += *buffer++;
        size  -= sizeof(unsigned short);
    }
    if (size)
    {
        cksum += *(unsigned char *)buffer;
    }
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);
    return (unsigned short)(~cksum);
}

void EthFrameSetIPHeader(unsigned char *FramePtr, unsigned char *PortDest, unsigned short PayloadSize)
{
   unsigned char *Buffer = (unsigned char *) (FramePtr);

   // начало MAC заголовока(14)
   for(unsigned char i=0;i<XTE_MAC_ADDR_SIZE;i++) Buffer[i] = MAC_DEST[i];
   for(unsigned char i=0;i<XTE_MAC_ADDR_SIZE;i++) Buffer[i+6] = MAC_SRC[i];
   Buffer[12] = 0x08; Buffer[13] = 0x00; // protocol type IP
   // конец MAC заголовока(14)
   
   // начало IPv4(20)
   Buffer[14] = 0x45;  // IPv4, заголовок 5x4=20 байт
   Buffer[15] = 0x00;  // 00
   *((unsigned short *)(Buffer+16)) = htons(PayloadSize+XTE_IP_UDP_HDR_SIZE);// длина IP пакета в байтах (16 бит) = полезная нагрузка + длина UDP заголовка(8) + длина IPv4 заголовка(20)
   Buffer[18] = 0x00;  // идентификатор (16 бит)
   Buffer[19] = 0x00;  // идентификатор (16 бит)
   Buffer[20] = 0x00;  // флаги
   Buffer[21] = 0x00;  // смещение
   Buffer[22] = 0x40;  // TTL
   Buffer[23] = 0x11;  // протоколол UDP
   Buffer[24] = 0x00;  // контрольная сумма IP заголовка (16 бит) вычисляется ниже!!!
   Buffer[25] = 0x00;  // контрольная сумма IP заголовка (16 бит) вычисляется ниже!!!
   for(unsigned char i=0;i<XTE_IP_ADDR_SIZE;i++) Buffer[i+26] = IP_SRC[i];  // IP  адрес отправителя
   for(unsigned char i=0;i<XTE_IP_ADDR_SIZE;i++) Buffer[i+30] = IP_DEST[i]; // IP  адрес получателя
   *((unsigned short *) (Buffer+24)) = (checksum((unsigned short *)(Buffer+XTE_ETH_HDR_SIZE), 20));//  ^^ вставляется выше ^^
   // конец IPv4(20)

   // начало UDP(8)
   Buffer[34] = CMDPort_SRC[0];
   Buffer[35] = CMDPort_SRC[1];   // source UDP port
   Buffer[36] = PortDest[0];
   Buffer[37] = PortDest[1];      // destination UDP port
   *((unsigned short *) (Buffer+38)) = htons(PayloadSize+XTE_UDP_HDR_SIZE); // длина UDP = длина IP - 20 (16 бит)
   Buffer[40] = 0x00;
   Buffer[41] = 0x00; // контрольная сумма UDP - не используется (16 бит)
   //конец UDP(8)
}

void EthFrameSetCmdHeader(unsigned char *Buffer, unsigned short CmdNum, unsigned int &PacketNum)
{
   unsigned char *DataPtr;
   DataPtr = (Buffer + XTE_ETH_IP_UDP_HDR_SIZE);

   *(unsigned int   *)(DataPtr) = 0xABCDDCBA;
   DataPtr += sizeof(unsigned int);
	*(unsigned short *)(DataPtr) = CmdNum;
   DataPtr += sizeof(unsigned short);
   *(unsigned int   *)(DataPtr) = PacketNum;
   DataPtr += sizeof(unsigned int);

   PacketNum++;
}
/*************************************************************************/
/********************************* ARP ***********************************/
/*************************************************************************/
void ARPBufPrepare(unsigned char *Buffer, unsigned int Type)
{
   switch(Type)
   {
      case 0://req
      {
         for(char i=0;i<XTE_MAC_ADDR_SIZE;i++) Buffer[i] = MAC_DEST[i];
         for(char i=0;i<XTE_MAC_ADDR_SIZE;i++) Buffer[i+6] = MAC_SRC[i];
   	   Buffer[12] = 0x08; Buffer[13] = 0x06; // protocol type ARP
         Buffer[14] = 0x00; Buffer[15] = 0x01; // HTYPE
   	   Buffer[16] = 0x08; Buffer[17] = 0x00; // PTYPE
   	   Buffer[18] = 0x06; Buffer[19] = 0x04; // PLEN
   	   Buffer[20] = 0x00; Buffer[21] = 0x01; // ARP Request
         for(char i=0;i<XTE_MAC_ADDR_SIZE;i++) Buffer[i+22] = MAC_SRC[i];
         for(char i=0;i<XTE_IP_ADDR_SIZE;i++) Buffer[i+28] = IP_SRC[i];
         for(char i=0;i<XTE_MAC_ADDR_SIZE;i++) Buffer[i+32] = 0x00;
         for(char i=0;i<XTE_IP_ADDR_SIZE;i++) Buffer[i+38] = IP_DEST[i];
   	   for(char i=42;i<60;i++) Buffer[i] = 0x00;
      }break;
      case 1://answer
      {
         for(char i=0;i<XTE_MAC_ADDR_SIZE;i++) Buffer[i] = MAC_DEST[i];
         for(char i=0;i<XTE_MAC_ADDR_SIZE;i++) Buffer[i+6] = MAC_SRC[i];
   	   Buffer[12] = 0x08; Buffer[13] = 0x06; // protocol type ARP
         Buffer[14] = 0x00; Buffer[15] = 0x01; // HTYPE
   	   Buffer[16] = 0x08; Buffer[17] = 0x00; // PTYPE
   	   Buffer[18] = 0x06; Buffer[19] = 0x04; // PLEN
   	   Buffer[20] = 0x00; Buffer[21] = 0x02; // ARP Answer
         for(char i=0;i<XTE_MAC_ADDR_SIZE;i++) Buffer[i+22] = MAC_SRC[i];
         for(char i=0;i<XTE_IP_ADDR_SIZE;i++) Buffer[i+28] = IP_SRC[i];
         for(char i=0;i<XTE_MAC_ADDR_SIZE;i++) Buffer[i+32] = MAC_DEST[i];
         for(char i=0;i<XTE_IP_ADDR_SIZE;i++) Buffer[i+38] = IP_DEST[i];
   	   for(char i=42;i<60;i++) Buffer[i] = 0x00;
      }break;
   }
};

bool CheckForARPRequest(unsigned char *FramePtr)
{
	unsigned char *Buffer = FramePtr;

	/*проверка на ARP запрос*/

	 if(Buffer[38]==IP_SRC[0] && Buffer[39]==IP_SRC[1] &&
		 Buffer[40]==IP_SRC[2] && Buffer[41]==IP_SRC[3])
	 {
		 if(Buffer[12]==0x08 && Buffer[13]==0x06) //ARP
		 {
			 if(Buffer[14]==0x00 && Buffer[15]==0x01) //Hardware type
			 {
				 if(Buffer[16]==0x08 && Buffer[17]==0x00) //IP
				 {
					 if(Buffer[18]==0x06 && Buffer[19]==0x04) //Length
					 {
						 if(Buffer[20]==0x00 && Buffer[21]==0x01) //Request
						 {
                     unsigned char MAC_DEST_BUF[XTE_MAC_ADDR_SIZE];
		               unsigned char IP_DEST_BUF[XTE_IP_ADDR_SIZE];
		               for(unsigned char i=0;i<XTE_MAC_ADDR_SIZE;i++) MAC_DEST_BUF[i] = MAC_DEST[i];
		               for(unsigned char i=0;i<XTE_IP_ADDR_SIZE;i++) IP_DEST_BUF[i] = IP_DEST[i];

                     for(unsigned char i=0;i<XTE_MAC_ADDR_SIZE;i++) MAC_DEST[i] = Buffer[i+22];
		               for(unsigned char i=0;i<XTE_IP_ADDR_SIZE;i++) IP_DEST[i] = Buffer[i+28];

                     SendARPAnswer(); // ВИНДА ОТПРАВЛЯЕТ ЕГО ЗА НАС ЕСЛИ НАСТРОЕН IP В СЕТЕВУХЕ

                     for(unsigned char i=0;i<XTE_MAC_ADDR_SIZE;i++) MAC_DEST[i] = MAC_DEST_BUF[i];
                     for(unsigned char i=0;i<XTE_IP_ADDR_SIZE;i++) IP_DEST[i] = IP_DEST_BUF[i];
                     return 1;
						 } else return 0;
					 } else return 0;
				 } else return 0;
			 } else return 0;
		 } else return 0;
	 } else return 0;
}

bool CheckForARPAnswer(unsigned char *FramePtr)
{
	unsigned char *Buffer = FramePtr;

	/*проверка на ARP ответ*/

	if (Buffer[0]==MAC_SRC[0] && Buffer[1]==MAC_SRC[1] &&
		 Buffer[2]==MAC_SRC[2] && Buffer[3]==MAC_SRC[3] &&
		 Buffer[4]==MAC_SRC[4] && Buffer[5]==MAC_SRC[5])
	{
	 if(Buffer[38]==IP_SRC[0] && Buffer[39]==IP_SRC[1] &&
		 Buffer[40]==IP_SRC[2] && Buffer[41]==IP_SRC[3])
	 {
		if(Buffer[12]==0x08 && Buffer[13]==0x06) //ARP
		{
			if(Buffer[14]==0x00 && Buffer[15]==0x01) //Hardware type
			{
				if(Buffer[16]==0x08 && Buffer[17]==0x00) //IP
				{
					if(Buffer[18]==0x06 && Buffer[19]==0x04) //Length
					{
						if(Buffer[20]==0x00 && Buffer[21]==0x02) //Answer
						{
							for(unsigned char i=0;i<XTE_MAC_ADDR_SIZE;i++) MAC_DEST[i] = Buffer[i+22];
							return 1;
						} else return 0;
					} else return 0;
				} else return 0;
			} else return 0;
		} else return 0;
	 } else return 0;
   } else return 0;
}

void SendARPRequest()
{
   char *Buffer = new char[60];
   ARPBufPrepare(Buffer, 0);
   //Hardware->SendData(60, Buffer);
   delete [] Buffer;
}

void SendARPAnswer()
{
   /*char *Buffer = new char[60];
   ARPBufPrepare(Buffer, 1);
   Hardware->SendData(60, Buffer);
   delete [] Buffer; */
}
/*************************************************************************/
/********************************* CHECK ANSWER **************************/
/*************************************************************************/
bool CheckForAnswer(unsigned char *Buffer, unsigned short &CmdNum, unsigned int &PacketNum)
{
   if((*((unsigned short *) (Buffer+42))==0xDCBA) && (*((unsigned short *) (Buffer+44))==0xABCD))
   {
	 if(Buffer[30]==IP_SRC[0]&&Buffer[31]==IP_SRC[1]&&Buffer[32]==IP_SRC[2]&&Buffer[33]==IP_SRC[3])
	 {
     if(Buffer[36]==DATAPort_SRC[0]&&Buffer[37]==DATAPort_SRC[1]) //Port
     {
       CmdNum = *((unsigned short *) (Buffer+46));
       PacketNum = *((unsigned int *)(Buffer+48));
       if(CmdNum != 0x3333)
       {
        //SendAck(PacketNum);
		  return 1;
      } else return 0;
     } else return 0;
	 } else return 0;
   } else return 0;
}
/*************************************************************************/
/********************************* ACK ***********************************/
/*************************************************************************/
bool CheckForAck(unsigned char *Buffer, unsigned int &PacketNum)
{
   if((*((unsigned short *) (Buffer+42))==0xDCBA) && (*((unsigned short *) (Buffer+44))==0xABCD))
   {
	   if(Buffer[30]==IP_SRC[0]&&Buffer[31]==IP_SRC[1]&&Buffer[32]==IP_SRC[2]&&Buffer[33]==IP_SRC[3])
	   {
		   if(Buffer[36]==DATAPort_SRC[0]&&Buffer[37]==DATAPort_SRC[1]) //Port
		   {
            if(*((unsigned short *) (Buffer+46))==0x3333)
            {
               PacketNum = *((unsigned int *)(Buffer+48));
			      return 1;
            }else return 0;
		   }else return 0;
   	}else return 0;
   }else return 0;
}

void SendAck(unsigned int PacketNum)
{
   /*unsigned short PayloadSize = XTE_CMD_HDR_SIZE+16;
   unsigned char *Buffer = new unsigned char[XTE_ETH_IP_UDP_HDR_SIZE+PayloadSize];
   unsigned char *DataPtr = Buffer;

	EthFrameSetIPHeader(Buffer, DATAPort_DEST, PayloadSize);
   EthFrameSetCmdHeader(Buffer, 0x3333, PacketNum);
   DataPtr = (Buffer + XTE_ETH_IP_UDP_HDR_SIZE + XTE_CMD_HDR_SIZE);

	char s[15] = "___POS_ACK!___";
	for(unsigned char i=0;i<sizeof(s);i++) *(DataPtr+i) = s[i];

   Hardware->SendData(XTE_ETH_IP_UDP_HDR_SIZE+PayloadSize, Buffer);
   delete [] Buffer;*/
}
/*************************************************************************/
/************************* OTHERS ****************************************/
/*************************************************************************/
void SendBulkPacket(unsigned int &PacketNum)
{
   unsigned short PayloadSize = XTE_CMD_HDR_SIZE+128;
   unsigned char *Buffer = new unsigned char[XTE_ETH_IP_UDP_HDR_SIZE+PayloadSize];
   unsigned char *DataPtr = Buffer;

	EthFrameSetIPHeader(Buffer, DATAPort_DEST, PayloadSize);
   EthFrameSetCmdHeader(Buffer, 0x1111, PacketNum);

   DataPtr = (DataPtr + XTE_ETH_IP_UDP_HDR_SIZE + XTE_CMD_HDR_SIZE);

	for(unsigned char i=0;i<128;i++) *(DataPtr+i) = i+1;

   formMain->SendData(PayloadSize+XTE_ETH_IP_UDP_HDR_SIZE, Buffer);
   delete [] Buffer;
}

void SendCmdPacket(unsigned short BufferSize, unsigned char *Buffer, unsigned short CmdNum, unsigned int &PacketNum)
{
   unsigned short PayloadSize = XTE_CMD_HDR_SIZE+BufferSize;
   unsigned char *TxBuffer = new unsigned char[XTE_ETH_IP_UDP_HDR_SIZE+PayloadSize];

	EthFrameSetIPHeader(TxBuffer, DATAPort_DEST, PayloadSize);

   // start payload data
   EthFrameSetCmdHeader(TxBuffer, CmdNum, PacketNum);
	for(unsigned char i=0;i<BufferSize;i++) *(TxBuffer+XTE_ETH_IP_UDP_HDR_SIZE+XTE_CMD_HDR_SIZE+i) = *(Buffer+i);
   // end payload data

   formMain->SendData(PayloadSize+XTE_ETH_IP_UDP_HDR_SIZE, TxBuffer);
   delete [] TxBuffer;
}
